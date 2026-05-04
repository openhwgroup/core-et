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
			 0xbd52f09e, /// 0x0
			 0x1ba7cb98, /// 0x4
			 0xc1bd94d5, /// 0x8
			 0xc5e16a2a, /// 0xc
			 0xa4d99330, /// 0x10
			 0x20cba215, /// 0x14
			 0x614df424, /// 0x18
			 0xbe1bc13c, /// 0x1c
			 0x3b6a485e, /// 0x20
			 0xe023f3f6, /// 0x24
			 0x49ace073, /// 0x28
			 0x7053e601, /// 0x2c
			 0x25e5923a, /// 0x30
			 0x2d5a7f65, /// 0x34
			 0x08f18378, /// 0x38
			 0xb8e2af7b, /// 0x3c
			 0x0652e317, /// 0x40
			 0xea45c792, /// 0x44
			 0x4a6a6014, /// 0x48
			 0xd1b48760, /// 0x4c
			 0x492eb73c, /// 0x50
			 0x053f4899, /// 0x54
			 0x0f6da7dd, /// 0x58
			 0xc46dd3ac, /// 0x5c
			 0x262df6a0, /// 0x60
			 0x4cb0672b, /// 0x64
			 0x54a45420, /// 0x68
			 0x89a54207, /// 0x6c
			 0x9e055e0b, /// 0x70
			 0xe63d3dd3, /// 0x74
			 0xc3e9d28c, /// 0x78
			 0x7b4e8108, /// 0x7c
			 0xac80789b, /// 0x80
			 0x6c2a6749, /// 0x84
			 0x2e76864b, /// 0x88
			 0xe8e46839, /// 0x8c
			 0x41b47eb0, /// 0x90
			 0xff9dbe2b, /// 0x94
			 0x6c0ef095, /// 0x98
			 0xe011edd8, /// 0x9c
			 0xfbe9a472, /// 0xa0
			 0x598c2ab4, /// 0xa4
			 0x61b70037, /// 0xa8
			 0x34d68add, /// 0xac
			 0x3e2d6c6e, /// 0xb0
			 0xd5da21f8, /// 0xb4
			 0xa306d192, /// 0xb8
			 0xf59e41b0, /// 0xbc
			 0xf26f8a91, /// 0xc0
			 0xb41f5c55, /// 0xc4
			 0x50a78ee5, /// 0xc8
			 0xf874683e, /// 0xcc
			 0xc3ed9c8d, /// 0xd0
			 0x69dc5ea0, /// 0xd4
			 0x0f4a7474, /// 0xd8
			 0xb93a3119, /// 0xdc
			 0xd24ef459, /// 0xe0
			 0xec816b08, /// 0xe4
			 0x77a3ce9b, /// 0xe8
			 0xd28dd9c3, /// 0xec
			 0xa2433304, /// 0xf0
			 0x6b119f9a, /// 0xf4
			 0xbc51cff8, /// 0xf8
			 0x3c17899e, /// 0xfc
			 0xe7c1769f, /// 0x100
			 0xed9ef1a0, /// 0x104
			 0x21f811d6, /// 0x108
			 0xd955e5e0, /// 0x10c
			 0xee1540ef, /// 0x110
			 0xc8b4d97a, /// 0x114
			 0xcb7ef671, /// 0x118
			 0x42030fb4, /// 0x11c
			 0x5e91f3db, /// 0x120
			 0x8a226349, /// 0x124
			 0x6b66871e, /// 0x128
			 0x870df049, /// 0x12c
			 0xba4c9d43, /// 0x130
			 0x0f67bd3d, /// 0x134
			 0x59edbd74, /// 0x138
			 0xfda4c58c, /// 0x13c
			 0x49283b5d, /// 0x140
			 0xd59be1f2, /// 0x144
			 0xca9a1a24, /// 0x148
			 0x084a7279, /// 0x14c
			 0x5217a3a0, /// 0x150
			 0x716d2304, /// 0x154
			 0x2d18c22f, /// 0x158
			 0x7ec2a809, /// 0x15c
			 0xf754dd35, /// 0x160
			 0x2ecd20f3, /// 0x164
			 0x00638d04, /// 0x168
			 0xb95625f8, /// 0x16c
			 0x5d7a1942, /// 0x170
			 0xee418e9e, /// 0x174
			 0xe2dc22ec, /// 0x178
			 0x2e4b60f7, /// 0x17c
			 0x976abe79, /// 0x180
			 0xa95adc9a, /// 0x184
			 0xa300103e, /// 0x188
			 0x35d6abb5, /// 0x18c
			 0x3811151b, /// 0x190
			 0x41f11e28, /// 0x194
			 0x471ba4d1, /// 0x198
			 0x747939df, /// 0x19c
			 0x707f54fa, /// 0x1a0
			 0x94da7ac2, /// 0x1a4
			 0x7252a723, /// 0x1a8
			 0x1a9ab0d7, /// 0x1ac
			 0xb6b7bf9f, /// 0x1b0
			 0xfc2aaf98, /// 0x1b4
			 0x1770e9b3, /// 0x1b8
			 0x4e7f52ba, /// 0x1bc
			 0x65a0edf6, /// 0x1c0
			 0x6bab0a33, /// 0x1c4
			 0xc505aeb1, /// 0x1c8
			 0x65c1a7e4, /// 0x1cc
			 0x418547f6, /// 0x1d0
			 0x11b8b075, /// 0x1d4
			 0x01a0e956, /// 0x1d8
			 0x28fd4486, /// 0x1dc
			 0x7a05453a, /// 0x1e0
			 0x9cfa2a17, /// 0x1e4
			 0x114e64d2, /// 0x1e8
			 0xe32f8e4a, /// 0x1ec
			 0x0dda8c04, /// 0x1f0
			 0xa6754edf, /// 0x1f4
			 0xedd5721f, /// 0x1f8
			 0x7c4b837f, /// 0x1fc
			 0x62bcb29e, /// 0x200
			 0xf3e30c22, /// 0x204
			 0xc51e55bf, /// 0x208
			 0x0c4f1ff6, /// 0x20c
			 0xd2cabb43, /// 0x210
			 0xff4e822b, /// 0x214
			 0xc9c8038a, /// 0x218
			 0x8520dfb1, /// 0x21c
			 0x0d3d05a5, /// 0x220
			 0x12ecd831, /// 0x224
			 0xcf3d63cb, /// 0x228
			 0x0c944b28, /// 0x22c
			 0x254044a5, /// 0x230
			 0x9e820046, /// 0x234
			 0xfb4bb418, /// 0x238
			 0x11b9f491, /// 0x23c
			 0xc100be1b, /// 0x240
			 0xf5cc2f2b, /// 0x244
			 0xff2f33f6, /// 0x248
			 0x85f86003, /// 0x24c
			 0x74de38b3, /// 0x250
			 0xd2ebfae4, /// 0x254
			 0xc67beb2f, /// 0x258
			 0x3bc54611, /// 0x25c
			 0x318579a6, /// 0x260
			 0x64bd23e8, /// 0x264
			 0x304e281c, /// 0x268
			 0x269d07d2, /// 0x26c
			 0x26eb110a, /// 0x270
			 0xd7d557ef, /// 0x274
			 0xc4f7108c, /// 0x278
			 0xc7b11efc, /// 0x27c
			 0xcd8cb1e0, /// 0x280
			 0x6a3c4a84, /// 0x284
			 0x5a369b83, /// 0x288
			 0xc43f3bd7, /// 0x28c
			 0x3de3b353, /// 0x290
			 0x21cd1a4d, /// 0x294
			 0x64d22bec, /// 0x298
			 0x4003845f, /// 0x29c
			 0xb6442678, /// 0x2a0
			 0x7fea370f, /// 0x2a4
			 0x40daafec, /// 0x2a8
			 0x881ea043, /// 0x2ac
			 0x1ef32b0d, /// 0x2b0
			 0x5c4768fc, /// 0x2b4
			 0xfd1cf500, /// 0x2b8
			 0x3d8d39d3, /// 0x2bc
			 0x2f1be54a, /// 0x2c0
			 0x85264ef4, /// 0x2c4
			 0xd2a3bf33, /// 0x2c8
			 0xb4b60166, /// 0x2cc
			 0x63cc727b, /// 0x2d0
			 0xad4c8fae, /// 0x2d4
			 0xd516732b, /// 0x2d8
			 0xadcd6f90, /// 0x2dc
			 0xb1849a14, /// 0x2e0
			 0x80ec78f9, /// 0x2e4
			 0xa5dfefe0, /// 0x2e8
			 0x5a193bdf, /// 0x2ec
			 0x424bbaae, /// 0x2f0
			 0x0df4fa87, /// 0x2f4
			 0xf02aa3fe, /// 0x2f8
			 0xbd598cb4, /// 0x2fc
			 0x7e2f95f1, /// 0x300
			 0xaed5b03d, /// 0x304
			 0x79a987ac, /// 0x308
			 0x7c1fa034, /// 0x30c
			 0xe2c7f325, /// 0x310
			 0x13448946, /// 0x314
			 0x48e60526, /// 0x318
			 0xb9e88da1, /// 0x31c
			 0x6a7c9a9c, /// 0x320
			 0x727ea6c1, /// 0x324
			 0x39460e68, /// 0x328
			 0xcc7fa1df, /// 0x32c
			 0xc681a503, /// 0x330
			 0x1eecbbdf, /// 0x334
			 0x3b4e44bf, /// 0x338
			 0x47bf0086, /// 0x33c
			 0xa5dada59, /// 0x340
			 0xea05e4d8, /// 0x344
			 0x60bfff8e, /// 0x348
			 0xc6e65178, /// 0x34c
			 0x72f3703a, /// 0x350
			 0xcbf7e9cb, /// 0x354
			 0x55c18372, /// 0x358
			 0x15a5bee5, /// 0x35c
			 0xb9a711ec, /// 0x360
			 0x6762733a, /// 0x364
			 0x56bfdcde, /// 0x368
			 0x86852441, /// 0x36c
			 0xffe1967a, /// 0x370
			 0xc63f22a5, /// 0x374
			 0xbedd7c28, /// 0x378
			 0x89027d0e, /// 0x37c
			 0x996eef17, /// 0x380
			 0xbdc7d1b5, /// 0x384
			 0x379648e0, /// 0x388
			 0x1a5788d2, /// 0x38c
			 0x646dd7cd, /// 0x390
			 0xa63e0e54, /// 0x394
			 0xb4999053, /// 0x398
			 0x9d9a8f56, /// 0x39c
			 0x3aafcfec, /// 0x3a0
			 0x0e93db97, /// 0x3a4
			 0x8c001801, /// 0x3a8
			 0x92022296, /// 0x3ac
			 0x1d96f692, /// 0x3b0
			 0x610be740, /// 0x3b4
			 0x3071e449, /// 0x3b8
			 0xf5125a24, /// 0x3bc
			 0x2c5ab2a2, /// 0x3c0
			 0x8ab32d56, /// 0x3c4
			 0xdc3b53dd, /// 0x3c8
			 0x0bda4547, /// 0x3cc
			 0xfc5600c2, /// 0x3d0
			 0x00850817, /// 0x3d4
			 0x2cfb5a7e, /// 0x3d8
			 0x93eacb89, /// 0x3dc
			 0xa2339ff6, /// 0x3e0
			 0x465966a0, /// 0x3e4
			 0x605398ee, /// 0x3e8
			 0x5bafe266, /// 0x3ec
			 0x2c6bb0f7, /// 0x3f0
			 0xc0869fcf, /// 0x3f4
			 0x67b006e2, /// 0x3f8
			 0xc19ccd7e, /// 0x3fc
			 0x0714f330, /// 0x400
			 0x6bd3c1cc, /// 0x404
			 0x308a8ce1, /// 0x408
			 0x3a79e437, /// 0x40c
			 0x7b964627, /// 0x410
			 0x2acdee2c, /// 0x414
			 0x297242f9, /// 0x418
			 0x5e94935b, /// 0x41c
			 0x6d637d67, /// 0x420
			 0x593d3e86, /// 0x424
			 0x13defbe6, /// 0x428
			 0xecc95d1e, /// 0x42c
			 0x872ccd35, /// 0x430
			 0x928ffce0, /// 0x434
			 0xe1b6c1d2, /// 0x438
			 0x250fb656, /// 0x43c
			 0x70635dfa, /// 0x440
			 0x2910e875, /// 0x444
			 0xf0823ffc, /// 0x448
			 0x40055e7f, /// 0x44c
			 0x695af8fd, /// 0x450
			 0xf51136f4, /// 0x454
			 0x4f3302da, /// 0x458
			 0x943018c4, /// 0x45c
			 0xa0680651, /// 0x460
			 0x36a083ea, /// 0x464
			 0xf40a26f7, /// 0x468
			 0x7a219146, /// 0x46c
			 0x7a35423c, /// 0x470
			 0x4a2b5942, /// 0x474
			 0x6401a6c7, /// 0x478
			 0xb1ee8485, /// 0x47c
			 0xb465da95, /// 0x480
			 0x86527b41, /// 0x484
			 0xabe02424, /// 0x488
			 0x19a1de64, /// 0x48c
			 0x90d045bb, /// 0x490
			 0x2e859c8a, /// 0x494
			 0x71210073, /// 0x498
			 0x43b93775, /// 0x49c
			 0x9d9a15b8, /// 0x4a0
			 0xf6564e23, /// 0x4a4
			 0xfdba851b, /// 0x4a8
			 0x6fc3610d, /// 0x4ac
			 0xf725452a, /// 0x4b0
			 0x7d199822, /// 0x4b4
			 0xa66eabbd, /// 0x4b8
			 0x60a9066b, /// 0x4bc
			 0x31138a10, /// 0x4c0
			 0xf2406bdb, /// 0x4c4
			 0xf1ba18fe, /// 0x4c8
			 0x122c857a, /// 0x4cc
			 0xe85b1dcf, /// 0x4d0
			 0x6ee416b9, /// 0x4d4
			 0x7ae41469, /// 0x4d8
			 0x3668837f, /// 0x4dc
			 0xe9a6e18f, /// 0x4e0
			 0x3f2749ef, /// 0x4e4
			 0x2615f7db, /// 0x4e8
			 0x34ded6b6, /// 0x4ec
			 0xe250f811, /// 0x4f0
			 0x7c885a11, /// 0x4f4
			 0xf4afd9d0, /// 0x4f8
			 0xa811edb6, /// 0x4fc
			 0x3dfe4645, /// 0x500
			 0x1eee12b1, /// 0x504
			 0xe6f9e5d1, /// 0x508
			 0xad251d20, /// 0x50c
			 0x0411ebd4, /// 0x510
			 0x8ebe759a, /// 0x514
			 0x01060ae4, /// 0x518
			 0xba01fdd9, /// 0x51c
			 0xbc8df3b1, /// 0x520
			 0x15f27b9b, /// 0x524
			 0x6ad6f466, /// 0x528
			 0x3def936e, /// 0x52c
			 0x3fadb2a0, /// 0x530
			 0xcef16856, /// 0x534
			 0x41b28657, /// 0x538
			 0x20461176, /// 0x53c
			 0xb1d6c1b3, /// 0x540
			 0xeba3cdee, /// 0x544
			 0xb9b5d5bc, /// 0x548
			 0x6366799b, /// 0x54c
			 0x837fb976, /// 0x550
			 0x8ae45747, /// 0x554
			 0x4602762f, /// 0x558
			 0xf7e03170, /// 0x55c
			 0x4b4da71d, /// 0x560
			 0x2e775a9b, /// 0x564
			 0x51801afb, /// 0x568
			 0xc2c7e374, /// 0x56c
			 0x54e177bb, /// 0x570
			 0x61889ab7, /// 0x574
			 0xe9c1938d, /// 0x578
			 0x81ec063b, /// 0x57c
			 0xd6e50658, /// 0x580
			 0xfaa02af5, /// 0x584
			 0x60bc4488, /// 0x588
			 0x9dce91a5, /// 0x58c
			 0x08fcdbf2, /// 0x590
			 0xfea3ba50, /// 0x594
			 0x047cfbb1, /// 0x598
			 0x306fba63, /// 0x59c
			 0x56c7688a, /// 0x5a0
			 0xc6522b42, /// 0x5a4
			 0xe32910dc, /// 0x5a8
			 0x9c8250dd, /// 0x5ac
			 0x21ba08c2, /// 0x5b0
			 0x73070bff, /// 0x5b4
			 0x3b9003ec, /// 0x5b8
			 0x281fcad1, /// 0x5bc
			 0xe9e53837, /// 0x5c0
			 0x6fe11f34, /// 0x5c4
			 0xf56bd95f, /// 0x5c8
			 0x08186467, /// 0x5cc
			 0xc05b904e, /// 0x5d0
			 0x1ec86250, /// 0x5d4
			 0x1eaf5c21, /// 0x5d8
			 0xfbb274f1, /// 0x5dc
			 0x59bff37c, /// 0x5e0
			 0x5309d9bd, /// 0x5e4
			 0xf02e2a8b, /// 0x5e8
			 0xb4629659, /// 0x5ec
			 0x5cfba519, /// 0x5f0
			 0x47703b5e, /// 0x5f4
			 0xd2c9bccd, /// 0x5f8
			 0x0fddba6b, /// 0x5fc
			 0xfa4820d1, /// 0x600
			 0xca6209cd, /// 0x604
			 0xd05be310, /// 0x608
			 0x494a9df3, /// 0x60c
			 0x2fb7114e, /// 0x610
			 0xa1029ef8, /// 0x614
			 0x71b98e1f, /// 0x618
			 0xb15fc435, /// 0x61c
			 0x23503cf0, /// 0x620
			 0xe2cf9361, /// 0x624
			 0x3e31f3cc, /// 0x628
			 0xeaf0615e, /// 0x62c
			 0xaab11168, /// 0x630
			 0xed64bc52, /// 0x634
			 0x13230e08, /// 0x638
			 0x6955cd99, /// 0x63c
			 0x33c6c779, /// 0x640
			 0xd91ba261, /// 0x644
			 0xde1f0fb2, /// 0x648
			 0x653d9c6f, /// 0x64c
			 0x21c63f56, /// 0x650
			 0x441b80fa, /// 0x654
			 0x1ee95ef7, /// 0x658
			 0x96d3478f, /// 0x65c
			 0x4a82e6fb, /// 0x660
			 0xf222d224, /// 0x664
			 0x69537077, /// 0x668
			 0x2647942e, /// 0x66c
			 0xd0639f5e, /// 0x670
			 0x94e7f0cd, /// 0x674
			 0x3c008c6d, /// 0x678
			 0x6da6b3b2, /// 0x67c
			 0x5109fec7, /// 0x680
			 0x5e1c611a, /// 0x684
			 0xeacc6c13, /// 0x688
			 0x3b84aa23, /// 0x68c
			 0xf571811f, /// 0x690
			 0x276f1094, /// 0x694
			 0x860a2531, /// 0x698
			 0x493b2b40, /// 0x69c
			 0x6b997058, /// 0x6a0
			 0xe3290c57, /// 0x6a4
			 0x8514ea6a, /// 0x6a8
			 0x03deac85, /// 0x6ac
			 0xb52d917b, /// 0x6b0
			 0xc7593fc2, /// 0x6b4
			 0x70b146d0, /// 0x6b8
			 0xe81f073c, /// 0x6bc
			 0x5b183368, /// 0x6c0
			 0x205de417, /// 0x6c4
			 0x21b66c5d, /// 0x6c8
			 0x336f10e8, /// 0x6cc
			 0x32a4fdaa, /// 0x6d0
			 0x2cdd6d51, /// 0x6d4
			 0x7eb5ed30, /// 0x6d8
			 0x4c61d681, /// 0x6dc
			 0x22a852b2, /// 0x6e0
			 0x7ace29f8, /// 0x6e4
			 0x6e2aff96, /// 0x6e8
			 0xc8467925, /// 0x6ec
			 0x10472b14, /// 0x6f0
			 0x1aeda838, /// 0x6f4
			 0xe9b2de38, /// 0x6f8
			 0xc9cd17e4, /// 0x6fc
			 0xf32ff498, /// 0x700
			 0x2ccd48a9, /// 0x704
			 0x22403f5e, /// 0x708
			 0xffc29822, /// 0x70c
			 0x6f8fa8a0, /// 0x710
			 0xe0d6c055, /// 0x714
			 0x63aa82b0, /// 0x718
			 0xd2af18d4, /// 0x71c
			 0x23193fbf, /// 0x720
			 0x8651a417, /// 0x724
			 0x767612ba, /// 0x728
			 0xe31dfeba, /// 0x72c
			 0xc5f7ca4e, /// 0x730
			 0x5ad6fef0, /// 0x734
			 0x4931dd59, /// 0x738
			 0xaacd61a3, /// 0x73c
			 0x62291bc4, /// 0x740
			 0xed6d1f0d, /// 0x744
			 0x041656b6, /// 0x748
			 0x0d51dae6, /// 0x74c
			 0xf992f339, /// 0x750
			 0x0405118e, /// 0x754
			 0x9df0c5db, /// 0x758
			 0xd888e562, /// 0x75c
			 0xb0ebdf86, /// 0x760
			 0x5a7f78b4, /// 0x764
			 0x66fc5262, /// 0x768
			 0x392ec355, /// 0x76c
			 0xedc17751, /// 0x770
			 0x04f73bfc, /// 0x774
			 0xb168361f, /// 0x778
			 0xdb36234d, /// 0x77c
			 0x52aeecca, /// 0x780
			 0xb2c2879e, /// 0x784
			 0x09cbbcb4, /// 0x788
			 0x2e0bed05, /// 0x78c
			 0x3041af84, /// 0x790
			 0xe8b8c074, /// 0x794
			 0x45f34e78, /// 0x798
			 0xd91c2048, /// 0x79c
			 0xcc1cc551, /// 0x7a0
			 0xeebe70e2, /// 0x7a4
			 0x4a449ec9, /// 0x7a8
			 0x1b96846d, /// 0x7ac
			 0x512e6e9f, /// 0x7b0
			 0xe513f45b, /// 0x7b4
			 0xfbf57264, /// 0x7b8
			 0x02ee9510, /// 0x7bc
			 0x697b2402, /// 0x7c0
			 0x2f5e57d6, /// 0x7c4
			 0x037a0660, /// 0x7c8
			 0x4e3c6d31, /// 0x7cc
			 0xe6059253, /// 0x7d0
			 0x3b05fe52, /// 0x7d4
			 0x99c780f0, /// 0x7d8
			 0xb6ee7fea, /// 0x7dc
			 0xb8cb2b9b, /// 0x7e0
			 0x0d1d3302, /// 0x7e4
			 0xba771aae, /// 0x7e8
			 0xb77ba4de, /// 0x7ec
			 0x214fe08a, /// 0x7f0
			 0x3259fbb5, /// 0x7f4
			 0xe33d882d, /// 0x7f8
			 0x8b249189, /// 0x7fc
			 0x01168486, /// 0x800
			 0xa7967cbf, /// 0x804
			 0x06903369, /// 0x808
			 0x9c930d5b, /// 0x80c
			 0xac1958d9, /// 0x810
			 0xbac96591, /// 0x814
			 0x7112f2d9, /// 0x818
			 0x16f21b8c, /// 0x81c
			 0x18f5f017, /// 0x820
			 0xd1341ee2, /// 0x824
			 0x320d7917, /// 0x828
			 0x9e42b190, /// 0x82c
			 0x437cf39c, /// 0x830
			 0xa74f8292, /// 0x834
			 0x8f4623fd, /// 0x838
			 0x74b8527f, /// 0x83c
			 0xbdb2794f, /// 0x840
			 0x5170f2b9, /// 0x844
			 0xdd950d27, /// 0x848
			 0xf8a4d4c0, /// 0x84c
			 0x520fcd85, /// 0x850
			 0x5a208e76, /// 0x854
			 0x9d209b81, /// 0x858
			 0x75ea6053, /// 0x85c
			 0x4cfa4c12, /// 0x860
			 0x17aefe75, /// 0x864
			 0xb8db9cde, /// 0x868
			 0x62b0bee5, /// 0x86c
			 0x25feb008, /// 0x870
			 0x309d711d, /// 0x874
			 0xa374d860, /// 0x878
			 0xe88b41cf, /// 0x87c
			 0xd5a7ad7b, /// 0x880
			 0x8e70b52d, /// 0x884
			 0x3dbd0542, /// 0x888
			 0x0ac2e360, /// 0x88c
			 0x8cca8d29, /// 0x890
			 0x95fbe590, /// 0x894
			 0xd96ddc3b, /// 0x898
			 0xa721cf65, /// 0x89c
			 0x25ca82c6, /// 0x8a0
			 0x964d89c0, /// 0x8a4
			 0x544feb7a, /// 0x8a8
			 0x7d6c48d3, /// 0x8ac
			 0x19011d02, /// 0x8b0
			 0x079ac386, /// 0x8b4
			 0x3d531045, /// 0x8b8
			 0xe687dd5a, /// 0x8bc
			 0x6d630a73, /// 0x8c0
			 0x7ff2a150, /// 0x8c4
			 0x77589206, /// 0x8c8
			 0x41fa44db, /// 0x8cc
			 0x26cb4518, /// 0x8d0
			 0xa943d193, /// 0x8d4
			 0x8eb61445, /// 0x8d8
			 0xce805f66, /// 0x8dc
			 0x52d407e0, /// 0x8e0
			 0xbb539448, /// 0x8e4
			 0x19b7f02b, /// 0x8e8
			 0x478d5d4f, /// 0x8ec
			 0x6a60d359, /// 0x8f0
			 0x0d8b6f35, /// 0x8f4
			 0x8566f7c3, /// 0x8f8
			 0x380b3a62, /// 0x8fc
			 0xe25ce52d, /// 0x900
			 0x26340d1f, /// 0x904
			 0x24f401d8, /// 0x908
			 0xcb5c9a6c, /// 0x90c
			 0x04795566, /// 0x910
			 0xf632f0f5, /// 0x914
			 0xe12bc7cc, /// 0x918
			 0xa0cbc8c0, /// 0x91c
			 0x76a09188, /// 0x920
			 0x7dd52962, /// 0x924
			 0x9f70ee17, /// 0x928
			 0xd597ac3a, /// 0x92c
			 0x504737f6, /// 0x930
			 0x5340863a, /// 0x934
			 0xd7edca2f, /// 0x938
			 0x2dd8d640, /// 0x93c
			 0x91a46815, /// 0x940
			 0x8b265687, /// 0x944
			 0xb81b96b8, /// 0x948
			 0xd562de9f, /// 0x94c
			 0x294f884b, /// 0x950
			 0xe128be1d, /// 0x954
			 0x3f3d5343, /// 0x958
			 0xd2cc8c47, /// 0x95c
			 0xf6a2d334, /// 0x960
			 0x7800b2fb, /// 0x964
			 0x381fd198, /// 0x968
			 0x27ef7ec3, /// 0x96c
			 0xfb7c85e6, /// 0x970
			 0x67da6a29, /// 0x974
			 0xaa942ed9, /// 0x978
			 0x58529834, /// 0x97c
			 0x7272e11d, /// 0x980
			 0x14133f85, /// 0x984
			 0x69374bb7, /// 0x988
			 0xad0cc973, /// 0x98c
			 0xaea52b15, /// 0x990
			 0x4e8c3567, /// 0x994
			 0x06645e64, /// 0x998
			 0x97faf26a, /// 0x99c
			 0x6a6a5305, /// 0x9a0
			 0xde064455, /// 0x9a4
			 0x04b54e55, /// 0x9a8
			 0x388d62cf, /// 0x9ac
			 0x2cc7b139, /// 0x9b0
			 0x92d6270e, /// 0x9b4
			 0x00313084, /// 0x9b8
			 0xd9af6d14, /// 0x9bc
			 0x5cd6e770, /// 0x9c0
			 0x160da05a, /// 0x9c4
			 0x122f5503, /// 0x9c8
			 0x1e47872b, /// 0x9cc
			 0xb7287ae8, /// 0x9d0
			 0x40702a3a, /// 0x9d4
			 0x62df01b7, /// 0x9d8
			 0xa6441491, /// 0x9dc
			 0xda29b131, /// 0x9e0
			 0xb06fc912, /// 0x9e4
			 0x0818857f, /// 0x9e8
			 0x53480f99, /// 0x9ec
			 0xc8c37000, /// 0x9f0
			 0x1fc7f5fe, /// 0x9f4
			 0x4143ea6e, /// 0x9f8
			 0x878b271b, /// 0x9fc
			 0x0fda9af8, /// 0xa00
			 0x39d7ba2a, /// 0xa04
			 0xd1287050, /// 0xa08
			 0xf2f45944, /// 0xa0c
			 0xf134595d, /// 0xa10
			 0x168f4a88, /// 0xa14
			 0xe2b04c76, /// 0xa18
			 0x24d8b290, /// 0xa1c
			 0x167a9e46, /// 0xa20
			 0x5f8371fb, /// 0xa24
			 0xf162a2e6, /// 0xa28
			 0x0c14a27d, /// 0xa2c
			 0x166c7ef4, /// 0xa30
			 0x0c27bf21, /// 0xa34
			 0x7a951b0b, /// 0xa38
			 0x0d6c4102, /// 0xa3c
			 0x2b8e3065, /// 0xa40
			 0x15356c24, /// 0xa44
			 0xb32d5781, /// 0xa48
			 0x35690b91, /// 0xa4c
			 0xa578b571, /// 0xa50
			 0x54a66be6, /// 0xa54
			 0x8cbe285e, /// 0xa58
			 0xffa7b9b0, /// 0xa5c
			 0xe6263714, /// 0xa60
			 0x204e33f7, /// 0xa64
			 0xef2da986, /// 0xa68
			 0x4152dde8, /// 0xa6c
			 0x0d6438ef, /// 0xa70
			 0x9e33dc16, /// 0xa74
			 0x482c0134, /// 0xa78
			 0xe461d7e0, /// 0xa7c
			 0xf7b16ecd, /// 0xa80
			 0x38868645, /// 0xa84
			 0xa6599f4e, /// 0xa88
			 0xd93b821b, /// 0xa8c
			 0xb989704f, /// 0xa90
			 0x587a69f4, /// 0xa94
			 0x4f4b67ca, /// 0xa98
			 0x45220b30, /// 0xa9c
			 0xc9c37126, /// 0xaa0
			 0xa8db7cf7, /// 0xaa4
			 0xcc9a9f2d, /// 0xaa8
			 0x16a7efef, /// 0xaac
			 0x8a52dfc9, /// 0xab0
			 0x7c4689fe, /// 0xab4
			 0x8ebb7283, /// 0xab8
			 0x66f43fa0, /// 0xabc
			 0xb2a94f47, /// 0xac0
			 0x83c71aeb, /// 0xac4
			 0x44e7c9eb, /// 0xac8
			 0xa829325b, /// 0xacc
			 0x71e6352c, /// 0xad0
			 0xeb6375a1, /// 0xad4
			 0xcf58a7da, /// 0xad8
			 0x4fc96ce1, /// 0xadc
			 0xa866870f, /// 0xae0
			 0xcb57d537, /// 0xae4
			 0x32c21525, /// 0xae8
			 0xc8300987, /// 0xaec
			 0x474c050d, /// 0xaf0
			 0x728b4e83, /// 0xaf4
			 0xe2450284, /// 0xaf8
			 0xfdc3335f, /// 0xafc
			 0x89c23a6f, /// 0xb00
			 0x372828d1, /// 0xb04
			 0x944dcbfe, /// 0xb08
			 0x754dd793, /// 0xb0c
			 0xe3cd4636, /// 0xb10
			 0x511d6e36, /// 0xb14
			 0xe432f19d, /// 0xb18
			 0x4b7e6dc9, /// 0xb1c
			 0x96c66dc1, /// 0xb20
			 0x6bb231ec, /// 0xb24
			 0x3e048000, /// 0xb28
			 0x311b4eec, /// 0xb2c
			 0xad7f48b1, /// 0xb30
			 0x643f09fc, /// 0xb34
			 0x2a6c25de, /// 0xb38
			 0xb42e81a7, /// 0xb3c
			 0x7aee9977, /// 0xb40
			 0x776a5669, /// 0xb44
			 0xe31cf776, /// 0xb48
			 0xccc1e23d, /// 0xb4c
			 0x5a0ad3a8, /// 0xb50
			 0x356c68d6, /// 0xb54
			 0xa7f0763e, /// 0xb58
			 0x392247f3, /// 0xb5c
			 0x4c5d2de7, /// 0xb60
			 0x6c25402a, /// 0xb64
			 0x6bc6ba5e, /// 0xb68
			 0xb6309657, /// 0xb6c
			 0x53d7f81d, /// 0xb70
			 0xabaa2a66, /// 0xb74
			 0x421c60ad, /// 0xb78
			 0x26652948, /// 0xb7c
			 0x49c32380, /// 0xb80
			 0xabdf1161, /// 0xb84
			 0x7e8a43b0, /// 0xb88
			 0x2edc558e, /// 0xb8c
			 0x33b7e560, /// 0xb90
			 0x9f0e33ee, /// 0xb94
			 0x9e25db07, /// 0xb98
			 0xd40b8f90, /// 0xb9c
			 0xbf816f11, /// 0xba0
			 0x297c8966, /// 0xba4
			 0xf7846189, /// 0xba8
			 0xfc0a491e, /// 0xbac
			 0x5cf0fc4a, /// 0xbb0
			 0x744649d3, /// 0xbb4
			 0x85249729, /// 0xbb8
			 0x8b4ce204, /// 0xbbc
			 0x7d171695, /// 0xbc0
			 0xb49ebf5d, /// 0xbc4
			 0x5927c5d8, /// 0xbc8
			 0xe2a3c12a, /// 0xbcc
			 0x27ba07f8, /// 0xbd0
			 0xcfa17768, /// 0xbd4
			 0x4a283c6d, /// 0xbd8
			 0x83f736e1, /// 0xbdc
			 0x9eb9d66b, /// 0xbe0
			 0xcf1b5855, /// 0xbe4
			 0xa48084ec, /// 0xbe8
			 0x8a361617, /// 0xbec
			 0x976ad121, /// 0xbf0
			 0x95124c60, /// 0xbf4
			 0x5cb21ed9, /// 0xbf8
			 0xcdd833ab, /// 0xbfc
			 0x73f499e9, /// 0xc00
			 0x6e416d0c, /// 0xc04
			 0x5f6dbc4f, /// 0xc08
			 0x43d8a79e, /// 0xc0c
			 0x76a73613, /// 0xc10
			 0xa0d6dc58, /// 0xc14
			 0x7cc39c09, /// 0xc18
			 0x10d7cf63, /// 0xc1c
			 0xe08d7ade, /// 0xc20
			 0xb038e116, /// 0xc24
			 0x589b86ab, /// 0xc28
			 0xc128451f, /// 0xc2c
			 0x205730b9, /// 0xc30
			 0x16cc2904, /// 0xc34
			 0x53958460, /// 0xc38
			 0x1b52e397, /// 0xc3c
			 0x7f97f0bc, /// 0xc40
			 0xfbd807f3, /// 0xc44
			 0x47bbbd26, /// 0xc48
			 0xfe3c570a, /// 0xc4c
			 0xcd028df2, /// 0xc50
			 0x8fee67dc, /// 0xc54
			 0x046bd0ef, /// 0xc58
			 0x5ffcd797, /// 0xc5c
			 0xb17ebe29, /// 0xc60
			 0x24a462b3, /// 0xc64
			 0x15308a9d, /// 0xc68
			 0x3dfda58a, /// 0xc6c
			 0x01b65f77, /// 0xc70
			 0x6803826f, /// 0xc74
			 0xbeecdd38, /// 0xc78
			 0xbfee5264, /// 0xc7c
			 0x16392d57, /// 0xc80
			 0xaf0416dd, /// 0xc84
			 0xc929832c, /// 0xc88
			 0x5bfa005c, /// 0xc8c
			 0x3e9b9889, /// 0xc90
			 0xc4a55a90, /// 0xc94
			 0xa915faf5, /// 0xc98
			 0xeabdf976, /// 0xc9c
			 0x32ad1164, /// 0xca0
			 0xc50c9d35, /// 0xca4
			 0x1429aafc, /// 0xca8
			 0xfccd3c2e, /// 0xcac
			 0xc1b52983, /// 0xcb0
			 0xc467fa96, /// 0xcb4
			 0x909bf51a, /// 0xcb8
			 0xcd412e39, /// 0xcbc
			 0x4dcdab74, /// 0xcc0
			 0x11be7409, /// 0xcc4
			 0x2ea6c2f8, /// 0xcc8
			 0xbbc846ee, /// 0xccc
			 0x8d6ab0d5, /// 0xcd0
			 0x3d16f6f9, /// 0xcd4
			 0x18dee57d, /// 0xcd8
			 0x72260d07, /// 0xcdc
			 0x3ff26d5c, /// 0xce0
			 0xf1c236a0, /// 0xce4
			 0xbbbe3f60, /// 0xce8
			 0x65371cd4, /// 0xcec
			 0x374ceb49, /// 0xcf0
			 0x0ca6c4c6, /// 0xcf4
			 0xd0e52668, /// 0xcf8
			 0xb8e7c0c2, /// 0xcfc
			 0x7273e372, /// 0xd00
			 0xa847fb21, /// 0xd04
			 0x2cd390cc, /// 0xd08
			 0x3921bc66, /// 0xd0c
			 0x7ff44d87, /// 0xd10
			 0x561b91b8, /// 0xd14
			 0x98f47ddd, /// 0xd18
			 0xc919cfdd, /// 0xd1c
			 0xccb5b2b5, /// 0xd20
			 0x2f98e6e3, /// 0xd24
			 0xc25a972c, /// 0xd28
			 0x0c6796bc, /// 0xd2c
			 0xfa85dc4e, /// 0xd30
			 0xaf066993, /// 0xd34
			 0x22970a3b, /// 0xd38
			 0xb151f8ef, /// 0xd3c
			 0x3e27b42f, /// 0xd40
			 0xc2d38979, /// 0xd44
			 0x468ced33, /// 0xd48
			 0xcbb94090, /// 0xd4c
			 0xc1eddd36, /// 0xd50
			 0x0d35d4c8, /// 0xd54
			 0x36f1e70a, /// 0xd58
			 0xf017b871, /// 0xd5c
			 0x0b93a68b, /// 0xd60
			 0xc8f4a604, /// 0xd64
			 0x0c2af283, /// 0xd68
			 0xc08db3a9, /// 0xd6c
			 0x687ef855, /// 0xd70
			 0x50ada1cb, /// 0xd74
			 0x55b46ebf, /// 0xd78
			 0x8afc4f86, /// 0xd7c
			 0x48bb39b3, /// 0xd80
			 0xd8ac5f1e, /// 0xd84
			 0xf76eeedb, /// 0xd88
			 0x7f5b212f, /// 0xd8c
			 0x95e8a41a, /// 0xd90
			 0xb3ee37e9, /// 0xd94
			 0x4c5bd243, /// 0xd98
			 0x712e3784, /// 0xd9c
			 0x4159a637, /// 0xda0
			 0x826775c6, /// 0xda4
			 0x3c90e52e, /// 0xda8
			 0x2dd4193e, /// 0xdac
			 0x554fa9b3, /// 0xdb0
			 0x989b592c, /// 0xdb4
			 0x1c074aaa, /// 0xdb8
			 0xb3df075a, /// 0xdbc
			 0x702d3051, /// 0xdc0
			 0xff2c5482, /// 0xdc4
			 0xcf76b30c, /// 0xdc8
			 0xbb494eee, /// 0xdcc
			 0x115fc1e6, /// 0xdd0
			 0xd6ab8ceb, /// 0xdd4
			 0x478373ac, /// 0xdd8
			 0x59040872, /// 0xddc
			 0xa3c1da82, /// 0xde0
			 0xb553c542, /// 0xde4
			 0xeabd6b6e, /// 0xde8
			 0x216adbde, /// 0xdec
			 0x8e6bf8ab, /// 0xdf0
			 0x87569ed7, /// 0xdf4
			 0x5ec55efc, /// 0xdf8
			 0xf95f4cf4, /// 0xdfc
			 0xfe781eb8, /// 0xe00
			 0x17e57d00, /// 0xe04
			 0xffcf28bd, /// 0xe08
			 0x6b2e7a3f, /// 0xe0c
			 0xb56cc562, /// 0xe10
			 0x22890e60, /// 0xe14
			 0xee6f5cd2, /// 0xe18
			 0xaae85786, /// 0xe1c
			 0x252cfa0d, /// 0xe20
			 0x68bba008, /// 0xe24
			 0xab6088bc, /// 0xe28
			 0x536a129b, /// 0xe2c
			 0x6d6b0191, /// 0xe30
			 0x95eb22a4, /// 0xe34
			 0x574ffbc0, /// 0xe38
			 0x6398c61f, /// 0xe3c
			 0x86ef6573, /// 0xe40
			 0xd40f303f, /// 0xe44
			 0xfa44ea0d, /// 0xe48
			 0x99387df0, /// 0xe4c
			 0x1ee44666, /// 0xe50
			 0x1f6b9922, /// 0xe54
			 0x60d2194e, /// 0xe58
			 0xa87ba69b, /// 0xe5c
			 0x8162d879, /// 0xe60
			 0x87d5aaca, /// 0xe64
			 0xe2dd5c0a, /// 0xe68
			 0x28baabf8, /// 0xe6c
			 0x01a1094e, /// 0xe70
			 0x2cf458c0, /// 0xe74
			 0xd0fc56c5, /// 0xe78
			 0xd4d81e97, /// 0xe7c
			 0x7b231f43, /// 0xe80
			 0xe7cfb184, /// 0xe84
			 0xf9d00325, /// 0xe88
			 0xf3d809b0, /// 0xe8c
			 0x553aae79, /// 0xe90
			 0x042c1882, /// 0xe94
			 0x2d020985, /// 0xe98
			 0x8c3fd245, /// 0xe9c
			 0x55562dc6, /// 0xea0
			 0x5f9bdc12, /// 0xea4
			 0xef851757, /// 0xea8
			 0x4d422384, /// 0xeac
			 0xf6405286, /// 0xeb0
			 0x19a3be09, /// 0xeb4
			 0xcebb7642, /// 0xeb8
			 0xb36bd9e7, /// 0xebc
			 0x161748bc, /// 0xec0
			 0xa01a6cb0, /// 0xec4
			 0x415e608b, /// 0xec8
			 0x9ae59016, /// 0xecc
			 0x8d677ebe, /// 0xed0
			 0xd8edb353, /// 0xed4
			 0x64323d41, /// 0xed8
			 0xc473e71e, /// 0xedc
			 0xcbce80ca, /// 0xee0
			 0x303e0ef9, /// 0xee4
			 0x3b8cb72c, /// 0xee8
			 0xd15390bd, /// 0xeec
			 0x271c22cf, /// 0xef0
			 0x1251d973, /// 0xef4
			 0x17615257, /// 0xef8
			 0x1e9b00f2, /// 0xefc
			 0xe3827a2b, /// 0xf00
			 0x2ebf0a8b, /// 0xf04
			 0x0eb6ae4a, /// 0xf08
			 0xb6d9cf15, /// 0xf0c
			 0x7b79d161, /// 0xf10
			 0x2af63476, /// 0xf14
			 0xdecd6eae, /// 0xf18
			 0x85754be8, /// 0xf1c
			 0x67b72878, /// 0xf20
			 0x15f618e4, /// 0xf24
			 0xc87205e0, /// 0xf28
			 0xcd1cb333, /// 0xf2c
			 0x4eac2f69, /// 0xf30
			 0x7e058238, /// 0xf34
			 0xd6456498, /// 0xf38
			 0x6f9f8c7e, /// 0xf3c
			 0xe4b92d22, /// 0xf40
			 0xc4b02728, /// 0xf44
			 0x161c768a, /// 0xf48
			 0xde9a9a8c, /// 0xf4c
			 0x9673d7b0, /// 0xf50
			 0x28aee704, /// 0xf54
			 0xa4e57264, /// 0xf58
			 0xba7a9e79, /// 0xf5c
			 0xd5349bbb, /// 0xf60
			 0x4eb60398, /// 0xf64
			 0x4ca7d140, /// 0xf68
			 0x92e265cd, /// 0xf6c
			 0xfd384964, /// 0xf70
			 0x61a04c67, /// 0xf74
			 0x784da256, /// 0xf78
			 0xeecf28f5, /// 0xf7c
			 0x2116882c, /// 0xf80
			 0x32dcbc62, /// 0xf84
			 0x75329290, /// 0xf88
			 0x0b576314, /// 0xf8c
			 0x1707500a, /// 0xf90
			 0x5df7f1d9, /// 0xf94
			 0xb57bfe34, /// 0xf98
			 0x07f6d525, /// 0xf9c
			 0x31914002, /// 0xfa0
			 0xed68cb1c, /// 0xfa4
			 0x238cf20e, /// 0xfa8
			 0xaf4cbe0d, /// 0xfac
			 0xa58b14f1, /// 0xfb0
			 0x662242b9, /// 0xfb4
			 0x096a04ab, /// 0xfb8
			 0xa08c2ddc, /// 0xfbc
			 0xb1646b2f, /// 0xfc0
			 0x8512a152, /// 0xfc4
			 0x50a6e21c, /// 0xfc8
			 0xe48f439c, /// 0xfcc
			 0xeb905387, /// 0xfd0
			 0x7cd34ffc, /// 0xfd4
			 0x43c3b979, /// 0xfd8
			 0x2d905453, /// 0xfdc
			 0xdf85d264, /// 0xfe0
			 0x4a0c379e, /// 0xfe4
			 0x3f4ed5e2, /// 0xfe8
			 0x8077838d, /// 0xfec
			 0x96520a2b, /// 0xff0
			 0x69ae7b3a, /// 0xff4
			 0x73a151c7, /// 0xff8
			 0x5a245136 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00004
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00010
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00018
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0001c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00024
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00028
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0002c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0003c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00040
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00044
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0004c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00050
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00058
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0005c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00060
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00068
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0006c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00074
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0007c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00080
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00084
			 0x0e000003,                                                  // Trailing 1s:                                /// 00088
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0008c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00090
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00094
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00098
			 0x0c400000,                                                  // Leading 1s:                                 /// 0009c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00100
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00104
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00108
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00110
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00118
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0011c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00120
			 0x33333333,                                                  // 4 random values                             /// 00124
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00128
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00130
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00134
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00138
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00148
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0014c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00150
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00154
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0015c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00160
			 0xcb000000,                                                  // -8388608.0                                  /// 00164
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0016c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00178
			 0x0e000003,                                                  // Trailing 1s:                                /// 0017c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x0c600000,                                                  // Leading 1s:                                 /// 00184
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00198
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0019c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f0
			 0x80000000,                                                  // -zero                                       /// 001f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00200
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0020c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000000,                                                  // -zero                                       /// 00214
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00218
			 0x0e000003,                                                  // Trailing 1s:                                /// 0021c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xcb000000,                                                  // -8388608.0                                  /// 00224
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00228
			 0x7fc00001,                                                  // signaling NaN                               /// 0022c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00230
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00238
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0023c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00240
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0024c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00250
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00000000,                                                  // zero                                        /// 0025c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00260
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00264
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0026c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00270
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00278
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00280
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00284
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00288
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80000000,                                                  // -zero                                       /// 00290
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00300
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00304
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00308
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00310
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00314
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x3f028f5c,                                                  // 0.51                                        /// 0031c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80011111,                                                  // -9.7958E-41                                 /// 00324
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00328
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0032c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00334
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00344
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00348
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00350
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00354
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xffc00001,                                                  // -signaling NaN                              /// 0035c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00360
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00364
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0036c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00374
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00378
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00380
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00388
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0038c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00390
			 0x0c600000,                                                  // Leading 1s:                                 /// 00394
			 0x00000000,                                                  // zero                                        /// 00398
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b0
			 0xbf028f5c,                                                  // -0.51                                       /// 003b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003dc
			 0x4b000000,                                                  // 8388608.0                                   /// 003e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ec
			 0x7f800000,                                                  // inf                                         /// 003f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00408
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00410
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00414
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00418
			 0x55555555,                                                  // 4 random values                             /// 0041c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00420
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00424
			 0x0e000001,                                                  // Trailing 1s:                                /// 00428
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00430
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00434
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00438
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0043c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0xbf028f5c,                                                  // -0.51                                       /// 00444
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x55555555,                                                  // 4 random values                             /// 00450
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00458
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0045c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00464
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00468
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0046c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e000001,                                                  // Trailing 1s:                                /// 00474
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00478
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c400000,                                                  // Leading 1s:                                 /// 00484
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00488
			 0x00011111,                                                  // 9.7958E-41                                  /// 0048c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0049c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00504
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00508
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0050c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x7f800000,                                                  // inf                                         /// 0051c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00520
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00524
			 0x3f028f5c,                                                  // 0.51                                        /// 00528
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00530
			 0x33333333,                                                  // 4 random values                             /// 00534
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00538
			 0x0c400000,                                                  // Leading 1s:                                 /// 0053c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00540
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00548
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0054c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00550
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00554
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0055c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00560
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00564
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00568
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e000001,                                                  // Trailing 1s:                                /// 00574
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00580
			 0xcb000000,                                                  // -8388608.0                                  /// 00584
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00588
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0058c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00590
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0xffc00001,                                                  // -signaling NaN                              /// 00598
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0059c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00600
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00604
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00608
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0060c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00610
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x0e000001,                                                  // Trailing 1s:                                /// 00618
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00624
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0062c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0063c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00640
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00644
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0064c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x80000000,                                                  // -zero                                       /// 0065c
			 0x4b000000,                                                  // 8388608.0                                   /// 00660
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00664
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00668
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0066c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00670
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00674
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00678
			 0x0c780000,                                                  // Leading 1s:                                 /// 0067c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00680
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00684
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00688
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x7f800000,                                                  // inf                                         /// 00694
			 0x55555555,                                                  // 4 random values                             /// 00698
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0069c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b4
			 0xffc00001,                                                  // -signaling NaN                              /// 006b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0xffc00001,                                                  // -signaling NaN                              /// 006f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f4
			 0xcb000000,                                                  // -8388608.0                                  /// 006f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00700
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00704
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00708
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0070c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00710
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00714
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00718
			 0x00000000,                                                  // zero                                        /// 0071c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00724
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00728
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00730
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00744
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00748
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0074c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00750
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00758
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0075c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00764
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00768
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00770
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00774
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0077c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00784
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00788
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00790
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00794
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x4b000000,                                                  // 8388608.0                                   /// 007cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x3f028f5c,                                                  // 0.51                                        /// 007ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0080c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00814
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0081c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00820
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00828
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00834
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00838
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00840
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00844
			 0xbf028f5c,                                                  // -0.51                                       /// 00848
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00850
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00854
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0085c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00860
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0086c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00870
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00878
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00884
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00888
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00898
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x3f028f5c,                                                  // 0.51                                        /// 008e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00900
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00904
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00908
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0090c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00910
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00924
			 0x0e000001,                                                  // Trailing 1s:                                /// 00928
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0092c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00938
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00940
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00944
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00948
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0094c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00954
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x3f028f5c,                                                  // 0.51                                        /// 0095c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00964
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00968
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0096c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00970
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00974
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0c780000,                                                  // Leading 1s:                                 /// 00984
			 0xffc00001,                                                  // -signaling NaN                              /// 00988
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00990
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00994
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00998
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0099c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00000000,                                                  // zero                                        /// 00a70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ab4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00abc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ac4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ac8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00acc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ae0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ae4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ae8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00afc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b1c
			 0xff800000,                                                  // -inf                                        /// 00b20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x80000000,                                                  // -zero                                       /// 00b68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000000,                                                  // -zero                                       /// 00ba0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be0
			 0xff800000,                                                  // -inf                                        /// 00be4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0xbf028f5c,                                                  // -0.51                                       /// 00c0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c74
			 0xff800000,                                                  // -inf                                        /// 00c78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0xffc00001,                                                  // -signaling NaN                              /// 00c88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ca4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ca8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ccc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cdc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x7fc00001,                                                  // signaling NaN                               /// 00d74
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xbf028f5c,                                                  // -0.51                                       /// 00d84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d94
			 0x55555555,                                                  // 4 random values                             /// 00d98
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00da4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00da8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dbc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x55555555,                                                  // 4 random values                             /// 00de8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00df4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e4c
			 0x00000000,                                                  // zero                                        /// 00e50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80000000,                                                  // -zero                                       /// 00e68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e84
			 0x7fc00001,                                                  // signaling NaN                               /// 00e88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e8c
			 0x80000000,                                                  // -zero                                       /// 00e90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ea0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb0
			 0xff800000,                                                  // -inf                                        /// 00eb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ebc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ec0
			 0x00000000,                                                  // zero                                        /// 00ec4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ec8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ecc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000000,                                                  // zero                                        /// 00ed4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eec
			 0x7f800000,                                                  // inf                                         /// 00ef0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f30
			 0x55555555,                                                  // 4 random values                             /// 00f34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f54
			 0x3f028f5c,                                                  // 0.51                                        /// 00f58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f70
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fa0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fa8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fb0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fbc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x33333333,                                                  // 4 random values                             /// 00fc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ff8
			 0x0e00003f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00000
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0000c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00010
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00018
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00034
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0003c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00050
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x4b000000,                                                  // 8388608.0                                   /// 00058
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0005c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00060
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00068
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0006c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00070
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0007c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00080
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00084
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0008c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00090
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0009c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e0
			 0x3f028f5c,                                                  // 0.51                                        /// 000e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00100
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00104
			 0x00011111,                                                  // 9.7958E-41                                  /// 00108
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00118
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0011c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00120
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0012c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00134
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00140
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00144
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0015c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00160
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00164
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0016c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00170
			 0x7fc00001,                                                  // signaling NaN                               /// 00174
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0017c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00188
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0018c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00190
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00198
			 0x80011111,                                                  // -9.7958E-41                                 /// 0019c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c8
			 0x55555555,                                                  // 4 random values                             /// 001cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xffc00001,                                                  // -signaling NaN                              /// 001e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7fc00001,                                                  // signaling NaN                               /// 00208
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00214
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00224
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x33333333,                                                  // 4 random values                             /// 00234
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00238
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0023c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00240
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00248
			 0xffc00001,                                                  // -signaling NaN                              /// 0024c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00250
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00254
			 0x55555555,                                                  // 4 random values                             /// 00258
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00260
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00264
			 0x80011111,                                                  // -9.7958E-41                                 /// 00268
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00270
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00274
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00280
			 0x33333333,                                                  // 4 random values                             /// 00284
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00288
			 0x0c400000,                                                  // Leading 1s:                                 /// 0028c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00290
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0029c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x4b000000,                                                  // 8388608.0                                   /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00300
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00304
			 0x55555555,                                                  // 4 random values                             /// 00308
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00318
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00320
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00324
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00328
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00330
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00338
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0033c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00340
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00344
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00348
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00350
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00358
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0035c
			 0xff800000,                                                  // -inf                                        /// 00360
			 0x0c600000,                                                  // Leading 1s:                                 /// 00364
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00368
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00370
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00378
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0037c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00388
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0038c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00394
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00398
			 0xcb000000,                                                  // -8388608.0                                  /// 0039c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f8
			 0x33333333,                                                  // 4 random values                             /// 003fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00400
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00404
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00408
			 0x80011111,                                                  // -9.7958E-41                                 /// 0040c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00410
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x00011111,                                                  // 9.7958E-41                                  /// 00418
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0041c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00424
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00428
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0042c
			 0x00000000,                                                  // zero                                        /// 00430
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0c700000,                                                  // Leading 1s:                                 /// 00438
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0043c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00440
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00444
			 0x0c600000,                                                  // Leading 1s:                                 /// 00448
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0044c
			 0x7f800000,                                                  // inf                                         /// 00450
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0045c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00468
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00474
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00488
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00490
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0049c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x55555555,                                                  // 4 random values                             /// 004bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000000,                                                  // zero                                        /// 004e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00500
			 0x0c600000,                                                  // Leading 1s:                                 /// 00504
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00508
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0050c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00514
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00518
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0051c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00520
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00524
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00528
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0052c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0053c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00544
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00548
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00558
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0055c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00560
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00564
			 0xff800000,                                                  // -inf                                        /// 00568
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0056c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00574
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0057c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00584
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00590
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00594
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0c700000,                                                  // Leading 1s:                                 /// 0059c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005b4
			 0x7fc00001,                                                  // signaling NaN                               /// 005b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xcb000000,                                                  // -8388608.0                                  /// 005dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00600
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00604
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0060c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00610
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00614
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00624
			 0x7f800000,                                                  // inf                                         /// 00628
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0062c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00630
			 0x7f800000,                                                  // inf                                         /// 00634
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00638
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0063c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00640
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00648
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00650
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00658
			 0x00011111,                                                  // 9.7958E-41                                  /// 0065c
			 0x4b000000,                                                  // 8388608.0                                   /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00668
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00674
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00678
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00680
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00684
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00688
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0068c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00694
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0xff800000,                                                  // -inf                                        /// 006e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00704
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00708
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0xff800000,                                                  // -inf                                        /// 00718
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0071c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00720
			 0xffc00001,                                                  // -signaling NaN                              /// 00724
			 0x0c600000,                                                  // Leading 1s:                                 /// 00728
			 0x80011111,                                                  // -9.7958E-41                                 /// 0072c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00730
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c400000,                                                  // Leading 1s:                                 /// 00738
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0073c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00744
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00754
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0075c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00760
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00764
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0076c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00770
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00778
			 0x0c780000,                                                  // Leading 1s:                                 /// 0077c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00780
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00784
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x4b000000,                                                  // 8388608.0                                   /// 00798
			 0x0e000003,                                                  // Trailing 1s:                                /// 0079c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00808
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00810
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00814
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00818
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0081c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00828
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0082c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00834
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00838
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00840
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00844
			 0x0c400000,                                                  // Leading 1s:                                 /// 00848
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0084c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00854
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00858
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0085c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00860
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00864
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00868
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0086c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00870
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7fc00001,                                                  // signaling NaN                               /// 00880
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00888
			 0x0c400000,                                                  // Leading 1s:                                 /// 0088c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00890
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00894
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00898
			 0x00011111,                                                  // 9.7958E-41                                  /// 0089c
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d0
			 0xbf028f5c,                                                  // -0.51                                       /// 008d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00900
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00904
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00910
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00914
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00918
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00920
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0092c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00930
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00934
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00938
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00940
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00944
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00948
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00950
			 0x0c780000,                                                  // Leading 1s:                                 /// 00954
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00958
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0095c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00960
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00964
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00968
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x7fc00001,                                                  // signaling NaN                               /// 00970
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00974
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00978
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0097c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00980
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x4b000000,                                                  // 8388608.0                                   /// 00988
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00990
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00994
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00998
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009cc
			 0xff800000,                                                  // -inf                                        /// 009d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0xff800000,                                                  // -inf                                        /// 00a0c
			 0x55555555,                                                  // 4 random values                             /// 00a10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x4b000000,                                                  // 8388608.0                                   /// 00a54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a68
			 0xbf028f5c,                                                  // -0.51                                       /// 00a6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aa4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ab4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00acc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ad0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0xff800000,                                                  // -inf                                        /// 00adc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ae8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af0
			 0x4b000000,                                                  // 8388608.0                                   /// 00af4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00af8
			 0xffc00001,                                                  // -signaling NaN                              /// 00afc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x7fc00001,                                                  // signaling NaN                               /// 00b34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ba8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bc0
			 0x33333333,                                                  // 4 random values                             /// 00bc4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bf0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bf4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c8c
			 0x80000000,                                                  // -zero                                       /// 00c90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c98
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00ca0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cc4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ccc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ce0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ce4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cfc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d40
			 0xffc00001,                                                  // -signaling NaN                              /// 00d44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x33333333,                                                  // 4 random values                             /// 00d54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00da0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00da4
			 0x80000000,                                                  // -zero                                       /// 00da8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ddc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7f800000,                                                  // inf                                         /// 00e24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0x00000000,                                                  // zero                                        /// 00e80
			 0x33333333,                                                  // 4 random values                             /// 00e84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e90
			 0xff800000,                                                  // -inf                                        /// 00e94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ebc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ec0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ec4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ecc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ed4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00edc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ee4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00efc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff8
			 0x00000200                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00004
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00008
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00014
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00024
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00034
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00038
			 0xff800000,                                                  // -inf                                        /// 0003c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00040
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00048
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x4b000000,                                                  // 8388608.0                                   /// 00050
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00054
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00060
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00064
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00068
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00070
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00074
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0007c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00080
			 0x0c600000,                                                  // Leading 1s:                                 /// 00084
			 0xbf028f5c,                                                  // -0.51                                       /// 00088
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0008c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00094
			 0x4b000000,                                                  // 8388608.0                                   /// 00098
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00104
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00110
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00114
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00118
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00120
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x3f028f5c,                                                  // 0.51                                        /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00144
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00148
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0014c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00150
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00158
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0015c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00160
			 0x0e000007,                                                  // Trailing 1s:                                /// 00164
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00168
			 0x3f028f5c,                                                  // 0.51                                        /// 0016c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00170
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00174
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x7f800000,                                                  // inf                                         /// 00180
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0e000003,                                                  // Trailing 1s:                                /// 00188
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0018c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00190
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00194
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0c700000,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x7f800000,                                                  // inf                                         /// 001a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x4b000000,                                                  // 8388608.0                                   /// 001e8
			 0xff800000,                                                  // -inf                                        /// 001ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00200
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00208
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0020c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00210
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00214
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0021c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00224
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0023c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0024c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00250
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00258
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00260
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00264
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00270
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00274
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0027c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00280
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00294
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00298
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c0
			 0xbf028f5c,                                                  // -0.51                                       /// 002c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d4
			 0x4b000000,                                                  // 8388608.0                                   /// 002d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00300
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00304
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00308
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00310
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00314
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0031c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00320
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00324
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00328
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0032c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00330
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00338
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0033c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00344
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00348
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0034c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00358
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0036c
			 0x3f028f5c,                                                  // 0.51                                        /// 00370
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00374
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00378
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0037c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00388
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0038c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00390
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00398
			 0x3f028f5c,                                                  // 0.51                                        /// 0039c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0xffc00001,                                                  // -signaling NaN                              /// 003cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003d0
			 0x80000000,                                                  // -zero                                       /// 003d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00400
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00404
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0040c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00410
			 0x0e000001,                                                  // Trailing 1s:                                /// 00414
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00418
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00420
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00428
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0042c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0043c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00440
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x80011111,                                                  // -9.7958E-41                                 /// 00448
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0044c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00450
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x3f028f5c,                                                  // 0.51                                        /// 00468
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0046c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00470
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00474
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00478
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0047c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00480
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00490
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00494
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00498
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0049c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000000,                                                  // zero                                        /// 004a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00504
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00508
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00510
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00514
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0xffc00001,                                                  // -signaling NaN                              /// 0051c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00520
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00528
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0053c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00544
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0054c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00550
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00554
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00558
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00000000,                                                  // zero                                        /// 00560
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00568
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00574
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0057c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00580
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00584
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00588
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0058c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00590
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0059c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a4
			 0x7f800000,                                                  // inf                                         /// 005a8
			 0x7fc00001,                                                  // signaling NaN                               /// 005ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 005f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00600
			 0xbf028f5c,                                                  // -0.51                                       /// 00604
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00610
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00614
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00618
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0061c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00620
			 0x00000000,                                                  // zero                                        /// 00624
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00628
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0062c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00630
			 0x0e000007,                                                  // Trailing 1s:                                /// 00634
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0063c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00640
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00644
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00648
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0064c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00650
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0065c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00660
			 0x0e000001,                                                  // Trailing 1s:                                /// 00664
			 0x4b000000,                                                  // 8388608.0                                   /// 00668
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0066c
			 0xffc00001,                                                  // -signaling NaN                              /// 00670
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00674
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00678
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0067c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00680
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00688
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00694
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006b8
			 0x4b000000,                                                  // 8388608.0                                   /// 006bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00700
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00704
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00708
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0070c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00710
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xbf028f5c,                                                  // -0.51                                       /// 00718
			 0xbf028f5c,                                                  // -0.51                                       /// 0071c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00724
			 0x00000000,                                                  // zero                                        /// 00728
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0072c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00734
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00738
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00744
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00748
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0074c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00750
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00754
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00758
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00764
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00774
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0077c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00780
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00784
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00788
			 0x0c400000,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00790
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00794
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00798
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0079c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x4b000000,                                                  // 8388608.0                                   /// 007a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0xbf028f5c,                                                  // -0.51                                       /// 007d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00804
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0080c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00810
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00818
			 0x3f028f5c,                                                  // 0.51                                        /// 0081c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00820
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00824
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00830
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00834
			 0x00011111,                                                  // 9.7958E-41                                  /// 00838
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00840
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00848
			 0x0c780000,                                                  // Leading 1s:                                 /// 0084c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00850
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00854
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0c700000,                                                  // Leading 1s:                                 /// 0085c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00860
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00864
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00868
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e000007,                                                  // Trailing 1s:                                /// 00874
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00878
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x33333333,                                                  // 4 random values                             /// 00880
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00884
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00888
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0088c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00898
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0089c
			 0x0c400000,                                                  // Leading 1s:                                 /// 008a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008ec
			 0x7f800000,                                                  // inf                                         /// 008f0
			 0xbf028f5c,                                                  // -0.51                                       /// 008f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00900
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00904
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00908
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0090c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00910
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00918
			 0x4b000000,                                                  // 8388608.0                                   /// 0091c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xff800000,                                                  // -inf                                        /// 0092c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00938
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0093c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00940
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00944
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00948
			 0x80000000,                                                  // -zero                                       /// 0094c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00950
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00954
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00958
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0095c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00964
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00968
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00974
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0097c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00984
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00988
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000000,                                                  // -zero                                       /// 00990
			 0xff800000,                                                  // -inf                                        /// 00994
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0099c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009bc
			 0x33333333,                                                  // 4 random values                             /// 009c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009c8
			 0x55555555,                                                  // 4 random values                             /// 009cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00000000,                                                  // zero                                        /// 009e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a24
			 0x7f800000,                                                  // inf                                         /// 00a28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x33333333,                                                  // 4 random values                             /// 00a78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a90
			 0xffc00001,                                                  // -signaling NaN                              /// 00a94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00abc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ac8
			 0x3f028f5c,                                                  // 0.51                                        /// 00acc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ae8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b80
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b98
			 0xff800000,                                                  // -inf                                        /// 00b9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ba4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba8
			 0x33333333,                                                  // 4 random values                             /// 00bac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bb0
			 0x33333333,                                                  // 4 random values                             /// 00bb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00be4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bf4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c40
			 0x7fc00001,                                                  // signaling NaN                               /// 00c44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c94
			 0x33333333,                                                  // 4 random values                             /// 00c98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ce4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00000000,                                                  // zero                                        /// 00cec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cf4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d64
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0xff800000,                                                  // -inf                                        /// 00da4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00da8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00db4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ddc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00df0
			 0x00000000,                                                  // zero                                        /// 00df4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ea8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ec8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ecc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ed0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00edc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ee4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00efc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0xffc00001,                                                  // -signaling NaN                              /// 00f18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f20
			 0x7fc00001,                                                  // signaling NaN                               /// 00f24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x80000000,                                                  // -zero                                       /// 00f7c
			 0xff800000,                                                  // -inf                                        /// 00f80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0xcb000000,                                                  // -8388608.0                                  /// 00f9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fa4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fe0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fe4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fec
			 0x00000000,                                                  // zero                                        /// 00ff0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e003fff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x9f366402, /// 0x0
			 0xbfa2dd1f, /// 0x4
			 0xcff29ea7, /// 0x8
			 0xca3230db, /// 0xc
			 0xd7c28679, /// 0x10
			 0x40a6434a, /// 0x14
			 0x0f8da5c4, /// 0x18
			 0x7a6b24cc, /// 0x1c
			 0xd44fc478, /// 0x20
			 0x2b3f3187, /// 0x24
			 0x5783e6e3, /// 0x28
			 0xe0eef7f9, /// 0x2c
			 0xa801d0db, /// 0x30
			 0x1f4f38b3, /// 0x34
			 0xe6036a8b, /// 0x38
			 0x862f0c1a, /// 0x3c
			 0xa92ff87a, /// 0x40
			 0xff5a376f, /// 0x44
			 0x683990f6, /// 0x48
			 0xb467f1f6, /// 0x4c
			 0x43c60bc6, /// 0x50
			 0xeb409fa6, /// 0x54
			 0x38a29cdd, /// 0x58
			 0x68dffbba, /// 0x5c
			 0x1f1372e1, /// 0x60
			 0x930f70c9, /// 0x64
			 0x80021cb8, /// 0x68
			 0x5191c8e4, /// 0x6c
			 0x77fc233e, /// 0x70
			 0x193b7b7d, /// 0x74
			 0xa2814c3c, /// 0x78
			 0xa8583025, /// 0x7c
			 0xc1ef8f77, /// 0x80
			 0x29ab0316, /// 0x84
			 0x47a40ede, /// 0x88
			 0x5da17204, /// 0x8c
			 0xa8b4f7d2, /// 0x90
			 0x48d001b7, /// 0x94
			 0x060fc078, /// 0x98
			 0x41a39aba, /// 0x9c
			 0xacf1b53f, /// 0xa0
			 0x7d8610e1, /// 0xa4
			 0x973cc737, /// 0xa8
			 0x5687cee1, /// 0xac
			 0x572ee37b, /// 0xb0
			 0xd848c5ab, /// 0xb4
			 0x958a3135, /// 0xb8
			 0x44add467, /// 0xbc
			 0x16ce8b74, /// 0xc0
			 0x136b346d, /// 0xc4
			 0x5ea29099, /// 0xc8
			 0xdc659c65, /// 0xcc
			 0xbd0cfd73, /// 0xd0
			 0xabc2de77, /// 0xd4
			 0x6c71e58b, /// 0xd8
			 0xe4675c61, /// 0xdc
			 0x58d7df22, /// 0xe0
			 0x4d781677, /// 0xe4
			 0xb6379444, /// 0xe8
			 0x2a7407aa, /// 0xec
			 0x922958c9, /// 0xf0
			 0xee2cc04b, /// 0xf4
			 0x82566acc, /// 0xf8
			 0x5df4cc68, /// 0xfc
			 0xae651616, /// 0x100
			 0xb542bd12, /// 0x104
			 0xba478c90, /// 0x108
			 0x0c32b799, /// 0x10c
			 0x5fef7f3a, /// 0x110
			 0x28bd8d7a, /// 0x114
			 0x02b8597c, /// 0x118
			 0xab77f66b, /// 0x11c
			 0x195e7235, /// 0x120
			 0x882293ee, /// 0x124
			 0xe19124be, /// 0x128
			 0xead51e43, /// 0x12c
			 0x20015c7e, /// 0x130
			 0x54b46bee, /// 0x134
			 0x2037b900, /// 0x138
			 0xb2e3bc80, /// 0x13c
			 0x027df547, /// 0x140
			 0xc087df1b, /// 0x144
			 0x44ae8d8e, /// 0x148
			 0x7a350eae, /// 0x14c
			 0x4da2dfc9, /// 0x150
			 0x71c35486, /// 0x154
			 0x873fe109, /// 0x158
			 0xdcb3fb1e, /// 0x15c
			 0xc3f9f774, /// 0x160
			 0x592ab359, /// 0x164
			 0xb62110d0, /// 0x168
			 0xaa12a5f9, /// 0x16c
			 0x6931a3f2, /// 0x170
			 0x2589b365, /// 0x174
			 0xa7543056, /// 0x178
			 0x7a783705, /// 0x17c
			 0x57e02339, /// 0x180
			 0x8734d07b, /// 0x184
			 0x11414c28, /// 0x188
			 0x94a01a0d, /// 0x18c
			 0xf0f09b2d, /// 0x190
			 0xb79113f6, /// 0x194
			 0x27beb5ea, /// 0x198
			 0xbb89eb2f, /// 0x19c
			 0x1fa7d756, /// 0x1a0
			 0xa03ac870, /// 0x1a4
			 0x74121231, /// 0x1a8
			 0x86d8be7b, /// 0x1ac
			 0x81ca7b4a, /// 0x1b0
			 0x75b32b32, /// 0x1b4
			 0x1f7756ee, /// 0x1b8
			 0xad93ed2a, /// 0x1bc
			 0xd545cf2e, /// 0x1c0
			 0x872e012d, /// 0x1c4
			 0xfb13cf6c, /// 0x1c8
			 0x166fe45f, /// 0x1cc
			 0x612435f5, /// 0x1d0
			 0x1a9c59b0, /// 0x1d4
			 0xbd586c90, /// 0x1d8
			 0xddb8108d, /// 0x1dc
			 0x1c8840c5, /// 0x1e0
			 0xe43339c6, /// 0x1e4
			 0x7253fb1f, /// 0x1e8
			 0xc4b8206c, /// 0x1ec
			 0x2095bc80, /// 0x1f0
			 0xbf3ef8c1, /// 0x1f4
			 0x58ed39f6, /// 0x1f8
			 0x4b0bab8d, /// 0x1fc
			 0xd855dd43, /// 0x200
			 0x30235a7d, /// 0x204
			 0x7ce209fe, /// 0x208
			 0x9e259847, /// 0x20c
			 0xdc72ce40, /// 0x210
			 0xc8788453, /// 0x214
			 0xd9a4ad7b, /// 0x218
			 0x495331ab, /// 0x21c
			 0x7d172b11, /// 0x220
			 0x43b9805b, /// 0x224
			 0xbd68db7c, /// 0x228
			 0x63dc6de2, /// 0x22c
			 0x1eab8691, /// 0x230
			 0xcbb6cc77, /// 0x234
			 0x11d9c08d, /// 0x238
			 0x649474c9, /// 0x23c
			 0x58f426b0, /// 0x240
			 0x4ed5004e, /// 0x244
			 0x33ec9457, /// 0x248
			 0x36a32167, /// 0x24c
			 0xf674d749, /// 0x250
			 0x23387aa3, /// 0x254
			 0xb1410c92, /// 0x258
			 0xb384969d, /// 0x25c
			 0x7914162e, /// 0x260
			 0x5203db3d, /// 0x264
			 0xe7ba7403, /// 0x268
			 0xe6d805fd, /// 0x26c
			 0x68133a4e, /// 0x270
			 0xb3abfa3d, /// 0x274
			 0xfbaf3327, /// 0x278
			 0xef537da3, /// 0x27c
			 0xf8555f61, /// 0x280
			 0xe929fcd5, /// 0x284
			 0x3fb07233, /// 0x288
			 0x7179c9b3, /// 0x28c
			 0x4e968281, /// 0x290
			 0x692ef8b1, /// 0x294
			 0x866e0105, /// 0x298
			 0xfe0c4162, /// 0x29c
			 0x95b47e3c, /// 0x2a0
			 0x85e57eca, /// 0x2a4
			 0x90e40a19, /// 0x2a8
			 0x469b836c, /// 0x2ac
			 0x6035981c, /// 0x2b0
			 0x627bfca1, /// 0x2b4
			 0x2aff859e, /// 0x2b8
			 0x959e8aad, /// 0x2bc
			 0xeceef279, /// 0x2c0
			 0x9760ce1a, /// 0x2c4
			 0x804006b8, /// 0x2c8
			 0xf5005f2d, /// 0x2cc
			 0x431af444, /// 0x2d0
			 0x3d64fd39, /// 0x2d4
			 0x37d10955, /// 0x2d8
			 0xb65a9873, /// 0x2dc
			 0x43996c08, /// 0x2e0
			 0x1104f914, /// 0x2e4
			 0x0a241e39, /// 0x2e8
			 0xd08ba20d, /// 0x2ec
			 0x5ca22dce, /// 0x2f0
			 0xe33a8414, /// 0x2f4
			 0x918f302e, /// 0x2f8
			 0x506eb906, /// 0x2fc
			 0x9d523a12, /// 0x300
			 0x6670b664, /// 0x304
			 0x316db6b0, /// 0x308
			 0xa4ee1eab, /// 0x30c
			 0xa7a6fbc1, /// 0x310
			 0x0c761aa0, /// 0x314
			 0x4b5fdd00, /// 0x318
			 0x34892194, /// 0x31c
			 0xa2412de5, /// 0x320
			 0x322d9f18, /// 0x324
			 0x52b6cbf9, /// 0x328
			 0xf6e3db5e, /// 0x32c
			 0xe4a64b01, /// 0x330
			 0xb114d422, /// 0x334
			 0x7d69ade3, /// 0x338
			 0x01513e25, /// 0x33c
			 0x6e06b444, /// 0x340
			 0x65819739, /// 0x344
			 0x5d789fd6, /// 0x348
			 0xd56138c2, /// 0x34c
			 0x2efd3f2c, /// 0x350
			 0xc4886f5f, /// 0x354
			 0x67587172, /// 0x358
			 0x439025be, /// 0x35c
			 0x7304fe21, /// 0x360
			 0x2c79758a, /// 0x364
			 0xd3aca27c, /// 0x368
			 0xece90df3, /// 0x36c
			 0x1928a455, /// 0x370
			 0xa9ee9c07, /// 0x374
			 0x2b38f3d8, /// 0x378
			 0xcfa0d26e, /// 0x37c
			 0x77d1e734, /// 0x380
			 0xf0c90aab, /// 0x384
			 0x6cd8e07f, /// 0x388
			 0xb96989f1, /// 0x38c
			 0x41398d42, /// 0x390
			 0x7717e06e, /// 0x394
			 0x8b57917c, /// 0x398
			 0x56aa0ba2, /// 0x39c
			 0x8e4e9006, /// 0x3a0
			 0x0a04def0, /// 0x3a4
			 0x88ee3401, /// 0x3a8
			 0xeee7a6e1, /// 0x3ac
			 0x1be2db7a, /// 0x3b0
			 0x6f52da99, /// 0x3b4
			 0x4e648e23, /// 0x3b8
			 0x364f7d6a, /// 0x3bc
			 0xeff7f569, /// 0x3c0
			 0x308cb7e1, /// 0x3c4
			 0xbcbef8b4, /// 0x3c8
			 0xe30aa13b, /// 0x3cc
			 0x654daadd, /// 0x3d0
			 0x20945530, /// 0x3d4
			 0x1579e3b3, /// 0x3d8
			 0x0ddf167b, /// 0x3dc
			 0x3a70f16b, /// 0x3e0
			 0x661338a9, /// 0x3e4
			 0xc548aef5, /// 0x3e8
			 0x66b5c99e, /// 0x3ec
			 0x19ae6735, /// 0x3f0
			 0x222f87be, /// 0x3f4
			 0x51c01e83, /// 0x3f8
			 0x0caf72f0, /// 0x3fc
			 0xd9bbf350, /// 0x400
			 0x5829f725, /// 0x404
			 0x892688bb, /// 0x408
			 0x7542ceb4, /// 0x40c
			 0xcfd6e8b8, /// 0x410
			 0xf38dd1bf, /// 0x414
			 0xe04c2310, /// 0x418
			 0xd52b5193, /// 0x41c
			 0x1be44014, /// 0x420
			 0x102f4325, /// 0x424
			 0xf7850287, /// 0x428
			 0x7708e50f, /// 0x42c
			 0x845e1e38, /// 0x430
			 0xbb8e220d, /// 0x434
			 0x54fcb42b, /// 0x438
			 0x76946263, /// 0x43c
			 0xe1f31e21, /// 0x440
			 0x74904189, /// 0x444
			 0x6b6e9b9d, /// 0x448
			 0x0cd6cd64, /// 0x44c
			 0x7dc4dfa9, /// 0x450
			 0x20979cc0, /// 0x454
			 0x296a3c25, /// 0x458
			 0xabfe79ac, /// 0x45c
			 0xe67dbd3c, /// 0x460
			 0x2802bd98, /// 0x464
			 0x0d033425, /// 0x468
			 0xf0985018, /// 0x46c
			 0xeb2f6171, /// 0x470
			 0x206be080, /// 0x474
			 0x6ea5cd96, /// 0x478
			 0xda192f3e, /// 0x47c
			 0x57c71a1a, /// 0x480
			 0x122b0655, /// 0x484
			 0x9a008b94, /// 0x488
			 0xff7b8098, /// 0x48c
			 0x48d46cf4, /// 0x490
			 0xc2e1fb82, /// 0x494
			 0xc9f83f4e, /// 0x498
			 0x7220696a, /// 0x49c
			 0xaea38b4b, /// 0x4a0
			 0x54834989, /// 0x4a4
			 0xa4e8277c, /// 0x4a8
			 0xcbdd29f8, /// 0x4ac
			 0x4ef45865, /// 0x4b0
			 0x0d847dd5, /// 0x4b4
			 0x045f121a, /// 0x4b8
			 0xf6f45524, /// 0x4bc
			 0x7f2c3402, /// 0x4c0
			 0x777d42c7, /// 0x4c4
			 0x43794765, /// 0x4c8
			 0xe616c2b7, /// 0x4cc
			 0x21735247, /// 0x4d0
			 0x4ed7b668, /// 0x4d4
			 0x25c42e53, /// 0x4d8
			 0xd892b8ce, /// 0x4dc
			 0xdaa4cb30, /// 0x4e0
			 0x4bdf9318, /// 0x4e4
			 0x72fbe900, /// 0x4e8
			 0x0f081bef, /// 0x4ec
			 0xc9650e48, /// 0x4f0
			 0x5ae842f5, /// 0x4f4
			 0xa7f07d28, /// 0x4f8
			 0x2e887e18, /// 0x4fc
			 0x8f7fb609, /// 0x500
			 0x2106bb8e, /// 0x504
			 0x0415151a, /// 0x508
			 0x9c88bce9, /// 0x50c
			 0xab19ac8a, /// 0x510
			 0x652736db, /// 0x514
			 0x81853d85, /// 0x518
			 0x6db0af99, /// 0x51c
			 0x383ca30a, /// 0x520
			 0xed9c494d, /// 0x524
			 0x185226c8, /// 0x528
			 0x651be8eb, /// 0x52c
			 0x9a2f1d72, /// 0x530
			 0x841ff7c0, /// 0x534
			 0x5982805e, /// 0x538
			 0x2834bb7e, /// 0x53c
			 0x3574f325, /// 0x540
			 0x48bc7757, /// 0x544
			 0x3db3464b, /// 0x548
			 0x70db6002, /// 0x54c
			 0xea2ae3e6, /// 0x550
			 0xf75070d7, /// 0x554
			 0xd1eead50, /// 0x558
			 0xa7b1e4a0, /// 0x55c
			 0xab41181f, /// 0x560
			 0x3ab5ab53, /// 0x564
			 0x06c55727, /// 0x568
			 0xe936dc4b, /// 0x56c
			 0x8c511017, /// 0x570
			 0x7b638b3d, /// 0x574
			 0x3cc740f1, /// 0x578
			 0xeb1cf70d, /// 0x57c
			 0xe83ab2a3, /// 0x580
			 0x9a361813, /// 0x584
			 0x0417f305, /// 0x588
			 0xba56f266, /// 0x58c
			 0x4097a6a5, /// 0x590
			 0x4f43d910, /// 0x594
			 0xf912dfec, /// 0x598
			 0xe7bbcc4a, /// 0x59c
			 0x1120a1e4, /// 0x5a0
			 0x239fb42b, /// 0x5a4
			 0xd618f5e7, /// 0x5a8
			 0xf84b684b, /// 0x5ac
			 0x094701a8, /// 0x5b0
			 0x7ff7182a, /// 0x5b4
			 0x6e7e45bf, /// 0x5b8
			 0xb1de6e9b, /// 0x5bc
			 0x199ef4d2, /// 0x5c0
			 0x73461d02, /// 0x5c4
			 0x0cb2d718, /// 0x5c8
			 0xe83491a1, /// 0x5cc
			 0xfaeaec28, /// 0x5d0
			 0xd46f5103, /// 0x5d4
			 0xce84156c, /// 0x5d8
			 0x90f8d6a6, /// 0x5dc
			 0x516da9b4, /// 0x5e0
			 0xdc27fe7b, /// 0x5e4
			 0xea4c228b, /// 0x5e8
			 0x54d15686, /// 0x5ec
			 0x2e4be02e, /// 0x5f0
			 0xfa7b925b, /// 0x5f4
			 0x87a4e946, /// 0x5f8
			 0x9c13296f, /// 0x5fc
			 0xb00da5d3, /// 0x600
			 0xe7f7e5d0, /// 0x604
			 0xaf9b5210, /// 0x608
			 0xa45b3187, /// 0x60c
			 0x01a875ca, /// 0x610
			 0xe6aba47e, /// 0x614
			 0xa9e77f2b, /// 0x618
			 0x22b7cfce, /// 0x61c
			 0x3a089329, /// 0x620
			 0x7d3cb211, /// 0x624
			 0x9b82e135, /// 0x628
			 0x7359d702, /// 0x62c
			 0x08cdd490, /// 0x630
			 0xd7725b22, /// 0x634
			 0xfa924fa8, /// 0x638
			 0xbd358482, /// 0x63c
			 0x87aaa55c, /// 0x640
			 0xe5dc7f39, /// 0x644
			 0x51273746, /// 0x648
			 0x116b2980, /// 0x64c
			 0x2e882bf4, /// 0x650
			 0x7e5ed2d2, /// 0x654
			 0xfcd672ee, /// 0x658
			 0xa4cbce6e, /// 0x65c
			 0xb2c2d66f, /// 0x660
			 0x4bb1a944, /// 0x664
			 0xd03384a9, /// 0x668
			 0x43c89a01, /// 0x66c
			 0xdb4676c0, /// 0x670
			 0x57dae824, /// 0x674
			 0xbd556009, /// 0x678
			 0x78f27014, /// 0x67c
			 0x486e3e2e, /// 0x680
			 0x0e5ecdd9, /// 0x684
			 0x1dfae563, /// 0x688
			 0x6e586a9c, /// 0x68c
			 0xaf7aa29e, /// 0x690
			 0x41b641a6, /// 0x694
			 0x65a3282b, /// 0x698
			 0xf95807e3, /// 0x69c
			 0xa5258847, /// 0x6a0
			 0x03ce05fb, /// 0x6a4
			 0x0ed5d2e0, /// 0x6a8
			 0x9bbd5c98, /// 0x6ac
			 0x5541a82f, /// 0x6b0
			 0x5fe1126a, /// 0x6b4
			 0x1e33f924, /// 0x6b8
			 0xd3e33a5f, /// 0x6bc
			 0x17d9e87e, /// 0x6c0
			 0x8c26e710, /// 0x6c4
			 0x708fc9a9, /// 0x6c8
			 0x74403d49, /// 0x6cc
			 0x6cf7d4a2, /// 0x6d0
			 0x2cf85a92, /// 0x6d4
			 0xe67df2db, /// 0x6d8
			 0x7fd3fddd, /// 0x6dc
			 0x1c4f6e8d, /// 0x6e0
			 0x2a0cdb3a, /// 0x6e4
			 0xeaf2e54d, /// 0x6e8
			 0xf8050d16, /// 0x6ec
			 0x86add2ae, /// 0x6f0
			 0x93635cfc, /// 0x6f4
			 0x97c410df, /// 0x6f8
			 0x1ef50252, /// 0x6fc
			 0x1ac08671, /// 0x700
			 0x4c8c4a8c, /// 0x704
			 0xf992fb96, /// 0x708
			 0x6e3f7183, /// 0x70c
			 0x201a4332, /// 0x710
			 0x9104221c, /// 0x714
			 0xa04b5c24, /// 0x718
			 0x9c641fb8, /// 0x71c
			 0x28358f95, /// 0x720
			 0x4bc5da12, /// 0x724
			 0xc633e31f, /// 0x728
			 0x6164411d, /// 0x72c
			 0x568e71dd, /// 0x730
			 0xef1dedb1, /// 0x734
			 0x41ea9fb4, /// 0x738
			 0xd991cba4, /// 0x73c
			 0x600e3114, /// 0x740
			 0x8be00f5f, /// 0x744
			 0xb1761879, /// 0x748
			 0x946a0007, /// 0x74c
			 0xa4a20073, /// 0x750
			 0xa4ba66b5, /// 0x754
			 0xa55828cc, /// 0x758
			 0xc6d7dc57, /// 0x75c
			 0x497c30b8, /// 0x760
			 0xab4431d6, /// 0x764
			 0x81ca90eb, /// 0x768
			 0xdeca1699, /// 0x76c
			 0x3096c10f, /// 0x770
			 0x9ff28e45, /// 0x774
			 0x351d2eab, /// 0x778
			 0x89208136, /// 0x77c
			 0x834e8653, /// 0x780
			 0xdc56474a, /// 0x784
			 0x10b35b13, /// 0x788
			 0xa13a3560, /// 0x78c
			 0x758240c9, /// 0x790
			 0xf8baa4d2, /// 0x794
			 0xe3f2089e, /// 0x798
			 0xc6330896, /// 0x79c
			 0x678b8544, /// 0x7a0
			 0xd93a50d9, /// 0x7a4
			 0xc3bb22c1, /// 0x7a8
			 0xb296f9db, /// 0x7ac
			 0xbe4badcc, /// 0x7b0
			 0x236f4ccc, /// 0x7b4
			 0x996cdd65, /// 0x7b8
			 0x1def39a5, /// 0x7bc
			 0xae10f2f1, /// 0x7c0
			 0x062f309f, /// 0x7c4
			 0x627b5dc5, /// 0x7c8
			 0x32c98f02, /// 0x7cc
			 0x4717dac7, /// 0x7d0
			 0x647eb454, /// 0x7d4
			 0xaa02a34e, /// 0x7d8
			 0xf5346da8, /// 0x7dc
			 0x1b4ce680, /// 0x7e0
			 0x24d64ceb, /// 0x7e4
			 0x5432e20d, /// 0x7e8
			 0x355720bd, /// 0x7ec
			 0xc55a2d38, /// 0x7f0
			 0x49fad836, /// 0x7f4
			 0x5067eab3, /// 0x7f8
			 0xbc0682a4, /// 0x7fc
			 0x1bf9094d, /// 0x800
			 0xc8151cdd, /// 0x804
			 0xb79ab89d, /// 0x808
			 0x450eb924, /// 0x80c
			 0x87838eb8, /// 0x810
			 0xaa6f7a25, /// 0x814
			 0x557d273e, /// 0x818
			 0x991a6891, /// 0x81c
			 0x1b912b91, /// 0x820
			 0x0ebf82f7, /// 0x824
			 0xc7ccb7c2, /// 0x828
			 0xeda4a41c, /// 0x82c
			 0xd950adcb, /// 0x830
			 0x64738d03, /// 0x834
			 0x8714036d, /// 0x838
			 0x1b67189f, /// 0x83c
			 0x870c95d6, /// 0x840
			 0x4148c15d, /// 0x844
			 0xb0c8b7b8, /// 0x848
			 0x0144b0d0, /// 0x84c
			 0xef1bc501, /// 0x850
			 0xebafff01, /// 0x854
			 0x38a6ba24, /// 0x858
			 0x3fe47bf4, /// 0x85c
			 0x247a750f, /// 0x860
			 0x75138d59, /// 0x864
			 0x453aa961, /// 0x868
			 0x493b5848, /// 0x86c
			 0xf1716143, /// 0x870
			 0xe7a200c9, /// 0x874
			 0xd543e227, /// 0x878
			 0x1eef2b80, /// 0x87c
			 0xffbe5a39, /// 0x880
			 0x11135d24, /// 0x884
			 0xad18d510, /// 0x888
			 0x28f855f5, /// 0x88c
			 0xdb33ae68, /// 0x890
			 0xf9079d64, /// 0x894
			 0x2af13238, /// 0x898
			 0xa489defb, /// 0x89c
			 0xfd3981bc, /// 0x8a0
			 0xbf45a6e6, /// 0x8a4
			 0x9f13e415, /// 0x8a8
			 0x5d9f6c18, /// 0x8ac
			 0x97562025, /// 0x8b0
			 0xb0ed0386, /// 0x8b4
			 0xdb281e29, /// 0x8b8
			 0xcb24ad1d, /// 0x8bc
			 0x234c975c, /// 0x8c0
			 0xb182a16d, /// 0x8c4
			 0xb971c21c, /// 0x8c8
			 0x3148bebd, /// 0x8cc
			 0xb48c3985, /// 0x8d0
			 0x8de1fb50, /// 0x8d4
			 0xfdb5365b, /// 0x8d8
			 0x3270cb9d, /// 0x8dc
			 0xed8ca762, /// 0x8e0
			 0x5ab253c3, /// 0x8e4
			 0xfae18062, /// 0x8e8
			 0x631d8345, /// 0x8ec
			 0x40f96a16, /// 0x8f0
			 0x001221ff, /// 0x8f4
			 0x2f05273f, /// 0x8f8
			 0x63bb1d37, /// 0x8fc
			 0x9b815f6f, /// 0x900
			 0xbbbcf3b7, /// 0x904
			 0xe968ae41, /// 0x908
			 0x54aa10a6, /// 0x90c
			 0x3d66c740, /// 0x910
			 0xc73916b8, /// 0x914
			 0x36b96c45, /// 0x918
			 0xd8836afb, /// 0x91c
			 0x2b0cf7e3, /// 0x920
			 0x2ec111c0, /// 0x924
			 0x3fc63e6c, /// 0x928
			 0x17a5bc23, /// 0x92c
			 0xee587c72, /// 0x930
			 0x465d4ecd, /// 0x934
			 0x08a100cd, /// 0x938
			 0x612791ea, /// 0x93c
			 0x3d7e4994, /// 0x940
			 0xae573b2c, /// 0x944
			 0x6af5b2e8, /// 0x948
			 0x6e68f4c6, /// 0x94c
			 0xf2268663, /// 0x950
			 0xce0556d7, /// 0x954
			 0x572407ff, /// 0x958
			 0xa743d71a, /// 0x95c
			 0xbb6a5759, /// 0x960
			 0xf5c968cb, /// 0x964
			 0x05dde699, /// 0x968
			 0xcc1aa10b, /// 0x96c
			 0xd0937ac8, /// 0x970
			 0xa6e184b5, /// 0x974
			 0xeeaf0836, /// 0x978
			 0xb404f26e, /// 0x97c
			 0x5cbf3f59, /// 0x980
			 0x9a78659b, /// 0x984
			 0xb1aa9d6a, /// 0x988
			 0x05c7eb05, /// 0x98c
			 0x41cf6f35, /// 0x990
			 0x053b246a, /// 0x994
			 0x08190528, /// 0x998
			 0xe9351914, /// 0x99c
			 0x5cded025, /// 0x9a0
			 0xfc2a6155, /// 0x9a4
			 0x3b1fc3d0, /// 0x9a8
			 0x8ed7ecc6, /// 0x9ac
			 0xab734adb, /// 0x9b0
			 0xd0db1515, /// 0x9b4
			 0x952e580e, /// 0x9b8
			 0x6fbf12c9, /// 0x9bc
			 0xadc1d51c, /// 0x9c0
			 0x265f4528, /// 0x9c4
			 0x455dcbdc, /// 0x9c8
			 0xa07d277f, /// 0x9cc
			 0x066278cd, /// 0x9d0
			 0xea4029d3, /// 0x9d4
			 0xd1fd808a, /// 0x9d8
			 0x9d7df893, /// 0x9dc
			 0xf9f9a720, /// 0x9e0
			 0x95c702dc, /// 0x9e4
			 0xe5f5c58c, /// 0x9e8
			 0x4c45f54b, /// 0x9ec
			 0x479bb43e, /// 0x9f0
			 0x4c87c68a, /// 0x9f4
			 0x8ab51cec, /// 0x9f8
			 0x75195eb1, /// 0x9fc
			 0x55fe72a6, /// 0xa00
			 0x558e0289, /// 0xa04
			 0xffdf3db1, /// 0xa08
			 0xcd608550, /// 0xa0c
			 0x873306e1, /// 0xa10
			 0x850d43f1, /// 0xa14
			 0x40055045, /// 0xa18
			 0x70638352, /// 0xa1c
			 0x93e378fb, /// 0xa20
			 0xdaddf843, /// 0xa24
			 0xde230270, /// 0xa28
			 0x0ff06ea0, /// 0xa2c
			 0x76d87ea6, /// 0xa30
			 0xc587f8cf, /// 0xa34
			 0xc58ca73b, /// 0xa38
			 0xe800a689, /// 0xa3c
			 0xc81b8dbd, /// 0xa40
			 0x347db37b, /// 0xa44
			 0x970101d0, /// 0xa48
			 0xd193852a, /// 0xa4c
			 0xa4d83811, /// 0xa50
			 0x15b8a52a, /// 0xa54
			 0x370e2687, /// 0xa58
			 0x19705eb1, /// 0xa5c
			 0x50f11fc7, /// 0xa60
			 0x95fe1864, /// 0xa64
			 0x11206b70, /// 0xa68
			 0x2d3332d4, /// 0xa6c
			 0x61a3b612, /// 0xa70
			 0xf9ebb81c, /// 0xa74
			 0xda8c29a5, /// 0xa78
			 0x96607569, /// 0xa7c
			 0x5c86670f, /// 0xa80
			 0x1be0487c, /// 0xa84
			 0x0e1be534, /// 0xa88
			 0x161d8b13, /// 0xa8c
			 0xd9d7cdf0, /// 0xa90
			 0x5bea9754, /// 0xa94
			 0xeb51a2a3, /// 0xa98
			 0x79262c55, /// 0xa9c
			 0xd77938df, /// 0xaa0
			 0xe6c6e28b, /// 0xaa4
			 0xf0eaa4d8, /// 0xaa8
			 0x55298470, /// 0xaac
			 0xcd2fd0e8, /// 0xab0
			 0x1179c05b, /// 0xab4
			 0x8d91cb11, /// 0xab8
			 0xd602f8eb, /// 0xabc
			 0x48015fe5, /// 0xac0
			 0x749dca9d, /// 0xac4
			 0xef05d2cd, /// 0xac8
			 0xecd4cdd8, /// 0xacc
			 0x8ed4a3ef, /// 0xad0
			 0x8b459893, /// 0xad4
			 0x321b86d7, /// 0xad8
			 0x9d3aaa89, /// 0xadc
			 0xb99a4754, /// 0xae0
			 0x971253b4, /// 0xae4
			 0x1c03f87a, /// 0xae8
			 0xe9495713, /// 0xaec
			 0xb87ce98b, /// 0xaf0
			 0x1a1f0a9d, /// 0xaf4
			 0xa84ccd43, /// 0xaf8
			 0x4e42fb03, /// 0xafc
			 0x11045d1c, /// 0xb00
			 0x6878a854, /// 0xb04
			 0x97768800, /// 0xb08
			 0x31ee8b08, /// 0xb0c
			 0x4f177948, /// 0xb10
			 0xb567d90f, /// 0xb14
			 0x2f61c713, /// 0xb18
			 0xdebd6c7e, /// 0xb1c
			 0xa381e050, /// 0xb20
			 0xe6e482cf, /// 0xb24
			 0x7e5441d6, /// 0xb28
			 0xc2b1ca8b, /// 0xb2c
			 0xb7a03332, /// 0xb30
			 0xf6bf541d, /// 0xb34
			 0x9ec4cd5b, /// 0xb38
			 0x329cbb70, /// 0xb3c
			 0x80345694, /// 0xb40
			 0xc5c35370, /// 0xb44
			 0xee4bc9b5, /// 0xb48
			 0x1a675522, /// 0xb4c
			 0x528577db, /// 0xb50
			 0x45104971, /// 0xb54
			 0x4da8dfbc, /// 0xb58
			 0x536a1e9b, /// 0xb5c
			 0xa0dabd2d, /// 0xb60
			 0xe0144ffb, /// 0xb64
			 0x4f73bb5d, /// 0xb68
			 0x2dfe6f04, /// 0xb6c
			 0x18a076b2, /// 0xb70
			 0x3588f513, /// 0xb74
			 0xc0d0f7f4, /// 0xb78
			 0xc297ec82, /// 0xb7c
			 0x1263a862, /// 0xb80
			 0xad00248d, /// 0xb84
			 0x35bd1d81, /// 0xb88
			 0x64744a38, /// 0xb8c
			 0x1188363d, /// 0xb90
			 0x22946cbc, /// 0xb94
			 0x4013f5e7, /// 0xb98
			 0x5350782f, /// 0xb9c
			 0xa36a7f1b, /// 0xba0
			 0x3e792113, /// 0xba4
			 0xdb8e241e, /// 0xba8
			 0x378a96a6, /// 0xbac
			 0xb3e3578a, /// 0xbb0
			 0xd2e966bb, /// 0xbb4
			 0x34c21f36, /// 0xbb8
			 0x13d9274b, /// 0xbbc
			 0x1bf15dd8, /// 0xbc0
			 0x289f194e, /// 0xbc4
			 0x8fb08301, /// 0xbc8
			 0xb7214a67, /// 0xbcc
			 0xdf59e967, /// 0xbd0
			 0x5e2d1e51, /// 0xbd4
			 0x7eb5c003, /// 0xbd8
			 0xbb981fd7, /// 0xbdc
			 0xff62d698, /// 0xbe0
			 0x717c79f8, /// 0xbe4
			 0xc5602a5a, /// 0xbe8
			 0x87e35f33, /// 0xbec
			 0x00e41882, /// 0xbf0
			 0xb1803a2c, /// 0xbf4
			 0xee060831, /// 0xbf8
			 0x2d370ad4, /// 0xbfc
			 0x11d51696, /// 0xc00
			 0xd957b26f, /// 0xc04
			 0xd8e63edd, /// 0xc08
			 0x54d3ff58, /// 0xc0c
			 0xebfa7979, /// 0xc10
			 0xa64d3dd0, /// 0xc14
			 0x228d96d3, /// 0xc18
			 0x979f9a20, /// 0xc1c
			 0xddfc4fd9, /// 0xc20
			 0xb8ed7135, /// 0xc24
			 0xf1675386, /// 0xc28
			 0x2766709b, /// 0xc2c
			 0xc8c3b378, /// 0xc30
			 0x5ab35062, /// 0xc34
			 0xa4230830, /// 0xc38
			 0x26604a9b, /// 0xc3c
			 0xdeedaaa7, /// 0xc40
			 0xf98679e3, /// 0xc44
			 0x3c97adec, /// 0xc48
			 0x37b6ade2, /// 0xc4c
			 0x0ac673a6, /// 0xc50
			 0xec313688, /// 0xc54
			 0x83f49945, /// 0xc58
			 0x7ad4a495, /// 0xc5c
			 0xe5ec1930, /// 0xc60
			 0x3739bbfa, /// 0xc64
			 0x2edeb913, /// 0xc68
			 0x62d8dc64, /// 0xc6c
			 0xfe116aff, /// 0xc70
			 0x74a24456, /// 0xc74
			 0x8e60e62d, /// 0xc78
			 0xabc9d551, /// 0xc7c
			 0x3b1dc0f8, /// 0xc80
			 0x186f825f, /// 0xc84
			 0xe6c4103e, /// 0xc88
			 0x894cefa0, /// 0xc8c
			 0x5bf7f6e0, /// 0xc90
			 0x3d49f99c, /// 0xc94
			 0xeb0885ec, /// 0xc98
			 0x6043a77c, /// 0xc9c
			 0xdfc055f7, /// 0xca0
			 0x8b6181ff, /// 0xca4
			 0xb8859cdf, /// 0xca8
			 0xd008f61b, /// 0xcac
			 0x70490c4d, /// 0xcb0
			 0x0670f073, /// 0xcb4
			 0x55bbbff1, /// 0xcb8
			 0xb4d099d8, /// 0xcbc
			 0x84d8802c, /// 0xcc0
			 0x7b809563, /// 0xcc4
			 0xa2f69d48, /// 0xcc8
			 0x5a839bc5, /// 0xccc
			 0x739d5882, /// 0xcd0
			 0x6dc4ebf1, /// 0xcd4
			 0x310849e7, /// 0xcd8
			 0xb46f9b18, /// 0xcdc
			 0xae549d16, /// 0xce0
			 0xb33751d3, /// 0xce4
			 0xde232374, /// 0xce8
			 0xed944b10, /// 0xcec
			 0x3b5c28ee, /// 0xcf0
			 0x50ebc324, /// 0xcf4
			 0x59b6d1e8, /// 0xcf8
			 0xd5b3de54, /// 0xcfc
			 0x3a142cdc, /// 0xd00
			 0x0cc441f3, /// 0xd04
			 0xa4edfd3d, /// 0xd08
			 0x9de18963, /// 0xd0c
			 0xd17c6030, /// 0xd10
			 0xd685a28b, /// 0xd14
			 0xf5c69c5e, /// 0xd18
			 0xa4c89f66, /// 0xd1c
			 0x907d78df, /// 0xd20
			 0x0f6cde07, /// 0xd24
			 0x8eadd79d, /// 0xd28
			 0x6c1ef50a, /// 0xd2c
			 0xa979dc78, /// 0xd30
			 0xf2b8f93a, /// 0xd34
			 0xecb7d99a, /// 0xd38
			 0xac4fa013, /// 0xd3c
			 0x5d9e01e3, /// 0xd40
			 0x2abcfcb0, /// 0xd44
			 0xa47feb70, /// 0xd48
			 0xa651f7e2, /// 0xd4c
			 0xa223c048, /// 0xd50
			 0xc82d2cce, /// 0xd54
			 0x5f7578d3, /// 0xd58
			 0xef685e7d, /// 0xd5c
			 0xba54e3e0, /// 0xd60
			 0x7009ca10, /// 0xd64
			 0x0705fa85, /// 0xd68
			 0x4f968e1b, /// 0xd6c
			 0xb9770bcc, /// 0xd70
			 0x36329bb7, /// 0xd74
			 0xb6d63ad6, /// 0xd78
			 0xd55baf8f, /// 0xd7c
			 0xd123c05f, /// 0xd80
			 0xab2c7928, /// 0xd84
			 0x04ddd022, /// 0xd88
			 0x8646263a, /// 0xd8c
			 0x30dde4c8, /// 0xd90
			 0x2bd70b1f, /// 0xd94
			 0x8edadac2, /// 0xd98
			 0xd631eee3, /// 0xd9c
			 0xc37aa855, /// 0xda0
			 0x01a9783e, /// 0xda4
			 0xffd58f8b, /// 0xda8
			 0x40b42895, /// 0xdac
			 0x9b121741, /// 0xdb0
			 0xf71d6200, /// 0xdb4
			 0x39bfb141, /// 0xdb8
			 0xda7320c0, /// 0xdbc
			 0xba990e7f, /// 0xdc0
			 0x89a38fa2, /// 0xdc4
			 0x1b13baba, /// 0xdc8
			 0xb945bb15, /// 0xdcc
			 0x8ed460ed, /// 0xdd0
			 0x88c1d0d0, /// 0xdd4
			 0x6203bdfd, /// 0xdd8
			 0x96c87244, /// 0xddc
			 0x2a501faf, /// 0xde0
			 0xb396c7f6, /// 0xde4
			 0xc29337c6, /// 0xde8
			 0x615b1fd3, /// 0xdec
			 0xa62248a1, /// 0xdf0
			 0x59a9353d, /// 0xdf4
			 0xcfa086c2, /// 0xdf8
			 0x1a4f3a9d, /// 0xdfc
			 0x7c4d795a, /// 0xe00
			 0x751602aa, /// 0xe04
			 0x22e89d2b, /// 0xe08
			 0xa1d68489, /// 0xe0c
			 0xb8f907ac, /// 0xe10
			 0xe07565db, /// 0xe14
			 0x4c547737, /// 0xe18
			 0xa0800446, /// 0xe1c
			 0x5770a908, /// 0xe20
			 0x3bfa58a8, /// 0xe24
			 0x5ce2c90f, /// 0xe28
			 0xbb80774a, /// 0xe2c
			 0x26dcd0e1, /// 0xe30
			 0x2016fd02, /// 0xe34
			 0x800cb191, /// 0xe38
			 0xda77b18f, /// 0xe3c
			 0x12739f25, /// 0xe40
			 0xfaf44fae, /// 0xe44
			 0xcf2c11b6, /// 0xe48
			 0x7ea07eb3, /// 0xe4c
			 0xee87328d, /// 0xe50
			 0xc08f57b1, /// 0xe54
			 0x0ece37dc, /// 0xe58
			 0x2643d4ea, /// 0xe5c
			 0x7ed020d1, /// 0xe60
			 0x550962c3, /// 0xe64
			 0xb744cb3a, /// 0xe68
			 0x85becdf2, /// 0xe6c
			 0x03cc3348, /// 0xe70
			 0x687808ad, /// 0xe74
			 0x90f2932a, /// 0xe78
			 0x39f04128, /// 0xe7c
			 0x7fa7e007, /// 0xe80
			 0xb8432419, /// 0xe84
			 0x9be4247a, /// 0xe88
			 0x0cb1240a, /// 0xe8c
			 0x87b93e1d, /// 0xe90
			 0xffaa2b19, /// 0xe94
			 0x8ec9c5a1, /// 0xe98
			 0xa0840705, /// 0xe9c
			 0xd944fd27, /// 0xea0
			 0xabb03380, /// 0xea4
			 0x2f40a05e, /// 0xea8
			 0x1584c705, /// 0xeac
			 0x2eb00472, /// 0xeb0
			 0xa554c762, /// 0xeb4
			 0x645e48f9, /// 0xeb8
			 0xd663fa6f, /// 0xebc
			 0x7d1c82a4, /// 0xec0
			 0xa7c10992, /// 0xec4
			 0x9106fb52, /// 0xec8
			 0x43ac675d, /// 0xecc
			 0xbd84874b, /// 0xed0
			 0xbdcda6d1, /// 0xed4
			 0xff6a4e9a, /// 0xed8
			 0xdac04612, /// 0xedc
			 0x396ce258, /// 0xee0
			 0x5f98e8d1, /// 0xee4
			 0x61a75fe3, /// 0xee8
			 0x6ea614fb, /// 0xeec
			 0x1292d0f5, /// 0xef0
			 0x4482e03f, /// 0xef4
			 0x11eede99, /// 0xef8
			 0xaded8088, /// 0xefc
			 0x262838ee, /// 0xf00
			 0x44ed589b, /// 0xf04
			 0xb13a06a3, /// 0xf08
			 0xe32d51e2, /// 0xf0c
			 0xa44a5c3e, /// 0xf10
			 0x7e22cad1, /// 0xf14
			 0x29b2c8db, /// 0xf18
			 0x005abc88, /// 0xf1c
			 0x8a4a0b5e, /// 0xf20
			 0x4081a946, /// 0xf24
			 0x9b301aa8, /// 0xf28
			 0xf48c3558, /// 0xf2c
			 0x9bf7bec1, /// 0xf30
			 0x6d1eb271, /// 0xf34
			 0x88f07164, /// 0xf38
			 0xac4a55a0, /// 0xf3c
			 0x547833f8, /// 0xf40
			 0xcd76479a, /// 0xf44
			 0xef523061, /// 0xf48
			 0x78c02c7f, /// 0xf4c
			 0x8d2ff613, /// 0xf50
			 0x8bbf15bb, /// 0xf54
			 0x2356f47d, /// 0xf58
			 0xa2b8776e, /// 0xf5c
			 0x297d116f, /// 0xf60
			 0x54e174da, /// 0xf64
			 0x1882a1b0, /// 0xf68
			 0x1a107163, /// 0xf6c
			 0xc864f767, /// 0xf70
			 0x7cf05b11, /// 0xf74
			 0xee4a788f, /// 0xf78
			 0xf5a2ec40, /// 0xf7c
			 0xdcd83fdd, /// 0xf80
			 0x9d027a7c, /// 0xf84
			 0x9f9a0d1d, /// 0xf88
			 0x011943c7, /// 0xf8c
			 0x092f9658, /// 0xf90
			 0x2e4d9d51, /// 0xf94
			 0x279c8eeb, /// 0xf98
			 0xd9a5463c, /// 0xf9c
			 0xf02e3795, /// 0xfa0
			 0xdb474e69, /// 0xfa4
			 0x719776fa, /// 0xfa8
			 0xbdbfa7a9, /// 0xfac
			 0x70fa6c03, /// 0xfb0
			 0x4fec5d24, /// 0xfb4
			 0x31dd43cf, /// 0xfb8
			 0x4f062e1c, /// 0xfbc
			 0xa7df08fb, /// 0xfc0
			 0x914b9c11, /// 0xfc4
			 0x4b26608b, /// 0xfc8
			 0x5e099591, /// 0xfcc
			 0xe236d300, /// 0xfd0
			 0xa88ef342, /// 0xfd4
			 0x40d04c90, /// 0xfd8
			 0xa2c9b35a, /// 0xfdc
			 0xb89971ad, /// 0xfe0
			 0x23c02f4b, /// 0xfe4
			 0xed3db87d, /// 0xfe8
			 0xca892f15, /// 0xfec
			 0xc9a08b25, /// 0xff0
			 0x6f6a0012, /// 0xff4
			 0x510fe2be, /// 0xff8
			 0x7bc99ab8 /// last
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
			 0x000001f4,
			 0x0000052c,
			 0x000006d4,
			 0x000002ec,
			 0x0000041c,
			 0x000001f0,
			 0x00000768,
			 0x000007c4,

			 /// vpu register f2
			 0x41700000,
			 0x41d00000,
			 0x41880000,
			 0x41880000,
			 0x40c00000,
			 0x41880000,
			 0x41700000,
			 0x40400000,

			 /// vpu register f3
			 0x40c00000,
			 0x40e00000,
			 0x41800000,
			 0x41d00000,
			 0x41a80000,
			 0x41d00000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f4
			 0x42000000,
			 0x41a00000,
			 0x40400000,
			 0x40e00000,
			 0x41980000,
			 0x41c80000,
			 0x41e80000,
			 0x41b00000,

			 /// vpu register f5
			 0x41f00000,
			 0x40000000,
			 0x40000000,
			 0x41a80000,
			 0x41f00000,
			 0x41d00000,
			 0x41100000,
			 0x41900000,

			 /// vpu register f6
			 0x41e80000,
			 0x41e00000,
			 0x41700000,
			 0x41500000,
			 0x41500000,
			 0x40e00000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f7
			 0x41900000,
			 0x41300000,
			 0x41f00000,
			 0x41700000,
			 0x41880000,
			 0x41e80000,
			 0x41a80000,
			 0x41f80000,

			 /// vpu register f8
			 0x41d80000,
			 0x3f800000,
			 0x41e00000,
			 0x41700000,
			 0x41b00000,
			 0x41e00000,
			 0x41500000,
			 0x41600000,

			 /// vpu register f9
			 0x40e00000,
			 0x41c00000,
			 0x40c00000,
			 0x41800000,
			 0x41100000,
			 0x41800000,
			 0x41600000,
			 0x40a00000,

			 /// vpu register f10
			 0x41f00000,
			 0x40e00000,
			 0x41b80000,
			 0x41b80000,
			 0x41c00000,
			 0x41200000,
			 0x41d00000,
			 0x40400000,

			 /// vpu register f11
			 0x41500000,
			 0x41d80000,
			 0x41800000,
			 0x41e00000,
			 0x40c00000,
			 0x42000000,
			 0x40800000,
			 0x41000000,

			 /// vpu register f12
			 0x41300000,
			 0x41100000,
			 0x41200000,
			 0x41e00000,
			 0x41f00000,
			 0x40e00000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f13
			 0x40800000,
			 0x41f80000,
			 0x42000000,
			 0x41500000,
			 0x41c80000,
			 0x41a00000,
			 0x41d00000,
			 0x41a80000,

			 /// vpu register f14
			 0x40000000,
			 0x41600000,
			 0x41c00000,
			 0x40a00000,
			 0x41900000,
			 0x40c00000,
			 0x41880000,
			 0x41400000,

			 /// vpu register f15
			 0x41200000,
			 0x41f00000,
			 0x3f800000,
			 0x41e00000,
			 0x41700000,
			 0x41600000,
			 0x41600000,
			 0x41400000,

			 /// vpu register f16
			 0x41300000,
			 0x41b00000,
			 0x41e00000,
			 0x41c80000,
			 0x40a00000,
			 0x41e00000,
			 0x41100000,
			 0x41d00000,

			 /// vpu register f17
			 0x41600000,
			 0x41100000,
			 0x41e80000,
			 0x41a80000,
			 0x41400000,
			 0x41300000,
			 0x40400000,
			 0x41a00000,

			 /// vpu register f18
			 0x41400000,
			 0x41b80000,
			 0x41800000,
			 0x41980000,
			 0x41200000,
			 0x40800000,
			 0x41d80000,
			 0x41a80000,

			 /// vpu register f19
			 0x41000000,
			 0x41f00000,
			 0x41b80000,
			 0x41400000,
			 0x41b00000,
			 0x41400000,
			 0x41a00000,
			 0x40c00000,

			 /// vpu register f20
			 0x40c00000,
			 0x41400000,
			 0x41e00000,
			 0x3f800000,
			 0x3f800000,
			 0x41c00000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f21
			 0x41800000,
			 0x41880000,
			 0x41400000,
			 0x41100000,
			 0x41f80000,
			 0x41c00000,
			 0x41500000,
			 0x41980000,

			 /// vpu register f22
			 0x41880000,
			 0x41000000,
			 0x41000000,
			 0x42000000,
			 0x41e00000,
			 0x41100000,
			 0x41300000,
			 0x41e00000,

			 /// vpu register f23
			 0x41400000,
			 0x41b80000,
			 0x41e00000,
			 0x42000000,
			 0x41f00000,
			 0x40a00000,
			 0x41600000,
			 0x41e00000,

			 /// vpu register f24
			 0x40400000,
			 0x41a00000,
			 0x41100000,
			 0x41c80000,
			 0x41400000,
			 0x40800000,
			 0x41000000,
			 0x42000000,

			 /// vpu register f25
			 0x41300000,
			 0x3f800000,
			 0x41c00000,
			 0x41880000,
			 0x42000000,
			 0x41c80000,
			 0x42000000,
			 0x40e00000,

			 /// vpu register f26
			 0x41e00000,
			 0x41600000,
			 0x41300000,
			 0x41a80000,
			 0x40800000,
			 0x40400000,
			 0x40400000,
			 0x41700000,

			 /// vpu register f27
			 0x41100000,
			 0x41e00000,
			 0x41c00000,
			 0x40a00000,
			 0x42000000,
			 0x41d00000,
			 0x41a80000,
			 0x40c00000,

			 /// vpu register f28
			 0x41000000,
			 0x41d80000,
			 0x41400000,
			 0x41e00000,
			 0x3f800000,
			 0x41b00000,
			 0x41600000,
			 0x41e00000,

			 /// vpu register f29
			 0x41000000,
			 0x40000000,
			 0x40a00000,
			 0x41b80000,
			 0x40e00000,
			 0x41400000,
			 0x41200000,
			 0x41f00000,

			 /// vpu register f30
			 0x42000000,
			 0x41a00000,
			 0x41500000,
			 0x41980000,
			 0x40a00000,
			 0x41d80000,
			 0x41c80000,
			 0x40000000,

			 /// vpu register f31
			 0x41200000,
			 0x41600000,
			 0x40000000,
			 0x41700000,
			 0x41e00000,
			 0x41d00000,
			 0x41400000,
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
		"nop\n"
		"fgb.ps f4, f1 (x13)\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f13, f1 (x15)\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f14, f1 (x13)\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f21, f1 (x13)\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x15)\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f20, f1 (x12)\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f5, f1 (x15)\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f11, f1 (x14)\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f29, f1 (x12)\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f4, f1 (x12)\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f22, f1 (x11)\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f29, f1 (x13)\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f10, f1 (x15)\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f13, f1 (x11)\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f28, f1 (x12)\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f30, f1 (x13)\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f21, f1 (x13)\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x11)\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f27, f1 (x12)\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f6, f1 (x15)\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f25, f1 (x11)\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f10, f1 (x11)\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f4, f1 (x11)\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f23, f1 (x14)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f18, f1 (x14)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f12, f1 (x11)\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f28, f1 (x12)\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f21, f1 (x12)\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f20, f1 (x12)\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x14)\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f10, f1 (x13)\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f4, f1 (x12)\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f22, f1 (x13)\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f17, f1 (x14)\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f23, f1 (x12)\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f22, f1 (x12)\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f30, f1 (x12)\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f5, f1 (x12)\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f27, f1 (x14)\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f24, f1 (x11)\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f5, f1 (x14)\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f18, f1 (x11)\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f8, f1 (x14)\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f26, f1 (x15)\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f28, f1 (x11)\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f20, f1 (x13)\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f23, f1 (x11)\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f9, f1 (x15)\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x12)\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f15, f1 (x14)\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f15, f1 (x15)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f20, f1 (x13)\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f29, f1 (x11)\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f16, f1 (x13)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f7, f1 (x12)\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f17, f1 (x12)\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f21, f1 (x13)\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f24, f1 (x14)\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f29, f1 (x13)\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f6, f1 (x12)\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f26, f1 (x15)\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f20, f1 (x15)\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f14, f1 (x13)\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f22, f1 (x15)\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f19, f1 (x14)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f15, f1 (x14)\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f12, f1 (x15)\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f4, f1 (x13)\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f25, f1 (x13)\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f5, f1 (x12)\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f20, f1 (x11)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f5, f1 (x14)\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x14)\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f21, f1 (x15)\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f13, f1 (x15)\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f4, f1 (x13)\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f26, f1 (x12)\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f23, f1 (x13)\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f25, f1 (x12)\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f14, f1 (x15)\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f16, f1 (x12)\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f5, f1 (x12)\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f30, f1 (x14)\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f29, f1 (x14)\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f12, f1 (x13)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f18, f1 (x13)\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x14)\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f22, f1 (x11)\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f7, f1 (x12)\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f14, f1 (x11)\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f10, f1 (x14)\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f3, f1 (x11)\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f4, f1 (x15)\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f23, f1 (x12)\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f2, f1 (x14)\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f14, f1 (x14)\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f28, f1 (x15)\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f30, f1 (x14)\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgb.ps f11, f1 (x12)\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
