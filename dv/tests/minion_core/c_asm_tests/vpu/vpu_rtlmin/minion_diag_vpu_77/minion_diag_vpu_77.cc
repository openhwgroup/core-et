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
			 0x7c441f9a, /// 0x0
			 0x2ddb22f3, /// 0x4
			 0xc7e17852, /// 0x8
			 0x77f23405, /// 0xc
			 0x57e59cda, /// 0x10
			 0x5e71d930, /// 0x14
			 0x2dda5d49, /// 0x18
			 0xda9b9c86, /// 0x1c
			 0x03be23b4, /// 0x20
			 0x2998b1f0, /// 0x24
			 0x95a8ad80, /// 0x28
			 0xb6c20512, /// 0x2c
			 0x7370f106, /// 0x30
			 0xb5fc2ebd, /// 0x34
			 0xc799b311, /// 0x38
			 0x2021fc07, /// 0x3c
			 0x78f92463, /// 0x40
			 0xd88967f1, /// 0x44
			 0xb8f328e8, /// 0x48
			 0x462b1c53, /// 0x4c
			 0xe4dd58c9, /// 0x50
			 0x1200c2c9, /// 0x54
			 0x33d86f28, /// 0x58
			 0x75ccc862, /// 0x5c
			 0x6cb3159e, /// 0x60
			 0x00b45b9b, /// 0x64
			 0xe7c5535e, /// 0x68
			 0x396c4257, /// 0x6c
			 0xb708e60b, /// 0x70
			 0x0fccdf09, /// 0x74
			 0x6092470b, /// 0x78
			 0x0f3b6deb, /// 0x7c
			 0xc39bcd36, /// 0x80
			 0xbf440048, /// 0x84
			 0xb491483a, /// 0x88
			 0xb6cf9b25, /// 0x8c
			 0x2928c883, /// 0x90
			 0xfeef2ff0, /// 0x94
			 0xba299c47, /// 0x98
			 0x1a22f115, /// 0x9c
			 0x849e587a, /// 0xa0
			 0x75e40902, /// 0xa4
			 0x9d5cf9af, /// 0xa8
			 0xf20247a9, /// 0xac
			 0x0928259d, /// 0xb0
			 0x08fcb1a9, /// 0xb4
			 0xc8507278, /// 0xb8
			 0x74650feb, /// 0xbc
			 0x679c3e0d, /// 0xc0
			 0xb771c63c, /// 0xc4
			 0xf412f81e, /// 0xc8
			 0xdc46321c, /// 0xcc
			 0x9c62407e, /// 0xd0
			 0x5406a9df, /// 0xd4
			 0xcb7b2028, /// 0xd8
			 0x0740bc71, /// 0xdc
			 0xf09f70ee, /// 0xe0
			 0x5ef27b27, /// 0xe4
			 0xd38d26de, /// 0xe8
			 0xf046f7ea, /// 0xec
			 0xefa1ecad, /// 0xf0
			 0x4f82d2bf, /// 0xf4
			 0x16997742, /// 0xf8
			 0x29811701, /// 0xfc
			 0x08c04cd9, /// 0x100
			 0x8bccccf9, /// 0x104
			 0xdbc1beae, /// 0x108
			 0x9bb0c548, /// 0x10c
			 0xd1a60c9d, /// 0x110
			 0x077cae91, /// 0x114
			 0x0b72671e, /// 0x118
			 0xe7886094, /// 0x11c
			 0x2ad6364f, /// 0x120
			 0xdeea6992, /// 0x124
			 0x1bb24b5e, /// 0x128
			 0xdc162567, /// 0x12c
			 0xbf01b607, /// 0x130
			 0xbb282db3, /// 0x134
			 0xc81ca75a, /// 0x138
			 0x31b3e1bc, /// 0x13c
			 0x65d0ae82, /// 0x140
			 0x1b00a7ec, /// 0x144
			 0x0b55ee37, /// 0x148
			 0x0bff3c14, /// 0x14c
			 0x71abdcab, /// 0x150
			 0x1cb396b0, /// 0x154
			 0xdea5e848, /// 0x158
			 0xd771017b, /// 0x15c
			 0xc0725060, /// 0x160
			 0xfcca3cec, /// 0x164
			 0x3280bf61, /// 0x168
			 0x738c137d, /// 0x16c
			 0x39fadcbc, /// 0x170
			 0xbd569fcb, /// 0x174
			 0x3c720cc1, /// 0x178
			 0xba11f549, /// 0x17c
			 0x086bdd1f, /// 0x180
			 0x750f3cc0, /// 0x184
			 0x1041520f, /// 0x188
			 0xfd3e1d03, /// 0x18c
			 0x1a3f60b6, /// 0x190
			 0x54b74ad1, /// 0x194
			 0xc2d72b69, /// 0x198
			 0xfa784697, /// 0x19c
			 0xbcb15c47, /// 0x1a0
			 0x9de603a3, /// 0x1a4
			 0x0f6083be, /// 0x1a8
			 0x79695752, /// 0x1ac
			 0xf1316f13, /// 0x1b0
			 0x872961dc, /// 0x1b4
			 0x970d7c1f, /// 0x1b8
			 0x19b4104c, /// 0x1bc
			 0x9882dbc8, /// 0x1c0
			 0x84927473, /// 0x1c4
			 0x16a9f6df, /// 0x1c8
			 0xfb4c0004, /// 0x1cc
			 0xa083a0e5, /// 0x1d0
			 0x39c1baa0, /// 0x1d4
			 0xd419a810, /// 0x1d8
			 0xfa213558, /// 0x1dc
			 0x72135cee, /// 0x1e0
			 0x730e951d, /// 0x1e4
			 0x5c58800f, /// 0x1e8
			 0xf203fa9e, /// 0x1ec
			 0x83f51914, /// 0x1f0
			 0x1c6d3b5e, /// 0x1f4
			 0x4fed6569, /// 0x1f8
			 0xbed2ecfa, /// 0x1fc
			 0x6752d192, /// 0x200
			 0xc036f6ab, /// 0x204
			 0x39e14c37, /// 0x208
			 0x3b1d3194, /// 0x20c
			 0x3584fbd0, /// 0x210
			 0xd0f27481, /// 0x214
			 0x4cb8f0e3, /// 0x218
			 0xcb8e92d1, /// 0x21c
			 0x9e988e1b, /// 0x220
			 0xb73d7469, /// 0x224
			 0xebd3d051, /// 0x228
			 0x49582e18, /// 0x22c
			 0x8b6a99eb, /// 0x230
			 0x16f04c15, /// 0x234
			 0x740eeb4d, /// 0x238
			 0x12bd8f77, /// 0x23c
			 0x95268134, /// 0x240
			 0x921fe118, /// 0x244
			 0x3d4aff72, /// 0x248
			 0x1398d5fd, /// 0x24c
			 0xf33f847d, /// 0x250
			 0x2124fad4, /// 0x254
			 0x8cf69390, /// 0x258
			 0xf7791d72, /// 0x25c
			 0x2a90d0ab, /// 0x260
			 0x2a6a44a6, /// 0x264
			 0xda170ab8, /// 0x268
			 0xc791cdae, /// 0x26c
			 0x2c977176, /// 0x270
			 0x26dce299, /// 0x274
			 0xb8aedcda, /// 0x278
			 0x14c5bb03, /// 0x27c
			 0xc80f3976, /// 0x280
			 0xb0a4a85a, /// 0x284
			 0xd3ca379d, /// 0x288
			 0xd293a855, /// 0x28c
			 0xfcdcd1dd, /// 0x290
			 0x5ef79974, /// 0x294
			 0x8f0fc0c7, /// 0x298
			 0x92d0f019, /// 0x29c
			 0x7c776806, /// 0x2a0
			 0x219e2276, /// 0x2a4
			 0xf665df27, /// 0x2a8
			 0x75f65516, /// 0x2ac
			 0x66802d30, /// 0x2b0
			 0xfbc14e65, /// 0x2b4
			 0x982b964c, /// 0x2b8
			 0x6400931f, /// 0x2bc
			 0x9f2e2c3f, /// 0x2c0
			 0x3600185b, /// 0x2c4
			 0x71c81ac8, /// 0x2c8
			 0x328fc787, /// 0x2cc
			 0x5ac71a70, /// 0x2d0
			 0x7aeee0c6, /// 0x2d4
			 0x59dec4a5, /// 0x2d8
			 0x3114901b, /// 0x2dc
			 0x0ba64705, /// 0x2e0
			 0x46119b8f, /// 0x2e4
			 0x32f9dceb, /// 0x2e8
			 0x6b54168d, /// 0x2ec
			 0xe9992245, /// 0x2f0
			 0xde2840f4, /// 0x2f4
			 0xddb31ccd, /// 0x2f8
			 0x140c059b, /// 0x2fc
			 0x778c8dbc, /// 0x300
			 0x272635ec, /// 0x304
			 0xca24d8e4, /// 0x308
			 0x4f2cffb8, /// 0x30c
			 0xcc63007c, /// 0x310
			 0xa65c4176, /// 0x314
			 0xdf7ee8a1, /// 0x318
			 0x030569d9, /// 0x31c
			 0x4a2a8187, /// 0x320
			 0xe70ee16d, /// 0x324
			 0x7a291e52, /// 0x328
			 0x1fcd4102, /// 0x32c
			 0x0ccbbc32, /// 0x330
			 0xd47d873d, /// 0x334
			 0xbb7b7a17, /// 0x338
			 0x3d4dc83f, /// 0x33c
			 0x4a076af9, /// 0x340
			 0xcbb4c628, /// 0x344
			 0x608b0ee7, /// 0x348
			 0xbc12681d, /// 0x34c
			 0x07bd4ad4, /// 0x350
			 0xb92a3bf9, /// 0x354
			 0xae7beb29, /// 0x358
			 0x7eaa12a9, /// 0x35c
			 0xb4bfed46, /// 0x360
			 0x5612c2de, /// 0x364
			 0xdaf86f86, /// 0x368
			 0x77da2e6c, /// 0x36c
			 0x3d47816e, /// 0x370
			 0xf521674e, /// 0x374
			 0x6c4081d4, /// 0x378
			 0x26c090d7, /// 0x37c
			 0xe58634a8, /// 0x380
			 0x54ec9b73, /// 0x384
			 0x4790242d, /// 0x388
			 0x87333342, /// 0x38c
			 0x98cf4493, /// 0x390
			 0x3553604c, /// 0x394
			 0x30563a81, /// 0x398
			 0x79cacbf7, /// 0x39c
			 0x1727b544, /// 0x3a0
			 0x9031783d, /// 0x3a4
			 0xc78de97a, /// 0x3a8
			 0xf0c34ca9, /// 0x3ac
			 0x78139a58, /// 0x3b0
			 0x82695e50, /// 0x3b4
			 0x5931dd72, /// 0x3b8
			 0x2247e2e0, /// 0x3bc
			 0x66d08427, /// 0x3c0
			 0x73dabd26, /// 0x3c4
			 0xe0d70506, /// 0x3c8
			 0x37b29f84, /// 0x3cc
			 0x8459c53e, /// 0x3d0
			 0xa652ab6c, /// 0x3d4
			 0x9a993ada, /// 0x3d8
			 0x30564f3a, /// 0x3dc
			 0x1e08dcb9, /// 0x3e0
			 0x0359cc5a, /// 0x3e4
			 0xfa9ea979, /// 0x3e8
			 0x6b56113f, /// 0x3ec
			 0x6eb50994, /// 0x3f0
			 0xb21b150a, /// 0x3f4
			 0xa9c3169f, /// 0x3f8
			 0x5c4e02e7, /// 0x3fc
			 0x7fded029, /// 0x400
			 0xb65d2b96, /// 0x404
			 0x0d993151, /// 0x408
			 0xeee0831e, /// 0x40c
			 0xd1cb7627, /// 0x410
			 0xdb879366, /// 0x414
			 0x2d5c1bf8, /// 0x418
			 0xfc3114c7, /// 0x41c
			 0x9da831a1, /// 0x420
			 0xa71552c3, /// 0x424
			 0x1ee4bdce, /// 0x428
			 0x78398ffe, /// 0x42c
			 0x40135780, /// 0x430
			 0xe53412c7, /// 0x434
			 0x0ea0c6bd, /// 0x438
			 0x1bf0fa5e, /// 0x43c
			 0x62815d2a, /// 0x440
			 0xb53743a3, /// 0x444
			 0x9faf61eb, /// 0x448
			 0xb48fa0b1, /// 0x44c
			 0xb9ac1b75, /// 0x450
			 0xb08b6b7d, /// 0x454
			 0xbf0491db, /// 0x458
			 0x31fb627f, /// 0x45c
			 0xb906bb9d, /// 0x460
			 0x708219c7, /// 0x464
			 0x67f9a5ab, /// 0x468
			 0x3b938851, /// 0x46c
			 0x03e3a7f0, /// 0x470
			 0xeab6dcc9, /// 0x474
			 0x68372bc8, /// 0x478
			 0xe8d44f21, /// 0x47c
			 0xd7f78f4f, /// 0x480
			 0xe5cbb432, /// 0x484
			 0x992a5375, /// 0x488
			 0xea8ccfb8, /// 0x48c
			 0x3b0c34e4, /// 0x490
			 0x04b4b990, /// 0x494
			 0xd964d934, /// 0x498
			 0x31fb5410, /// 0x49c
			 0x1e0bb5f0, /// 0x4a0
			 0x4fb71122, /// 0x4a4
			 0x60fe0fee, /// 0x4a8
			 0x4bcfb9e5, /// 0x4ac
			 0x9b8da0ac, /// 0x4b0
			 0x7d070a3c, /// 0x4b4
			 0x8da99712, /// 0x4b8
			 0xdd146d4f, /// 0x4bc
			 0x4549ac74, /// 0x4c0
			 0xfe4667a1, /// 0x4c4
			 0x4f175213, /// 0x4c8
			 0x5b6d4b68, /// 0x4cc
			 0x7401e28e, /// 0x4d0
			 0x68a72e46, /// 0x4d4
			 0xf44b3a2a, /// 0x4d8
			 0x38774dcd, /// 0x4dc
			 0x78b6d9ad, /// 0x4e0
			 0x5d995f9a, /// 0x4e4
			 0xa91b792e, /// 0x4e8
			 0x054b9071, /// 0x4ec
			 0xdf7facde, /// 0x4f0
			 0xcd9ce1b4, /// 0x4f4
			 0xb32b7e63, /// 0x4f8
			 0x84cfbc7e, /// 0x4fc
			 0x24927e64, /// 0x500
			 0x1ef2f994, /// 0x504
			 0x1a8fa072, /// 0x508
			 0x564912ef, /// 0x50c
			 0xe287f5e6, /// 0x510
			 0xa79d2c85, /// 0x514
			 0x3b3c4c95, /// 0x518
			 0xa84a167d, /// 0x51c
			 0xb1ccc911, /// 0x520
			 0x51476317, /// 0x524
			 0xcea52ba8, /// 0x528
			 0x4354cf2d, /// 0x52c
			 0xc52ff26b, /// 0x530
			 0x031902e7, /// 0x534
			 0x63ab187f, /// 0x538
			 0x63cb3f9d, /// 0x53c
			 0xaf936bf9, /// 0x540
			 0xb7154f47, /// 0x544
			 0x84b4d2b8, /// 0x548
			 0xd5bb4ffc, /// 0x54c
			 0x77072137, /// 0x550
			 0xd1537bd8, /// 0x554
			 0x015b61bf, /// 0x558
			 0x370f9def, /// 0x55c
			 0x3a6acd55, /// 0x560
			 0x91a709d8, /// 0x564
			 0x018700ce, /// 0x568
			 0xdddd7034, /// 0x56c
			 0x9f68ead9, /// 0x570
			 0x1300c5c3, /// 0x574
			 0xb54b2cdf, /// 0x578
			 0xc2fc6fe5, /// 0x57c
			 0xcc40d491, /// 0x580
			 0x458097cc, /// 0x584
			 0x1a50cccc, /// 0x588
			 0x7470f0c1, /// 0x58c
			 0x2dddced2, /// 0x590
			 0xeedb7223, /// 0x594
			 0x850b226f, /// 0x598
			 0x3ecf6238, /// 0x59c
			 0x15232bcc, /// 0x5a0
			 0x9b2e454f, /// 0x5a4
			 0x94788bcb, /// 0x5a8
			 0xa55ed3a8, /// 0x5ac
			 0x576ceec0, /// 0x5b0
			 0x62038fea, /// 0x5b4
			 0x86f5af07, /// 0x5b8
			 0xdd654023, /// 0x5bc
			 0x6d25cb69, /// 0x5c0
			 0x9329f88c, /// 0x5c4
			 0x0c2e0252, /// 0x5c8
			 0x17a5afbb, /// 0x5cc
			 0xf73e2325, /// 0x5d0
			 0xf4dd4a6c, /// 0x5d4
			 0xd8599a6f, /// 0x5d8
			 0xae16d70c, /// 0x5dc
			 0x7fde4bb9, /// 0x5e0
			 0x49139c73, /// 0x5e4
			 0xd1974bea, /// 0x5e8
			 0x877affa2, /// 0x5ec
			 0x341ec579, /// 0x5f0
			 0xb53eeeaf, /// 0x5f4
			 0x23b85b9f, /// 0x5f8
			 0xc7e23b92, /// 0x5fc
			 0x935fc0ee, /// 0x600
			 0xfbfeb7e6, /// 0x604
			 0x17cc1e83, /// 0x608
			 0x4cc73018, /// 0x60c
			 0xb9e6344c, /// 0x610
			 0xb8d93f73, /// 0x614
			 0x9d2d07fe, /// 0x618
			 0x77e05f6e, /// 0x61c
			 0x86973afd, /// 0x620
			 0xbea9a200, /// 0x624
			 0xa28e0ebe, /// 0x628
			 0xc8a98604, /// 0x62c
			 0x7dadfafb, /// 0x630
			 0xb74de7b3, /// 0x634
			 0x74df0bc8, /// 0x638
			 0xcdcd7d97, /// 0x63c
			 0x1f7b483e, /// 0x640
			 0xc3a17e6f, /// 0x644
			 0xf39e16c5, /// 0x648
			 0x03cfb6be, /// 0x64c
			 0x83f6034d, /// 0x650
			 0x4ab201b9, /// 0x654
			 0xa40c77ec, /// 0x658
			 0x4603a1e9, /// 0x65c
			 0xcd11c354, /// 0x660
			 0xd8f977b8, /// 0x664
			 0x6b5667e2, /// 0x668
			 0xeaf86351, /// 0x66c
			 0x1c80a255, /// 0x670
			 0x736b256f, /// 0x674
			 0x0295163e, /// 0x678
			 0x9f97a78c, /// 0x67c
			 0x4bd7f4f5, /// 0x680
			 0x455e493a, /// 0x684
			 0xc4b0df9d, /// 0x688
			 0x06d6fada, /// 0x68c
			 0xd8898178, /// 0x690
			 0x9f4e15fc, /// 0x694
			 0x8fb80a80, /// 0x698
			 0xb883cd04, /// 0x69c
			 0x53180feb, /// 0x6a0
			 0x6637eb6e, /// 0x6a4
			 0x98e3e5dc, /// 0x6a8
			 0xb8f18804, /// 0x6ac
			 0x02bcd473, /// 0x6b0
			 0x84c366a9, /// 0x6b4
			 0xc533ebfe, /// 0x6b8
			 0x953802fe, /// 0x6bc
			 0x102701d2, /// 0x6c0
			 0x7c5cb36b, /// 0x6c4
			 0xbc0880f3, /// 0x6c8
			 0x7fda3272, /// 0x6cc
			 0x7647cd53, /// 0x6d0
			 0x4234f88a, /// 0x6d4
			 0xd9174a70, /// 0x6d8
			 0x02bd77ba, /// 0x6dc
			 0x2e798dc0, /// 0x6e0
			 0x876ee05f, /// 0x6e4
			 0x603267bb, /// 0x6e8
			 0x03cf55a4, /// 0x6ec
			 0xdd3a49b2, /// 0x6f0
			 0x66e19934, /// 0x6f4
			 0x5f4ce316, /// 0x6f8
			 0xa1d67ff4, /// 0x6fc
			 0x9fcaec80, /// 0x700
			 0x3feae173, /// 0x704
			 0xc0237f98, /// 0x708
			 0x6d0bc31a, /// 0x70c
			 0xc5d04d6e, /// 0x710
			 0x5629f263, /// 0x714
			 0x76ccf373, /// 0x718
			 0xcaf40dcc, /// 0x71c
			 0x7d946161, /// 0x720
			 0xf48425a2, /// 0x724
			 0x8f33a9ad, /// 0x728
			 0x8879ea83, /// 0x72c
			 0xcd078118, /// 0x730
			 0xabd5aec0, /// 0x734
			 0xd2568f64, /// 0x738
			 0xce09d2c3, /// 0x73c
			 0xc841165d, /// 0x740
			 0x2743f293, /// 0x744
			 0x1098db00, /// 0x748
			 0x53a6c8d9, /// 0x74c
			 0x36b3e91d, /// 0x750
			 0xe6c87f78, /// 0x754
			 0x41dbe62c, /// 0x758
			 0xcec79447, /// 0x75c
			 0x2cb9a4e5, /// 0x760
			 0x1de91943, /// 0x764
			 0x98a9d00f, /// 0x768
			 0x73db9b0e, /// 0x76c
			 0x2b2ce906, /// 0x770
			 0x4f7bbb69, /// 0x774
			 0xd4570629, /// 0x778
			 0x9264710c, /// 0x77c
			 0x231b0b07, /// 0x780
			 0xd65082b8, /// 0x784
			 0xb1719f94, /// 0x788
			 0xaf274cca, /// 0x78c
			 0x288d1a1b, /// 0x790
			 0xd0d3debe, /// 0x794
			 0x0505f9fc, /// 0x798
			 0x3259fb29, /// 0x79c
			 0x019abc27, /// 0x7a0
			 0x1f9da68d, /// 0x7a4
			 0xef251eab, /// 0x7a8
			 0xeb00eea1, /// 0x7ac
			 0x2d0b2d87, /// 0x7b0
			 0x287fe4b1, /// 0x7b4
			 0x4e5edef5, /// 0x7b8
			 0x2acc294a, /// 0x7bc
			 0x94bf72d6, /// 0x7c0
			 0xedfc7b80, /// 0x7c4
			 0xa9699c57, /// 0x7c8
			 0x5bceebff, /// 0x7cc
			 0x8a2be684, /// 0x7d0
			 0x17641e65, /// 0x7d4
			 0x9fd188b1, /// 0x7d8
			 0x83e384cc, /// 0x7dc
			 0x5f8a208a, /// 0x7e0
			 0x11997510, /// 0x7e4
			 0x8c10869d, /// 0x7e8
			 0x03eba6c9, /// 0x7ec
			 0x68bae086, /// 0x7f0
			 0xa7499001, /// 0x7f4
			 0x9cb5ffd8, /// 0x7f8
			 0x509cd01b, /// 0x7fc
			 0xd06bca18, /// 0x800
			 0x7cf86c21, /// 0x804
			 0xa3084a22, /// 0x808
			 0xde14b1ba, /// 0x80c
			 0xddeae7ef, /// 0x810
			 0xd986e5ce, /// 0x814
			 0x2dd36772, /// 0x818
			 0x0a8bd57b, /// 0x81c
			 0xbd2f452e, /// 0x820
			 0x6791e518, /// 0x824
			 0x35517fc1, /// 0x828
			 0x65e58324, /// 0x82c
			 0x8161f77d, /// 0x830
			 0xc7f3cbe1, /// 0x834
			 0xaa9aaa59, /// 0x838
			 0x8bd593bb, /// 0x83c
			 0xfc3b44ba, /// 0x840
			 0x337d4676, /// 0x844
			 0x89b1411d, /// 0x848
			 0x707c796f, /// 0x84c
			 0xc743cf39, /// 0x850
			 0xaa03dfea, /// 0x854
			 0x67e30a8b, /// 0x858
			 0x301f3064, /// 0x85c
			 0xaf15ed81, /// 0x860
			 0x4cc1eb23, /// 0x864
			 0xee35aa21, /// 0x868
			 0xd0c101da, /// 0x86c
			 0xc4098a18, /// 0x870
			 0x8328772d, /// 0x874
			 0x684dcc62, /// 0x878
			 0x060e151c, /// 0x87c
			 0xc818a5fe, /// 0x880
			 0xe907ae07, /// 0x884
			 0x4e3dcd54, /// 0x888
			 0x7d151176, /// 0x88c
			 0x47b35688, /// 0x890
			 0x717c47d3, /// 0x894
			 0xb2fb3881, /// 0x898
			 0x8e0ccecd, /// 0x89c
			 0x596dbff0, /// 0x8a0
			 0x77b465f5, /// 0x8a4
			 0xc77ec225, /// 0x8a8
			 0x679c3fa2, /// 0x8ac
			 0x9301f3b9, /// 0x8b0
			 0xa64a1510, /// 0x8b4
			 0x250afcc9, /// 0x8b8
			 0x701e888c, /// 0x8bc
			 0x167942c4, /// 0x8c0
			 0x8f7f0cce, /// 0x8c4
			 0xf586101e, /// 0x8c8
			 0x1c1c65c9, /// 0x8cc
			 0xb30bcf17, /// 0x8d0
			 0xa06ffc88, /// 0x8d4
			 0xf171873a, /// 0x8d8
			 0x84a5b829, /// 0x8dc
			 0xba04daa4, /// 0x8e0
			 0xbd9ab60a, /// 0x8e4
			 0x56e40386, /// 0x8e8
			 0xc618b20b, /// 0x8ec
			 0x04390e2c, /// 0x8f0
			 0xcbac10f9, /// 0x8f4
			 0xefb4bb4b, /// 0x8f8
			 0xd19406ec, /// 0x8fc
			 0xfd5b42d9, /// 0x900
			 0xc2c1b4b4, /// 0x904
			 0xa95402b1, /// 0x908
			 0x16a0e1d9, /// 0x90c
			 0x76e9fe57, /// 0x910
			 0xe69c582e, /// 0x914
			 0x59be8ff1, /// 0x918
			 0xd5740d18, /// 0x91c
			 0x12441cbb, /// 0x920
			 0x51152c2b, /// 0x924
			 0x94042882, /// 0x928
			 0x59fc74ea, /// 0x92c
			 0x9e2cf9cb, /// 0x930
			 0x20e2908b, /// 0x934
			 0x72e93a62, /// 0x938
			 0x9a4aafbd, /// 0x93c
			 0x267d6804, /// 0x940
			 0x5a15e78c, /// 0x944
			 0xcfec840d, /// 0x948
			 0x38727c8f, /// 0x94c
			 0x4391e0b2, /// 0x950
			 0x8a4e0825, /// 0x954
			 0x5f02df1a, /// 0x958
			 0x28958de1, /// 0x95c
			 0x498d4ab8, /// 0x960
			 0xf3dd7040, /// 0x964
			 0x59ca6340, /// 0x968
			 0xdece0741, /// 0x96c
			 0xec02aadd, /// 0x970
			 0x9d8a7340, /// 0x974
			 0x0f9771d3, /// 0x978
			 0xdefddffa, /// 0x97c
			 0x3e2af2d1, /// 0x980
			 0x212a79fd, /// 0x984
			 0x094734e3, /// 0x988
			 0x6be0f1f9, /// 0x98c
			 0xc2dd43b4, /// 0x990
			 0x8d28c7ac, /// 0x994
			 0x7ed1529c, /// 0x998
			 0x63e59045, /// 0x99c
			 0xc7afb4aa, /// 0x9a0
			 0x7cefa439, /// 0x9a4
			 0x4af63af5, /// 0x9a8
			 0x4451baa2, /// 0x9ac
			 0x452734f0, /// 0x9b0
			 0x574ead19, /// 0x9b4
			 0x33063c85, /// 0x9b8
			 0xe527ec3e, /// 0x9bc
			 0x981b56a0, /// 0x9c0
			 0x09ec2356, /// 0x9c4
			 0xa963e146, /// 0x9c8
			 0x5ad40c3f, /// 0x9cc
			 0x3a011e06, /// 0x9d0
			 0xf3093aff, /// 0x9d4
			 0xa64bf357, /// 0x9d8
			 0x2f3914f1, /// 0x9dc
			 0x3ee884bd, /// 0x9e0
			 0x0832fc4a, /// 0x9e4
			 0xb069c540, /// 0x9e8
			 0x5aaaecd7, /// 0x9ec
			 0xbbd0b7c4, /// 0x9f0
			 0xd5fda955, /// 0x9f4
			 0xd47789b0, /// 0x9f8
			 0x13c4f48f, /// 0x9fc
			 0xc92307c9, /// 0xa00
			 0xe6a28380, /// 0xa04
			 0x0dd4a05a, /// 0xa08
			 0x3eae3be6, /// 0xa0c
			 0x37e8710d, /// 0xa10
			 0xad5a219c, /// 0xa14
			 0x831a6460, /// 0xa18
			 0x823e33db, /// 0xa1c
			 0xa97018e1, /// 0xa20
			 0xe3ffd556, /// 0xa24
			 0x9e8b139d, /// 0xa28
			 0xdf00ec5f, /// 0xa2c
			 0x73206824, /// 0xa30
			 0x317ffcd9, /// 0xa34
			 0xa7d48e59, /// 0xa38
			 0xd4525223, /// 0xa3c
			 0x935187e2, /// 0xa40
			 0xb5b2d967, /// 0xa44
			 0xd580e0c7, /// 0xa48
			 0x7a892ac9, /// 0xa4c
			 0x2e4e955a, /// 0xa50
			 0x7b1b831b, /// 0xa54
			 0x86786bd3, /// 0xa58
			 0x463da0f4, /// 0xa5c
			 0x3bc2e5b0, /// 0xa60
			 0xd979ff02, /// 0xa64
			 0xcbc73a2a, /// 0xa68
			 0xc7635aff, /// 0xa6c
			 0xa95a24d1, /// 0xa70
			 0x24dc2fb1, /// 0xa74
			 0x38c52477, /// 0xa78
			 0x3f659d5b, /// 0xa7c
			 0x4c367f14, /// 0xa80
			 0x19ef17af, /// 0xa84
			 0x16248e0b, /// 0xa88
			 0x32035e61, /// 0xa8c
			 0xf0ca604c, /// 0xa90
			 0xf72c6581, /// 0xa94
			 0x1cdca3bd, /// 0xa98
			 0x36eb6a0d, /// 0xa9c
			 0x6c410bbf, /// 0xaa0
			 0x8d4ead0a, /// 0xaa4
			 0xca32a95d, /// 0xaa8
			 0x8cde4223, /// 0xaac
			 0xe16d9d2c, /// 0xab0
			 0x819a9a10, /// 0xab4
			 0x7a0115b6, /// 0xab8
			 0xd0957815, /// 0xabc
			 0xc058b125, /// 0xac0
			 0xefd91597, /// 0xac4
			 0x7409b25a, /// 0xac8
			 0x230c5add, /// 0xacc
			 0x344a1b76, /// 0xad0
			 0x9abb1372, /// 0xad4
			 0x8408e4f8, /// 0xad8
			 0x523cb438, /// 0xadc
			 0x1189bcdc, /// 0xae0
			 0xf4dd2b92, /// 0xae4
			 0xdcac390e, /// 0xae8
			 0x6e9d8e04, /// 0xaec
			 0xf14f00ec, /// 0xaf0
			 0xc9626c82, /// 0xaf4
			 0xc6811f0b, /// 0xaf8
			 0xa46f25a9, /// 0xafc
			 0xa6725ba6, /// 0xb00
			 0x9176d2a7, /// 0xb04
			 0xb2478f40, /// 0xb08
			 0xefef0bb0, /// 0xb0c
			 0x60f27afe, /// 0xb10
			 0xd2c98df4, /// 0xb14
			 0x34620319, /// 0xb18
			 0x9682c4ac, /// 0xb1c
			 0xf4ce586c, /// 0xb20
			 0xdf278912, /// 0xb24
			 0xd03762f2, /// 0xb28
			 0x8cda241b, /// 0xb2c
			 0xafce86d2, /// 0xb30
			 0x74e72389, /// 0xb34
			 0x3b170cb6, /// 0xb38
			 0x178784df, /// 0xb3c
			 0xcc4d7190, /// 0xb40
			 0x13020b3f, /// 0xb44
			 0xdd94a675, /// 0xb48
			 0x3a24cb1e, /// 0xb4c
			 0x96afd093, /// 0xb50
			 0x9fec12c3, /// 0xb54
			 0xa63eef75, /// 0xb58
			 0xf4a0c92f, /// 0xb5c
			 0x306fece3, /// 0xb60
			 0x54e1c544, /// 0xb64
			 0x2173d577, /// 0xb68
			 0x4acf1034, /// 0xb6c
			 0x15244508, /// 0xb70
			 0x11e3b443, /// 0xb74
			 0x6691a6eb, /// 0xb78
			 0x7564d412, /// 0xb7c
			 0x78e326b2, /// 0xb80
			 0xbe213bc2, /// 0xb84
			 0x1811cbc6, /// 0xb88
			 0xb9a205df, /// 0xb8c
			 0x705f0b3d, /// 0xb90
			 0x51cd3f7f, /// 0xb94
			 0x3697c947, /// 0xb98
			 0x0c7d4171, /// 0xb9c
			 0x00b93b58, /// 0xba0
			 0x84053f7e, /// 0xba4
			 0xe83670f9, /// 0xba8
			 0x8f6fc232, /// 0xbac
			 0x0ce55da7, /// 0xbb0
			 0x4ddb76f9, /// 0xbb4
			 0x6eb4bcac, /// 0xbb8
			 0x88f939f4, /// 0xbbc
			 0xe4434e66, /// 0xbc0
			 0x01e80885, /// 0xbc4
			 0x15420dff, /// 0xbc8
			 0x0520e435, /// 0xbcc
			 0xcabe4a37, /// 0xbd0
			 0xea43cbe2, /// 0xbd4
			 0x3811b3aa, /// 0xbd8
			 0xb9bc4817, /// 0xbdc
			 0x581a7921, /// 0xbe0
			 0xd2b82e77, /// 0xbe4
			 0x6087b8b9, /// 0xbe8
			 0x6ad379e1, /// 0xbec
			 0x592773d2, /// 0xbf0
			 0x5ae4dc0f, /// 0xbf4
			 0x3b803857, /// 0xbf8
			 0x4fbab54f, /// 0xbfc
			 0x95bef663, /// 0xc00
			 0x94b356c2, /// 0xc04
			 0xbfe7ba4d, /// 0xc08
			 0xbea003bb, /// 0xc0c
			 0xc8d49082, /// 0xc10
			 0x2d396d8f, /// 0xc14
			 0x4ddd97c3, /// 0xc18
			 0x756daabe, /// 0xc1c
			 0x5b38c33a, /// 0xc20
			 0x7c851ff4, /// 0xc24
			 0xaa47074a, /// 0xc28
			 0xb42079d8, /// 0xc2c
			 0x5800321d, /// 0xc30
			 0x75e7db89, /// 0xc34
			 0x081b401d, /// 0xc38
			 0x4afc9da0, /// 0xc3c
			 0x743b28d8, /// 0xc40
			 0x92b2a3de, /// 0xc44
			 0x16a1cbd0, /// 0xc48
			 0xa28e6602, /// 0xc4c
			 0xce53442c, /// 0xc50
			 0x0aa34f20, /// 0xc54
			 0x7f4b0009, /// 0xc58
			 0x4591cf8a, /// 0xc5c
			 0x9100be97, /// 0xc60
			 0x00432439, /// 0xc64
			 0x7856cfc9, /// 0xc68
			 0x49fe72ce, /// 0xc6c
			 0x7f89116a, /// 0xc70
			 0x83537c10, /// 0xc74
			 0x830fc008, /// 0xc78
			 0x8f12d6fd, /// 0xc7c
			 0x294e9f99, /// 0xc80
			 0x24351e6c, /// 0xc84
			 0xee38e99e, /// 0xc88
			 0xdd8dfcd1, /// 0xc8c
			 0x3acb72ba, /// 0xc90
			 0x4e31b425, /// 0xc94
			 0xb4bd0ee5, /// 0xc98
			 0x97503160, /// 0xc9c
			 0x3bbf1b93, /// 0xca0
			 0xe32cd141, /// 0xca4
			 0xeeac20aa, /// 0xca8
			 0xce20662f, /// 0xcac
			 0xa0d6aab2, /// 0xcb0
			 0xb5511911, /// 0xcb4
			 0x5dfc4d3b, /// 0xcb8
			 0x6b672a6f, /// 0xcbc
			 0xb0fc96d8, /// 0xcc0
			 0x7158fd15, /// 0xcc4
			 0x00161985, /// 0xcc8
			 0xefc38021, /// 0xccc
			 0xe3fd550b, /// 0xcd0
			 0xbfe743a8, /// 0xcd4
			 0x5a3cee3f, /// 0xcd8
			 0xb593bba6, /// 0xcdc
			 0x31be26af, /// 0xce0
			 0x9a5cc548, /// 0xce4
			 0x0318d021, /// 0xce8
			 0xd88b7d07, /// 0xcec
			 0xc7bc088e, /// 0xcf0
			 0xa610bf65, /// 0xcf4
			 0xd581b20c, /// 0xcf8
			 0x8c1e2444, /// 0xcfc
			 0x1c31a660, /// 0xd00
			 0x07f1144d, /// 0xd04
			 0xb60617fe, /// 0xd08
			 0x00b2c902, /// 0xd0c
			 0x03f23b9d, /// 0xd10
			 0xb4bb3d63, /// 0xd14
			 0xafdde40a, /// 0xd18
			 0x8b7c181e, /// 0xd1c
			 0xafbbc233, /// 0xd20
			 0x1abbe1ce, /// 0xd24
			 0x6e4d4ae7, /// 0xd28
			 0x42ace9b1, /// 0xd2c
			 0xd98f955c, /// 0xd30
			 0x5f3ccf0b, /// 0xd34
			 0x5686431c, /// 0xd38
			 0xc229d77a, /// 0xd3c
			 0x6b7795f9, /// 0xd40
			 0x241674b0, /// 0xd44
			 0x797b7ccc, /// 0xd48
			 0x3b2338a7, /// 0xd4c
			 0x872d8672, /// 0xd50
			 0xc54da766, /// 0xd54
			 0x07edeb60, /// 0xd58
			 0x45eed0fd, /// 0xd5c
			 0x1d89cfec, /// 0xd60
			 0x7b09dd9e, /// 0xd64
			 0x54db8598, /// 0xd68
			 0x44f90a5c, /// 0xd6c
			 0x93c688e4, /// 0xd70
			 0x1dc40407, /// 0xd74
			 0x4f5c2b1f, /// 0xd78
			 0x2e5d874c, /// 0xd7c
			 0xc19e39eb, /// 0xd80
			 0xaf1a5ca3, /// 0xd84
			 0x600e0880, /// 0xd88
			 0x65c0d3ca, /// 0xd8c
			 0x9d3a265c, /// 0xd90
			 0xafbd1ec4, /// 0xd94
			 0x29b8807f, /// 0xd98
			 0x3427d755, /// 0xd9c
			 0x3b824545, /// 0xda0
			 0x1b86d929, /// 0xda4
			 0xff9e2d3f, /// 0xda8
			 0x4b829759, /// 0xdac
			 0x3d5acbc8, /// 0xdb0
			 0x9dfab0e9, /// 0xdb4
			 0xbc9d7129, /// 0xdb8
			 0x152feb17, /// 0xdbc
			 0x9b349150, /// 0xdc0
			 0xea22fba6, /// 0xdc4
			 0x42c24dd2, /// 0xdc8
			 0x26785427, /// 0xdcc
			 0xf8d24b42, /// 0xdd0
			 0x7f96fac2, /// 0xdd4
			 0xd4211fb0, /// 0xdd8
			 0xd5dd848f, /// 0xddc
			 0x4e3e12fa, /// 0xde0
			 0xa8be9b18, /// 0xde4
			 0x73b501c8, /// 0xde8
			 0x42f571fb, /// 0xdec
			 0x01798924, /// 0xdf0
			 0xf7bf4e3d, /// 0xdf4
			 0x6e3b67ba, /// 0xdf8
			 0x4ec096f4, /// 0xdfc
			 0x66ce62e2, /// 0xe00
			 0xa92c8c79, /// 0xe04
			 0x7a1b6ee2, /// 0xe08
			 0x3223add3, /// 0xe0c
			 0xda65643b, /// 0xe10
			 0x8569ee2f, /// 0xe14
			 0x64c27990, /// 0xe18
			 0xd35695f7, /// 0xe1c
			 0x3995d637, /// 0xe20
			 0x387b62aa, /// 0xe24
			 0x57cb04c6, /// 0xe28
			 0x35f59f44, /// 0xe2c
			 0x9b5552f4, /// 0xe30
			 0x9a8c8c42, /// 0xe34
			 0x7e39e032, /// 0xe38
			 0x941bdf49, /// 0xe3c
			 0xbdd4349f, /// 0xe40
			 0xa097f57a, /// 0xe44
			 0x9830b177, /// 0xe48
			 0x9c6dac86, /// 0xe4c
			 0x97471bd8, /// 0xe50
			 0xfc719d38, /// 0xe54
			 0xd90f98c5, /// 0xe58
			 0xf1b7c2e8, /// 0xe5c
			 0x25a55d1d, /// 0xe60
			 0xe943b5a0, /// 0xe64
			 0x87543bfb, /// 0xe68
			 0xa13834ca, /// 0xe6c
			 0x8b9e749d, /// 0xe70
			 0x035a3cc0, /// 0xe74
			 0xdb017e55, /// 0xe78
			 0xbb34f511, /// 0xe7c
			 0xede1974b, /// 0xe80
			 0xdd6609f5, /// 0xe84
			 0xa63e7b6f, /// 0xe88
			 0x53fda124, /// 0xe8c
			 0x22b79a08, /// 0xe90
			 0xe147d4b6, /// 0xe94
			 0x82733193, /// 0xe98
			 0x0fc9a1e5, /// 0xe9c
			 0xce4ae124, /// 0xea0
			 0x9eda7634, /// 0xea4
			 0xdad282aa, /// 0xea8
			 0x6a660f81, /// 0xeac
			 0x8ec59b4a, /// 0xeb0
			 0x53bcb6bb, /// 0xeb4
			 0x75f505b7, /// 0xeb8
			 0xe9ccbc85, /// 0xebc
			 0x82951555, /// 0xec0
			 0xa133b337, /// 0xec4
			 0x86b00dc5, /// 0xec8
			 0x6319a2d7, /// 0xecc
			 0x95f8ac1b, /// 0xed0
			 0x6ae20b69, /// 0xed4
			 0xf0006a78, /// 0xed8
			 0x810d1755, /// 0xedc
			 0x874d4dcb, /// 0xee0
			 0x69cfb81c, /// 0xee4
			 0x2187c64f, /// 0xee8
			 0x7cadd467, /// 0xeec
			 0x1b3710b0, /// 0xef0
			 0xfa11afa5, /// 0xef4
			 0xcfc57a9b, /// 0xef8
			 0xf35ea4a7, /// 0xefc
			 0xb6ede49f, /// 0xf00
			 0xac6b1911, /// 0xf04
			 0x70beabd4, /// 0xf08
			 0x8daac93e, /// 0xf0c
			 0x8475df5b, /// 0xf10
			 0x5e712318, /// 0xf14
			 0x5f92c9e1, /// 0xf18
			 0xbeda5acf, /// 0xf1c
			 0x4f3923c8, /// 0xf20
			 0xf5417896, /// 0xf24
			 0xf36a3878, /// 0xf28
			 0x5ae98c92, /// 0xf2c
			 0x0316abc9, /// 0xf30
			 0xd662985d, /// 0xf34
			 0x60dea657, /// 0xf38
			 0x15880b07, /// 0xf3c
			 0xb8376b19, /// 0xf40
			 0x1ee105e1, /// 0xf44
			 0x8ce1e625, /// 0xf48
			 0xb0a0d842, /// 0xf4c
			 0xe99ff0cc, /// 0xf50
			 0x9f6c06ab, /// 0xf54
			 0x31108221, /// 0xf58
			 0x7895d104, /// 0xf5c
			 0xf71655b5, /// 0xf60
			 0x354e53a2, /// 0xf64
			 0x0a7ef75c, /// 0xf68
			 0xf4846b25, /// 0xf6c
			 0xb52f3838, /// 0xf70
			 0x43612307, /// 0xf74
			 0xc60834a5, /// 0xf78
			 0xdf37dd1f, /// 0xf7c
			 0x868bd714, /// 0xf80
			 0x927902a9, /// 0xf84
			 0x6aecd917, /// 0xf88
			 0x52b17f1b, /// 0xf8c
			 0x0401ac75, /// 0xf90
			 0xb8b29e1f, /// 0xf94
			 0xefd64e8d, /// 0xf98
			 0x7a029367, /// 0xf9c
			 0x085bf124, /// 0xfa0
			 0x0f0b9bc7, /// 0xfa4
			 0x042b7428, /// 0xfa8
			 0x66c552f2, /// 0xfac
			 0x515a1a0c, /// 0xfb0
			 0x3be1c6ea, /// 0xfb4
			 0x4aa0e3e7, /// 0xfb8
			 0x4e19fdee, /// 0xfbc
			 0xf10b14f9, /// 0xfc0
			 0xed23109a, /// 0xfc4
			 0x61cf8094, /// 0xfc8
			 0x42a30343, /// 0xfcc
			 0x67831693, /// 0xfd0
			 0x1d6ce430, /// 0xfd4
			 0x6d23a50c, /// 0xfd8
			 0x4dfcecb4, /// 0xfdc
			 0xb610cada, /// 0xfe0
			 0x155fbe7d, /// 0xfe4
			 0x20ead43e, /// 0xfe8
			 0xd0444fcb, /// 0xfec
			 0x5f8d4775, /// 0xff0
			 0x660680c1, /// 0xff4
			 0x2512ec6c, /// 0xff8
			 0x430c051d /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0ba0c68f, /// 0x0
			 0xb34a2b9d, /// 0x4
			 0xbfa08f0e, /// 0x8
			 0xdd13b31a, /// 0xc
			 0xdc56ffcb, /// 0x10
			 0x081b4f4b, /// 0x14
			 0xf1ddd8f4, /// 0x18
			 0x6f5727df, /// 0x1c
			 0xacc25661, /// 0x20
			 0xc4a6f418, /// 0x24
			 0x1e13da04, /// 0x28
			 0x68f25c5b, /// 0x2c
			 0x517d9be0, /// 0x30
			 0xfa2e153b, /// 0x34
			 0x4572bcb4, /// 0x38
			 0x76eb3ef2, /// 0x3c
			 0x2474a32e, /// 0x40
			 0x3db2290e, /// 0x44
			 0x141efc08, /// 0x48
			 0x4dafb754, /// 0x4c
			 0x97eb0543, /// 0x50
			 0x62c9d9ba, /// 0x54
			 0x8dc03cdb, /// 0x58
			 0x097ac5a4, /// 0x5c
			 0xd3e03441, /// 0x60
			 0x6c38c8d4, /// 0x64
			 0x745ad6e1, /// 0x68
			 0x0f55e593, /// 0x6c
			 0x5cb22cd2, /// 0x70
			 0x0ba156df, /// 0x74
			 0xfe882daa, /// 0x78
			 0x28363af3, /// 0x7c
			 0x89daef0c, /// 0x80
			 0x801fe338, /// 0x84
			 0x5c6a2a34, /// 0x88
			 0x9e49d433, /// 0x8c
			 0x57a342ab, /// 0x90
			 0x7c24598a, /// 0x94
			 0xf5a690c6, /// 0x98
			 0xec9a9675, /// 0x9c
			 0x8b63f0d4, /// 0xa0
			 0x242b9d4b, /// 0xa4
			 0x755b3b22, /// 0xa8
			 0x0ff49885, /// 0xac
			 0x8f32ccbf, /// 0xb0
			 0x1f75a915, /// 0xb4
			 0x0ade23f2, /// 0xb8
			 0xebb78c0b, /// 0xbc
			 0xd65ecf90, /// 0xc0
			 0x1d0b2dbf, /// 0xc4
			 0x09031bf8, /// 0xc8
			 0x16845926, /// 0xcc
			 0x33b082ab, /// 0xd0
			 0x7b5ea4a9, /// 0xd4
			 0x6c85f893, /// 0xd8
			 0xd3ba093e, /// 0xdc
			 0xf1d2dee2, /// 0xe0
			 0xfcf2b66e, /// 0xe4
			 0x5ca0fbea, /// 0xe8
			 0x5c0e78f1, /// 0xec
			 0xf6f22b02, /// 0xf0
			 0xf7c730a8, /// 0xf4
			 0x65d2c005, /// 0xf8
			 0x79c8726a, /// 0xfc
			 0x09763725, /// 0x100
			 0xbde7df7e, /// 0x104
			 0xf5aa334d, /// 0x108
			 0x71dd20df, /// 0x10c
			 0xd9dacb0c, /// 0x110
			 0x5700b6fa, /// 0x114
			 0xbb59031c, /// 0x118
			 0x1d43f7bd, /// 0x11c
			 0xf8f3aeac, /// 0x120
			 0xb2a0540c, /// 0x124
			 0xc0c43006, /// 0x128
			 0x91d69040, /// 0x12c
			 0x968cf191, /// 0x130
			 0xe10339ce, /// 0x134
			 0x2fe4891f, /// 0x138
			 0xb1ea6969, /// 0x13c
			 0xf87969e5, /// 0x140
			 0x6ebc719e, /// 0x144
			 0xd5b244b1, /// 0x148
			 0xdfe8dcd9, /// 0x14c
			 0x150d03db, /// 0x150
			 0x2fd0c56f, /// 0x154
			 0xdb5d889a, /// 0x158
			 0x39db0198, /// 0x15c
			 0x0ee801b3, /// 0x160
			 0x261c9c10, /// 0x164
			 0x9df9df64, /// 0x168
			 0xe48fe7ee, /// 0x16c
			 0xaa0fbeb7, /// 0x170
			 0x88bcdd57, /// 0x174
			 0x07fdfb66, /// 0x178
			 0x5f484a70, /// 0x17c
			 0x7ab81b13, /// 0x180
			 0x4e7f29d8, /// 0x184
			 0x415eca4f, /// 0x188
			 0x939b779e, /// 0x18c
			 0xaa6911cb, /// 0x190
			 0x46473d06, /// 0x194
			 0x65a47cc3, /// 0x198
			 0xf131738f, /// 0x19c
			 0x794a7884, /// 0x1a0
			 0x931fe1c9, /// 0x1a4
			 0x61fce4b9, /// 0x1a8
			 0x3fd49abd, /// 0x1ac
			 0x7aa46d9a, /// 0x1b0
			 0x0a7b28cc, /// 0x1b4
			 0x2db5b8bd, /// 0x1b8
			 0x0a287777, /// 0x1bc
			 0x09b73a3a, /// 0x1c0
			 0xbcfa2b92, /// 0x1c4
			 0x105fa432, /// 0x1c8
			 0xfbf5667e, /// 0x1cc
			 0x414782ea, /// 0x1d0
			 0xafd0d609, /// 0x1d4
			 0xfc72a2e2, /// 0x1d8
			 0xb6de6c7a, /// 0x1dc
			 0x32cceb3d, /// 0x1e0
			 0xa36657fe, /// 0x1e4
			 0xa8453881, /// 0x1e8
			 0xd77db8b2, /// 0x1ec
			 0x679d3d39, /// 0x1f0
			 0xacebf43d, /// 0x1f4
			 0x74eb19ec, /// 0x1f8
			 0x120a2778, /// 0x1fc
			 0x58f9d701, /// 0x200
			 0x75ba055e, /// 0x204
			 0xd53a14e3, /// 0x208
			 0xe56dbc72, /// 0x20c
			 0xee06797b, /// 0x210
			 0xf450d6d3, /// 0x214
			 0x14411a39, /// 0x218
			 0x5a18d17f, /// 0x21c
			 0x966ef35d, /// 0x220
			 0x9a0a6cdb, /// 0x224
			 0xf1a8fe10, /// 0x228
			 0x148f2e34, /// 0x22c
			 0xc2bf07a2, /// 0x230
			 0xc2bf16b0, /// 0x234
			 0xcdc5d106, /// 0x238
			 0x6c93a00c, /// 0x23c
			 0x5adaf89d, /// 0x240
			 0x67a5da09, /// 0x244
			 0xd315af0e, /// 0x248
			 0xdca46568, /// 0x24c
			 0x3673c0d2, /// 0x250
			 0x8d0bee49, /// 0x254
			 0xd488a728, /// 0x258
			 0x726a4a16, /// 0x25c
			 0xc6500e94, /// 0x260
			 0x71f9482c, /// 0x264
			 0x18fcca5a, /// 0x268
			 0x6a4da930, /// 0x26c
			 0x3cddc1cc, /// 0x270
			 0xc98a63aa, /// 0x274
			 0xda432f1c, /// 0x278
			 0xa335413b, /// 0x27c
			 0xa320ea78, /// 0x280
			 0x5ded0e1e, /// 0x284
			 0x32c44adc, /// 0x288
			 0xd8733b55, /// 0x28c
			 0x826d1e6f, /// 0x290
			 0x7ff3daa1, /// 0x294
			 0xfe614811, /// 0x298
			 0x72b69238, /// 0x29c
			 0xded763fa, /// 0x2a0
			 0x6444c754, /// 0x2a4
			 0xb590cdb0, /// 0x2a8
			 0x8c029af7, /// 0x2ac
			 0x00bd78d8, /// 0x2b0
			 0x3a36ed22, /// 0x2b4
			 0x44985c25, /// 0x2b8
			 0xf262cf4d, /// 0x2bc
			 0xf34606f5, /// 0x2c0
			 0xb5ed2e4f, /// 0x2c4
			 0xa1ec2223, /// 0x2c8
			 0x28e0187a, /// 0x2cc
			 0xab4c5a4d, /// 0x2d0
			 0x8b485520, /// 0x2d4
			 0x566eeee2, /// 0x2d8
			 0x0934ed2b, /// 0x2dc
			 0x5e6a7a77, /// 0x2e0
			 0x155098c5, /// 0x2e4
			 0x909835ef, /// 0x2e8
			 0x1bcdc025, /// 0x2ec
			 0x927ba97d, /// 0x2f0
			 0x7a524ca9, /// 0x2f4
			 0xfb111b77, /// 0x2f8
			 0x8e1f8ee3, /// 0x2fc
			 0x86d9e09f, /// 0x300
			 0x8873c598, /// 0x304
			 0xa112a22c, /// 0x308
			 0x29573b70, /// 0x30c
			 0x06f60f8d, /// 0x310
			 0x004470e4, /// 0x314
			 0xe8c81acb, /// 0x318
			 0x9892b8ba, /// 0x31c
			 0x0b231904, /// 0x320
			 0xf8f79d1f, /// 0x324
			 0xfa38510a, /// 0x328
			 0x2fe06e3d, /// 0x32c
			 0x922a2ee4, /// 0x330
			 0x8f83e56b, /// 0x334
			 0x6df10405, /// 0x338
			 0x2ad7af72, /// 0x33c
			 0x83098dd4, /// 0x340
			 0xbce019e2, /// 0x344
			 0x58471d76, /// 0x348
			 0xcf13ba3b, /// 0x34c
			 0x7bded1b4, /// 0x350
			 0x59cd3309, /// 0x354
			 0xc88e91cf, /// 0x358
			 0x506361ce, /// 0x35c
			 0x63230283, /// 0x360
			 0xb715a279, /// 0x364
			 0x765954cb, /// 0x368
			 0xb3aa54c3, /// 0x36c
			 0xc36c3634, /// 0x370
			 0x4c139af7, /// 0x374
			 0x3f9fd6cb, /// 0x378
			 0xd2cdc856, /// 0x37c
			 0xbb524896, /// 0x380
			 0xae2b6c4e, /// 0x384
			 0x5888cfa6, /// 0x388
			 0x5054c925, /// 0x38c
			 0x504f4e1f, /// 0x390
			 0xec2b383f, /// 0x394
			 0x10c29c52, /// 0x398
			 0xa036e078, /// 0x39c
			 0xa7ad4d47, /// 0x3a0
			 0x8a948508, /// 0x3a4
			 0xf2cedfe7, /// 0x3a8
			 0x5721554c, /// 0x3ac
			 0xf64f3e22, /// 0x3b0
			 0xcdf082c5, /// 0x3b4
			 0x5ca4e10d, /// 0x3b8
			 0x536da6b2, /// 0x3bc
			 0xbcc1727d, /// 0x3c0
			 0xeaf042ec, /// 0x3c4
			 0xbbc9f701, /// 0x3c8
			 0x5cd90ffd, /// 0x3cc
			 0x3bef7b98, /// 0x3d0
			 0x23bad5a6, /// 0x3d4
			 0x899e3f77, /// 0x3d8
			 0x0759f110, /// 0x3dc
			 0x2d4000b5, /// 0x3e0
			 0x11c71a3f, /// 0x3e4
			 0x29483f2b, /// 0x3e8
			 0x64548640, /// 0x3ec
			 0x716775ba, /// 0x3f0
			 0x5f465eda, /// 0x3f4
			 0xe97bdb6e, /// 0x3f8
			 0x7f57f753, /// 0x3fc
			 0x66ab6f6b, /// 0x400
			 0x3ac7473b, /// 0x404
			 0x16c4d0e6, /// 0x408
			 0x442b2fbd, /// 0x40c
			 0xf133505a, /// 0x410
			 0xd95b1d48, /// 0x414
			 0x176af4a2, /// 0x418
			 0x2a5334c3, /// 0x41c
			 0x9a5bfa8d, /// 0x420
			 0xb5174b90, /// 0x424
			 0x190696f6, /// 0x428
			 0x59f231b2, /// 0x42c
			 0x49e4de15, /// 0x430
			 0x959f3a8c, /// 0x434
			 0x705a6674, /// 0x438
			 0xe97142a2, /// 0x43c
			 0xdeb22d0f, /// 0x440
			 0x67c16b1a, /// 0x444
			 0x14e008fd, /// 0x448
			 0x0d3b1bcf, /// 0x44c
			 0x581ee4e2, /// 0x450
			 0x1c95c71a, /// 0x454
			 0x1104d618, /// 0x458
			 0xdc77d1ec, /// 0x45c
			 0xe9925dc2, /// 0x460
			 0x50e9bcce, /// 0x464
			 0x194f3c0d, /// 0x468
			 0xee856cb7, /// 0x46c
			 0xc8c5d4fb, /// 0x470
			 0x9bfa1a4a, /// 0x474
			 0x817794de, /// 0x478
			 0x94caa3ca, /// 0x47c
			 0xb42ae461, /// 0x480
			 0x78f57b5d, /// 0x484
			 0x12694a50, /// 0x488
			 0x89c6a9d2, /// 0x48c
			 0x3581742b, /// 0x490
			 0xaa2b25c7, /// 0x494
			 0xa895fcfd, /// 0x498
			 0xdb5c07e2, /// 0x49c
			 0x09f2afef, /// 0x4a0
			 0xd1656af4, /// 0x4a4
			 0xe3765075, /// 0x4a8
			 0x6d34865e, /// 0x4ac
			 0xb6b970a9, /// 0x4b0
			 0x1f0afbff, /// 0x4b4
			 0xb4afedd1, /// 0x4b8
			 0xb8144d38, /// 0x4bc
			 0xfe8ea9a6, /// 0x4c0
			 0xbc71c804, /// 0x4c4
			 0x7bf2599b, /// 0x4c8
			 0xf345a541, /// 0x4cc
			 0x6b0ed6d6, /// 0x4d0
			 0xff690975, /// 0x4d4
			 0x2895fecd, /// 0x4d8
			 0xf4e23497, /// 0x4dc
			 0xdf85af31, /// 0x4e0
			 0x7fd180f6, /// 0x4e4
			 0x1a1b067a, /// 0x4e8
			 0xaec7966f, /// 0x4ec
			 0xf6a2e839, /// 0x4f0
			 0x8b3de16b, /// 0x4f4
			 0x6341fe4f, /// 0x4f8
			 0x089975a1, /// 0x4fc
			 0xce3ece45, /// 0x500
			 0xbe2087a7, /// 0x504
			 0x6bb50f8b, /// 0x508
			 0x78c0910f, /// 0x50c
			 0xb5240e1f, /// 0x510
			 0x02e87e52, /// 0x514
			 0xfb0a3565, /// 0x518
			 0xcb90ca09, /// 0x51c
			 0x65cefb7e, /// 0x520
			 0x27e072bd, /// 0x524
			 0xad370afe, /// 0x528
			 0xe831e1ee, /// 0x52c
			 0xa7ad94f4, /// 0x530
			 0x9957c59e, /// 0x534
			 0x534fedd2, /// 0x538
			 0x19af8f12, /// 0x53c
			 0x8ea3c42f, /// 0x540
			 0xc1ca8a41, /// 0x544
			 0x20f8aa46, /// 0x548
			 0x97b81f5a, /// 0x54c
			 0x6784e5e3, /// 0x550
			 0xdc708f30, /// 0x554
			 0x8968ff52, /// 0x558
			 0x068de88d, /// 0x55c
			 0xc8ffdbb3, /// 0x560
			 0x2efcc6ed, /// 0x564
			 0xe3ede1a6, /// 0x568
			 0xdf2de052, /// 0x56c
			 0x7c2197e1, /// 0x570
			 0xa8d2a844, /// 0x574
			 0x0d7ced98, /// 0x578
			 0x24898d40, /// 0x57c
			 0xbfc8fc7b, /// 0x580
			 0x98bfd381, /// 0x584
			 0x11ffd511, /// 0x588
			 0xbb003826, /// 0x58c
			 0x5b700c0c, /// 0x590
			 0x4507c54b, /// 0x594
			 0x5957b99e, /// 0x598
			 0x45b51050, /// 0x59c
			 0x1e4e30aa, /// 0x5a0
			 0x1cb5b717, /// 0x5a4
			 0xcf52354c, /// 0x5a8
			 0x65d230e7, /// 0x5ac
			 0xe2357378, /// 0x5b0
			 0x05f0ad60, /// 0x5b4
			 0x92ee0221, /// 0x5b8
			 0x167ec040, /// 0x5bc
			 0x9078d5c5, /// 0x5c0
			 0x6bdbec45, /// 0x5c4
			 0x2f57eaa9, /// 0x5c8
			 0xf5348124, /// 0x5cc
			 0x23e4f5d7, /// 0x5d0
			 0xaf7b6793, /// 0x5d4
			 0x3a7a861c, /// 0x5d8
			 0xff419362, /// 0x5dc
			 0x1cf1dd7b, /// 0x5e0
			 0xffcd1858, /// 0x5e4
			 0xa337c1a6, /// 0x5e8
			 0xd705a83b, /// 0x5ec
			 0xb5a6d25b, /// 0x5f0
			 0xf1a6c79e, /// 0x5f4
			 0x3f027624, /// 0x5f8
			 0x364c0f76, /// 0x5fc
			 0x90fd292d, /// 0x600
			 0x618eeb01, /// 0x604
			 0x108f8b9d, /// 0x608
			 0x1b4e73c7, /// 0x60c
			 0x53c18e05, /// 0x610
			 0xed0ee52e, /// 0x614
			 0x29bfe242, /// 0x618
			 0xecf9cc2f, /// 0x61c
			 0xe7d47d6d, /// 0x620
			 0x5daf41de, /// 0x624
			 0xfd958967, /// 0x628
			 0xbe36aa20, /// 0x62c
			 0x8613b718, /// 0x630
			 0x09172748, /// 0x634
			 0x9c65ad60, /// 0x638
			 0x784a693d, /// 0x63c
			 0x073d85b2, /// 0x640
			 0xf36a2a21, /// 0x644
			 0x15dfeff8, /// 0x648
			 0xa3259f73, /// 0x64c
			 0x1661c75f, /// 0x650
			 0x6d47ea25, /// 0x654
			 0x5e509dbf, /// 0x658
			 0x04a88ef9, /// 0x65c
			 0x070f1ae1, /// 0x660
			 0x232f43d7, /// 0x664
			 0x7674ed1f, /// 0x668
			 0x0b73012d, /// 0x66c
			 0xc046d1b7, /// 0x670
			 0x620c1f96, /// 0x674
			 0x44b75704, /// 0x678
			 0xa0d66073, /// 0x67c
			 0x9b1b2aa3, /// 0x680
			 0xd5451bdc, /// 0x684
			 0xf7b8eb77, /// 0x688
			 0x3a5d0e27, /// 0x68c
			 0x39f4233c, /// 0x690
			 0x544d8397, /// 0x694
			 0x57cd06d9, /// 0x698
			 0x8842d5ee, /// 0x69c
			 0xe5f0f36b, /// 0x6a0
			 0xdaf82bfa, /// 0x6a4
			 0xabfa9319, /// 0x6a8
			 0x7af1d77a, /// 0x6ac
			 0x181d1d03, /// 0x6b0
			 0x38aa688e, /// 0x6b4
			 0x6ff8c035, /// 0x6b8
			 0x22d8b384, /// 0x6bc
			 0x6edbcbe2, /// 0x6c0
			 0x61e1746b, /// 0x6c4
			 0xe28105a3, /// 0x6c8
			 0x1a187084, /// 0x6cc
			 0xe9d93b22, /// 0x6d0
			 0xe7109498, /// 0x6d4
			 0x2cdcbaee, /// 0x6d8
			 0x4cb5fbf0, /// 0x6dc
			 0x74d716b6, /// 0x6e0
			 0x86a4dfeb, /// 0x6e4
			 0x1c13f709, /// 0x6e8
			 0x6f96ef27, /// 0x6ec
			 0x125f7c50, /// 0x6f0
			 0x1670171a, /// 0x6f4
			 0x4ec21878, /// 0x6f8
			 0xb6386951, /// 0x6fc
			 0x0ca50af5, /// 0x700
			 0x2f5cafc0, /// 0x704
			 0xd494eced, /// 0x708
			 0x08706607, /// 0x70c
			 0xae75029f, /// 0x710
			 0xe4812a87, /// 0x714
			 0x6a52db0c, /// 0x718
			 0x57317bbe, /// 0x71c
			 0x75dee6f6, /// 0x720
			 0x2ecc36a1, /// 0x724
			 0xcb0f06fc, /// 0x728
			 0x9aadc54c, /// 0x72c
			 0x45f4e5ca, /// 0x730
			 0xf5870f1c, /// 0x734
			 0x3398a46f, /// 0x738
			 0x7d8d874e, /// 0x73c
			 0x8a8bdfb0, /// 0x740
			 0x81f1e5cb, /// 0x744
			 0x4043c356, /// 0x748
			 0xbcba2f7d, /// 0x74c
			 0xf6963913, /// 0x750
			 0x2c11aea8, /// 0x754
			 0x80ffcb54, /// 0x758
			 0x980f37b6, /// 0x75c
			 0x8081628a, /// 0x760
			 0xe7df941e, /// 0x764
			 0x71aa1a41, /// 0x768
			 0x7e9eefa6, /// 0x76c
			 0xf85e0217, /// 0x770
			 0xdaa1004a, /// 0x774
			 0xff290cc2, /// 0x778
			 0x10a3b8bf, /// 0x77c
			 0x39195a36, /// 0x780
			 0x85c643d5, /// 0x784
			 0x508fe8df, /// 0x788
			 0x317e7abc, /// 0x78c
			 0xbe90171d, /// 0x790
			 0xfa6f2afc, /// 0x794
			 0x77f7f08c, /// 0x798
			 0xe9b3d205, /// 0x79c
			 0x697a58bd, /// 0x7a0
			 0x3386274e, /// 0x7a4
			 0x3f554937, /// 0x7a8
			 0x54edd763, /// 0x7ac
			 0xc4642d0c, /// 0x7b0
			 0xbbde75ba, /// 0x7b4
			 0x16c83c98, /// 0x7b8
			 0xc2e56561, /// 0x7bc
			 0xa8b5282b, /// 0x7c0
			 0xc293435d, /// 0x7c4
			 0x7abcf34d, /// 0x7c8
			 0x19119e4b, /// 0x7cc
			 0x83174bdc, /// 0x7d0
			 0x9e3c2c95, /// 0x7d4
			 0xb4abbac2, /// 0x7d8
			 0xab183bce, /// 0x7dc
			 0xa7ba04b5, /// 0x7e0
			 0x33d440a2, /// 0x7e4
			 0xd6020a41, /// 0x7e8
			 0x9a251430, /// 0x7ec
			 0x47ff857d, /// 0x7f0
			 0xd7ef5660, /// 0x7f4
			 0x9f4b707c, /// 0x7f8
			 0x4257de73, /// 0x7fc
			 0x3e554c3d, /// 0x800
			 0x529580e8, /// 0x804
			 0x569b51ab, /// 0x808
			 0x419bd275, /// 0x80c
			 0x3eca2a91, /// 0x810
			 0x2e78e45a, /// 0x814
			 0x809207e2, /// 0x818
			 0x139d2e49, /// 0x81c
			 0x41d9e129, /// 0x820
			 0x1b93bf3c, /// 0x824
			 0x0805b888, /// 0x828
			 0xa1a0bde3, /// 0x82c
			 0xcc414df1, /// 0x830
			 0x4e4f2656, /// 0x834
			 0x9a9209ac, /// 0x838
			 0x81ded7fc, /// 0x83c
			 0xeed5bed6, /// 0x840
			 0x7cbad6ff, /// 0x844
			 0x71087e27, /// 0x848
			 0xdb8b37d2, /// 0x84c
			 0x227c5457, /// 0x850
			 0xc9a28674, /// 0x854
			 0xfaa0c782, /// 0x858
			 0x09400606, /// 0x85c
			 0x32a7cc81, /// 0x860
			 0xb6981168, /// 0x864
			 0x5f499107, /// 0x868
			 0xc83c42a9, /// 0x86c
			 0x058c7596, /// 0x870
			 0xd5e8cc36, /// 0x874
			 0x6ad53487, /// 0x878
			 0xe8923e45, /// 0x87c
			 0x6e19204f, /// 0x880
			 0x3a279f68, /// 0x884
			 0x54747e82, /// 0x888
			 0x0a2e30eb, /// 0x88c
			 0x7715fbd4, /// 0x890
			 0x30be1baa, /// 0x894
			 0x1c18d1ef, /// 0x898
			 0x6b611b88, /// 0x89c
			 0xfd99da24, /// 0x8a0
			 0xd3a5fc65, /// 0x8a4
			 0x1a0a55c4, /// 0x8a8
			 0x95e411d4, /// 0x8ac
			 0xb05b8fe6, /// 0x8b0
			 0x8987cbc3, /// 0x8b4
			 0x7066a234, /// 0x8b8
			 0xdf6ac91d, /// 0x8bc
			 0xe5f2c932, /// 0x8c0
			 0x989bb9f1, /// 0x8c4
			 0xd6e8f219, /// 0x8c8
			 0x4691fec8, /// 0x8cc
			 0x38b48bd4, /// 0x8d0
			 0x421fd253, /// 0x8d4
			 0xd1a03f30, /// 0x8d8
			 0x4f510376, /// 0x8dc
			 0x2d3c7529, /// 0x8e0
			 0x5c88d10a, /// 0x8e4
			 0xa7497b5e, /// 0x8e8
			 0x3c34e63a, /// 0x8ec
			 0xf425156c, /// 0x8f0
			 0x727d7b94, /// 0x8f4
			 0xd58623e1, /// 0x8f8
			 0x3f99a67f, /// 0x8fc
			 0x7d1e514b, /// 0x900
			 0x02c7fd95, /// 0x904
			 0x69cf51c5, /// 0x908
			 0xf73eb06c, /// 0x90c
			 0xbce5c156, /// 0x910
			 0x487ddab1, /// 0x914
			 0xc565b138, /// 0x918
			 0x3b9f7009, /// 0x91c
			 0x453454e0, /// 0x920
			 0x36854a9b, /// 0x924
			 0x46c468e3, /// 0x928
			 0x1be0c9cd, /// 0x92c
			 0x7a0cf543, /// 0x930
			 0xa343f1a6, /// 0x934
			 0x36a80632, /// 0x938
			 0xfbacdec0, /// 0x93c
			 0xb5282d75, /// 0x940
			 0xbb511166, /// 0x944
			 0x976d0cb2, /// 0x948
			 0x17866fdc, /// 0x94c
			 0x61261103, /// 0x950
			 0xa0ba1971, /// 0x954
			 0x833e1cb4, /// 0x958
			 0x91aec756, /// 0x95c
			 0xe8f69072, /// 0x960
			 0xe59a51f2, /// 0x964
			 0x173ac4c2, /// 0x968
			 0x3c619419, /// 0x96c
			 0x005617b5, /// 0x970
			 0xfd132930, /// 0x974
			 0xcead9713, /// 0x978
			 0xe1151b04, /// 0x97c
			 0xc221a692, /// 0x980
			 0x115dae16, /// 0x984
			 0xc80c5c6f, /// 0x988
			 0x828eaaec, /// 0x98c
			 0xfdcb1324, /// 0x990
			 0xf6bbed4c, /// 0x994
			 0x8d88174b, /// 0x998
			 0x6f80dcd1, /// 0x99c
			 0xec532d5e, /// 0x9a0
			 0x6d08ee7c, /// 0x9a4
			 0x2d77a798, /// 0x9a8
			 0x1305a238, /// 0x9ac
			 0xf34ed4e0, /// 0x9b0
			 0xc04fcaaa, /// 0x9b4
			 0x7db80736, /// 0x9b8
			 0x27c8bf27, /// 0x9bc
			 0x2ba256fd, /// 0x9c0
			 0x821f136c, /// 0x9c4
			 0xe282cc40, /// 0x9c8
			 0x442211c5, /// 0x9cc
			 0x800a999b, /// 0x9d0
			 0x60a23c72, /// 0x9d4
			 0xc78eb52b, /// 0x9d8
			 0x4451c730, /// 0x9dc
			 0x8845f16c, /// 0x9e0
			 0x72770366, /// 0x9e4
			 0x3e440dfb, /// 0x9e8
			 0x36eef610, /// 0x9ec
			 0x6d1a5099, /// 0x9f0
			 0x7c16c67c, /// 0x9f4
			 0xcc6f2776, /// 0x9f8
			 0xe5449a5d, /// 0x9fc
			 0x92357b95, /// 0xa00
			 0xfc7ed3e8, /// 0xa04
			 0xa6b8ea32, /// 0xa08
			 0x0ee18cb3, /// 0xa0c
			 0x514d9b08, /// 0xa10
			 0xac6ea4e1, /// 0xa14
			 0x3f02c497, /// 0xa18
			 0x5f13c257, /// 0xa1c
			 0x1d2ecb4c, /// 0xa20
			 0x40ce4cdc, /// 0xa24
			 0x0d7145ec, /// 0xa28
			 0x89d1f68a, /// 0xa2c
			 0xd94d1323, /// 0xa30
			 0x1f64dece, /// 0xa34
			 0x5a3d25f1, /// 0xa38
			 0x83db504d, /// 0xa3c
			 0x92178193, /// 0xa40
			 0xc51cca88, /// 0xa44
			 0xfc0fe5e5, /// 0xa48
			 0xcb744428, /// 0xa4c
			 0xccd45cf6, /// 0xa50
			 0x045a3079, /// 0xa54
			 0x2fbde712, /// 0xa58
			 0xbf699150, /// 0xa5c
			 0x5533fc9e, /// 0xa60
			 0x718991c8, /// 0xa64
			 0x5368b32b, /// 0xa68
			 0x21c58241, /// 0xa6c
			 0xa34a6a90, /// 0xa70
			 0x05f1fde4, /// 0xa74
			 0x1a6002ce, /// 0xa78
			 0xf5496a1b, /// 0xa7c
			 0xdf3bc75a, /// 0xa80
			 0x8ed9883b, /// 0xa84
			 0xda9970b8, /// 0xa88
			 0xea95e970, /// 0xa8c
			 0xe7106231, /// 0xa90
			 0xa61620f4, /// 0xa94
			 0xf6349b8f, /// 0xa98
			 0x5d4c8fde, /// 0xa9c
			 0x361c47d2, /// 0xaa0
			 0xfbe35620, /// 0xaa4
			 0xcaec4ff1, /// 0xaa8
			 0x3fbf7295, /// 0xaac
			 0xe0704dd8, /// 0xab0
			 0x8ef9f5f7, /// 0xab4
			 0x6d19cf77, /// 0xab8
			 0x6ca9f44c, /// 0xabc
			 0xcba7f36b, /// 0xac0
			 0xb2fba509, /// 0xac4
			 0xbbe23370, /// 0xac8
			 0xfdeba590, /// 0xacc
			 0xa0dd4853, /// 0xad0
			 0xb0c932ff, /// 0xad4
			 0x3677a643, /// 0xad8
			 0x2ad29cdb, /// 0xadc
			 0x6fb893d3, /// 0xae0
			 0x940438f1, /// 0xae4
			 0x9a4d3cb4, /// 0xae8
			 0xe7e909ed, /// 0xaec
			 0x45085769, /// 0xaf0
			 0x4177e736, /// 0xaf4
			 0x7e536006, /// 0xaf8
			 0x736658ae, /// 0xafc
			 0x339dce78, /// 0xb00
			 0xa8e7ce4e, /// 0xb04
			 0x17fbe011, /// 0xb08
			 0x3c45b1d0, /// 0xb0c
			 0x931910d8, /// 0xb10
			 0x8fe308d5, /// 0xb14
			 0x78e1b8f0, /// 0xb18
			 0x2cfdc8fe, /// 0xb1c
			 0x8e4fceac, /// 0xb20
			 0x7eef6680, /// 0xb24
			 0x5814811c, /// 0xb28
			 0x0cb3a6fd, /// 0xb2c
			 0x7f85fb0a, /// 0xb30
			 0x76a5a5dc, /// 0xb34
			 0x976a3f2a, /// 0xb38
			 0x7abef150, /// 0xb3c
			 0xed479652, /// 0xb40
			 0xa5aade2d, /// 0xb44
			 0xbe8aa71a, /// 0xb48
			 0x0b66a9ca, /// 0xb4c
			 0x6c2d91aa, /// 0xb50
			 0x1334ba2b, /// 0xb54
			 0xa2b8dc02, /// 0xb58
			 0xca42c5d2, /// 0xb5c
			 0x5289feba, /// 0xb60
			 0xda757422, /// 0xb64
			 0x1d512c4d, /// 0xb68
			 0x0cbc3f7c, /// 0xb6c
			 0x2af9bf68, /// 0xb70
			 0x5bf9b178, /// 0xb74
			 0xd0c0b768, /// 0xb78
			 0x276668df, /// 0xb7c
			 0xc28d9c88, /// 0xb80
			 0x105acf93, /// 0xb84
			 0x88209419, /// 0xb88
			 0x7b3394b9, /// 0xb8c
			 0x174ae915, /// 0xb90
			 0xc739529a, /// 0xb94
			 0xae78528c, /// 0xb98
			 0x62b0242a, /// 0xb9c
			 0xeaca907b, /// 0xba0
			 0xce2b1d96, /// 0xba4
			 0xe9ca3f12, /// 0xba8
			 0xbc5f913e, /// 0xbac
			 0x8bffa403, /// 0xbb0
			 0x3c1cdc65, /// 0xbb4
			 0x263ed48f, /// 0xbb8
			 0x065f9d23, /// 0xbbc
			 0x6dba9015, /// 0xbc0
			 0xffbf0026, /// 0xbc4
			 0xc5fe103a, /// 0xbc8
			 0x736e4364, /// 0xbcc
			 0x79a7de2f, /// 0xbd0
			 0x183c58c9, /// 0xbd4
			 0xc91a66d8, /// 0xbd8
			 0x43032931, /// 0xbdc
			 0x0f63984c, /// 0xbe0
			 0x72f0adf3, /// 0xbe4
			 0x45d1231f, /// 0xbe8
			 0x6fe9b1dc, /// 0xbec
			 0x57c6930a, /// 0xbf0
			 0x50015115, /// 0xbf4
			 0x7042a98b, /// 0xbf8
			 0xef13e793, /// 0xbfc
			 0x2343a103, /// 0xc00
			 0x29ee9ff7, /// 0xc04
			 0x36bb22ef, /// 0xc08
			 0x787ca64f, /// 0xc0c
			 0x7da448f1, /// 0xc10
			 0x7928550e, /// 0xc14
			 0x12bf8178, /// 0xc18
			 0x543514b5, /// 0xc1c
			 0xd4242736, /// 0xc20
			 0xff3b689d, /// 0xc24
			 0x4c90ce3a, /// 0xc28
			 0xc7d3ce9f, /// 0xc2c
			 0x750fc481, /// 0xc30
			 0x420fb851, /// 0xc34
			 0x55a5ebbd, /// 0xc38
			 0x295dc416, /// 0xc3c
			 0x7432b954, /// 0xc40
			 0xe2e9e0d6, /// 0xc44
			 0x6a32a241, /// 0xc48
			 0x4a68772d, /// 0xc4c
			 0x07d6470f, /// 0xc50
			 0x126966aa, /// 0xc54
			 0x43c99bcb, /// 0xc58
			 0x2017f79f, /// 0xc5c
			 0xfae3fb1d, /// 0xc60
			 0xb4a4fa87, /// 0xc64
			 0xbc938086, /// 0xc68
			 0x98471c6e, /// 0xc6c
			 0xc6bff56e, /// 0xc70
			 0xde29d6b4, /// 0xc74
			 0x247ecfe0, /// 0xc78
			 0xa97fd0c6, /// 0xc7c
			 0x79d3d6a5, /// 0xc80
			 0xfbee7367, /// 0xc84
			 0x75b9b2b9, /// 0xc88
			 0x775c710c, /// 0xc8c
			 0x952b80d1, /// 0xc90
			 0xaa5ea4ac, /// 0xc94
			 0xe2a2121c, /// 0xc98
			 0x891e89dd, /// 0xc9c
			 0x20604207, /// 0xca0
			 0x7a53143a, /// 0xca4
			 0x440437f1, /// 0xca8
			 0xe357d132, /// 0xcac
			 0xad72f4ff, /// 0xcb0
			 0x8c5fe7cd, /// 0xcb4
			 0x03975346, /// 0xcb8
			 0xea36f3fd, /// 0xcbc
			 0x4be0f0ec, /// 0xcc0
			 0x0840022c, /// 0xcc4
			 0xa03c3082, /// 0xcc8
			 0x66828004, /// 0xccc
			 0x65c1192d, /// 0xcd0
			 0x884b0673, /// 0xcd4
			 0x0f460537, /// 0xcd8
			 0x70f9c85b, /// 0xcdc
			 0x1b085feb, /// 0xce0
			 0x6fb747cb, /// 0xce4
			 0x61716bfe, /// 0xce8
			 0xb144b578, /// 0xcec
			 0x775fa166, /// 0xcf0
			 0xcbb4e1ce, /// 0xcf4
			 0xe4cb4bf7, /// 0xcf8
			 0x200ac058, /// 0xcfc
			 0x0abd06a2, /// 0xd00
			 0x968b1227, /// 0xd04
			 0x601cfcfc, /// 0xd08
			 0xa39460da, /// 0xd0c
			 0x6d279cb6, /// 0xd10
			 0xe2531813, /// 0xd14
			 0xec00e9e9, /// 0xd18
			 0x1249ac01, /// 0xd1c
			 0x934d6b23, /// 0xd20
			 0xac65dba4, /// 0xd24
			 0xa3344d30, /// 0xd28
			 0x7f83b2bf, /// 0xd2c
			 0xe5893c57, /// 0xd30
			 0x7dbd8bfa, /// 0xd34
			 0x85f9d288, /// 0xd38
			 0x5b0862e0, /// 0xd3c
			 0x529d18d0, /// 0xd40
			 0x445d6faf, /// 0xd44
			 0xf555e92b, /// 0xd48
			 0xa0eee094, /// 0xd4c
			 0x1e4d6918, /// 0xd50
			 0x070a639a, /// 0xd54
			 0x1f92579d, /// 0xd58
			 0x47356851, /// 0xd5c
			 0x7782cb02, /// 0xd60
			 0x554fc4b6, /// 0xd64
			 0xd35b7112, /// 0xd68
			 0xab249575, /// 0xd6c
			 0x64eb324f, /// 0xd70
			 0xcd5d4c40, /// 0xd74
			 0x6e2b27e4, /// 0xd78
			 0x3cc6e99d, /// 0xd7c
			 0x0a1c4c7c, /// 0xd80
			 0x5b1ae6e9, /// 0xd84
			 0x76bc0583, /// 0xd88
			 0xa7871ca4, /// 0xd8c
			 0x8c8f8e0d, /// 0xd90
			 0x8d0b4a3f, /// 0xd94
			 0x5955dde9, /// 0xd98
			 0xebbcbda8, /// 0xd9c
			 0x0681f02d, /// 0xda0
			 0xcbe66eae, /// 0xda4
			 0x8f5d89ba, /// 0xda8
			 0xf5648ee5, /// 0xdac
			 0x58e602be, /// 0xdb0
			 0x61dc52b6, /// 0xdb4
			 0x2afe43f5, /// 0xdb8
			 0xbae46e4e, /// 0xdbc
			 0x9ebcfb37, /// 0xdc0
			 0xb7d1becd, /// 0xdc4
			 0x1e8322bc, /// 0xdc8
			 0x4572f03a, /// 0xdcc
			 0x3aeea14b, /// 0xdd0
			 0xad07e027, /// 0xdd4
			 0x64cda4e2, /// 0xdd8
			 0x880b2e32, /// 0xddc
			 0xb2b12f93, /// 0xde0
			 0x184b5c45, /// 0xde4
			 0x549f3beb, /// 0xde8
			 0xb1744aa2, /// 0xdec
			 0x0c7c8bb2, /// 0xdf0
			 0x39d4fbb4, /// 0xdf4
			 0x49076f38, /// 0xdf8
			 0x48697352, /// 0xdfc
			 0x1cff9d66, /// 0xe00
			 0xa152ff43, /// 0xe04
			 0x8e9f2d50, /// 0xe08
			 0xec153d67, /// 0xe0c
			 0x80414f75, /// 0xe10
			 0xb2423dc1, /// 0xe14
			 0x2df53c0b, /// 0xe18
			 0xd0a884d9, /// 0xe1c
			 0x821fd89a, /// 0xe20
			 0xed9280d7, /// 0xe24
			 0xb47deef8, /// 0xe28
			 0x6778de2d, /// 0xe2c
			 0x73db56c8, /// 0xe30
			 0xbbd9cb10, /// 0xe34
			 0x5e0fb8ae, /// 0xe38
			 0x59489c47, /// 0xe3c
			 0xdd3f29f2, /// 0xe40
			 0x9da85796, /// 0xe44
			 0x083380fd, /// 0xe48
			 0xa01fb284, /// 0xe4c
			 0x3781f1b4, /// 0xe50
			 0x87a81ac3, /// 0xe54
			 0xf51b1d43, /// 0xe58
			 0xbacdb3a1, /// 0xe5c
			 0x8aad2b05, /// 0xe60
			 0xc5fd6046, /// 0xe64
			 0xd86410bb, /// 0xe68
			 0x3a5c7bee, /// 0xe6c
			 0xe536d6bb, /// 0xe70
			 0xdcf539a3, /// 0xe74
			 0xc2ca34ed, /// 0xe78
			 0x0a518275, /// 0xe7c
			 0x4e83a686, /// 0xe80
			 0x826d4042, /// 0xe84
			 0x02b3e28c, /// 0xe88
			 0x371afe73, /// 0xe8c
			 0x1cb9b20e, /// 0xe90
			 0x5edd15a5, /// 0xe94
			 0x070310da, /// 0xe98
			 0x7efd8930, /// 0xe9c
			 0x6a77d84a, /// 0xea0
			 0x16715ad5, /// 0xea4
			 0x50b56ea6, /// 0xea8
			 0xd2a811fc, /// 0xeac
			 0x694a9014, /// 0xeb0
			 0xf01e41b8, /// 0xeb4
			 0xe2430e75, /// 0xeb8
			 0x8b52a0df, /// 0xebc
			 0x94bcb3b7, /// 0xec0
			 0x0b6bf8ad, /// 0xec4
			 0x2fdce5d0, /// 0xec8
			 0x9eaca650, /// 0xecc
			 0x359534f0, /// 0xed0
			 0xe8414ba7, /// 0xed4
			 0x3f7895a3, /// 0xed8
			 0x857685ea, /// 0xedc
			 0x2f31653c, /// 0xee0
			 0xd24e82fe, /// 0xee4
			 0xc020e059, /// 0xee8
			 0x48080e37, /// 0xeec
			 0x86affb1e, /// 0xef0
			 0x597e7401, /// 0xef4
			 0x90c05400, /// 0xef8
			 0x6d5364e7, /// 0xefc
			 0x67cce1be, /// 0xf00
			 0x16457258, /// 0xf04
			 0x95899eb0, /// 0xf08
			 0x3fc698da, /// 0xf0c
			 0x6d9901f4, /// 0xf10
			 0x19513ee1, /// 0xf14
			 0x91042ba7, /// 0xf18
			 0x01ac7900, /// 0xf1c
			 0xf1ef0a5e, /// 0xf20
			 0xb855b171, /// 0xf24
			 0x480e638b, /// 0xf28
			 0xadd7a0c6, /// 0xf2c
			 0xf36dc448, /// 0xf30
			 0x895c76dc, /// 0xf34
			 0xc8263a3c, /// 0xf38
			 0x411ed824, /// 0xf3c
			 0x5a045038, /// 0xf40
			 0x75bdec88, /// 0xf44
			 0xe76bc3b8, /// 0xf48
			 0x9919cf57, /// 0xf4c
			 0x62360f85, /// 0xf50
			 0x98f7ce2a, /// 0xf54
			 0x9513fa93, /// 0xf58
			 0x5b30f5d4, /// 0xf5c
			 0xba739f0f, /// 0xf60
			 0x64dbda44, /// 0xf64
			 0x69cd1bab, /// 0xf68
			 0x27d1d5d1, /// 0xf6c
			 0xeed5ad06, /// 0xf70
			 0x35e7b4fd, /// 0xf74
			 0x026f226b, /// 0xf78
			 0xa8068921, /// 0xf7c
			 0x38641476, /// 0xf80
			 0x4add0418, /// 0xf84
			 0x4c779cdf, /// 0xf88
			 0x32700f44, /// 0xf8c
			 0x2fae2f98, /// 0xf90
			 0x595c3239, /// 0xf94
			 0x8551a305, /// 0xf98
			 0x7191699e, /// 0xf9c
			 0xeabd67be, /// 0xfa0
			 0x67367e16, /// 0xfa4
			 0x76a24c54, /// 0xfa8
			 0x418741eb, /// 0xfac
			 0x97da9de3, /// 0xfb0
			 0xd0824dfc, /// 0xfb4
			 0x7fb75925, /// 0xfb8
			 0x981ca75f, /// 0xfbc
			 0xc572b54a, /// 0xfc0
			 0x47521239, /// 0xfc4
			 0x60384955, /// 0xfc8
			 0x1e85561d, /// 0xfcc
			 0x1816c312, /// 0xfd0
			 0x438070f7, /// 0xfd4
			 0x380dc690, /// 0xfd8
			 0xd666fa99, /// 0xfdc
			 0x31a04a4a, /// 0xfe0
			 0x6d877b87, /// 0xfe4
			 0xaeaa4ecd, /// 0xfe8
			 0xa06a0013, /// 0xfec
			 0x69d1d31e, /// 0xff0
			 0xf521ed1e, /// 0xff4
			 0x872f7a33, /// 0xff8
			 0xcf33733d /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00000
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00004
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00008
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0000c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00010
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00014
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00018
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0001c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00020
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00024
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0002c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00030
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00038
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0003c
			 0xcb000000,                                                  // -8388608.0                                  /// 00040
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0004c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00054
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00058
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0005c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00060
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00064
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00068
			 0x0e000003,                                                  // Trailing 1s:                                /// 0006c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00070
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00074
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00078
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0007c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00080
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00084
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00088
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00098
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00000000,                                                  // zero                                        /// 000cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00100
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00104
			 0x7f800000,                                                  // inf                                         /// 00108
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0010c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00110
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00114
			 0x0c400000,                                                  // Leading 1s:                                 /// 00118
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00120
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00128
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0012c
			 0xff800000,                                                  // -inf                                        /// 00130
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00134
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0013c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00144
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00148
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00150
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00154
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00158
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00168
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0016c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00180
			 0x0c600000,                                                  // Leading 1s:                                 /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x80011111,                                                  // -9.7958E-41                                 /// 0018c
			 0x7f800000,                                                  // inf                                         /// 00190
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000000,                                                  // -zero                                       /// 00198
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0019c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xff800000,                                                  // -inf                                        /// 001e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00214
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00218
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00228
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0022c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00230
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00248
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0024c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00250
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00254
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0025c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00264
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0026c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00270
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00274
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0027c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00280
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0028c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00294
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00298
			 0x7fc00001,                                                  // signaling NaN                               /// 0029c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b0
			 0xff800000,                                                  // -inf                                        /// 002b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002cc
			 0x4b000000,                                                  // 8388608.0                                   /// 002d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0xbf028f5c,                                                  // -0.51                                       /// 00304
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00308
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00314
			 0xffc00001,                                                  // -signaling NaN                              /// 00318
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00320
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00324
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00328
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00334
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00344
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0034c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00350
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00358
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00360
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00368
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0036c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00370
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00374
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0037c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00380
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00384
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x7f800000,                                                  // inf                                         /// 00390
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00394
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00398
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f8
			 0xff800000,                                                  // -inf                                        /// 003fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000000,                                                  // zero                                        /// 00404
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00408
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00418
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00424
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00428
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0xbf028f5c,                                                  // -0.51                                       /// 00434
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00438
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00440
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00444
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00448
			 0x0e000003,                                                  // Trailing 1s:                                /// 0044c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00450
			 0x33333333,                                                  // 4 random values                             /// 00454
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00458
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00460
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00464
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00468
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00470
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00478
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00484
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00488
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0048c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00490
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00494
			 0x0c400000,                                                  // Leading 1s:                                 /// 00498
			 0x3f028f5c,                                                  // 0.51                                        /// 0049c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a0
			 0x00000000,                                                  // zero                                        /// 004a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b4
			 0xff800000,                                                  // -inf                                        /// 004b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004dc
			 0x3f028f5c,                                                  // 0.51                                        /// 004e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ec
			 0xbf028f5c,                                                  // -0.51                                       /// 004f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00504
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00508
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00510
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00514
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0c600000,                                                  // Leading 1s:                                 /// 0051c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00520
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00528
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00530
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x7f800000,                                                  // inf                                         /// 00538
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00540
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00544
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00548
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0054c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00550
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00568
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0056c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00578
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0057c
			 0xbf028f5c,                                                  // -0.51                                       /// 00580
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00588
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00598
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0059c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x00000000,                                                  // zero                                        /// 005a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e000007,                                                  // Trailing 1s:                                /// 00604
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00610
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00618
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00628
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0062c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00630
			 0x0c600000,                                                  // Leading 1s:                                 /// 00634
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00638
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0063c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00644
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00648
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00650
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00654
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00658
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0065c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00668
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0066c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00670
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00674
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00678
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00680
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00684
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00688
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0068c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00690
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00698
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0069c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00700
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00708
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0070c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00714
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00718
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0071c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xcb000000,                                                  // -8388608.0                                  /// 00724
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80011111,                                                  // -9.7958E-41                                 /// 0072c
			 0xbf028f5c,                                                  // -0.51                                       /// 00730
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00734
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00738
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0073c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00740
			 0xbf028f5c,                                                  // -0.51                                       /// 00744
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00750
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0075c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00760
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00764
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0076c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00770
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00780
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00784
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000000,                                                  // zero                                        /// 0078c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00790
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0079c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00804
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00808
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0080c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00810
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00814
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00818
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0081c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00824
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00828
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00838
			 0x0c780000,                                                  // Leading 1s:                                 /// 0083c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00840
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00844
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00848
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0084c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00850
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0085c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00860
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00868
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0086c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x55555555,                                                  // 4 random values                             /// 00874
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c780000,                                                  // Leading 1s:                                 /// 0087c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00880
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00884
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00888
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0089c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0e000001,                                                  // Trailing 1s:                                /// 00904
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00908
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0090c
			 0xffc00001,                                                  // -signaling NaN                              /// 00910
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00918
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xbf028f5c,                                                  // -0.51                                       /// 00928
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00930
			 0x0c780000,                                                  // Leading 1s:                                 /// 00934
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00938
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0093c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00940
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00944
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00948
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0094c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00950
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00954
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00960
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00968
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0096c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00970
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00974
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00978
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00980
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00984
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00988
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0098c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00994
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00998
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0099c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a0
			 0x3f028f5c,                                                  // 0.51                                        /// 009a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c4
			 0xff800000,                                                  // -inf                                        /// 009c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xbf028f5c,                                                  // -0.51                                       /// 009f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xff800000,                                                  // -inf                                        /// 00a18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a8c
			 0x7f800000,                                                  // inf                                         /// 00a90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x55555555,                                                  // 4 random values                             /// 00a98
			 0xffc00001,                                                  // -signaling NaN                              /// 00a9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aac
			 0x80000000,                                                  // -zero                                       /// 00ab0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00abc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00acc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ae0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ae8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00afc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b2c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xbf028f5c,                                                  // -0.51                                       /// 00b78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x4b000000,                                                  // 8388608.0                                   /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c3c
			 0x00000000,                                                  // zero                                        /// 00c40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c78
			 0xffc00001,                                                  // -signaling NaN                              /// 00c7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x33333333,                                                  // 4 random values                             /// 00c84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x7fc00001,                                                  // signaling NaN                               /// 00c94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ca0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ce4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ce8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0x55555555,                                                  // 4 random values                             /// 00d2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d44
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x7f800000,                                                  // inf                                         /// 00d6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0xff800000,                                                  // -inf                                        /// 00d84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d94
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d98
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00db8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00de0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00de8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00df8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ea0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ec4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ed4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ef8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f08
			 0x33333333,                                                  // 4 random values                             /// 00f0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f14
			 0xffc00001,                                                  // -signaling NaN                              /// 00f18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f40
			 0x00000000,                                                  // zero                                        /// 00f44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00fa0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fe0
			 0x33333333,                                                  // 4 random values                             /// 00fe4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff8
			 0x00400000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00004
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00008
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0000c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00010
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00018
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0002c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00030
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00034
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00038
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0003c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00040
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00044
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00048
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00050
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00060
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00064
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00078
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00080
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00084
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00088
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0008c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00090
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00100
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00104
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x00011111,                                                  // 9.7958E-41                                  /// 00114
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00118
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0011c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00120
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00128
			 0x0e000003,                                                  // Trailing 1s:                                /// 0012c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00130
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00134
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00138
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0013c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00148
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00150
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00158
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0015c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00160
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00164
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0016c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00178
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00184
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0018c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00190
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00194
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00198
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0019c
			 0xff800000,                                                  // -inf                                        /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x7fc00001,                                                  // signaling NaN                               /// 001ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00204
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x00000000,                                                  // zero                                        /// 0020c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00210
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00214
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00218
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0021c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00220
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0c780000,                                                  // Leading 1s:                                 /// 00234
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00238
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0c780000,                                                  // Leading 1s:                                 /// 00244
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00248
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0024c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0025c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00264
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0026c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00270
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00274
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00278
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0027c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00280
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00284
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00288
			 0xbf028f5c,                                                  // -0.51                                       /// 0028c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00290
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00298
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b0
			 0x55555555,                                                  // 4 random values                             /// 002b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0xbf028f5c,                                                  // -0.51                                       /// 002fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00300
			 0x0e000001,                                                  // Trailing 1s:                                /// 00304
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00308
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0030c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00314
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0e000003,                                                  // Trailing 1s:                                /// 00324
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0032c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00330
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00334
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00338
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00344
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0034c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00350
			 0x0e000007,                                                  // Trailing 1s:                                /// 00354
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0e000003,                                                  // Trailing 1s:                                /// 00364
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00378
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0037c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff800000,                                                  // -inf                                        /// 00384
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0038c
			 0x33333333,                                                  // 4 random values                             /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003d8
			 0xff800000,                                                  // -inf                                        /// 003dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0xff800000,                                                  // -inf                                        /// 003f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00400
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00404
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00410
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0041c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00420
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00424
			 0x0e000003,                                                  // Trailing 1s:                                /// 00428
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0042c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0043c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00440
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00444
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00448
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0044c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00450
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00460
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00468
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x7fc00001,                                                  // signaling NaN                               /// 00470
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00474
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x7fc00001,                                                  // signaling NaN                               /// 0047c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00480
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00484
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000000,                                                  // -zero                                       /// 00494
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004b4
			 0x55555555,                                                  // 4 random values                             /// 004b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0xcb000000,                                                  // -8388608.0                                  /// 004d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 004f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00500
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xffc00001,                                                  // -signaling NaN                              /// 00520
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00524
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00000000,                                                  // zero                                        /// 00540
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00544
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0054c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00554
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00558
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0055c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x0e000001,                                                  // Trailing 1s:                                /// 00568
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00574
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0057c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00580
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00588
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a0
			 0x7f800000,                                                  // inf                                         /// 005a4
			 0x7f800000,                                                  // inf                                         /// 005a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00600
			 0x3f028f5c,                                                  // 0.51                                        /// 00604
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0060c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00614
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00618
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0061c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00620
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0062c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00630
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00638
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00640
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00648
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0064c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00650
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00658
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0065c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00660
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00664
			 0x4b000000,                                                  // 8388608.0                                   /// 00668
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0066c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00674
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00678
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0067c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00684
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00690
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00694
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0069c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x3f028f5c,                                                  // 0.51                                        /// 006a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00700
			 0x80000000,                                                  // -zero                                       /// 00704
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00708
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0070c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00710
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00718
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0072c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00730
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00734
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00738
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00740
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00754
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0075c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00768
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0076c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00778
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00784
			 0xcb000000,                                                  // -8388608.0                                  /// 00788
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0078c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00798
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0079c
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00800
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00808
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0080c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00810
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00820
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00824
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00828
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00838
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0083c
			 0xbf028f5c,                                                  // -0.51                                       /// 00840
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00844
			 0x0e000003,                                                  // Trailing 1s:                                /// 00848
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0084c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00850
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00854
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00860
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00864
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0086c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00870
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00874
			 0x0c700000,                                                  // Leading 1s:                                 /// 00878
			 0x0c780000,                                                  // Leading 1s:                                 /// 0087c
			 0x00000000,                                                  // zero                                        /// 00880
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00884
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00890
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00894
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0089c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0xff800000,                                                  // -inf                                        /// 008c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00900
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00904
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0091c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00920
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00928
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00930
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00934
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00948
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00950
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00958
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0095c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00964
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00974
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00978
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0097c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00980
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00984
			 0x7f800000,                                                  // inf                                         /// 00988
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00990
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x3f028f5c,                                                  // 0.51                                        /// 009b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b4
			 0x7fc00001,                                                  // signaling NaN                               /// 009b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xbf028f5c,                                                  // -0.51                                       /// 009c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a34
			 0x00000000,                                                  // zero                                        /// 00a38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a4c
			 0x80000000,                                                  // -zero                                       /// 00a50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0xcb000000,                                                  // -8388608.0                                  /// 00a7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a80
			 0x7fc00001,                                                  // signaling NaN                               /// 00a84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa0
			 0xff800000,                                                  // -inf                                        /// 00aa4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aa8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00abc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00aec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00af4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00afc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b98
			 0x3f028f5c,                                                  // 0.51                                        /// 00b9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000000,                                                  // -zero                                       /// 00bb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bc4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bf0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bf4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c88
			 0x3f028f5c,                                                  // 0.51                                        /// 00c8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cc8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0xff800000,                                                  // -inf                                        /// 00d68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d84
			 0xff800000,                                                  // -inf                                        /// 00d88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f800000,                                                  // inf                                         /// 00db0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ddc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00de4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00df0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e1c
			 0xff800000,                                                  // -inf                                        /// 00e20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000000,                                                  // zero                                        /// 00e38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e68
			 0x80000000,                                                  // -zero                                       /// 00e6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e78
			 0x4b000000,                                                  // 8388608.0                                   /// 00e7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x7f800000,                                                  // inf                                         /// 00e8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ea4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ea8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ee4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00efc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f00
			 0xffc00001,                                                  // -signaling NaN                              /// 00f04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fbc
			 0xff800000,                                                  // -inf                                        /// 00fc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fcc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fe8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x3f800001 // 1  + 1ulp                                     // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00000
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00004
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00014
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00018
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0001c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00024
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00028
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00030
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00034
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00038
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00048
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0004c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00050
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00058
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0005c
			 0x00000000,                                                  // zero                                        /// 00060
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00064
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00068
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00070
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00074
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0xffc00001,                                                  // -signaling NaN                              /// 00088
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0008c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00090
			 0x0c780000,                                                  // Leading 1s:                                 /// 00094
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e8
			 0x4b000000,                                                  // 8388608.0                                   /// 000ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00108
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00118
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0011c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00120
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00124
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00130
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00138
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0013c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00144
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00148
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0014c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00150
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0015c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00164
			 0x00011111,                                                  // 9.7958E-41                                  /// 00168
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00170
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00174
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0017c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x3f028f5c,                                                  // 0.51                                        /// 00188
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0018c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00190
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00198
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0019c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x33333333,                                                  // 4 random values                             /// 001a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00208
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0020c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00210
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00214
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00218
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0021c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00224
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000000,                                                  // zero                                        /// 0022c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00230
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00234
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00238
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00240
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00244
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00248
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0024c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00250
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00258
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0025c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x4b000000,                                                  // 8388608.0                                   /// 00264
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00268
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00270
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00274
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00284
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00288
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0028c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00294
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00304
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00308
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0030c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00310
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00314
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x7f800000,                                                  // inf                                         /// 00320
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00324
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x7f800000,                                                  // inf                                         /// 00330
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00334
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00338
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0033c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00340
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00348
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0034c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00354
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00358
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x4b000000,                                                  // 8388608.0                                   /// 00364
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00370
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00374
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00378
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00384
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00388
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00390
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00394
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0039c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003c4
			 0xff800000,                                                  // -inf                                        /// 003c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00400
			 0x0c780000,                                                  // Leading 1s:                                 /// 00404
			 0x0c700000,                                                  // Leading 1s:                                 /// 00408
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0040c
			 0x33333333,                                                  // 4 random values                             /// 00410
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00414
			 0xff800000,                                                  // -inf                                        /// 00418
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0041c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00420
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00424
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00428
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0042c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00430
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00434
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00438
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00444
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0044c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x33333333,                                                  // 4 random values                             /// 00454
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0045c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00000000,                                                  // zero                                        /// 00468
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0046c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00470
			 0x0c600000,                                                  // Leading 1s:                                 /// 00474
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0047c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00480
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00484
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00488
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0048c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00494
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00498
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00500
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00514
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00518
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0051c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00520
			 0x0e000007,                                                  // Trailing 1s:                                /// 00524
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00528
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0052c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00530
			 0x0e000003,                                                  // Trailing 1s:                                /// 00534
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00538
			 0x7f800000,                                                  // inf                                         /// 0053c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00544
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00548
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0054c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00550
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00554
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00564
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00568
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0056c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00570
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00578
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0057c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00584
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00588
			 0x00000000,                                                  // zero                                        /// 0058c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00594
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00598
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0059c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005bc
			 0xcb000000,                                                  // -8388608.0                                  /// 005c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00600
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00604
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00608
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0060c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00610
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00614
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00618
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0061c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00620
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00624
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00628
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0062c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00630
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00634
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0063c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00644
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00648
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0064c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00650
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0065c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00660
			 0x00000000,                                                  // zero                                        /// 00664
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00674
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00678
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0067c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00684
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00688
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00690
			 0x0c700000,                                                  // Leading 1s:                                 /// 00694
			 0x7fc00001,                                                  // signaling NaN                               /// 00698
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0069c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006b0
			 0x55555555,                                                  // 4 random values                             /// 006b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00700
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00710
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00720
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00724
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0072c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00730
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00734
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80011111,                                                  // -9.7958E-41                                 /// 00748
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0074c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00750
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00754
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00758
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0075c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00774
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00778
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0077c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00780
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00784
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00788
			 0x0e000001,                                                  // Trailing 1s:                                /// 0078c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00790
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00794
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0079c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007a4
			 0x4b000000,                                                  // 8388608.0                                   /// 007a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00000000,                                                  // zero                                        /// 007b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007bc
			 0xff800000,                                                  // -inf                                        /// 007c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007cc
			 0x4b000000,                                                  // 8388608.0                                   /// 007d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f0
			 0x4b000000,                                                  // 8388608.0                                   /// 007f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7fc00001,                                                  // signaling NaN                               /// 007fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00808
			 0x0c700000,                                                  // Leading 1s:                                 /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00814
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00818
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00828
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xff800000,                                                  // -inf                                        /// 00830
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00834
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00838
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00844
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0084c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0xff800000,                                                  // -inf                                        /// 00854
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00858
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00860
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00864
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0086c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00874
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00878
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00880
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00884
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00888
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0088c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00894
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0089c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00908
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x80011111,                                                  // -9.7958E-41                                 /// 00914
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00918
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00920
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00928
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00930
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00934
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0093c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00944
			 0x33333333,                                                  // 4 random values                             /// 00948
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0094c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00954
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00958
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0095c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00968
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00970
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00974
			 0x33333333,                                                  // 4 random values                             /// 00978
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00984
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00988
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0098c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00990
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00998
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009bc
			 0xffc00001,                                                  // -signaling NaN                              /// 009c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x7fc00001,                                                  // signaling NaN                               /// 009cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a00
			 0x3f028f5c,                                                  // 0.51                                        /// 00a04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a18
			 0xffc00001,                                                  // -signaling NaN                              /// 00a1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000000,                                                  // zero                                        /// 00a2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a64
			 0xbf028f5c,                                                  // -0.51                                       /// 00a68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00abc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00acc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00adc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ae8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00afc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b2c
			 0x00000000,                                                  // zero                                        /// 00b30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x4b000000,                                                  // 8388608.0                                   /// 00b98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd0
			 0x55555555,                                                  // 4 random values                             /// 00bd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00be8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xcb000000,                                                  // -8388608.0                                  /// 00c14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c28
			 0x00000000,                                                  // zero                                        /// 00c2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c4c
			 0x33333333,                                                  // 4 random values                             /// 00c50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c7c
			 0xff800000,                                                  // -inf                                        /// 00c80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c90
			 0x33333333,                                                  // 4 random values                             /// 00c94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c98
			 0x7fc00001,                                                  // signaling NaN                               /// 00c9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ca8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cb8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ccc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cdc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cf8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cfc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d04
			 0xcb000000,                                                  // -8388608.0                                  /// 00d08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00db4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00db8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dc0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dc4
			 0x00000000,                                                  // zero                                        /// 00dc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00df4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00df8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e30
			 0x55555555,                                                  // 4 random values                             /// 00e34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ea4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ebc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ec4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ecc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00edc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ee4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f08
			 0x7fc00001,                                                  // signaling NaN                               /// 00f0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f20
			 0x33333333,                                                  // 4 random values                             /// 00f24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f98
			 0xcb000000,                                                  // -8388608.0                                  /// 00f9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fa4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fbc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x7f800000,                                                  // inf                                         /// 00fd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fe0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff8
			 0x80800001                                                  // none of the mantissa set to +3ulp           /// last
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
			 0x00000768,
			 0x00000354,
			 0x000003cc,
			 0x0000073c,
			 0x000001c0,
			 0x000007c0,
			 0x00000684,
			 0x00000750,

			 /// vpu register f2
			 0x40c00000,
			 0x41600000,
			 0x41e00000,
			 0x41c80000,
			 0x41d80000,
			 0x41980000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f3
			 0x41b80000,
			 0x41d00000,
			 0x41a80000,
			 0x41e80000,
			 0x3f800000,
			 0x41b80000,
			 0x41000000,
			 0x41500000,

			 /// vpu register f4
			 0x40c00000,
			 0x41e00000,
			 0x41880000,
			 0x41d80000,
			 0x41700000,
			 0x41600000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f5
			 0x41f00000,
			 0x40800000,
			 0x41700000,
			 0x41100000,
			 0x40400000,
			 0x41000000,
			 0x41a00000,
			 0x41c80000,

			 /// vpu register f6
			 0x40a00000,
			 0x40e00000,
			 0x41700000,
			 0x41a00000,
			 0x40000000,
			 0x41e00000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f7
			 0x41c00000,
			 0x40a00000,
			 0x41100000,
			 0x41b00000,
			 0x40a00000,
			 0x41d80000,
			 0x41d00000,
			 0x40000000,

			 /// vpu register f8
			 0x41c00000,
			 0x42000000,
			 0x41d00000,
			 0x41b80000,
			 0x41c80000,
			 0x41100000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41800000,
			 0x42000000,
			 0x41200000,
			 0x41a00000,
			 0x40a00000,
			 0x40400000,
			 0x41400000,
			 0x40c00000,

			 /// vpu register f10
			 0x41d80000,
			 0x41b00000,
			 0x41000000,
			 0x41700000,
			 0x41980000,
			 0x41f80000,
			 0x41880000,
			 0x41c80000,

			 /// vpu register f11
			 0x40000000,
			 0x40e00000,
			 0x41300000,
			 0x41d00000,
			 0x41e00000,
			 0x40400000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f12
			 0x41300000,
			 0x40c00000,
			 0x40400000,
			 0x41f00000,
			 0x40e00000,
			 0x41880000,
			 0x40400000,
			 0x41100000,

			 /// vpu register f13
			 0x41a80000,
			 0x41d00000,
			 0x40800000,
			 0x41a80000,
			 0x41c80000,
			 0x41a00000,
			 0x41800000,
			 0x40000000,

			 /// vpu register f14
			 0x42000000,
			 0x41a00000,
			 0x41a00000,
			 0x41980000,
			 0x41900000,
			 0x41f00000,
			 0x42000000,
			 0x41f00000,

			 /// vpu register f15
			 0x41a00000,
			 0x41b80000,
			 0x41400000,
			 0x41c00000,
			 0x40800000,
			 0x41d80000,
			 0x41b80000,
			 0x41a80000,

			 /// vpu register f16
			 0x41f00000,
			 0x41100000,
			 0x41500000,
			 0x42000000,
			 0x41700000,
			 0x40a00000,
			 0x40a00000,
			 0x41900000,

			 /// vpu register f17
			 0x40e00000,
			 0x41600000,
			 0x41a00000,
			 0x41d00000,
			 0x41100000,
			 0x41300000,
			 0x42000000,
			 0x41f80000,

			 /// vpu register f18
			 0x41b80000,
			 0x41100000,
			 0x41a00000,
			 0x41980000,
			 0x41f80000,
			 0x41a80000,
			 0x40e00000,
			 0x41800000,

			 /// vpu register f19
			 0x41800000,
			 0x41b80000,
			 0x3f800000,
			 0x40800000,
			 0x40c00000,
			 0x41f00000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f20
			 0x41000000,
			 0x41a00000,
			 0x41880000,
			 0x40e00000,
			 0x42000000,
			 0x41980000,
			 0x41e00000,
			 0x41e80000,

			 /// vpu register f21
			 0x41200000,
			 0x41b80000,
			 0x40e00000,
			 0x41900000,
			 0x40a00000,
			 0x41a00000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f22
			 0x41200000,
			 0x41100000,
			 0x41000000,
			 0x41300000,
			 0x40000000,
			 0x41b80000,
			 0x41f00000,
			 0x40400000,

			 /// vpu register f23
			 0x41100000,
			 0x3f800000,
			 0x41500000,
			 0x41d80000,
			 0x41700000,
			 0x41d80000,
			 0x41b00000,
			 0x41a80000,

			 /// vpu register f24
			 0x41b00000,
			 0x41000000,
			 0x40400000,
			 0x41980000,
			 0x41300000,
			 0x41600000,
			 0x40800000,
			 0x40000000,

			 /// vpu register f25
			 0x40000000,
			 0x41400000,
			 0x41f00000,
			 0x41f80000,
			 0x41100000,
			 0x40e00000,
			 0x41600000,
			 0x3f800000,

			 /// vpu register f26
			 0x40800000,
			 0x41900000,
			 0x40e00000,
			 0x40e00000,
			 0x41a00000,
			 0x41800000,
			 0x41000000,
			 0x41980000,

			 /// vpu register f27
			 0x40c00000,
			 0x42000000,
			 0x41000000,
			 0x40e00000,
			 0x40c00000,
			 0x41700000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f28
			 0x41500000,
			 0x41500000,
			 0x3f800000,
			 0x41400000,
			 0x41d80000,
			 0x41880000,
			 0x41800000,
			 0x41800000,

			 /// vpu register f29
			 0x41000000,
			 0x41f00000,
			 0x41f00000,
			 0x41500000,
			 0x41c80000,
			 0x41500000,
			 0x41c00000,
			 0x42000000,

			 /// vpu register f30
			 0x40e00000,
			 0x41200000,
			 0x41900000,
			 0x41700000,
			 0x41d00000,
			 0x41a00000,
			 0x41800000,
			 0x41c80000,

			 /// vpu register f31
			 0x42000000,
			 0x41a00000,
			 0x40e00000,
			 0x41e80000,
			 0x41c00000,
			 0x41700000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f6, f27\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f8, f18\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f23\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f21, f16\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f10, f12\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f15, f2\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f5, f19\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f12, f14\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f20, f7\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f30, f13\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f18, f17\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f6, f11\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f30, f7\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f19, f15\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f22, f9\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f22, f25\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f2, f27\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f8, f14\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f18, f8\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f19, f19\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f27, f16\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f19, f2\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f16, f21\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f22, f27\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f19, f1\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f11, f6\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f24, f1\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f26, f2\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f6, f30\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f28, f15\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f18, f6\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f6, f22\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f30, f7\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f16, f23\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f25, f24\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f18, f2\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f10, f22\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f5, f14\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f12, f27\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f28, f18\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f27, f15\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f18, f15\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f26, f15\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f2, f22\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f12, f24\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f28, f25\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f8, f6\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f30, f25\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f25, f19\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f28, f25\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f26, f3\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f5\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f17, f5\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f9, f9\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f7, f9\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f29, f13\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f15, f23\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f3, f13\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f25, f21\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f23, f28\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f3, f3\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f9, f7\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f23\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f5, f30\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f5, f14\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f15, f18\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f10, f16\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f27, f9\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f22, f0\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f27, f14\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f7, f16\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f12\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f15, f7\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f15, f22\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f15, f7\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f23, f5\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f21, f3\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f12, f24\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f12, f4\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f22, f27\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f17, f10\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f25, f1\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f21, f22\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f12\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f7, f10\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f9, f2\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f4, f20\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f28, f30\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f19\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f16, f17\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f29, f22\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f8, f9\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f2, f25\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f29, f27\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f9, f26\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f21, f1\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f13, f13\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f27, f14\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un24 f11, f21\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
