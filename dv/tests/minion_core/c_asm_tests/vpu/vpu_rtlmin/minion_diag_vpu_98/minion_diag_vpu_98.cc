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
			 0xe74a7a89, /// 0x0
			 0x9746da6e, /// 0x4
			 0x8179ebc3, /// 0x8
			 0x14ac07fb, /// 0xc
			 0x8cc5dd82, /// 0x10
			 0x977e8c5c, /// 0x14
			 0xeaaf850f, /// 0x18
			 0xc6390143, /// 0x1c
			 0x532efe4e, /// 0x20
			 0x2258ae14, /// 0x24
			 0x7c5a94f7, /// 0x28
			 0xdef486e8, /// 0x2c
			 0x8f270b00, /// 0x30
			 0x1fa10a8c, /// 0x34
			 0x849ce6ab, /// 0x38
			 0xe8764d19, /// 0x3c
			 0xd4999c3d, /// 0x40
			 0x02eb0a5d, /// 0x44
			 0xb9c530bc, /// 0x48
			 0x3af30ff6, /// 0x4c
			 0x996046f0, /// 0x50
			 0x55a328a5, /// 0x54
			 0x89b3df2f, /// 0x58
			 0x95bc47ac, /// 0x5c
			 0x27ef7fb3, /// 0x60
			 0xd190a318, /// 0x64
			 0x89b1e202, /// 0x68
			 0x4e9075dd, /// 0x6c
			 0xd46c52b7, /// 0x70
			 0xcc0fe7c1, /// 0x74
			 0x27668c3a, /// 0x78
			 0x3473cb6c, /// 0x7c
			 0xb39900d1, /// 0x80
			 0x91ace7bf, /// 0x84
			 0x280e6de1, /// 0x88
			 0x4b8bfa2f, /// 0x8c
			 0x857e1582, /// 0x90
			 0xbfee93e0, /// 0x94
			 0xb7c4f9b6, /// 0x98
			 0x56e22187, /// 0x9c
			 0x611773e5, /// 0xa0
			 0xaf36b1e8, /// 0xa4
			 0x86adf878, /// 0xa8
			 0x3544391f, /// 0xac
			 0x863053c2, /// 0xb0
			 0x41d9ca70, /// 0xb4
			 0x220199ad, /// 0xb8
			 0x47992894, /// 0xbc
			 0x1743694a, /// 0xc0
			 0x1cc19ff7, /// 0xc4
			 0xc81f21f2, /// 0xc8
			 0x506ca9eb, /// 0xcc
			 0x2264c1e0, /// 0xd0
			 0xaca60b90, /// 0xd4
			 0x651fd42a, /// 0xd8
			 0x03bd2cf5, /// 0xdc
			 0x0fb71a5c, /// 0xe0
			 0x9f745c90, /// 0xe4
			 0x6808245c, /// 0xe8
			 0x63a46dbc, /// 0xec
			 0x10cb750e, /// 0xf0
			 0xaf0bfd0a, /// 0xf4
			 0xfa7000c2, /// 0xf8
			 0x64f7df31, /// 0xfc
			 0x66fbd94a, /// 0x100
			 0xd8f19067, /// 0x104
			 0xb5924642, /// 0x108
			 0x12016e3a, /// 0x10c
			 0xd82191a3, /// 0x110
			 0x1253c4dc, /// 0x114
			 0x98762e04, /// 0x118
			 0x27aaa672, /// 0x11c
			 0xebc1ebcd, /// 0x120
			 0x5e81403d, /// 0x124
			 0x76c062f0, /// 0x128
			 0xd0f1956b, /// 0x12c
			 0xeede9b10, /// 0x130
			 0xfba6dad6, /// 0x134
			 0xd7847be9, /// 0x138
			 0x9723cac3, /// 0x13c
			 0x3b1c0563, /// 0x140
			 0x7a0b4a0d, /// 0x144
			 0x5549855c, /// 0x148
			 0xe2f85d07, /// 0x14c
			 0xcdad9e64, /// 0x150
			 0x6be7f22d, /// 0x154
			 0x414d66cf, /// 0x158
			 0x6f0b435a, /// 0x15c
			 0xe44d27e6, /// 0x160
			 0xff214c4b, /// 0x164
			 0x3becdce2, /// 0x168
			 0xbd61483b, /// 0x16c
			 0xc453c9e1, /// 0x170
			 0x80fdea4d, /// 0x174
			 0x9a83e766, /// 0x178
			 0xdc9815a2, /// 0x17c
			 0xac1492f8, /// 0x180
			 0x8e8f9d76, /// 0x184
			 0x03ee234f, /// 0x188
			 0xc2bf24c3, /// 0x18c
			 0xd77370bb, /// 0x190
			 0x81ab3046, /// 0x194
			 0xc8804168, /// 0x198
			 0xe555d14e, /// 0x19c
			 0x84ebaf74, /// 0x1a0
			 0x1ddf0257, /// 0x1a4
			 0x9cdf575b, /// 0x1a8
			 0x1a003592, /// 0x1ac
			 0xf6bda5bc, /// 0x1b0
			 0xdd47c80e, /// 0x1b4
			 0xf85a2980, /// 0x1b8
			 0xad98f878, /// 0x1bc
			 0xd5306358, /// 0x1c0
			 0xd4339384, /// 0x1c4
			 0x53df7084, /// 0x1c8
			 0xcb67d4b5, /// 0x1cc
			 0xe98c12fd, /// 0x1d0
			 0x0a384e7d, /// 0x1d4
			 0xf781ae7c, /// 0x1d8
			 0xd09685e6, /// 0x1dc
			 0x94500ae2, /// 0x1e0
			 0xcd1b1253, /// 0x1e4
			 0x82847133, /// 0x1e8
			 0xd5e81b07, /// 0x1ec
			 0x0df3ac2f, /// 0x1f0
			 0xb255b3a9, /// 0x1f4
			 0xc128cb6f, /// 0x1f8
			 0xed632947, /// 0x1fc
			 0x280ca6fa, /// 0x200
			 0x35c0dd53, /// 0x204
			 0x63f906ea, /// 0x208
			 0x1cbc2bfb, /// 0x20c
			 0xa19ba3c6, /// 0x210
			 0xf948075a, /// 0x214
			 0x72f2e76a, /// 0x218
			 0x89aac544, /// 0x21c
			 0x02498662, /// 0x220
			 0xce61f036, /// 0x224
			 0xcdf8f168, /// 0x228
			 0x7764ac27, /// 0x22c
			 0x9c2638f9, /// 0x230
			 0xd9aa8f9b, /// 0x234
			 0x5c571879, /// 0x238
			 0x44f07bc8, /// 0x23c
			 0xedbbaa75, /// 0x240
			 0xa8ffb94f, /// 0x244
			 0x6893726c, /// 0x248
			 0xca2a89bd, /// 0x24c
			 0x3b85f86d, /// 0x250
			 0xca4f3ab4, /// 0x254
			 0x6941966b, /// 0x258
			 0x2623ec95, /// 0x25c
			 0xc8478d0d, /// 0x260
			 0x1a611226, /// 0x264
			 0xc0cc729e, /// 0x268
			 0x95411f12, /// 0x26c
			 0x09f2f880, /// 0x270
			 0x4de2ff91, /// 0x274
			 0x8c5ab060, /// 0x278
			 0xc07f5c34, /// 0x27c
			 0x1a970815, /// 0x280
			 0xb7071d7c, /// 0x284
			 0x574f6baa, /// 0x288
			 0xbc5e362d, /// 0x28c
			 0x921fdd17, /// 0x290
			 0x40460d70, /// 0x294
			 0x18fba6b1, /// 0x298
			 0x34c7419a, /// 0x29c
			 0x04dcc0b0, /// 0x2a0
			 0x97a35006, /// 0x2a4
			 0xa6bef498, /// 0x2a8
			 0x973ba826, /// 0x2ac
			 0xb6ca6c98, /// 0x2b0
			 0xfdfb99b5, /// 0x2b4
			 0xdb4570d7, /// 0x2b8
			 0x8ddc4719, /// 0x2bc
			 0xebaa91c0, /// 0x2c0
			 0xe61e4c9d, /// 0x2c4
			 0x10719752, /// 0x2c8
			 0xfbea4095, /// 0x2cc
			 0x841669a5, /// 0x2d0
			 0x5c62f19e, /// 0x2d4
			 0x197dde7c, /// 0x2d8
			 0xfc066b3a, /// 0x2dc
			 0xad2fbe9d, /// 0x2e0
			 0xff229df6, /// 0x2e4
			 0xf37978a5, /// 0x2e8
			 0xc3a15b3b, /// 0x2ec
			 0xca8ae683, /// 0x2f0
			 0x579b97f2, /// 0x2f4
			 0xfa22f1f6, /// 0x2f8
			 0xb4af6626, /// 0x2fc
			 0x803818a0, /// 0x300
			 0x3d148d92, /// 0x304
			 0x56a00c0a, /// 0x308
			 0x75387fc8, /// 0x30c
			 0x3b2f9bc5, /// 0x310
			 0x8b39388d, /// 0x314
			 0x5fa2c36a, /// 0x318
			 0xed344c78, /// 0x31c
			 0x245a5be9, /// 0x320
			 0xafa3e1c1, /// 0x324
			 0x673883f0, /// 0x328
			 0x6ca62ad6, /// 0x32c
			 0xb3c91889, /// 0x330
			 0x3b75f813, /// 0x334
			 0xb7f7bbfd, /// 0x338
			 0x58393181, /// 0x33c
			 0x92bb27ed, /// 0x340
			 0x2daa9862, /// 0x344
			 0x6f2c6d8b, /// 0x348
			 0xdc8db74c, /// 0x34c
			 0x8251a642, /// 0x350
			 0xaa3a5e53, /// 0x354
			 0xa830bbeb, /// 0x358
			 0xb6942bda, /// 0x35c
			 0x1f07f607, /// 0x360
			 0x865a73c7, /// 0x364
			 0xd8dd196d, /// 0x368
			 0x59ead8a5, /// 0x36c
			 0xede7f47b, /// 0x370
			 0xe51ac0e0, /// 0x374
			 0x46f81dc8, /// 0x378
			 0x7f3fdfc7, /// 0x37c
			 0xe682162f, /// 0x380
			 0x4469ab97, /// 0x384
			 0xbda41467, /// 0x388
			 0xc4d940d9, /// 0x38c
			 0xebfd0015, /// 0x390
			 0x737f7aa2, /// 0x394
			 0x79b29e21, /// 0x398
			 0x5c3c1391, /// 0x39c
			 0x60ec97b1, /// 0x3a0
			 0xac75160f, /// 0x3a4
			 0x2d745421, /// 0x3a8
			 0xe34431e1, /// 0x3ac
			 0xfd398dca, /// 0x3b0
			 0x8a08100d, /// 0x3b4
			 0x8128ba02, /// 0x3b8
			 0xbddc58b4, /// 0x3bc
			 0x899eb7b0, /// 0x3c0
			 0x99838c69, /// 0x3c4
			 0x0be1b92d, /// 0x3c8
			 0xc47fc91d, /// 0x3cc
			 0x0b21525d, /// 0x3d0
			 0x6775ee46, /// 0x3d4
			 0x6c00c944, /// 0x3d8
			 0xf2dfe6d2, /// 0x3dc
			 0x5a8a3756, /// 0x3e0
			 0xe43f050b, /// 0x3e4
			 0x93658f65, /// 0x3e8
			 0xf210580c, /// 0x3ec
			 0x1d3ba712, /// 0x3f0
			 0x0cc2485f, /// 0x3f4
			 0x741447f5, /// 0x3f8
			 0x8677efdd, /// 0x3fc
			 0xd18bd175, /// 0x400
			 0xafa91239, /// 0x404
			 0x7fb09871, /// 0x408
			 0x85922d04, /// 0x40c
			 0x5812b5fa, /// 0x410
			 0x19cda332, /// 0x414
			 0xd80744e3, /// 0x418
			 0xb06d82fd, /// 0x41c
			 0x03b20ae6, /// 0x420
			 0xdca43ff2, /// 0x424
			 0xb7d897ef, /// 0x428
			 0x9deecb47, /// 0x42c
			 0x4916a8d8, /// 0x430
			 0x5c4e982c, /// 0x434
			 0xbb567113, /// 0x438
			 0x0c6d49d4, /// 0x43c
			 0x5879a116, /// 0x440
			 0x522b0f53, /// 0x444
			 0xb52e6f16, /// 0x448
			 0x33b24ad0, /// 0x44c
			 0x6a98783e, /// 0x450
			 0xeaacd9dd, /// 0x454
			 0x2e0c09ef, /// 0x458
			 0x7e4b0571, /// 0x45c
			 0x2719d7a0, /// 0x460
			 0xd040f9fc, /// 0x464
			 0x608e6c31, /// 0x468
			 0x28c8e47b, /// 0x46c
			 0xaa831047, /// 0x470
			 0x2457db7b, /// 0x474
			 0x1e467777, /// 0x478
			 0xa2471485, /// 0x47c
			 0x5107efa2, /// 0x480
			 0x05843b36, /// 0x484
			 0xd74aef8c, /// 0x488
			 0x1915acf7, /// 0x48c
			 0xfd6b7e4d, /// 0x490
			 0x4ceaafc6, /// 0x494
			 0xa80514a2, /// 0x498
			 0x2238fc25, /// 0x49c
			 0x4578c58f, /// 0x4a0
			 0x3f4edff9, /// 0x4a4
			 0xc0433033, /// 0x4a8
			 0xe6c520e4, /// 0x4ac
			 0x8bf91b01, /// 0x4b0
			 0x1a41f74f, /// 0x4b4
			 0x2be933d3, /// 0x4b8
			 0xd295c9eb, /// 0x4bc
			 0xf1067b50, /// 0x4c0
			 0x63433243, /// 0x4c4
			 0xb41183ef, /// 0x4c8
			 0x20f2fe9f, /// 0x4cc
			 0x4c5bfde5, /// 0x4d0
			 0x115428b0, /// 0x4d4
			 0x0db6eb3c, /// 0x4d8
			 0xb3b0bcc8, /// 0x4dc
			 0x396739e1, /// 0x4e0
			 0x885868e9, /// 0x4e4
			 0xd29b6a36, /// 0x4e8
			 0x964ee2e3, /// 0x4ec
			 0xc461cfcc, /// 0x4f0
			 0x8b3cc8ae, /// 0x4f4
			 0x01617a61, /// 0x4f8
			 0xab0a54f5, /// 0x4fc
			 0x90bb11fd, /// 0x500
			 0x9931b99c, /// 0x504
			 0x1df3073d, /// 0x508
			 0x3faefaa8, /// 0x50c
			 0x44a26e1c, /// 0x510
			 0x505dd52f, /// 0x514
			 0x9231cab3, /// 0x518
			 0xd8371f67, /// 0x51c
			 0x0a7df491, /// 0x520
			 0x372dfa92, /// 0x524
			 0xde68638a, /// 0x528
			 0xca023dbf, /// 0x52c
			 0x532f424b, /// 0x530
			 0x4d3e5ea0, /// 0x534
			 0x2b42b066, /// 0x538
			 0xa55334b5, /// 0x53c
			 0xdaba3145, /// 0x540
			 0x1a6a239f, /// 0x544
			 0xc9333e21, /// 0x548
			 0xb995d5f5, /// 0x54c
			 0x64ff0396, /// 0x550
			 0x8e469847, /// 0x554
			 0x0d842b3e, /// 0x558
			 0xbc1b9934, /// 0x55c
			 0xe735e0e8, /// 0x560
			 0x7c614ebf, /// 0x564
			 0x26d283c1, /// 0x568
			 0xb4f24312, /// 0x56c
			 0x318c9b19, /// 0x570
			 0xbcd1ac30, /// 0x574
			 0xaeff7a9d, /// 0x578
			 0xcf54da76, /// 0x57c
			 0x2004c296, /// 0x580
			 0x91e688d9, /// 0x584
			 0xd4798a0c, /// 0x588
			 0xf033b9ef, /// 0x58c
			 0x42b16643, /// 0x590
			 0x3765580f, /// 0x594
			 0x3b353c02, /// 0x598
			 0xeb006a77, /// 0x59c
			 0x0a710563, /// 0x5a0
			 0x8298abf0, /// 0x5a4
			 0x08d76736, /// 0x5a8
			 0x9a4769ac, /// 0x5ac
			 0x2b1f70e5, /// 0x5b0
			 0x6cfa0f2b, /// 0x5b4
			 0x5ba7752f, /// 0x5b8
			 0x28b33966, /// 0x5bc
			 0x7903183e, /// 0x5c0
			 0x4acc81db, /// 0x5c4
			 0x500e340a, /// 0x5c8
			 0xf691f524, /// 0x5cc
			 0xc466b7f7, /// 0x5d0
			 0xf372e444, /// 0x5d4
			 0xd7f169b2, /// 0x5d8
			 0xe3fea76e, /// 0x5dc
			 0x65b97ee7, /// 0x5e0
			 0x7d678e97, /// 0x5e4
			 0x52dd03c0, /// 0x5e8
			 0xcf4ce303, /// 0x5ec
			 0xb0d6bc98, /// 0x5f0
			 0xae0b400b, /// 0x5f4
			 0x1ffa6e3e, /// 0x5f8
			 0x9cf039ec, /// 0x5fc
			 0x0eadf9b4, /// 0x600
			 0x4bdba453, /// 0x604
			 0x258ed027, /// 0x608
			 0xabad12b1, /// 0x60c
			 0xa7a3c519, /// 0x610
			 0x0c91d70a, /// 0x614
			 0x2074b89d, /// 0x618
			 0xfff97272, /// 0x61c
			 0x5d28f01e, /// 0x620
			 0x6d4c0ab0, /// 0x624
			 0x45dd5e25, /// 0x628
			 0xa434a184, /// 0x62c
			 0x5f2d97a4, /// 0x630
			 0x1e5e457c, /// 0x634
			 0xffaba44d, /// 0x638
			 0xbc6dc5d0, /// 0x63c
			 0x64236cea, /// 0x640
			 0x479be8b0, /// 0x644
			 0xdc67be40, /// 0x648
			 0x8bc77cb7, /// 0x64c
			 0x428b9413, /// 0x650
			 0x8ee1a6fb, /// 0x654
			 0x818d2197, /// 0x658
			 0x2f52008d, /// 0x65c
			 0xd129c8ef, /// 0x660
			 0x75fc271c, /// 0x664
			 0x6e478b22, /// 0x668
			 0xf44c66ea, /// 0x66c
			 0xcce7d5d3, /// 0x670
			 0xe668f189, /// 0x674
			 0x79b28af1, /// 0x678
			 0x01634aea, /// 0x67c
			 0x0a69f53f, /// 0x680
			 0xe2f0ef79, /// 0x684
			 0x8e64a157, /// 0x688
			 0xda99dc7a, /// 0x68c
			 0xacb1c736, /// 0x690
			 0x45277d16, /// 0x694
			 0x3b33a738, /// 0x698
			 0xcbab5031, /// 0x69c
			 0xa0ef5800, /// 0x6a0
			 0x40005f4f, /// 0x6a4
			 0x690f0b3a, /// 0x6a8
			 0x998d0c07, /// 0x6ac
			 0xaa6950ce, /// 0x6b0
			 0xf744ccaf, /// 0x6b4
			 0xf1d67b86, /// 0x6b8
			 0x8f0ceef0, /// 0x6bc
			 0xbbb93424, /// 0x6c0
			 0x8c1966e5, /// 0x6c4
			 0x73eb887f, /// 0x6c8
			 0xdbc34099, /// 0x6cc
			 0x9495bcea, /// 0x6d0
			 0xd099a029, /// 0x6d4
			 0xb43d35df, /// 0x6d8
			 0x6a29aa6b, /// 0x6dc
			 0x13489b89, /// 0x6e0
			 0xcc6be95e, /// 0x6e4
			 0xe3867d86, /// 0x6e8
			 0xeff4e13e, /// 0x6ec
			 0x24cbeac1, /// 0x6f0
			 0x1d06b9c9, /// 0x6f4
			 0x3b847948, /// 0x6f8
			 0x6b0e94f2, /// 0x6fc
			 0xac83b58a, /// 0x700
			 0x0cb35ba6, /// 0x704
			 0x2660d142, /// 0x708
			 0x5be913b1, /// 0x70c
			 0xb78b01b3, /// 0x710
			 0x00fa34c4, /// 0x714
			 0x07e97d51, /// 0x718
			 0xa91e1abf, /// 0x71c
			 0xd5756678, /// 0x720
			 0xadddd59e, /// 0x724
			 0xcbe10097, /// 0x728
			 0x7d9ca483, /// 0x72c
			 0xbec575e1, /// 0x730
			 0x3dbbf011, /// 0x734
			 0x09729bbc, /// 0x738
			 0x3421ebe1, /// 0x73c
			 0x83e558e0, /// 0x740
			 0xc56e7747, /// 0x744
			 0x40af457a, /// 0x748
			 0x55643ac3, /// 0x74c
			 0x7164a716, /// 0x750
			 0xe995840b, /// 0x754
			 0x2eafcfb7, /// 0x758
			 0xc7b62807, /// 0x75c
			 0x58bcd35e, /// 0x760
			 0x991bad0d, /// 0x764
			 0x222ea170, /// 0x768
			 0x93a45f7a, /// 0x76c
			 0x4f25e9b9, /// 0x770
			 0xbd0e31c0, /// 0x774
			 0x6d9cfa5f, /// 0x778
			 0xcd9fde07, /// 0x77c
			 0x727fd8a4, /// 0x780
			 0x7c27f684, /// 0x784
			 0x3095ac9e, /// 0x788
			 0x484dfd74, /// 0x78c
			 0x8d1a787c, /// 0x790
			 0xfbca6910, /// 0x794
			 0xf460716e, /// 0x798
			 0x7166cc77, /// 0x79c
			 0xb5d33378, /// 0x7a0
			 0x42ef28b1, /// 0x7a4
			 0x8d09425d, /// 0x7a8
			 0x20603001, /// 0x7ac
			 0x1afd12c0, /// 0x7b0
			 0x46e27113, /// 0x7b4
			 0x5a371cf9, /// 0x7b8
			 0x27084a9c, /// 0x7bc
			 0x74b91254, /// 0x7c0
			 0xa2b2e018, /// 0x7c4
			 0xc75af86e, /// 0x7c8
			 0x30fd9810, /// 0x7cc
			 0x9c28953c, /// 0x7d0
			 0xa857220f, /// 0x7d4
			 0xc251c569, /// 0x7d8
			 0xda178fc0, /// 0x7dc
			 0x5bff5e99, /// 0x7e0
			 0xb0f05517, /// 0x7e4
			 0x2bbf5c20, /// 0x7e8
			 0xd917aa1a, /// 0x7ec
			 0x466f8cef, /// 0x7f0
			 0x2dcb233a, /// 0x7f4
			 0x2918d86b, /// 0x7f8
			 0xc4330b0d, /// 0x7fc
			 0x875b8215, /// 0x800
			 0xe0ef297a, /// 0x804
			 0x1b94ec0a, /// 0x808
			 0x45979319, /// 0x80c
			 0xc826c2d0, /// 0x810
			 0xd97edc9a, /// 0x814
			 0xa04c582d, /// 0x818
			 0x466b2473, /// 0x81c
			 0x10386ab3, /// 0x820
			 0xd516a748, /// 0x824
			 0x14befe8c, /// 0x828
			 0x074851d3, /// 0x82c
			 0x7e4477fe, /// 0x830
			 0x58ae2564, /// 0x834
			 0x58078e08, /// 0x838
			 0x05454c08, /// 0x83c
			 0xd2ac3766, /// 0x840
			 0x4ef4a35c, /// 0x844
			 0x3245f3b4, /// 0x848
			 0x909c22b1, /// 0x84c
			 0xbabe3f99, /// 0x850
			 0x939a6e88, /// 0x854
			 0xd46e9fc3, /// 0x858
			 0x2d632f70, /// 0x85c
			 0x06d9efe5, /// 0x860
			 0xee225d61, /// 0x864
			 0x0f286177, /// 0x868
			 0x51f10d80, /// 0x86c
			 0x87a40237, /// 0x870
			 0x5997025f, /// 0x874
			 0xbe3d84f2, /// 0x878
			 0xad1e5933, /// 0x87c
			 0x0738b34f, /// 0x880
			 0xc5ca37c8, /// 0x884
			 0x2d1b0a93, /// 0x888
			 0x6f30ebde, /// 0x88c
			 0x815e396f, /// 0x890
			 0xdf7e1f0a, /// 0x894
			 0x04f9f5c3, /// 0x898
			 0x788fc5cc, /// 0x89c
			 0xe548072e, /// 0x8a0
			 0xa0d874b2, /// 0x8a4
			 0xab2d7b9e, /// 0x8a8
			 0x58d4b475, /// 0x8ac
			 0x5491f857, /// 0x8b0
			 0x1ff06366, /// 0x8b4
			 0x56b4180c, /// 0x8b8
			 0x7ebcfaa8, /// 0x8bc
			 0x94651d2f, /// 0x8c0
			 0x29bf65b5, /// 0x8c4
			 0x6b1a1979, /// 0x8c8
			 0xb75b522f, /// 0x8cc
			 0xde7caacb, /// 0x8d0
			 0xbacf3c61, /// 0x8d4
			 0x268cfc5c, /// 0x8d8
			 0xf097b10f, /// 0x8dc
			 0xd156e9e4, /// 0x8e0
			 0x5fad6dba, /// 0x8e4
			 0xed2c3d0b, /// 0x8e8
			 0x68077379, /// 0x8ec
			 0x333684a7, /// 0x8f0
			 0xdeb54a33, /// 0x8f4
			 0xa541745e, /// 0x8f8
			 0xf3fd8c6f, /// 0x8fc
			 0xb1eab366, /// 0x900
			 0x71d49421, /// 0x904
			 0x5dc6214f, /// 0x908
			 0x8fdc33ad, /// 0x90c
			 0x29f511df, /// 0x910
			 0x46542a9f, /// 0x914
			 0x9995cd02, /// 0x918
			 0xdd1699bb, /// 0x91c
			 0x95bd0cef, /// 0x920
			 0x3b1ae307, /// 0x924
			 0x2e957e00, /// 0x928
			 0x8cb81b3a, /// 0x92c
			 0xad6933e3, /// 0x930
			 0xf619230c, /// 0x934
			 0x0d882d4a, /// 0x938
			 0x556c9f0e, /// 0x93c
			 0xc4030cf7, /// 0x940
			 0xb900ce96, /// 0x944
			 0x6d09f988, /// 0x948
			 0x6d27a524, /// 0x94c
			 0x922279be, /// 0x950
			 0x50499e38, /// 0x954
			 0xad1c86ad, /// 0x958
			 0x36150269, /// 0x95c
			 0x22eef041, /// 0x960
			 0x9d3f5d56, /// 0x964
			 0xdc9150f4, /// 0x968
			 0x635bd19c, /// 0x96c
			 0x062a63e0, /// 0x970
			 0xe3c31d21, /// 0x974
			 0x2bd8da37, /// 0x978
			 0x73fbdb24, /// 0x97c
			 0x867ce41d, /// 0x980
			 0x36688a85, /// 0x984
			 0x6d88f80f, /// 0x988
			 0xb37a6c62, /// 0x98c
			 0x2f00cfdd, /// 0x990
			 0x5b21a768, /// 0x994
			 0xd947cce4, /// 0x998
			 0xb5bbe281, /// 0x99c
			 0xaaa10e45, /// 0x9a0
			 0x79ee4927, /// 0x9a4
			 0x5cbb40fd, /// 0x9a8
			 0x51e4e1e1, /// 0x9ac
			 0x1b816d09, /// 0x9b0
			 0x111953f0, /// 0x9b4
			 0x775790a2, /// 0x9b8
			 0xdf586c64, /// 0x9bc
			 0xdc569016, /// 0x9c0
			 0x60ffa9dc, /// 0x9c4
			 0x544b0577, /// 0x9c8
			 0x4a5de163, /// 0x9cc
			 0xb6cdf306, /// 0x9d0
			 0x6652dec4, /// 0x9d4
			 0xbcdb389d, /// 0x9d8
			 0x4ccbc581, /// 0x9dc
			 0x1ba8a87e, /// 0x9e0
			 0xcb3248fc, /// 0x9e4
			 0x3383e669, /// 0x9e8
			 0x4f486285, /// 0x9ec
			 0xc63a9620, /// 0x9f0
			 0x6d344133, /// 0x9f4
			 0xc5e901e5, /// 0x9f8
			 0xdf341305, /// 0x9fc
			 0x753e5068, /// 0xa00
			 0x3d2f08b9, /// 0xa04
			 0x9a2bb167, /// 0xa08
			 0xc366147d, /// 0xa0c
			 0x207acc86, /// 0xa10
			 0xd5bd617f, /// 0xa14
			 0xe5274065, /// 0xa18
			 0x69e89df1, /// 0xa1c
			 0xdcd3d195, /// 0xa20
			 0x1d7f0e69, /// 0xa24
			 0x50d8d77e, /// 0xa28
			 0x92cfca69, /// 0xa2c
			 0x953612d7, /// 0xa30
			 0x115d337a, /// 0xa34
			 0xe0e7f637, /// 0xa38
			 0xb35116dd, /// 0xa3c
			 0xbdcfa3b8, /// 0xa40
			 0x62e4d595, /// 0xa44
			 0xaf132e26, /// 0xa48
			 0x522b9970, /// 0xa4c
			 0x38ffcdb1, /// 0xa50
			 0xcc618c41, /// 0xa54
			 0x0382a9e9, /// 0xa58
			 0x1374e90d, /// 0xa5c
			 0x66ed767f, /// 0xa60
			 0xa882572c, /// 0xa64
			 0xb4b42c15, /// 0xa68
			 0xf6f06670, /// 0xa6c
			 0xa9909fcd, /// 0xa70
			 0x9e6a487f, /// 0xa74
			 0x1676454e, /// 0xa78
			 0xb47333d8, /// 0xa7c
			 0xbdb41d47, /// 0xa80
			 0x7d828aaf, /// 0xa84
			 0x4f1b5eae, /// 0xa88
			 0x644e0f09, /// 0xa8c
			 0xf8a00510, /// 0xa90
			 0xb75377dc, /// 0xa94
			 0x7b88ed14, /// 0xa98
			 0x2d4f1c66, /// 0xa9c
			 0xfe6b2baf, /// 0xaa0
			 0xb68ee084, /// 0xaa4
			 0x2db499f1, /// 0xaa8
			 0x76f09a34, /// 0xaac
			 0xeb184671, /// 0xab0
			 0x9bb25526, /// 0xab4
			 0x300661bb, /// 0xab8
			 0x21f467cb, /// 0xabc
			 0x5d6ccb9f, /// 0xac0
			 0x9e02a030, /// 0xac4
			 0xda5af89f, /// 0xac8
			 0xf4bd1f6e, /// 0xacc
			 0xc8d0df5d, /// 0xad0
			 0x47f5b486, /// 0xad4
			 0xb55decbe, /// 0xad8
			 0x43ed67c8, /// 0xadc
			 0x510a0d71, /// 0xae0
			 0x18a02070, /// 0xae4
			 0xed925490, /// 0xae8
			 0xa89be3c6, /// 0xaec
			 0xa9442895, /// 0xaf0
			 0x6e661f90, /// 0xaf4
			 0x0f804f47, /// 0xaf8
			 0xc22d3085, /// 0xafc
			 0x45c79515, /// 0xb00
			 0x638c27d5, /// 0xb04
			 0x06b59015, /// 0xb08
			 0xbb78136d, /// 0xb0c
			 0x7be02c27, /// 0xb10
			 0x686321d8, /// 0xb14
			 0x4c388bad, /// 0xb18
			 0xe211fef5, /// 0xb1c
			 0x8e06e529, /// 0xb20
			 0xab4c33b0, /// 0xb24
			 0x6a43555d, /// 0xb28
			 0xcee87a33, /// 0xb2c
			 0x236e57ff, /// 0xb30
			 0x48ecfaba, /// 0xb34
			 0x6564c773, /// 0xb38
			 0x4dd5f6f6, /// 0xb3c
			 0x762f4da8, /// 0xb40
			 0x17f94f51, /// 0xb44
			 0xc4fd5d54, /// 0xb48
			 0x82efe73d, /// 0xb4c
			 0x71d60159, /// 0xb50
			 0x412e542a, /// 0xb54
			 0x24856cee, /// 0xb58
			 0xb5e47c14, /// 0xb5c
			 0xfc43dff6, /// 0xb60
			 0xfebc9681, /// 0xb64
			 0xc19bb057, /// 0xb68
			 0xf18b45e2, /// 0xb6c
			 0xcbd92d8d, /// 0xb70
			 0x3da09fe1, /// 0xb74
			 0x2d8c0e83, /// 0xb78
			 0x6bc29c8d, /// 0xb7c
			 0x3e31b3a3, /// 0xb80
			 0xe2894b54, /// 0xb84
			 0xd8ed5979, /// 0xb88
			 0x549441a2, /// 0xb8c
			 0x11e677fd, /// 0xb90
			 0x424d0bf9, /// 0xb94
			 0xc4732e61, /// 0xb98
			 0x982b5fa3, /// 0xb9c
			 0xe1f3131c, /// 0xba0
			 0x3ece3d89, /// 0xba4
			 0x8f6e1aa0, /// 0xba8
			 0x34f380fe, /// 0xbac
			 0xbc48020a, /// 0xbb0
			 0xd9534a40, /// 0xbb4
			 0x96a37661, /// 0xbb8
			 0xa3724ec9, /// 0xbbc
			 0x3a0d07fb, /// 0xbc0
			 0x03ee6469, /// 0xbc4
			 0xccf62cf1, /// 0xbc8
			 0x0c50bce8, /// 0xbcc
			 0x1654e7be, /// 0xbd0
			 0x8d134d58, /// 0xbd4
			 0xc313eaae, /// 0xbd8
			 0x727e7a43, /// 0xbdc
			 0xd1763752, /// 0xbe0
			 0x7bf17bd5, /// 0xbe4
			 0x3ecf982c, /// 0xbe8
			 0x3133a111, /// 0xbec
			 0x2035ee7e, /// 0xbf0
			 0x8134330f, /// 0xbf4
			 0x41451389, /// 0xbf8
			 0x17be784b, /// 0xbfc
			 0xddf07b17, /// 0xc00
			 0xa2aeb558, /// 0xc04
			 0xa1cda885, /// 0xc08
			 0xc20fc973, /// 0xc0c
			 0x3d613e7e, /// 0xc10
			 0x2346be65, /// 0xc14
			 0xeb08aa18, /// 0xc18
			 0xc3bcdc48, /// 0xc1c
			 0x1413576e, /// 0xc20
			 0x8f9dbb81, /// 0xc24
			 0x24f8e1f9, /// 0xc28
			 0x77afcbf3, /// 0xc2c
			 0xeeec2990, /// 0xc30
			 0xfdb6c61d, /// 0xc34
			 0xa6ec39ad, /// 0xc38
			 0xe10940c0, /// 0xc3c
			 0x534fce66, /// 0xc40
			 0xf9e07af4, /// 0xc44
			 0xe108f169, /// 0xc48
			 0xfc968342, /// 0xc4c
			 0x90370a8e, /// 0xc50
			 0x5ad4bede, /// 0xc54
			 0x5b97ac59, /// 0xc58
			 0x4adad143, /// 0xc5c
			 0x3d0fc739, /// 0xc60
			 0xf7d49bd4, /// 0xc64
			 0x9d49832a, /// 0xc68
			 0xb93b9c52, /// 0xc6c
			 0x5996c531, /// 0xc70
			 0x80270c18, /// 0xc74
			 0x439e42b9, /// 0xc78
			 0x99240fc0, /// 0xc7c
			 0x5d53dd04, /// 0xc80
			 0x7263da11, /// 0xc84
			 0x127bff72, /// 0xc88
			 0x927c7a95, /// 0xc8c
			 0x5f6b44e3, /// 0xc90
			 0xfe73e93c, /// 0xc94
			 0xee34b7f7, /// 0xc98
			 0xc295fd0e, /// 0xc9c
			 0x7cf01907, /// 0xca0
			 0x7034b70e, /// 0xca4
			 0x89bf798e, /// 0xca8
			 0x57b3a687, /// 0xcac
			 0x72d9c9a0, /// 0xcb0
			 0x3665b0c7, /// 0xcb4
			 0x63183337, /// 0xcb8
			 0x002a4e4f, /// 0xcbc
			 0x7b5d23e7, /// 0xcc0
			 0x0cf4b395, /// 0xcc4
			 0x9d7bca15, /// 0xcc8
			 0x69895646, /// 0xccc
			 0xbe507abc, /// 0xcd0
			 0xcb5cb642, /// 0xcd4
			 0x911712e1, /// 0xcd8
			 0x9aecff45, /// 0xcdc
			 0xa57f4f60, /// 0xce0
			 0xdbee6f5d, /// 0xce4
			 0x6f901a98, /// 0xce8
			 0x3e78fbd4, /// 0xcec
			 0x488bf5e6, /// 0xcf0
			 0x730a893b, /// 0xcf4
			 0x354630f0, /// 0xcf8
			 0x6c903d3b, /// 0xcfc
			 0xb8baec17, /// 0xd00
			 0xab6b445c, /// 0xd04
			 0xa5ec6bbb, /// 0xd08
			 0xe505b49d, /// 0xd0c
			 0x8dc52de9, /// 0xd10
			 0x586148b7, /// 0xd14
			 0xc4b050d8, /// 0xd18
			 0x4e4335e5, /// 0xd1c
			 0x3b8ec312, /// 0xd20
			 0xb9f69d41, /// 0xd24
			 0x34bc1c51, /// 0xd28
			 0xa7e0edae, /// 0xd2c
			 0xdba6baba, /// 0xd30
			 0x4e6c346b, /// 0xd34
			 0x6e491e47, /// 0xd38
			 0x43f62a69, /// 0xd3c
			 0x08328aed, /// 0xd40
			 0x08867707, /// 0xd44
			 0xe2e12866, /// 0xd48
			 0x3371b98e, /// 0xd4c
			 0x85f9682e, /// 0xd50
			 0x005f62c3, /// 0xd54
			 0x170ac527, /// 0xd58
			 0x5fd6d19b, /// 0xd5c
			 0x035a8a0c, /// 0xd60
			 0x37297974, /// 0xd64
			 0x81f31c2b, /// 0xd68
			 0xb4663715, /// 0xd6c
			 0xb7eecdba, /// 0xd70
			 0xcfebf9a6, /// 0xd74
			 0xfa9aa181, /// 0xd78
			 0x1bff08da, /// 0xd7c
			 0x61d907eb, /// 0xd80
			 0xe7f701be, /// 0xd84
			 0x26ed76dd, /// 0xd88
			 0xeaef34f4, /// 0xd8c
			 0xdd79f822, /// 0xd90
			 0x497a73c2, /// 0xd94
			 0xe5be569c, /// 0xd98
			 0x66d3a4dc, /// 0xd9c
			 0xb22441a4, /// 0xda0
			 0x03b3e3b5, /// 0xda4
			 0xef0ad098, /// 0xda8
			 0xfee2f442, /// 0xdac
			 0xe4f1e61e, /// 0xdb0
			 0x06414ed8, /// 0xdb4
			 0x013410d0, /// 0xdb8
			 0xa2973a0b, /// 0xdbc
			 0x2a462684, /// 0xdc0
			 0x16784df0, /// 0xdc4
			 0xb7273285, /// 0xdc8
			 0xc90a526c, /// 0xdcc
			 0xb0303f78, /// 0xdd0
			 0x76ac1ab1, /// 0xdd4
			 0x6242ad26, /// 0xdd8
			 0xd7e1a2c6, /// 0xddc
			 0x4ebad06c, /// 0xde0
			 0x761bcd7f, /// 0xde4
			 0xb8de362d, /// 0xde8
			 0x77493222, /// 0xdec
			 0xd528481b, /// 0xdf0
			 0x2588f7e7, /// 0xdf4
			 0xa88fc739, /// 0xdf8
			 0xd77c6423, /// 0xdfc
			 0xc0e163b8, /// 0xe00
			 0xd01462c3, /// 0xe04
			 0x906ccd66, /// 0xe08
			 0x31a79530, /// 0xe0c
			 0x8750bba6, /// 0xe10
			 0x44578895, /// 0xe14
			 0x918e33d5, /// 0xe18
			 0xb77c50a0, /// 0xe1c
			 0x8005514d, /// 0xe20
			 0xb2432c65, /// 0xe24
			 0x5dc594f5, /// 0xe28
			 0xb300c7e2, /// 0xe2c
			 0xe680ace4, /// 0xe30
			 0xd02afa36, /// 0xe34
			 0xc00b0dd0, /// 0xe38
			 0xf623e6b4, /// 0xe3c
			 0xdabcd64d, /// 0xe40
			 0x0ea0bbf7, /// 0xe44
			 0x5b2c7097, /// 0xe48
			 0x6864d457, /// 0xe4c
			 0x8660513d, /// 0xe50
			 0xc3be3cef, /// 0xe54
			 0x7be048d1, /// 0xe58
			 0x11d64a9a, /// 0xe5c
			 0xafebc926, /// 0xe60
			 0xce22c898, /// 0xe64
			 0xaaa8c1e5, /// 0xe68
			 0x374642e4, /// 0xe6c
			 0x716cd75b, /// 0xe70
			 0x68ad60aa, /// 0xe74
			 0x47eade0b, /// 0xe78
			 0xad10cec0, /// 0xe7c
			 0x47724555, /// 0xe80
			 0x50397a03, /// 0xe84
			 0x75236b59, /// 0xe88
			 0xe758beeb, /// 0xe8c
			 0x54556c56, /// 0xe90
			 0x72b68d85, /// 0xe94
			 0xc20c8bbd, /// 0xe98
			 0x3cd6c470, /// 0xe9c
			 0x82f0b29e, /// 0xea0
			 0x14ebef26, /// 0xea4
			 0xf938072d, /// 0xea8
			 0xf03a1d05, /// 0xeac
			 0x3dab1ddf, /// 0xeb0
			 0xc42187a0, /// 0xeb4
			 0x6fc29587, /// 0xeb8
			 0x08013271, /// 0xebc
			 0x91ad45b6, /// 0xec0
			 0xd6724f35, /// 0xec4
			 0xcd5618ca, /// 0xec8
			 0x7aa788cf, /// 0xecc
			 0xc74792a1, /// 0xed0
			 0x0b7f7324, /// 0xed4
			 0xf48961d3, /// 0xed8
			 0xd84f0974, /// 0xedc
			 0x0d8a0c5a, /// 0xee0
			 0x8c9427e0, /// 0xee4
			 0x6a92e48c, /// 0xee8
			 0x226fe5c9, /// 0xeec
			 0x9969927f, /// 0xef0
			 0x99efde23, /// 0xef4
			 0x05096b3c, /// 0xef8
			 0x536235cb, /// 0xefc
			 0x786228e2, /// 0xf00
			 0x2f64fa1a, /// 0xf04
			 0xadb6a72f, /// 0xf08
			 0x7c91a004, /// 0xf0c
			 0x3f8b0894, /// 0xf10
			 0x4a3714d4, /// 0xf14
			 0x622dcd09, /// 0xf18
			 0x23608f6d, /// 0xf1c
			 0x007b13a5, /// 0xf20
			 0x04a5d1c7, /// 0xf24
			 0xed8d5e19, /// 0xf28
			 0x9b7bedd3, /// 0xf2c
			 0x9541295e, /// 0xf30
			 0x54170485, /// 0xf34
			 0x6c32420d, /// 0xf38
			 0xc52708e0, /// 0xf3c
			 0x41f24fc7, /// 0xf40
			 0xf43b061e, /// 0xf44
			 0xc32e82f1, /// 0xf48
			 0xaf414b70, /// 0xf4c
			 0x0a085529, /// 0xf50
			 0x8a361947, /// 0xf54
			 0x2de3316e, /// 0xf58
			 0xb8953bd5, /// 0xf5c
			 0xeeaf38a5, /// 0xf60
			 0xf0d15a96, /// 0xf64
			 0x4edfbf74, /// 0xf68
			 0x6430336d, /// 0xf6c
			 0x77763c2c, /// 0xf70
			 0x24c8a7cf, /// 0xf74
			 0x326584af, /// 0xf78
			 0x61f8554c, /// 0xf7c
			 0x3392df9b, /// 0xf80
			 0x17d912f2, /// 0xf84
			 0x766c92da, /// 0xf88
			 0x88199bd4, /// 0xf8c
			 0xdbc7eccd, /// 0xf90
			 0xfffe36c4, /// 0xf94
			 0xab77ba01, /// 0xf98
			 0x3613addd, /// 0xf9c
			 0x453bbd6e, /// 0xfa0
			 0xd72014e1, /// 0xfa4
			 0xd77a5f52, /// 0xfa8
			 0x66072520, /// 0xfac
			 0xd44d2b77, /// 0xfb0
			 0xf0991672, /// 0xfb4
			 0xa2f38c10, /// 0xfb8
			 0x49fc33c7, /// 0xfbc
			 0x8139fd81, /// 0xfc0
			 0x675449a1, /// 0xfc4
			 0x2b4c6ff4, /// 0xfc8
			 0x21efcaf6, /// 0xfcc
			 0xb13aa6ab, /// 0xfd0
			 0x0a507b1b, /// 0xfd4
			 0xb8b62021, /// 0xfd8
			 0x032b0e43, /// 0xfdc
			 0x34206eba, /// 0xfe0
			 0x25f659e7, /// 0xfe4
			 0x9c02b124, /// 0xfe8
			 0xeecce062, /// 0xfec
			 0xc36953e3, /// 0xff0
			 0x6119482d, /// 0xff4
			 0xb2abf380, /// 0xff8
			 0x0225f0c9 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00000
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00004
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00010
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00014
			 0x0c700000,                                                  // Leading 1s:                                 /// 00018
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00020
			 0x0e000007,                                                  // Trailing 1s:                                /// 00024
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c700000,                                                  // Leading 1s:                                 /// 00034
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00038
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0003c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0c400000,                                                  // Leading 1s:                                 /// 00044
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00048
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0004c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00050
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00054
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00058
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00060
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00064
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00068
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0006c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00070
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00078
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00080
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00084
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0008c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00090
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00094
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00098
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0009c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b8
			 0xbf028f5c,                                                  // -0.51                                       /// 000bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00100
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00104
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00108
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00110
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00114
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00124
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00128
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0012c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00134
			 0xcb000000,                                                  // -8388608.0                                  /// 00138
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00140
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00144
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00148
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0014c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0xbf028f5c,                                                  // -0.51                                       /// 00154
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0015c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00160
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00164
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00168
			 0x00011111,                                                  // 9.7958E-41                                  /// 0016c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00170
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00174
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00178
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0017c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00180
			 0x0c600000,                                                  // Leading 1s:                                 /// 00184
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00188
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00194
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x00011111,                                                  // 9.7958E-41                                  /// 0019c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b4
			 0x00000000,                                                  // zero                                        /// 001b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00200
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x33333333,                                                  // 4 random values                             /// 00208
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00214
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00218
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00220
			 0x00011111,                                                  // 9.7958E-41                                  /// 00224
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00228
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0022c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00234
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00238
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00248
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00250
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00254
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00258
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0025c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00260
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00268
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0026c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00270
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00274
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00278
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x33333333,                                                  // 4 random values                             /// 00280
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00284
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00288
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0028c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00290
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00294
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00298
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0029c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b4
			 0xcb000000,                                                  // -8388608.0                                  /// 002b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d0
			 0xffc00001,                                                  // -signaling NaN                              /// 002d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00300
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00304
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00308
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0030c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00318
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00324
			 0x55555555,                                                  // 4 random values                             /// 00328
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00338
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0033c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00340
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00344
			 0xbf028f5c,                                                  // -0.51                                       /// 00348
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0034c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00350
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00354
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0035c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00360
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0036c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00370
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00374
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00378
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0037c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00384
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00390
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00398
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0039c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003dc
			 0x00000000,                                                  // zero                                        /// 003e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003e4
			 0x80000000,                                                  // -zero                                       /// 003e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00404
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00408
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x7f800000,                                                  // inf                                         /// 00410
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0xff800000,                                                  // -inf                                        /// 00418
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00420
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00424
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00430
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00434
			 0x0e000001,                                                  // Trailing 1s:                                /// 00438
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00444
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00454
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0045c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00460
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00464
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00468
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00470
			 0x0c600000,                                                  // Leading 1s:                                 /// 00474
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0047c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00484
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00488
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0048c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00494
			 0x0e000001,                                                  // Trailing 1s:                                /// 00498
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00500
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00508
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00510
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00514
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00518
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00520
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00524
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00528
			 0x0c780000,                                                  // Leading 1s:                                 /// 0052c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00530
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00534
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00538
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0053c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00544
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00548
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0054c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00550
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00554
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00558
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0055c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00560
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00568
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00570
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00578
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00588
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00590
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00594
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00598
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0059c
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00600
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00604
			 0x0c400000,                                                  // Leading 1s:                                 /// 00608
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0060c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00610
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x3f028f5c,                                                  // 0.51                                        /// 00618
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0061c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0xcb000000,                                                  // -8388608.0                                  /// 00624
			 0x0e000007,                                                  // Trailing 1s:                                /// 00628
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00634
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00638
			 0x00000000,                                                  // zero                                        /// 0063c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00640
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00644
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00648
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00658
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0065c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00664
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x80000000,                                                  // -zero                                       /// 00670
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0067c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00680
			 0x00000000,                                                  // zero                                        /// 00684
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00688
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00690
			 0x00000000,                                                  // zero                                        /// 00694
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00698
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006ec
			 0xcb000000,                                                  // -8388608.0                                  /// 006f0
			 0x7f800000,                                                  // inf                                         /// 006f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00700
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00708
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0070c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00710
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0071c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00724
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0072c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00734
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00740
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00744
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0074c
			 0x4b000000,                                                  // 8388608.0                                   /// 00750
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00754
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00758
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x3f028f5c,                                                  // 0.51                                        /// 00760
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00764
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00768
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00770
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00774
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0077c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00780
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00784
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00788
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00790
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00794
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00798
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0079c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a8
			 0xbf028f5c,                                                  // -0.51                                       /// 007ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007bc
			 0xff800000,                                                  // -inf                                        /// 007c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00800
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00804
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00810
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00818
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00820
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00828
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00830
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x4b000000,                                                  // 8388608.0                                   /// 00838
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0083c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00848
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0084c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00854
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00858
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0085c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00860
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00864
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00868
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00878
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0087c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00884
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00888
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00000000,                                                  // zero                                        /// 00890
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00898
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x7fc00001,                                                  // signaling NaN                               /// 008a8
			 0xff800000,                                                  // -inf                                        /// 008ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00900
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00904
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00908
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0090c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0091c
			 0x80000000,                                                  // -zero                                       /// 00920
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00924
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00928
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0092c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00934
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00940
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00948
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00954
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00958
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0095c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00960
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00964
			 0x0c700000,                                                  // Leading 1s:                                 /// 00968
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00970
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0097c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00980
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00984
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00988
			 0xffc00001,                                                  // -signaling NaN                              /// 0098c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00998
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0099c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009ec
			 0xcb000000,                                                  // -8388608.0                                  /// 009f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a34
			 0xbf028f5c,                                                  // -0.51                                       /// 00a38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a40
			 0xcb000000,                                                  // -8388608.0                                  /// 00a44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aa0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00abc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ae4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba4
			 0x55555555,                                                  // 4 random values                             /// 00ba8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb4
			 0xff800000,                                                  // -inf                                        /// 00bb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bbc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00be4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c44
			 0x7fc00001,                                                  // signaling NaN                               /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cdc
			 0x55555555,                                                  // 4 random values                             /// 00ce0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x33333333,                                                  // 4 random values                             /// 00d30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80000000,                                                  // -zero                                       /// 00d44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d8c
			 0x55555555,                                                  // 4 random values                             /// 00d90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00da0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dac
			 0x00000000,                                                  // zero                                        /// 00db0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dcc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e1c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ea8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x55555555,                                                  // 4 random values                             /// 00ec4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ec8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ed0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ed8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f08
			 0xcb000000,                                                  // -8388608.0                                  /// 00f0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f3c
			 0x55555555,                                                  // 4 random values                             /// 00f40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f78
			 0xbf028f5c,                                                  // -0.51                                       /// 00f7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fa8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80000000,                                                  // -zero                                       /// 00fc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fcc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fdc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fe8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00800001                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00000
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00004
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00008
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00010
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00028
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0003c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00040
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00048
			 0x0c400000,                                                  // Leading 1s:                                 /// 0004c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00050
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00054
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0005c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00060
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00068
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0006c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00070
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00074
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00078
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00080
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00084
			 0x00000000,                                                  // zero                                        /// 00088
			 0x0c600000,                                                  // Leading 1s:                                 /// 0008c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00090
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00094
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x3f028f5c,                                                  // 0.51                                        /// 000c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 000d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00100
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00104
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0010c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00114
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00118
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0011c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00120
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00130
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0013c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00140
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00144
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0014c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00154
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0015c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00160
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00164
			 0x7f800000,                                                  // inf                                         /// 00168
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00180
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00184
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00188
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00194
			 0x0c600000,                                                  // Leading 1s:                                 /// 00198
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001a0
			 0x7fc00001,                                                  // signaling NaN                               /// 001a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00200
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00208
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0020c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00214
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000000,                                                  // zero                                        /// 0021c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00220
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00224
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00234
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00238
			 0x00011111,                                                  // 9.7958E-41                                  /// 0023c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00240
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00244
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00254
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00260
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00264
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0026c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00270
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x33333333,                                                  // 4 random values                             /// 00278
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0027c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00288
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00290
			 0x0c600000,                                                  // Leading 1s:                                 /// 00294
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00298
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e4
			 0xbf028f5c,                                                  // -0.51                                       /// 002e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00304
			 0x80000000,                                                  // -zero                                       /// 00308
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0030c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0031c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00324
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00328
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0032c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00330
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00334
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00000000,                                                  // zero                                        /// 0033c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00340
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00344
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00348
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00350
			 0x4b000000,                                                  // 8388608.0                                   /// 00354
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0035c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00360
			 0x3f028f5c,                                                  // 0.51                                        /// 00364
			 0x0e000001,                                                  // Trailing 1s:                                /// 00368
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00374
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00380
			 0x0c400000,                                                  // Leading 1s:                                 /// 00384
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0038c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00390
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00394
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0039c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003a4
			 0x00000000,                                                  // zero                                        /// 003a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00400
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00404
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0040c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00410
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00414
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00418
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0041c
			 0x7fc00001,                                                  // signaling NaN                               /// 00420
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00424
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0042c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00438
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00444
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80000000,                                                  // -zero                                       /// 00450
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00454
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00458
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x80011111,                                                  // -9.7958E-41                                 /// 00464
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00468
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0046c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00470
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00480
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00484
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00488
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00498
			 0x0e000007,                                                  // Trailing 1s:                                /// 0049c
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 004b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00508
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0050c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00514
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00518
			 0x7fc00001,                                                  // signaling NaN                               /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00528
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0052c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00530
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00538
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0053c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0e000003,                                                  // Trailing 1s:                                /// 00544
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00548
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0054c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0055c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00560
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00564
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00568
			 0x0e000003,                                                  // Trailing 1s:                                /// 0056c
			 0x33333333,                                                  // 4 random values                             /// 00570
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00574
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0057c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00580
			 0x00000000,                                                  // zero                                        /// 00584
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0c400000,                                                  // Leading 1s:                                 /// 0058c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00590
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0059c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x7f800000,                                                  // inf                                         /// 005e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00604
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00614
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00618
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0061c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00624
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00628
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0062c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x0c700000,                                                  // Leading 1s:                                 /// 00634
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00638
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0063c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00648
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00650
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0065c
			 0x7fc00001,                                                  // signaling NaN                               /// 00660
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00664
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0066c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00670
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00680
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00684
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0068c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00698
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x3f028f5c,                                                  // 0.51                                        /// 006ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x3f028f5c,                                                  // 0.51                                        /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00700
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0070c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00710
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00714
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x33333333,                                                  // 4 random values                             /// 00720
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00724
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0072c
			 0xff800000,                                                  // -inf                                        /// 00730
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0073c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00740
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0074c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00750
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x7f800000,                                                  // inf                                         /// 00758
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0075c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00760
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00764
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00768
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0076c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00770
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00774
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00778
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0077c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00780
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00788
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0078c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00794
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00798
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0xbf028f5c,                                                  // -0.51                                       /// 007a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x55555555,                                                  // 4 random values                             /// 007d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007e0
			 0x7f800000,                                                  // inf                                         /// 007e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00800
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x7fc00001,                                                  // signaling NaN                               /// 00808
			 0x80011111,                                                  // -9.7958E-41                                 /// 0080c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00810
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00814
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00820
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00824
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00828
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0082c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00830
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00834
			 0x80011111,                                                  // -9.7958E-41                                 /// 00838
			 0x4b000000,                                                  // 8388608.0                                   /// 0083c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00848
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00854
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00858
			 0x00000000,                                                  // zero                                        /// 0085c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00864
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00868
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0086c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00874
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00880
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00884
			 0x0e000003,                                                  // Trailing 1s:                                /// 00888
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0088c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00894
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00898
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0089c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f0
			 0xff800000,                                                  // -inf                                        /// 008f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00910
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0xffc00001,                                                  // -signaling NaN                              /// 00920
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00924
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x0c600000,                                                  // Leading 1s:                                 /// 0092c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00930
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00934
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00938
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0093c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00940
			 0x0e000003,                                                  // Trailing 1s:                                /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00948
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0094c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00954
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00958
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00964
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00968
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x00000000,                                                  // zero                                        /// 00974
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00978
			 0x80000000,                                                  // -zero                                       /// 0097c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00988
			 0x00000000,                                                  // zero                                        /// 0098c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00990
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00994
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00998
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xbf028f5c,                                                  // -0.51                                       /// 00a48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00abc
			 0xff800000,                                                  // -inf                                        /// 00ac0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ac4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ac8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00acc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b34
			 0x7fc00001,                                                  // signaling NaN                               /// 00b38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b58
			 0xbf028f5c,                                                  // -0.51                                       /// 00b5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xffc00001,                                                  // -signaling NaN                              /// 00b64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bd4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00be0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00be8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bec
			 0xff800000,                                                  // -inf                                        /// 00bf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80000000,                                                  // -zero                                       /// 00bfc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ca0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ca8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x80000000,                                                  // -zero                                       /// 00cc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ccc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0xcb000000,                                                  // -8388608.0                                  /// 00d08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d10
			 0x00000000,                                                  // zero                                        /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0xffc00001,                                                  // -signaling NaN                              /// 00d3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00da0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00da8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00db8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x7fc00001,                                                  // signaling NaN                               /// 00de0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e20
			 0x4b000000,                                                  // 8388608.0                                   /// 00e24
			 0xff800000,                                                  // -inf                                        /// 00e28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x7fc00001,                                                  // signaling NaN                               /// 00e48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e60
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ea4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec0
			 0x7f800000,                                                  // inf                                         /// 00ec4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ed0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ee4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ef0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00efc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fa4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fa8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fdc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fe4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x7f800000,                                                  // inf                                         /// 00fec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x80000000,                                                  // -zero                                       /// 00ff4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff8
			 0x0c7fc000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00004
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0xcb000000,                                                  // -8388608.0                                  /// 00018
			 0x80000000,                                                  // -zero                                       /// 0001c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00028
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0002c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00034
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00038
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00040
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00048
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0004c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00050
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00058
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0005c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00060
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00068
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00074
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00078
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00084
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00088
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e000001,                                                  // Trailing 1s:                                /// 000a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x00000000,                                                  // zero                                        /// 000f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00100
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00104
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00108
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0010c
			 0xffc00001,                                                  // -signaling NaN                              /// 00110
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00114
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00124
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00130
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00134
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0013c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00140
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00144
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0014c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00150
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xbf028f5c,                                                  // -0.51                                       /// 00158
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00160
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0016c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00174
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00178
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00180
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00184
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0018c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00194
			 0x00011111,                                                  // 9.7958E-41                                  /// 00198
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0019c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001a4
			 0x00000000,                                                  // zero                                        /// 001a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x00000000,                                                  // zero                                        /// 001d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00204
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0020c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00210
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00218
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x55555555,                                                  // 4 random values                             /// 00220
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0xffc00001,                                                  // -signaling NaN                              /// 00228
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00230
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00234
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00238
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0023c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00248
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00250
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00258
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0025c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00260
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00264
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00268
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0026c
			 0xffc00001,                                                  // -signaling NaN                              /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00274
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0e000003,                                                  // Trailing 1s:                                /// 0027c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00280
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00290
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00298
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0029c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00300
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e000007,                                                  // Trailing 1s:                                /// 00308
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0030c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00318
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0031c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00324
			 0x0c780000,                                                  // Leading 1s:                                 /// 00328
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00330
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00338
			 0x7fc00001,                                                  // signaling NaN                               /// 0033c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00340
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0xbf028f5c,                                                  // -0.51                                       /// 00350
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00354
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0035c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00360
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00364
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00368
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0036c
			 0x7fc00001,                                                  // signaling NaN                               /// 00370
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00378
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x7fc00001,                                                  // signaling NaN                               /// 00380
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00384
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00388
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0038c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00390
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003e4
			 0x7f800000,                                                  // inf                                         /// 003e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00400
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00404
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00408
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00410
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00420
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00424
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00428
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00444
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00448
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0044c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00464
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00468
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0046c
			 0x3f028f5c,                                                  // 0.51                                        /// 00470
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00480
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00484
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00488
			 0x00011111,                                                  // 9.7958E-41                                  /// 0048c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x7fc00001,                                                  // signaling NaN                               /// 00494
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0049c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a8
			 0xffc00001,                                                  // -signaling NaN                              /// 004ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004bc
			 0x7f800000,                                                  // inf                                         /// 004c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00504
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00508
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00510
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00518
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0051c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00524
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00528
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0052c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0xffc00001,                                                  // -signaling NaN                              /// 00538
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0053c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00544
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00548
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x3f028f5c,                                                  // 0.51                                        /// 00550
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00554
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0055c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00568
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0056c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00570
			 0x3f028f5c,                                                  // 0.51                                        /// 00574
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00578
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0057c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00580
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00588
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00590
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00594
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00598
			 0x0e000007,                                                  // Trailing 1s:                                /// 0059c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e0
			 0x00000000,                                                  // zero                                        /// 005e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00600
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00610
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00614
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0061c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00620
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00624
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x80011111,                                                  // -9.7958E-41                                 /// 0062c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00630
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00638
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0063c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00644
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00648
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00658
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xff800000,                                                  // -inf                                        /// 00660
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00664
			 0x0e000003,                                                  // Trailing 1s:                                /// 00668
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00674
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00678
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00680
			 0x0e000001,                                                  // Trailing 1s:                                /// 00684
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00688
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0068c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00694
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00698
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0069c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x33333333,                                                  // 4 random values                             /// 006b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00708
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0070c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00718
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00728
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0072c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00738
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0073c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00740
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00748
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00750
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x7f800000,                                                  // inf                                         /// 00758
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0075c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00764
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0076c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00770
			 0x80000000,                                                  // -zero                                       /// 00774
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0077c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0078c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00794
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00798
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0079c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x55555555,                                                  // 4 random values                             /// 007a8
			 0x80000000,                                                  // -zero                                       /// 007ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007e4
			 0x7fc00001,                                                  // signaling NaN                               /// 007e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00800
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00804
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00808
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00814
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00818
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0081c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00820
			 0x7fc00001,                                                  // signaling NaN                               /// 00824
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00828
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00830
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00834
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00838
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00844
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00848
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00850
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00858
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00860
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00870
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00874
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00878
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00884
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00888
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00894
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00898
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0089c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x3f028f5c,                                                  // 0.51                                        /// 008c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x7f800000,                                                  // inf                                         /// 008d0
			 0x3f028f5c,                                                  // 0.51                                        /// 008d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00900
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00904
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0090c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00910
			 0x33333333,                                                  // 4 random values                             /// 00914
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0091c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00924
			 0x0e000001,                                                  // Trailing 1s:                                /// 00928
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0092c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00934
			 0xbf028f5c,                                                  // -0.51                                       /// 00938
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x55555555,                                                  // 4 random values                             /// 00940
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00944
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00948
			 0x80000000,                                                  // -zero                                       /// 0094c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00950
			 0xffc00001,                                                  // -signaling NaN                              /// 00954
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0095c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00964
			 0x00011111,                                                  // 9.7958E-41                                  /// 00968
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00970
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00974
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00978
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0097c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00980
			 0x0c400000,                                                  // Leading 1s:                                 /// 00984
			 0x33333333,                                                  // 4 random values                             /// 00988
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0098c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0xcb000000,                                                  // -8388608.0                                  /// 009b0
			 0x7f800000,                                                  // inf                                         /// 009b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009cc
			 0x33333333,                                                  // 4 random values                             /// 009d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009d4
			 0xcb000000,                                                  // -8388608.0                                  /// 009d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009f4
			 0xffc00001,                                                  // -signaling NaN                              /// 009f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a28
			 0x7f800000,                                                  // inf                                         /// 00a2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x33333333,                                                  // 4 random values                             /// 00a60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f800000,                                                  // inf                                         /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa0
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00abc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ac0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ad0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ad4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00adc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ae8
			 0xff800000,                                                  // -inf                                        /// 00aec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00af4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b10
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b20
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b44
			 0x33333333,                                                  // 4 random values                             /// 00b48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b68
			 0xffc00001,                                                  // -signaling NaN                              /// 00b6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x55555555,                                                  // 4 random values                             /// 00be0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00000000,                                                  // zero                                        /// 00bf0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c18
			 0x7fc00001,                                                  // signaling NaN                               /// 00c1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c58
			 0x3f028f5c,                                                  // 0.51                                        /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0xcb000000,                                                  // -8388608.0                                  /// 00c68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c88
			 0x7fc00001,                                                  // signaling NaN                               /// 00c8c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ca8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x33333333,                                                  // 4 random values                             /// 00cc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ccc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d6c
			 0xff800000,                                                  // -inf                                        /// 00d70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80000000,                                                  // -zero                                       /// 00d84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00da4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00db0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ddc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00de0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e6c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e88
			 0x00000000,                                                  // zero                                        /// 00e8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eb4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ecc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ed0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ed4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00edc
			 0x55555555,                                                  // 4 random values                             /// 00ee0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f44
			 0xff800000,                                                  // -inf                                        /// 00f48
			 0x7fc00001,                                                  // signaling NaN                               /// 00f4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fc8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fcc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00fd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ff4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff8
			 0x80800002                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00004
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00008
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00010
			 0x80000000,                                                  // -zero                                       /// 00014
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00018
			 0x7f800000,                                                  // inf                                         /// 0001c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00020
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00024
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00028
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0002c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00030
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00034
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00038
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0003c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00050
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00058
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0005c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00064
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0006c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0007c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00084
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00088
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0008c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0xbf028f5c,                                                  // -0.51                                       /// 00098
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0009c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000a8
			 0x33333333,                                                  // 4 random values                             /// 000ac
			 0xffc00001,                                                  // -signaling NaN                              /// 000b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f4
			 0x3f028f5c,                                                  // 0.51                                        /// 000f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00104
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00108
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x0c400000,                                                  // Leading 1s:                                 /// 0011c
			 0x33333333,                                                  // 4 random values                             /// 00120
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00000000,                                                  // zero                                        /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00130
			 0x80000000,                                                  // -zero                                       /// 00134
			 0x0c780000,                                                  // Leading 1s:                                 /// 00138
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0013c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00140
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00144
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0014c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00154
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00158
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0015c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00160
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00164
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00168
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0016c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00170
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00178
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xcb000000,                                                  // -8388608.0                                  /// 00180
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00184
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00188
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0018c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00190
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00194
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00198
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0xff800000,                                                  // -inf                                        /// 001bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00200
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00204
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00208
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0020c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00214
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00218
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00220
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00224
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0022c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00230
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00234
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00238
			 0x0c600000,                                                  // Leading 1s:                                 /// 0023c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00244
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00250
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00254
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00258
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0025c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00264
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00268
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00270
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00274
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00278
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0027c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xff800000,                                                  // -inf                                        /// 00284
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00290
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0029c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c4
			 0x33333333,                                                  // 4 random values                             /// 002c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00304
			 0x0c700000,                                                  // Leading 1s:                                 /// 00308
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00310
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00318
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0031c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00320
			 0x3f028f5c,                                                  // 0.51                                        /// 00324
			 0xbf028f5c,                                                  // -0.51                                       /// 00328
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00334
			 0x0c700000,                                                  // Leading 1s:                                 /// 00338
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00340
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00350
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00354
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x55555555,                                                  // 4 random values                             /// 0035c
			 0xbf028f5c,                                                  // -0.51                                       /// 00360
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00364
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00368
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00370
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00374
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00380
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00384
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0038c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00390
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00394
			 0x00011111,                                                  // 9.7958E-41                                  /// 00398
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0039c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003ac
			 0xff800000,                                                  // -inf                                        /// 003b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00400
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00404
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00408
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0040c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00410
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00414
			 0xff800000,                                                  // -inf                                        /// 00418
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0041c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00420
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00424
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00428
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0042c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00434
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0043c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00448
			 0xffc00001,                                                  // -signaling NaN                              /// 0044c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00450
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0045c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00460
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00464
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00468
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0046c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00470
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0047c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00480
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00488
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0048c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00490
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00494
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00498
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00500
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00508
			 0x0c780000,                                                  // Leading 1s:                                 /// 0050c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00510
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00514
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x3f028f5c,                                                  // 0.51                                        /// 0051c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00524
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0052c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00530
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00534
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00538
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0053c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x7fc00001,                                                  // signaling NaN                               /// 00548
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00550
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00554
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00564
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00570
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0xbf028f5c,                                                  // -0.51                                       /// 00578
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0057c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00588
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80000000,                                                  // -zero                                       /// 00594
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00598
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xffc00001,                                                  // -signaling NaN                              /// 005f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0e000003,                                                  // Trailing 1s:                                /// 00608
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0060c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00610
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0061c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00620
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00624
			 0x33333333,                                                  // 4 random values                             /// 00628
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00630
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00634
			 0x0c400000,                                                  // Leading 1s:                                 /// 00638
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0xcb000000,                                                  // -8388608.0                                  /// 00644
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00648
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0064c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00658
			 0x00000000,                                                  // zero                                        /// 0065c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00660
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00668
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0066c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00670
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00678
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00680
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00684
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00694
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00698
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0069c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00700
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00704
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00708
			 0x0e000003,                                                  // Trailing 1s:                                /// 0070c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00710
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00714
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00718
			 0xcb000000,                                                  // -8388608.0                                  /// 0071c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00728
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0072c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00730
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00750
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00758
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0075c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00760
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0076c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c700000,                                                  // Leading 1s:                                 /// 00774
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00778
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0077c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00788
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0078c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00790
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00798
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0079c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x7fc00001,                                                  // signaling NaN                               /// 007ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0080c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00814
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0081c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00824
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00834
			 0x55555555,                                                  // 4 random values                             /// 00838
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x55555555,                                                  // 4 random values                             /// 00840
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00844
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0084c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00850
			 0x0c400000,                                                  // Leading 1s:                                 /// 00854
			 0x0c600000,                                                  // Leading 1s:                                 /// 00858
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00860
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00864
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00868
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0086c
			 0x3f028f5c,                                                  // 0.51                                        /// 00870
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00874
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0087c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00880
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00884
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x00011111,                                                  // 9.7958E-41                                  /// 0088c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00894
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00898
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0089c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e0
			 0x7fc00001,                                                  // signaling NaN                               /// 008e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00908
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00910
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00914
			 0x0e000007,                                                  // Trailing 1s:                                /// 00918
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0091c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00920
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00928
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0092c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00930
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00938
			 0x7fc00001,                                                  // signaling NaN                               /// 0093c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0xffc00001,                                                  // -signaling NaN                              /// 0094c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00950
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00954
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00958
			 0xcb000000,                                                  // -8388608.0                                  /// 0095c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00964
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0096c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x3f028f5c,                                                  // 0.51                                        /// 00974
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00978
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0097c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00980
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00984
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00988
			 0x7f800000,                                                  // inf                                         /// 0098c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00990
			 0xbf028f5c,                                                  // -0.51                                       /// 00994
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00998
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0099c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a00
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ab4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ac0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ad4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aec
			 0xbf028f5c,                                                  // -0.51                                       /// 00af0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00afc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b14
			 0x33333333,                                                  // 4 random values                             /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x33333333,                                                  // 4 random values                             /// 00b88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b94
			 0x4b000000,                                                  // 8388608.0                                   /// 00b98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x33333333,                                                  // 4 random values                             /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00be4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00be8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bec
			 0x7f800000,                                                  // inf                                         /// 00bf0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bf8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000000,                                                  // zero                                        /// 00cf4
			 0x55555555,                                                  // 4 random values                             /// 00cf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cfc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7f800000,                                                  // inf                                         /// 00d14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00da4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dd8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ddc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00de4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e00
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000000,                                                  // -zero                                       /// 00e70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000000,                                                  // -zero                                       /// 00e8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ec0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ec4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ecc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ed0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00edc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ee0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ee4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00efc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f10
			 0xff800000,                                                  // -inf                                        /// 00f14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x7f800000,                                                  // inf                                         /// 00f64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x33333333,                                                  // 4 random values                             /// 00f9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x33333333,                                                  // 4 random values                             /// 00fa8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x80020000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x00000268,
			 0x00000584,
			 0x000004cc,
			 0x00000644,
			 0x000005ec,
			 0x0000056c,
			 0x000003c8,
			 0x00000134,

			 /// vpu register f2
			 0x40c00000,
			 0x41600000,
			 0x41c00000,
			 0x41a00000,
			 0x41700000,
			 0x41d00000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f3
			 0x40e00000,
			 0x41800000,
			 0x41700000,
			 0x41e00000,
			 0x40000000,
			 0x41d80000,
			 0x41900000,
			 0x41400000,

			 /// vpu register f4
			 0x40a00000,
			 0x41200000,
			 0x41300000,
			 0x41d80000,
			 0x41700000,
			 0x41d80000,
			 0x41000000,
			 0x41a80000,

			 /// vpu register f5
			 0x40a00000,
			 0x41200000,
			 0x41300000,
			 0x41d00000,
			 0x41100000,
			 0x41b80000,
			 0x41a80000,
			 0x41f00000,

			 /// vpu register f6
			 0x40800000,
			 0x3f800000,
			 0x40e00000,
			 0x40e00000,
			 0x41d00000,
			 0x40800000,
			 0x41a00000,
			 0x41a80000,

			 /// vpu register f7
			 0x41e00000,
			 0x41f00000,
			 0x41500000,
			 0x41800000,
			 0x41a80000,
			 0x41880000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f8
			 0x41d80000,
			 0x41500000,
			 0x41980000,
			 0x41e80000,
			 0x40000000,
			 0x41d00000,
			 0x40e00000,
			 0x41d00000,

			 /// vpu register f9
			 0x42000000,
			 0x41700000,
			 0x41900000,
			 0x41b00000,
			 0x41400000,
			 0x41200000,
			 0x42000000,
			 0x40000000,

			 /// vpu register f10
			 0x41600000,
			 0x41980000,
			 0x41a00000,
			 0x41d80000,
			 0x40800000,
			 0x41200000,
			 0x41800000,
			 0x41f00000,

			 /// vpu register f11
			 0x40a00000,
			 0x41980000,
			 0x41f00000,
			 0x3f800000,
			 0x41100000,
			 0x41000000,
			 0x41200000,
			 0x41d80000,

			 /// vpu register f12
			 0x40000000,
			 0x42000000,
			 0x3f800000,
			 0x41200000,
			 0x41a80000,
			 0x41500000,
			 0x41a00000,
			 0x41000000,

			 /// vpu register f13
			 0x41d00000,
			 0x41100000,
			 0x41f00000,
			 0x41e80000,
			 0x41d00000,
			 0x41e00000,
			 0x41e80000,
			 0x41f00000,

			 /// vpu register f14
			 0x41b00000,
			 0x41980000,
			 0x40a00000,
			 0x41800000,
			 0x41a80000,
			 0x41600000,
			 0x41d80000,
			 0x41d00000,

			 /// vpu register f15
			 0x41400000,
			 0x42000000,
			 0x41a00000,
			 0x41f80000,
			 0x41900000,
			 0x41900000,
			 0x41980000,
			 0x41100000,

			 /// vpu register f16
			 0x41c80000,
			 0x41a00000,
			 0x41f00000,
			 0x41b00000,
			 0x41100000,
			 0x41b80000,
			 0x41a80000,
			 0x41800000,

			 /// vpu register f17
			 0x41d80000,
			 0x40c00000,
			 0x40e00000,
			 0x40800000,
			 0x41700000,
			 0x40800000,
			 0x41a80000,
			 0x40c00000,

			 /// vpu register f18
			 0x41b80000,
			 0x41400000,
			 0x41700000,
			 0x40a00000,
			 0x41000000,
			 0x40c00000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f19
			 0x41f00000,
			 0x40800000,
			 0x41600000,
			 0x41900000,
			 0x41b80000,
			 0x41d80000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f20
			 0x3f800000,
			 0x41c80000,
			 0x41c00000,
			 0x41d00000,
			 0x41f00000,
			 0x41200000,
			 0x41100000,
			 0x40c00000,

			 /// vpu register f21
			 0x41f00000,
			 0x41a80000,
			 0x41500000,
			 0x41800000,
			 0x41880000,
			 0x41300000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f22
			 0x41e00000,
			 0x41c80000,
			 0x41200000,
			 0x41200000,
			 0x40800000,
			 0x40a00000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f23
			 0x41e80000,
			 0x41a00000,
			 0x41b80000,
			 0x41c00000,
			 0x41400000,
			 0x41b80000,
			 0x41300000,
			 0x41a00000,

			 /// vpu register f24
			 0x41f80000,
			 0x41e80000,
			 0x40800000,
			 0x40a00000,
			 0x41200000,
			 0x41700000,
			 0x41a80000,
			 0x41900000,

			 /// vpu register f25
			 0x40e00000,
			 0x41000000,
			 0x41200000,
			 0x41e00000,
			 0x41500000,
			 0x41880000,
			 0x41500000,
			 0x41b80000,

			 /// vpu register f26
			 0x41e00000,
			 0x41a00000,
			 0x41d80000,
			 0x41c00000,
			 0x41200000,
			 0x41b00000,
			 0x40c00000,
			 0x41900000,

			 /// vpu register f27
			 0x41100000,
			 0x3f800000,
			 0x41100000,
			 0x40e00000,
			 0x41300000,
			 0x41400000,
			 0x3f800000,
			 0x41f00000,

			 /// vpu register f28
			 0x41b80000,
			 0x41980000,
			 0x40000000,
			 0x40e00000,
			 0x41000000,
			 0x41e00000,
			 0x40400000,
			 0x42000000,

			 /// vpu register f29
			 0x41200000,
			 0x41200000,
			 0x41c00000,
			 0x40a00000,
			 0x40000000,
			 0x3f800000,
			 0x41d00000,
			 0x41a80000,

			 /// vpu register f30
			 0x41f80000,
			 0x40e00000,
			 0x41a80000,
			 0x41f00000,
			 0x3f800000,
			 0x3f800000,
			 0x41400000,
			 0x40e00000,

			 /// vpu register f31
			 0x41e80000,
			 0x41a80000,
			 0x41400000,
			 0x41500000,
			 0x41a80000,
			 0x41c00000,
			 0x41e00000,
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
		"fround.ps f27, f6, rdn\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f15, f10, rup\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f8, f4, rmm\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f30, f11, rmm\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f11, f0, rup\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f15, f19, rup\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f14, f2, rdn\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f28, f27, rdn\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f9, rdn\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f0, rmm\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f15, f21, dyn\n"                           ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f15, f11, dyn\n"                           ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f25, f2, rup\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f3, f26, dyn\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f8, f14, rup\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f27, f29, rtz\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f9, f28, rne\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f20, f19, rtz\n"                           ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f4, f27, rup\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f6, f22, rmm\n"                            ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f14, f24, rne\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f9, f25, rdn\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f14, f21, rtz\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f8, f4, rtz\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f24, f24, rdn\n"                           ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f2, f12, rup\n"                            ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f12, f30, rdn\n"                           ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f28, f11, rdn\n"                           ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f27, f9, rne\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f8, f6, rtz\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f3, rup\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f9, f23, rdn\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f6, f19, rmm\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f21, f14, rne\n"                           ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f8, f23, rup\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f14, rne\n"                           ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f14, rtz\n"                           ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f25, f4, rup\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f26, f28, rup\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f16, f30, rmm\n"                           ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f2, f18, rmm\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f12, f3, rtz\n"                            ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f19, f19, rmm\n"                           ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f9, f29, rne\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f27, f13, rup\n"                           ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f12, f8, rmm\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f4, f4, dyn\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f22, rmm\n"                           ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f19, f29, rup\n"                           ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f16, f5, rtz\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f20, f17, rtz\n"                           ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f5, f21, dyn\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f27, f22, rup\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f11, f26, rup\n"                           ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f21, f10, dyn\n"                           ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f3, rdn\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f27, f9, rne\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f21, f11, dyn\n"                           ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f2, f2, rmm\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f26, f22, rmm\n"                           ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f19, rne\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f6, f17, rup\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f16, rne\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f7, rmm\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f4, f7, rne\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f3, f2, rup\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f2, f22, rtz\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f22, f27, rup\n"                           ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f24, f11, rtz\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f19, f25, rdn\n"                           ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f2, rne\n"                            ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f13, rtz\n"                           ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f4, f29, dyn\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f25, f3, rup\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f7, f0, rup\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f6, f27, rup\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f13, f24, rmm\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f22, f17, rne\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f23, f11, rup\n"                           ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f9, f14, rne\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f20, f1, dyn\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f25, f13, rne\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f25, f12, rdn\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f30, rdn\n"                           ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f8, f25, rdn\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f15, f13, rdn\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f12, f23, rdn\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f18, f13, rne\n"                           ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f9, f3, rup\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f27, f0, rdn\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f28, f28, rdn\n"                           ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f3, f4, rne\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f11, f23, rup\n"                           ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f2, f30, rmm\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f24, f27, rtz\n"                           ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f17, f22, rmm\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f4, f12, dyn\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f30, f19, rdn\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fround.ps f26, f16, rdn\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
