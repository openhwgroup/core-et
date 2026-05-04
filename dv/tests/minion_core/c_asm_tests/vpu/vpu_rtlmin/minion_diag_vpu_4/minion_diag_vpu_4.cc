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
			 0x2880a467, /// 0x0
			 0x2a925028, /// 0x4
			 0x8801cc61, /// 0x8
			 0x9822a87b, /// 0xc
			 0xce53351c, /// 0x10
			 0x83325dc6, /// 0x14
			 0x15f4f496, /// 0x18
			 0x18341e7c, /// 0x1c
			 0x81aadbaa, /// 0x20
			 0x80be3e26, /// 0x24
			 0xe002b63c, /// 0x28
			 0x7c4c71e5, /// 0x2c
			 0x9fc9d20b, /// 0x30
			 0x9386cb07, /// 0x34
			 0xe24e25d2, /// 0x38
			 0x2fd03c13, /// 0x3c
			 0x7d324430, /// 0x40
			 0x60f03fcf, /// 0x44
			 0x65dfa3e0, /// 0x48
			 0xcf97e641, /// 0x4c
			 0xa129bad4, /// 0x50
			 0x261b0b5a, /// 0x54
			 0xb811b5a5, /// 0x58
			 0xf145cd79, /// 0x5c
			 0x7bd370f6, /// 0x60
			 0x1a98ac79, /// 0x64
			 0xbad13544, /// 0x68
			 0x2de550ab, /// 0x6c
			 0x0b8c3fcc, /// 0x70
			 0x6ef88886, /// 0x74
			 0x7927c5d3, /// 0x78
			 0xca5da547, /// 0x7c
			 0xd50289f1, /// 0x80
			 0xdde45aa4, /// 0x84
			 0xba4c845d, /// 0x88
			 0xe3760906, /// 0x8c
			 0x765333c7, /// 0x90
			 0x904d8b38, /// 0x94
			 0x746aebee, /// 0x98
			 0x4131406f, /// 0x9c
			 0x3d5ca90a, /// 0xa0
			 0xba752481, /// 0xa4
			 0x07fb6fed, /// 0xa8
			 0xe40c4eec, /// 0xac
			 0x016244d1, /// 0xb0
			 0x26503be5, /// 0xb4
			 0x8f09c988, /// 0xb8
			 0xea87a2be, /// 0xbc
			 0x2c9b57fe, /// 0xc0
			 0x9d2fab19, /// 0xc4
			 0x69f111e4, /// 0xc8
			 0x00e52552, /// 0xcc
			 0x779875e3, /// 0xd0
			 0x6bb2d124, /// 0xd4
			 0xff40239b, /// 0xd8
			 0x78a25df0, /// 0xdc
			 0x04191f07, /// 0xe0
			 0x4fb64662, /// 0xe4
			 0x3bb685ca, /// 0xe8
			 0x55381b66, /// 0xec
			 0xef4178d2, /// 0xf0
			 0x6dbda550, /// 0xf4
			 0xa1c703c6, /// 0xf8
			 0x7bfb17da, /// 0xfc
			 0xb3b50d3e, /// 0x100
			 0x6d653189, /// 0x104
			 0x399fcb1c, /// 0x108
			 0xb9b5d973, /// 0x10c
			 0xe351e0b5, /// 0x110
			 0x08e59448, /// 0x114
			 0x71599423, /// 0x118
			 0x55dae6d8, /// 0x11c
			 0xc5ae724b, /// 0x120
			 0xd35518f9, /// 0x124
			 0x5149c85a, /// 0x128
			 0xc6d0b750, /// 0x12c
			 0x027440b3, /// 0x130
			 0xfa9a5899, /// 0x134
			 0x82471e4a, /// 0x138
			 0xfab91d4e, /// 0x13c
			 0x7bbdc83e, /// 0x140
			 0xe1d3c431, /// 0x144
			 0xe2671f38, /// 0x148
			 0x315707b5, /// 0x14c
			 0x3856b486, /// 0x150
			 0xbb1c1968, /// 0x154
			 0x3cdce336, /// 0x158
			 0x33409dec, /// 0x15c
			 0x73966ec8, /// 0x160
			 0xeabbac47, /// 0x164
			 0x1cbb432b, /// 0x168
			 0xd87528d3, /// 0x16c
			 0xfaf5f003, /// 0x170
			 0xbcce61a0, /// 0x174
			 0x5bdd261f, /// 0x178
			 0xfdd2e2f0, /// 0x17c
			 0x838cc60a, /// 0x180
			 0x3771ac0e, /// 0x184
			 0x9ef99e5f, /// 0x188
			 0x3f899139, /// 0x18c
			 0x479478e2, /// 0x190
			 0xc5691111, /// 0x194
			 0xf889386c, /// 0x198
			 0x919a32f4, /// 0x19c
			 0xf7cc73ac, /// 0x1a0
			 0xa0b21bc9, /// 0x1a4
			 0x95c56ab3, /// 0x1a8
			 0xaebb451f, /// 0x1ac
			 0x082e670b, /// 0x1b0
			 0xaa0bb051, /// 0x1b4
			 0x9fbfcf00, /// 0x1b8
			 0x59f9f50e, /// 0x1bc
			 0x9be6a720, /// 0x1c0
			 0xd4468449, /// 0x1c4
			 0x5041ddbc, /// 0x1c8
			 0x332c71eb, /// 0x1cc
			 0x08bb8d89, /// 0x1d0
			 0x35bbebc3, /// 0x1d4
			 0xb96725e3, /// 0x1d8
			 0xbc7e243f, /// 0x1dc
			 0x1545c282, /// 0x1e0
			 0x1b1d5c7c, /// 0x1e4
			 0xd8e0116b, /// 0x1e8
			 0xdfe9fa1d, /// 0x1ec
			 0xaffc089c, /// 0x1f0
			 0x2bad71c7, /// 0x1f4
			 0x8a180daf, /// 0x1f8
			 0x9de494cd, /// 0x1fc
			 0xa477c344, /// 0x200
			 0xd0090444, /// 0x204
			 0x88d0437c, /// 0x208
			 0x2d5ae65a, /// 0x20c
			 0x3fe0d714, /// 0x210
			 0x3ecde72e, /// 0x214
			 0xd6924055, /// 0x218
			 0x5578e9ae, /// 0x21c
			 0x2e3259b6, /// 0x220
			 0x19d10c97, /// 0x224
			 0x5b7a9632, /// 0x228
			 0x0a87a078, /// 0x22c
			 0xe5733528, /// 0x230
			 0x7a141427, /// 0x234
			 0x91b18993, /// 0x238
			 0x4a18180e, /// 0x23c
			 0x47b18f17, /// 0x240
			 0xe1a046c0, /// 0x244
			 0x077b7255, /// 0x248
			 0xad73bee7, /// 0x24c
			 0xfaddca53, /// 0x250
			 0x0c000fc4, /// 0x254
			 0xa2e1c347, /// 0x258
			 0xa6af0e30, /// 0x25c
			 0xe78c040a, /// 0x260
			 0xb9a8284c, /// 0x264
			 0xa3092457, /// 0x268
			 0x276bd5fe, /// 0x26c
			 0xe0de09bb, /// 0x270
			 0x04bf73c5, /// 0x274
			 0xe68c0849, /// 0x278
			 0x4debbf11, /// 0x27c
			 0x146c1c90, /// 0x280
			 0x6a94f95d, /// 0x284
			 0xb593c697, /// 0x288
			 0x2f240a44, /// 0x28c
			 0x38258ba9, /// 0x290
			 0xaa6f1dea, /// 0x294
			 0xe69a9685, /// 0x298
			 0xeafa1e70, /// 0x29c
			 0xb8e67cc4, /// 0x2a0
			 0x10ea6814, /// 0x2a4
			 0x2ce47dd2, /// 0x2a8
			 0x57d44227, /// 0x2ac
			 0x255af0ba, /// 0x2b0
			 0xf7f49556, /// 0x2b4
			 0x43616e3e, /// 0x2b8
			 0x95bb0c4a, /// 0x2bc
			 0x5a406a64, /// 0x2c0
			 0xd76fde80, /// 0x2c4
			 0x8076e775, /// 0x2c8
			 0x38099e58, /// 0x2cc
			 0x01a720af, /// 0x2d0
			 0x3fa6e76e, /// 0x2d4
			 0x2a1d5922, /// 0x2d8
			 0x291ea104, /// 0x2dc
			 0xf8f3c82c, /// 0x2e0
			 0x54106fe5, /// 0x2e4
			 0x01c3946e, /// 0x2e8
			 0xa0af8cde, /// 0x2ec
			 0x98f07997, /// 0x2f0
			 0x8e945c69, /// 0x2f4
			 0x58ae3a16, /// 0x2f8
			 0xbaea2d6b, /// 0x2fc
			 0x1dbc52eb, /// 0x300
			 0x5b53ce03, /// 0x304
			 0xf020c111, /// 0x308
			 0xd505a9ee, /// 0x30c
			 0x62d11e69, /// 0x310
			 0x0af0b23d, /// 0x314
			 0x2486b928, /// 0x318
			 0x8a6828f0, /// 0x31c
			 0x847400c9, /// 0x320
			 0xc3d4d991, /// 0x324
			 0xc740eb84, /// 0x328
			 0x6dbb4c9e, /// 0x32c
			 0x9070bd8c, /// 0x330
			 0x1b53c80c, /// 0x334
			 0xac0bb151, /// 0x338
			 0x41d956a7, /// 0x33c
			 0x2208fd9a, /// 0x340
			 0x24920db8, /// 0x344
			 0xf45bbd97, /// 0x348
			 0x316f115e, /// 0x34c
			 0x51359ba5, /// 0x350
			 0x9ccc4558, /// 0x354
			 0xafe0b40a, /// 0x358
			 0x126f3adf, /// 0x35c
			 0xe7c6460f, /// 0x360
			 0x1242577e, /// 0x364
			 0xb1296f0a, /// 0x368
			 0xc873c63e, /// 0x36c
			 0x2be842b6, /// 0x370
			 0xd1a4af41, /// 0x374
			 0xb0a70d33, /// 0x378
			 0xa469983a, /// 0x37c
			 0x130a63ca, /// 0x380
			 0x57e588f0, /// 0x384
			 0x49fa6108, /// 0x388
			 0x2bf4959d, /// 0x38c
			 0xb231e967, /// 0x390
			 0x507a1bd2, /// 0x394
			 0xa438d540, /// 0x398
			 0x5120bbd3, /// 0x39c
			 0x760ebcad, /// 0x3a0
			 0xd96cc0fd, /// 0x3a4
			 0x87e05a5c, /// 0x3a8
			 0x343543e2, /// 0x3ac
			 0xf77707cf, /// 0x3b0
			 0x46317cfb, /// 0x3b4
			 0x5ea4eef8, /// 0x3b8
			 0x47ae01c6, /// 0x3bc
			 0x5c1d9264, /// 0x3c0
			 0xdfb6b07d, /// 0x3c4
			 0x54ab85fb, /// 0x3c8
			 0x1d69846e, /// 0x3cc
			 0x44dcf828, /// 0x3d0
			 0x9796635f, /// 0x3d4
			 0x4a03c19b, /// 0x3d8
			 0xeadf9111, /// 0x3dc
			 0x626f68b7, /// 0x3e0
			 0x5cc52729, /// 0x3e4
			 0x2ce25ce5, /// 0x3e8
			 0xd96e007f, /// 0x3ec
			 0x060106e5, /// 0x3f0
			 0x1ff09cc3, /// 0x3f4
			 0xc741e370, /// 0x3f8
			 0xfb9a4165, /// 0x3fc
			 0xcada2f06, /// 0x400
			 0x289af336, /// 0x404
			 0x218b5e54, /// 0x408
			 0xb4eb457a, /// 0x40c
			 0x102b0d36, /// 0x410
			 0xe39f2c84, /// 0x414
			 0x06fe523d, /// 0x418
			 0x00bdbf00, /// 0x41c
			 0xd4e55bd5, /// 0x420
			 0x12db73e1, /// 0x424
			 0x288d1173, /// 0x428
			 0xe13ba04e, /// 0x42c
			 0xcf547084, /// 0x430
			 0x3a7fe855, /// 0x434
			 0xf636d7b0, /// 0x438
			 0x24ee13d7, /// 0x43c
			 0x677b57f2, /// 0x440
			 0xaef4017a, /// 0x444
			 0x6b326257, /// 0x448
			 0x684234d0, /// 0x44c
			 0xd6e9783d, /// 0x450
			 0xc5db6987, /// 0x454
			 0xd72c9e85, /// 0x458
			 0x651b9ac0, /// 0x45c
			 0x1a626d86, /// 0x460
			 0xd6519fce, /// 0x464
			 0x25a7910d, /// 0x468
			 0x052774e3, /// 0x46c
			 0xe29ff578, /// 0x470
			 0xc1672a5b, /// 0x474
			 0x8f4f23f9, /// 0x478
			 0xde6ec8c7, /// 0x47c
			 0x859d2095, /// 0x480
			 0xe8a43c61, /// 0x484
			 0xb2dfb0af, /// 0x488
			 0x2bc5d65b, /// 0x48c
			 0x93859c9e, /// 0x490
			 0xd247ce01, /// 0x494
			 0xd73325a8, /// 0x498
			 0x863f0220, /// 0x49c
			 0x92fc85a6, /// 0x4a0
			 0xa62c890b, /// 0x4a4
			 0x48eb35a8, /// 0x4a8
			 0x2ae31c9e, /// 0x4ac
			 0x647cca45, /// 0x4b0
			 0x98f7a285, /// 0x4b4
			 0xb8c47d9f, /// 0x4b8
			 0xf7c8cb97, /// 0x4bc
			 0xeab822dc, /// 0x4c0
			 0x5930d1a5, /// 0x4c4
			 0x3a22dcc8, /// 0x4c8
			 0x62b6a67e, /// 0x4cc
			 0x5d76a71b, /// 0x4d0
			 0x0c22a240, /// 0x4d4
			 0x8cfdc802, /// 0x4d8
			 0xf3efae20, /// 0x4dc
			 0x2279ab48, /// 0x4e0
			 0xe21923f8, /// 0x4e4
			 0x975a8969, /// 0x4e8
			 0xd64ef4dc, /// 0x4ec
			 0x2e8fe94c, /// 0x4f0
			 0xc915c8f6, /// 0x4f4
			 0xf928f154, /// 0x4f8
			 0x20d7200d, /// 0x4fc
			 0x8a9c75eb, /// 0x500
			 0x79cf75cd, /// 0x504
			 0x8689c83a, /// 0x508
			 0xc36c1a42, /// 0x50c
			 0x354ed7eb, /// 0x510
			 0x489de6f2, /// 0x514
			 0xf3b36885, /// 0x518
			 0x121c19db, /// 0x51c
			 0xde2c425f, /// 0x520
			 0x3b8b0bd9, /// 0x524
			 0x443278b6, /// 0x528
			 0x20d9aadb, /// 0x52c
			 0x64b2c46b, /// 0x530
			 0x95f3a70d, /// 0x534
			 0x03d6f727, /// 0x538
			 0x701713d2, /// 0x53c
			 0x76e64bd6, /// 0x540
			 0x50e13d92, /// 0x544
			 0x568ddc22, /// 0x548
			 0x6d60ca17, /// 0x54c
			 0x80823096, /// 0x550
			 0xa8fb420b, /// 0x554
			 0xfdece7cb, /// 0x558
			 0x0d61af96, /// 0x55c
			 0x639eda89, /// 0x560
			 0x9487f9bb, /// 0x564
			 0x4591a158, /// 0x568
			 0x194c2992, /// 0x56c
			 0xaffa93f0, /// 0x570
			 0xae4ca2c2, /// 0x574
			 0x2c17d811, /// 0x578
			 0x4fcc12b7, /// 0x57c
			 0xbb578386, /// 0x580
			 0x0f5a2795, /// 0x584
			 0x5b160dbb, /// 0x588
			 0x011fc429, /// 0x58c
			 0xc11e5708, /// 0x590
			 0xb849c238, /// 0x594
			 0x4037a38d, /// 0x598
			 0xdbdbd6b3, /// 0x59c
			 0x5768ff1e, /// 0x5a0
			 0xed510e24, /// 0x5a4
			 0xfb55c844, /// 0x5a8
			 0x4ba10915, /// 0x5ac
			 0x3af25e27, /// 0x5b0
			 0x95e1d270, /// 0x5b4
			 0xc9df14a2, /// 0x5b8
			 0xe2e63b8b, /// 0x5bc
			 0xa023f31d, /// 0x5c0
			 0x2b29a32c, /// 0x5c4
			 0x9c7ba836, /// 0x5c8
			 0xabc44f45, /// 0x5cc
			 0xbeb8d717, /// 0x5d0
			 0xc752f4ed, /// 0x5d4
			 0xc6c0b445, /// 0x5d8
			 0x1c6e3c5b, /// 0x5dc
			 0x06858ee2, /// 0x5e0
			 0x1868d541, /// 0x5e4
			 0xfee6820d, /// 0x5e8
			 0xcdca95bd, /// 0x5ec
			 0x0aa90de1, /// 0x5f0
			 0x458b054f, /// 0x5f4
			 0x85ef6612, /// 0x5f8
			 0xdeef66f2, /// 0x5fc
			 0xd387196f, /// 0x600
			 0x9268a9e2, /// 0x604
			 0x5a044c90, /// 0x608
			 0xde9238df, /// 0x60c
			 0x7f9cd008, /// 0x610
			 0x762329c8, /// 0x614
			 0xd33fc984, /// 0x618
			 0x87b9f16d, /// 0x61c
			 0xf9b5e809, /// 0x620
			 0xec440844, /// 0x624
			 0x374b3bb0, /// 0x628
			 0x195f65ce, /// 0x62c
			 0xc3a47642, /// 0x630
			 0xee7771d7, /// 0x634
			 0x2efc2810, /// 0x638
			 0x6989e27b, /// 0x63c
			 0x62e17140, /// 0x640
			 0xefabb6ec, /// 0x644
			 0x978ab114, /// 0x648
			 0x0008ac40, /// 0x64c
			 0x4b7648ec, /// 0x650
			 0xcdbfe5f5, /// 0x654
			 0xb9890c50, /// 0x658
			 0x6dc20950, /// 0x65c
			 0xdccdca33, /// 0x660
			 0xc987a2ab, /// 0x664
			 0x5e7a423c, /// 0x668
			 0x64bcf0d6, /// 0x66c
			 0x368a7bed, /// 0x670
			 0xcb601f96, /// 0x674
			 0x4649b7a6, /// 0x678
			 0x4e8a3f0f, /// 0x67c
			 0xa3566a8b, /// 0x680
			 0xb1a77e3f, /// 0x684
			 0x1090c9c0, /// 0x688
			 0xefc3b42f, /// 0x68c
			 0x88fedd6b, /// 0x690
			 0x6cb16896, /// 0x694
			 0xf196f880, /// 0x698
			 0x265ac6f8, /// 0x69c
			 0xb3042512, /// 0x6a0
			 0x8d1e0f9e, /// 0x6a4
			 0x859c7edc, /// 0x6a8
			 0xe68f79ef, /// 0x6ac
			 0x198fd35a, /// 0x6b0
			 0x56ce1792, /// 0x6b4
			 0x5b3b51c8, /// 0x6b8
			 0x9ac5559e, /// 0x6bc
			 0xaaaf11ee, /// 0x6c0
			 0xd9921ab2, /// 0x6c4
			 0xd7b63649, /// 0x6c8
			 0xc7dba2fb, /// 0x6cc
			 0x7f3eaf24, /// 0x6d0
			 0x913d989a, /// 0x6d4
			 0xe451672c, /// 0x6d8
			 0xb60d39ea, /// 0x6dc
			 0x7dba6a78, /// 0x6e0
			 0xd3cbfee1, /// 0x6e4
			 0x0e0614bb, /// 0x6e8
			 0x672038e3, /// 0x6ec
			 0x1154c4f0, /// 0x6f0
			 0x9301b0f5, /// 0x6f4
			 0xd08c19ed, /// 0x6f8
			 0x65a226fa, /// 0x6fc
			 0x82584e26, /// 0x700
			 0xdaeb2bbb, /// 0x704
			 0x43425326, /// 0x708
			 0x18c418c1, /// 0x70c
			 0xb67f30a0, /// 0x710
			 0x3e5e47e0, /// 0x714
			 0x0da28a46, /// 0x718
			 0xb59a0113, /// 0x71c
			 0x60054975, /// 0x720
			 0xceaa50b4, /// 0x724
			 0x73b97f1c, /// 0x728
			 0xc79f7a9b, /// 0x72c
			 0x9a3e1453, /// 0x730
			 0x05483216, /// 0x734
			 0x1d6b713f, /// 0x738
			 0xf2c6599a, /// 0x73c
			 0x312e0012, /// 0x740
			 0x1e7ce16d, /// 0x744
			 0xf7a34c07, /// 0x748
			 0xe5241229, /// 0x74c
			 0x1d276578, /// 0x750
			 0x80abb19d, /// 0x754
			 0xd3d63a3f, /// 0x758
			 0xa631666a, /// 0x75c
			 0x626af067, /// 0x760
			 0x337216ab, /// 0x764
			 0x4711f9c3, /// 0x768
			 0x002d956d, /// 0x76c
			 0xe89fb452, /// 0x770
			 0x00c3e317, /// 0x774
			 0xa587028c, /// 0x778
			 0xcb1d4917, /// 0x77c
			 0xcefc450f, /// 0x780
			 0x0dba47bc, /// 0x784
			 0x0feb89c0, /// 0x788
			 0x457829a3, /// 0x78c
			 0x52ee46fa, /// 0x790
			 0x0aac59fb, /// 0x794
			 0x253da82b, /// 0x798
			 0x9d0b8389, /// 0x79c
			 0x1c4062bb, /// 0x7a0
			 0x7b9234d8, /// 0x7a4
			 0x6f827920, /// 0x7a8
			 0x169efafa, /// 0x7ac
			 0x8038e09e, /// 0x7b0
			 0x28f04b6c, /// 0x7b4
			 0xdcae363a, /// 0x7b8
			 0x4e6bde9c, /// 0x7bc
			 0x1a77e779, /// 0x7c0
			 0x7595edfe, /// 0x7c4
			 0xdcc58b1f, /// 0x7c8
			 0x3a13605c, /// 0x7cc
			 0x481651ad, /// 0x7d0
			 0x366461e0, /// 0x7d4
			 0x62c2aba5, /// 0x7d8
			 0xa12a7b11, /// 0x7dc
			 0x9333e6c9, /// 0x7e0
			 0xba8db4bc, /// 0x7e4
			 0x01e848d3, /// 0x7e8
			 0x015d74fa, /// 0x7ec
			 0x54492785, /// 0x7f0
			 0xf2ab4f14, /// 0x7f4
			 0xcd56907c, /// 0x7f8
			 0x38d29e66, /// 0x7fc
			 0xd093104e, /// 0x800
			 0xa2a26cbc, /// 0x804
			 0xcdec3073, /// 0x808
			 0x91e9ae8a, /// 0x80c
			 0x1afa88d8, /// 0x810
			 0x891ab432, /// 0x814
			 0xf86c49e9, /// 0x818
			 0xee2766dd, /// 0x81c
			 0x6fb72bd8, /// 0x820
			 0xbf54b4fa, /// 0x824
			 0x17ac4bc4, /// 0x828
			 0xea5e4e18, /// 0x82c
			 0x7f50dfc5, /// 0x830
			 0x8430fae3, /// 0x834
			 0x7e2e8390, /// 0x838
			 0xd8791280, /// 0x83c
			 0x558ec218, /// 0x840
			 0x7c46e8b9, /// 0x844
			 0x2de7f277, /// 0x848
			 0xfd869ef7, /// 0x84c
			 0xd9506c18, /// 0x850
			 0xc2a0e05f, /// 0x854
			 0x79cefcfd, /// 0x858
			 0x08fa7879, /// 0x85c
			 0xf91d278a, /// 0x860
			 0x1b30bf4a, /// 0x864
			 0xa2eef27e, /// 0x868
			 0xed6277d3, /// 0x86c
			 0xb9e6f264, /// 0x870
			 0x988d0971, /// 0x874
			 0x37d00d4a, /// 0x878
			 0x817d6e10, /// 0x87c
			 0x46cbe403, /// 0x880
			 0x9b65932b, /// 0x884
			 0xb60df9dc, /// 0x888
			 0x1cf6213a, /// 0x88c
			 0x8a9f6a69, /// 0x890
			 0xe81e5bc3, /// 0x894
			 0x39dc428f, /// 0x898
			 0xb29943d0, /// 0x89c
			 0x98300bd1, /// 0x8a0
			 0x6f3835f0, /// 0x8a4
			 0xad14e107, /// 0x8a8
			 0xc3bb2b1f, /// 0x8ac
			 0x9dd47341, /// 0x8b0
			 0x10901bc3, /// 0x8b4
			 0x9427bef3, /// 0x8b8
			 0x88791581, /// 0x8bc
			 0x639ef34b, /// 0x8c0
			 0x2d746881, /// 0x8c4
			 0xd8cf6c8f, /// 0x8c8
			 0x3ad28af9, /// 0x8cc
			 0xc488f237, /// 0x8d0
			 0xa2e18bbc, /// 0x8d4
			 0x46b0155d, /// 0x8d8
			 0xbf3320a4, /// 0x8dc
			 0xf202695e, /// 0x8e0
			 0x0993bba3, /// 0x8e4
			 0x924a9dc3, /// 0x8e8
			 0x1d613bbb, /// 0x8ec
			 0xa0bbf39e, /// 0x8f0
			 0x0d5fc8a1, /// 0x8f4
			 0x9d1a0a1f, /// 0x8f8
			 0xce20692d, /// 0x8fc
			 0x88f581c0, /// 0x900
			 0x9062fb7f, /// 0x904
			 0x36ac32bd, /// 0x908
			 0xaf69227a, /// 0x90c
			 0x956bc41e, /// 0x910
			 0x224a57e0, /// 0x914
			 0xf2ca4991, /// 0x918
			 0x7035efbe, /// 0x91c
			 0x1eb75448, /// 0x920
			 0x48f55d93, /// 0x924
			 0x89b4ca14, /// 0x928
			 0xb2772bde, /// 0x92c
			 0xa059373c, /// 0x930
			 0xae8d8168, /// 0x934
			 0x85d72cdc, /// 0x938
			 0x57fe52e3, /// 0x93c
			 0x62b619d2, /// 0x940
			 0x78b1dcfa, /// 0x944
			 0x62d83d4b, /// 0x948
			 0x268ea434, /// 0x94c
			 0x235eb689, /// 0x950
			 0xcf0da036, /// 0x954
			 0xe44d469a, /// 0x958
			 0x6d59a453, /// 0x95c
			 0x1570a9a0, /// 0x960
			 0x6b510f5a, /// 0x964
			 0xeab63f8e, /// 0x968
			 0x8584ab5c, /// 0x96c
			 0xb2ea3285, /// 0x970
			 0xde6f2a04, /// 0x974
			 0xcaebea8a, /// 0x978
			 0x184928d8, /// 0x97c
			 0x2e62502f, /// 0x980
			 0x750ed006, /// 0x984
			 0x0f462831, /// 0x988
			 0x9479e326, /// 0x98c
			 0x7f035e26, /// 0x990
			 0x9a45092b, /// 0x994
			 0x460ce9ae, /// 0x998
			 0xe5157b2d, /// 0x99c
			 0x05ed542d, /// 0x9a0
			 0xf98ad9bc, /// 0x9a4
			 0xff7b91e2, /// 0x9a8
			 0x9e867001, /// 0x9ac
			 0x54dc4fc6, /// 0x9b0
			 0x0354f9dd, /// 0x9b4
			 0x068a82ac, /// 0x9b8
			 0x019a980d, /// 0x9bc
			 0xa02e2bec, /// 0x9c0
			 0x6f8b1bdd, /// 0x9c4
			 0x26694a6f, /// 0x9c8
			 0x52bdfe39, /// 0x9cc
			 0x9f440638, /// 0x9d0
			 0xe2ca6229, /// 0x9d4
			 0x8dcdcbe4, /// 0x9d8
			 0xce667ba5, /// 0x9dc
			 0x7e852dc2, /// 0x9e0
			 0xc0171cb5, /// 0x9e4
			 0x79907c05, /// 0x9e8
			 0x5b8472fd, /// 0x9ec
			 0x5916c1e4, /// 0x9f0
			 0x24c81dae, /// 0x9f4
			 0xa917d43c, /// 0x9f8
			 0xb0ed973f, /// 0x9fc
			 0x6c5e0838, /// 0xa00
			 0xbf2e9186, /// 0xa04
			 0xfbd258c6, /// 0xa08
			 0xc28aa645, /// 0xa0c
			 0xb6ca4170, /// 0xa10
			 0x1f96d9e8, /// 0xa14
			 0x34b5854d, /// 0xa18
			 0x212fd834, /// 0xa1c
			 0x1565ccf6, /// 0xa20
			 0x36c86954, /// 0xa24
			 0x72d74b5a, /// 0xa28
			 0xc8937baf, /// 0xa2c
			 0x867b4ec8, /// 0xa30
			 0x5041e371, /// 0xa34
			 0xce4b7d81, /// 0xa38
			 0x89ad0b8f, /// 0xa3c
			 0xbba15087, /// 0xa40
			 0x82c16839, /// 0xa44
			 0xde6fbbdc, /// 0xa48
			 0x25b073f3, /// 0xa4c
			 0x5738f228, /// 0xa50
			 0x6513e509, /// 0xa54
			 0x843ce530, /// 0xa58
			 0x116ad7c7, /// 0xa5c
			 0x85c045e3, /// 0xa60
			 0xcf6ffa31, /// 0xa64
			 0xdc67e3b6, /// 0xa68
			 0x510116d1, /// 0xa6c
			 0x087019e0, /// 0xa70
			 0xf66e9011, /// 0xa74
			 0x700ca249, /// 0xa78
			 0x50ff63a8, /// 0xa7c
			 0x3cfa659e, /// 0xa80
			 0xb4d7b0c8, /// 0xa84
			 0x98c0965c, /// 0xa88
			 0x75455195, /// 0xa8c
			 0xb67325c5, /// 0xa90
			 0xe9431ae4, /// 0xa94
			 0x20290459, /// 0xa98
			 0x9d8efc16, /// 0xa9c
			 0x137da09b, /// 0xaa0
			 0xda944b52, /// 0xaa4
			 0xd1afeb8d, /// 0xaa8
			 0x8c2da883, /// 0xaac
			 0xb7f906d9, /// 0xab0
			 0xdfbb19cb, /// 0xab4
			 0xb99484c2, /// 0xab8
			 0xe21764a7, /// 0xabc
			 0xd48cca7e, /// 0xac0
			 0x31d44bc0, /// 0xac4
			 0x64a408d3, /// 0xac8
			 0xbb01b75f, /// 0xacc
			 0x0e9bf33e, /// 0xad0
			 0xb2060b46, /// 0xad4
			 0xf56550af, /// 0xad8
			 0xd0ebd2f2, /// 0xadc
			 0x4822fe2e, /// 0xae0
			 0x7e99789c, /// 0xae4
			 0x93a8e051, /// 0xae8
			 0x6b7f6190, /// 0xaec
			 0x12dc7432, /// 0xaf0
			 0xbdf5db25, /// 0xaf4
			 0x39a46453, /// 0xaf8
			 0x8192a80c, /// 0xafc
			 0x381c5e32, /// 0xb00
			 0x0e00ad00, /// 0xb04
			 0x37e9a209, /// 0xb08
			 0x61e699eb, /// 0xb0c
			 0xf2479914, /// 0xb10
			 0xb8f610fd, /// 0xb14
			 0xaff9d34f, /// 0xb18
			 0xc83952d1, /// 0xb1c
			 0xb6bda9f6, /// 0xb20
			 0x2ab27a4a, /// 0xb24
			 0xfb922039, /// 0xb28
			 0x5730832b, /// 0xb2c
			 0xe951df7e, /// 0xb30
			 0xe6245cae, /// 0xb34
			 0x772a9299, /// 0xb38
			 0xd9e9de0c, /// 0xb3c
			 0xa6b4654b, /// 0xb40
			 0x19ce60f6, /// 0xb44
			 0x0cf0b30e, /// 0xb48
			 0x4b3a9ef1, /// 0xb4c
			 0x2df9173a, /// 0xb50
			 0xeb7e87de, /// 0xb54
			 0xba3c9de3, /// 0xb58
			 0x194d0151, /// 0xb5c
			 0x20d8f2bb, /// 0xb60
			 0xc7db4776, /// 0xb64
			 0x432d8357, /// 0xb68
			 0xc80ac47b, /// 0xb6c
			 0xe8f7dad0, /// 0xb70
			 0x0d28a65e, /// 0xb74
			 0x720d1e24, /// 0xb78
			 0xc9a1addb, /// 0xb7c
			 0x329a826f, /// 0xb80
			 0x4d973b36, /// 0xb84
			 0xef9631d1, /// 0xb88
			 0x5872c2ac, /// 0xb8c
			 0x9db56764, /// 0xb90
			 0xe1e6b370, /// 0xb94
			 0x5d75a95b, /// 0xb98
			 0x1f7a306a, /// 0xb9c
			 0x2a6f656e, /// 0xba0
			 0x900c96c2, /// 0xba4
			 0x9c512b4b, /// 0xba8
			 0xd4d2c072, /// 0xbac
			 0x6e03f057, /// 0xbb0
			 0x27f3d9cd, /// 0xbb4
			 0x67e1bc91, /// 0xbb8
			 0x4bc29f3a, /// 0xbbc
			 0x393b21ad, /// 0xbc0
			 0x15771806, /// 0xbc4
			 0x4f7be264, /// 0xbc8
			 0x90ad1e62, /// 0xbcc
			 0x7e2db28b, /// 0xbd0
			 0xa5783e19, /// 0xbd4
			 0xabc8ca13, /// 0xbd8
			 0xaa11effb, /// 0xbdc
			 0xbe0c945d, /// 0xbe0
			 0xa00722a1, /// 0xbe4
			 0x04bdc414, /// 0xbe8
			 0x8a22793a, /// 0xbec
			 0xa76b8ebf, /// 0xbf0
			 0xc433639d, /// 0xbf4
			 0x3fd8ef0a, /// 0xbf8
			 0xb767d001, /// 0xbfc
			 0x77a9c6ce, /// 0xc00
			 0xee247e05, /// 0xc04
			 0xcd4e5cf0, /// 0xc08
			 0x4eb13faa, /// 0xc0c
			 0xfa9092f2, /// 0xc10
			 0x0b538f99, /// 0xc14
			 0x363f54b3, /// 0xc18
			 0x1d7be7ee, /// 0xc1c
			 0x92f453f8, /// 0xc20
			 0x952e8aee, /// 0xc24
			 0x851ce4a5, /// 0xc28
			 0x276d74f8, /// 0xc2c
			 0xa47f3d7c, /// 0xc30
			 0x908a54cc, /// 0xc34
			 0xaeec3985, /// 0xc38
			 0xc9e53029, /// 0xc3c
			 0x3d06586b, /// 0xc40
			 0xc4ef46f6, /// 0xc44
			 0x9eb48c25, /// 0xc48
			 0x935860e1, /// 0xc4c
			 0xb1e67813, /// 0xc50
			 0x481d497e, /// 0xc54
			 0xc76ce9db, /// 0xc58
			 0x19daeca2, /// 0xc5c
			 0x9e859384, /// 0xc60
			 0x91751125, /// 0xc64
			 0x724ff07f, /// 0xc68
			 0xbe0ff208, /// 0xc6c
			 0xe8055569, /// 0xc70
			 0x34338573, /// 0xc74
			 0xc97b58ef, /// 0xc78
			 0x073f9332, /// 0xc7c
			 0xe622dba6, /// 0xc80
			 0xb43ea607, /// 0xc84
			 0xed247f83, /// 0xc88
			 0xb8124009, /// 0xc8c
			 0x00045785, /// 0xc90
			 0xb0c3bfab, /// 0xc94
			 0xb378dafe, /// 0xc98
			 0x49be0a6e, /// 0xc9c
			 0x5c6908a3, /// 0xca0
			 0xd5c39e03, /// 0xca4
			 0xec1a118f, /// 0xca8
			 0x52b00aba, /// 0xcac
			 0xe9c03d6e, /// 0xcb0
			 0x5127e33d, /// 0xcb4
			 0xec1a8b85, /// 0xcb8
			 0xc0eb868b, /// 0xcbc
			 0xb18ac72c, /// 0xcc0
			 0xfa33fce8, /// 0xcc4
			 0x3869dc5e, /// 0xcc8
			 0x351428c9, /// 0xccc
			 0x3c80ea80, /// 0xcd0
			 0xe015dd5f, /// 0xcd4
			 0xa0b2a8bd, /// 0xcd8
			 0xef777054, /// 0xcdc
			 0x4fd24ff9, /// 0xce0
			 0xcf918efa, /// 0xce4
			 0x74261f7f, /// 0xce8
			 0x1ad7a794, /// 0xcec
			 0xb777d9ed, /// 0xcf0
			 0x6072c1bb, /// 0xcf4
			 0x23fbbc18, /// 0xcf8
			 0x22803689, /// 0xcfc
			 0x07c10c1b, /// 0xd00
			 0x700dadfd, /// 0xd04
			 0xc09f19b5, /// 0xd08
			 0x52d8810a, /// 0xd0c
			 0x85bce2b4, /// 0xd10
			 0x831eaf98, /// 0xd14
			 0x054c771e, /// 0xd18
			 0xb1a509f8, /// 0xd1c
			 0xa1839fcd, /// 0xd20
			 0xca67feb2, /// 0xd24
			 0x049ef249, /// 0xd28
			 0x6b43499a, /// 0xd2c
			 0x9ead329a, /// 0xd30
			 0x7d4e3786, /// 0xd34
			 0xacd6a18b, /// 0xd38
			 0x475ce78a, /// 0xd3c
			 0x23f55226, /// 0xd40
			 0x2922045c, /// 0xd44
			 0x405dc1c4, /// 0xd48
			 0xfc9f1809, /// 0xd4c
			 0xd4a250b2, /// 0xd50
			 0x661ea401, /// 0xd54
			 0xe102cece, /// 0xd58
			 0xcff48a6d, /// 0xd5c
			 0xed7094d8, /// 0xd60
			 0xed52c52e, /// 0xd64
			 0xb53dc1fc, /// 0xd68
			 0x78cd2392, /// 0xd6c
			 0x0729a9a9, /// 0xd70
			 0x76ee1e50, /// 0xd74
			 0xf3c38553, /// 0xd78
			 0x0b2d91ac, /// 0xd7c
			 0x0bcaf165, /// 0xd80
			 0x269aeded, /// 0xd84
			 0xd4128562, /// 0xd88
			 0x00e60f15, /// 0xd8c
			 0x14fb54d5, /// 0xd90
			 0x1440969c, /// 0xd94
			 0x4c5a7b4b, /// 0xd98
			 0x67c9d730, /// 0xd9c
			 0x4d06f368, /// 0xda0
			 0x6d3f9be8, /// 0xda4
			 0x902002d0, /// 0xda8
			 0x6e4b311a, /// 0xdac
			 0xc354b310, /// 0xdb0
			 0xbed6156e, /// 0xdb4
			 0xc706d99c, /// 0xdb8
			 0x366f1304, /// 0xdbc
			 0xfb23ac4a, /// 0xdc0
			 0x25b9c8ed, /// 0xdc4
			 0x336b622f, /// 0xdc8
			 0xd28401cc, /// 0xdcc
			 0xefad5412, /// 0xdd0
			 0xdaeb8a22, /// 0xdd4
			 0x79b04ab6, /// 0xdd8
			 0xb3d9c25d, /// 0xddc
			 0x81cf3941, /// 0xde0
			 0x47f741ef, /// 0xde4
			 0x193cb575, /// 0xde8
			 0x99bb18b2, /// 0xdec
			 0x2640cec4, /// 0xdf0
			 0x611b91d0, /// 0xdf4
			 0x3fa49b31, /// 0xdf8
			 0x6dc187a3, /// 0xdfc
			 0x98eea067, /// 0xe00
			 0xcbb50458, /// 0xe04
			 0xdd2d8a88, /// 0xe08
			 0xdaba8c59, /// 0xe0c
			 0x8484b0eb, /// 0xe10
			 0xc054b3dc, /// 0xe14
			 0x651443fb, /// 0xe18
			 0xc012bc20, /// 0xe1c
			 0xd8bed8b1, /// 0xe20
			 0x6e27d37e, /// 0xe24
			 0x7e938ead, /// 0xe28
			 0x857acec7, /// 0xe2c
			 0x3afa85c5, /// 0xe30
			 0x65fdc7b3, /// 0xe34
			 0x8b1b7f54, /// 0xe38
			 0x2cb84795, /// 0xe3c
			 0xb2dd8c53, /// 0xe40
			 0xb2e20202, /// 0xe44
			 0xb17afb8e, /// 0xe48
			 0x208832e5, /// 0xe4c
			 0xb717e8d1, /// 0xe50
			 0xd90bec24, /// 0xe54
			 0x9efacaf1, /// 0xe58
			 0x9d81672f, /// 0xe5c
			 0x4a3c82cb, /// 0xe60
			 0xd07a8eb9, /// 0xe64
			 0x1c480dfd, /// 0xe68
			 0x1e77e218, /// 0xe6c
			 0x26449513, /// 0xe70
			 0xef122f9b, /// 0xe74
			 0x4b2215da, /// 0xe78
			 0x94f56cd7, /// 0xe7c
			 0xa1ada8ae, /// 0xe80
			 0x76d4bb7b, /// 0xe84
			 0x03d91169, /// 0xe88
			 0xb4f0ac4d, /// 0xe8c
			 0xa35d8183, /// 0xe90
			 0xb43622e5, /// 0xe94
			 0x08a34d6d, /// 0xe98
			 0x209f7070, /// 0xe9c
			 0xf2605f67, /// 0xea0
			 0xc0c6577c, /// 0xea4
			 0x4e1bbcf2, /// 0xea8
			 0x9e967102, /// 0xeac
			 0x70e49dde, /// 0xeb0
			 0xe9da09be, /// 0xeb4
			 0x9b602b1a, /// 0xeb8
			 0xec6af428, /// 0xebc
			 0x46b8cf4e, /// 0xec0
			 0xda25ee9a, /// 0xec4
			 0x3da7d1d3, /// 0xec8
			 0x0d551aef, /// 0xecc
			 0x7db50c2c, /// 0xed0
			 0x77fac25e, /// 0xed4
			 0x1cdcb9aa, /// 0xed8
			 0x2fddf713, /// 0xedc
			 0x951c3968, /// 0xee0
			 0x0224445b, /// 0xee4
			 0xd30fe4e9, /// 0xee8
			 0xb7db8c3b, /// 0xeec
			 0x8ef8b350, /// 0xef0
			 0xb7a83309, /// 0xef4
			 0x4cbda05f, /// 0xef8
			 0x52f053c2, /// 0xefc
			 0x0e435daa, /// 0xf00
			 0xd7eac114, /// 0xf04
			 0x2ceac51e, /// 0xf08
			 0xa0bf3333, /// 0xf0c
			 0xd45831ba, /// 0xf10
			 0x9a09f318, /// 0xf14
			 0x2367776d, /// 0xf18
			 0x7a625e51, /// 0xf1c
			 0x628471b5, /// 0xf20
			 0x9b046174, /// 0xf24
			 0xc1b2810c, /// 0xf28
			 0x71b303f8, /// 0xf2c
			 0xd919eb8c, /// 0xf30
			 0x47a3171c, /// 0xf34
			 0x119c51eb, /// 0xf38
			 0x2f6b47f1, /// 0xf3c
			 0x5fd9aa27, /// 0xf40
			 0xcb782c9e, /// 0xf44
			 0xb8dbc7da, /// 0xf48
			 0xe37700eb, /// 0xf4c
			 0x05986e7b, /// 0xf50
			 0x9e0ef880, /// 0xf54
			 0xfa066e24, /// 0xf58
			 0xbc153d40, /// 0xf5c
			 0xe921b1f4, /// 0xf60
			 0x9b7ea0d4, /// 0xf64
			 0xb7827aab, /// 0xf68
			 0x45776a78, /// 0xf6c
			 0x2c2f923a, /// 0xf70
			 0x70ef9154, /// 0xf74
			 0x838e0da5, /// 0xf78
			 0xf82b13d5, /// 0xf7c
			 0xc43e85f1, /// 0xf80
			 0x6b9e274d, /// 0xf84
			 0x7e74be5d, /// 0xf88
			 0xbc3fe5a6, /// 0xf8c
			 0x3f6c6880, /// 0xf90
			 0x10d57876, /// 0xf94
			 0x145bfb77, /// 0xf98
			 0x6b2e0bd7, /// 0xf9c
			 0x8ba404e0, /// 0xfa0
			 0xa6c37d7d, /// 0xfa4
			 0x9e7038b6, /// 0xfa8
			 0xf4de0525, /// 0xfac
			 0x3af4569d, /// 0xfb0
			 0x0587aa0f, /// 0xfb4
			 0x7fe1a80f, /// 0xfb8
			 0x615a548a, /// 0xfbc
			 0x133279a4, /// 0xfc0
			 0xa9a9d85d, /// 0xfc4
			 0xb4b26974, /// 0xfc8
			 0xabb50703, /// 0xfcc
			 0x5c7aa66a, /// 0xfd0
			 0xfa7e0567, /// 0xfd4
			 0xb6705fe5, /// 0xfd8
			 0x8e88bf3d, /// 0xfdc
			 0xe7deafeb, /// 0xfe0
			 0x8ff94005, /// 0xfe4
			 0x5c85a9e3, /// 0xfe8
			 0x6a44375a, /// 0xfec
			 0xb539c452, /// 0xff0
			 0xd55a91bd, /// 0xff4
			 0x8a76405a, /// 0xff8
			 0x8b8c9af5 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x61e9ce55, /// 0x0
			 0x53062e61, /// 0x4
			 0xe9c88c1f, /// 0x8
			 0xe63b16ce, /// 0xc
			 0x19775cf5, /// 0x10
			 0xee404e75, /// 0x14
			 0xf9aa10fb, /// 0x18
			 0xf447c090, /// 0x1c
			 0xaded268d, /// 0x20
			 0xbfbc3358, /// 0x24
			 0xafb74d29, /// 0x28
			 0xd8738de4, /// 0x2c
			 0x69308601, /// 0x30
			 0xb3b48158, /// 0x34
			 0xca340ba1, /// 0x38
			 0xdbee56c9, /// 0x3c
			 0x3dca89b4, /// 0x40
			 0x493d362c, /// 0x44
			 0xf403f870, /// 0x48
			 0xd0f100cc, /// 0x4c
			 0x50196f76, /// 0x50
			 0x9ddc2670, /// 0x54
			 0xecc4ca8c, /// 0x58
			 0x20c5a334, /// 0x5c
			 0x34ddfcf3, /// 0x60
			 0xcf22f45e, /// 0x64
			 0x1c7c3845, /// 0x68
			 0x08dd98ba, /// 0x6c
			 0xb0d03b66, /// 0x70
			 0x4f48a3e7, /// 0x74
			 0x7bf321a5, /// 0x78
			 0x8a303bca, /// 0x7c
			 0x77cc756e, /// 0x80
			 0xf50d23ec, /// 0x84
			 0x7ae33bb8, /// 0x88
			 0x82945091, /// 0x8c
			 0x2b7dc481, /// 0x90
			 0xef8400e9, /// 0x94
			 0x7f42cb4c, /// 0x98
			 0x021fec3e, /// 0x9c
			 0x9130cb14, /// 0xa0
			 0xc022bda7, /// 0xa4
			 0x01f1b5d8, /// 0xa8
			 0xfd9e3f31, /// 0xac
			 0x0d6a066b, /// 0xb0
			 0x0deb0e9b, /// 0xb4
			 0xa9f17c64, /// 0xb8
			 0x2e6016ce, /// 0xbc
			 0x014c5cf2, /// 0xc0
			 0xba589f38, /// 0xc4
			 0x3159073b, /// 0xc8
			 0x2aa131d5, /// 0xcc
			 0xfcb2c862, /// 0xd0
			 0x0adc7344, /// 0xd4
			 0xabfd6239, /// 0xd8
			 0xed39df76, /// 0xdc
			 0x494c3eaa, /// 0xe0
			 0xd80cd0c2, /// 0xe4
			 0xee929c87, /// 0xe8
			 0x09acecd5, /// 0xec
			 0xff6772ae, /// 0xf0
			 0x25660f34, /// 0xf4
			 0xd949e50a, /// 0xf8
			 0xaeccdf1f, /// 0xfc
			 0x41d0045e, /// 0x100
			 0xd7946326, /// 0x104
			 0xe278612f, /// 0x108
			 0x7c4854eb, /// 0x10c
			 0x86745856, /// 0x110
			 0xc759cfc4, /// 0x114
			 0x82a993d9, /// 0x118
			 0xe47739dc, /// 0x11c
			 0x5d7b45e6, /// 0x120
			 0x1b44e857, /// 0x124
			 0x17e2cd06, /// 0x128
			 0x589f5506, /// 0x12c
			 0xeb0261f9, /// 0x130
			 0xb3efdbcd, /// 0x134
			 0x53c8b222, /// 0x138
			 0xc34b80f8, /// 0x13c
			 0xa0412647, /// 0x140
			 0x0fc51589, /// 0x144
			 0x3a12a647, /// 0x148
			 0xc354f4e2, /// 0x14c
			 0xf3abc8b2, /// 0x150
			 0x8d6a07a9, /// 0x154
			 0x5902c993, /// 0x158
			 0x4784fca4, /// 0x15c
			 0x90b73466, /// 0x160
			 0xea399f9d, /// 0x164
			 0xadc2f1e1, /// 0x168
			 0xfcb8157c, /// 0x16c
			 0xdd590daf, /// 0x170
			 0x9083851c, /// 0x174
			 0x68fb130b, /// 0x178
			 0xc6ab3243, /// 0x17c
			 0xfe120f61, /// 0x180
			 0xa098d3dd, /// 0x184
			 0x55e5dd27, /// 0x188
			 0xbab9931b, /// 0x18c
			 0xae89180f, /// 0x190
			 0x7d6c34ac, /// 0x194
			 0xd8b2f4ba, /// 0x198
			 0x1a58f1e4, /// 0x19c
			 0x27346c62, /// 0x1a0
			 0x1880de55, /// 0x1a4
			 0x9e2b5967, /// 0x1a8
			 0x2618b5b2, /// 0x1ac
			 0xea6ca1f9, /// 0x1b0
			 0xab57e400, /// 0x1b4
			 0x3b464d08, /// 0x1b8
			 0xba050928, /// 0x1bc
			 0x95c69528, /// 0x1c0
			 0xab66e7e6, /// 0x1c4
			 0x206b74b2, /// 0x1c8
			 0x795b9486, /// 0x1cc
			 0xcf185b56, /// 0x1d0
			 0x83225c55, /// 0x1d4
			 0xc087e524, /// 0x1d8
			 0x298fcca1, /// 0x1dc
			 0x9162cbe9, /// 0x1e0
			 0x8f12bdaf, /// 0x1e4
			 0x64c18f8d, /// 0x1e8
			 0x93523a31, /// 0x1ec
			 0xe40c4871, /// 0x1f0
			 0x7776039f, /// 0x1f4
			 0x1d34747d, /// 0x1f8
			 0x43b34721, /// 0x1fc
			 0x639d05cf, /// 0x200
			 0x153a0d5a, /// 0x204
			 0x059efeb4, /// 0x208
			 0x9f6f60d6, /// 0x20c
			 0xccb88238, /// 0x210
			 0xa0861c75, /// 0x214
			 0xd76a446c, /// 0x218
			 0xb39e12c1, /// 0x21c
			 0xb6dab40a, /// 0x220
			 0xd5112c93, /// 0x224
			 0x446bd1c2, /// 0x228
			 0x5cfaf84f, /// 0x22c
			 0xf66cb786, /// 0x230
			 0x0561dc5f, /// 0x234
			 0x1414ab53, /// 0x238
			 0xcd2760dc, /// 0x23c
			 0xf5585c23, /// 0x240
			 0x7ffb51b3, /// 0x244
			 0x0f7ea0ca, /// 0x248
			 0x8c907bba, /// 0x24c
			 0xd4f9f1e6, /// 0x250
			 0x9b96101c, /// 0x254
			 0xc6e8b627, /// 0x258
			 0x91122a16, /// 0x25c
			 0x831655dd, /// 0x260
			 0x2ebfaeb5, /// 0x264
			 0x039ac3f8, /// 0x268
			 0xc010af49, /// 0x26c
			 0xb1c47c5d, /// 0x270
			 0xad2e54e3, /// 0x274
			 0x40cc2dff, /// 0x278
			 0x79fcd87f, /// 0x27c
			 0x875b58ed, /// 0x280
			 0x8abbe2fa, /// 0x284
			 0x68a56071, /// 0x288
			 0x5dba009d, /// 0x28c
			 0x481eadf7, /// 0x290
			 0xb5882f3d, /// 0x294
			 0x37f37b0d, /// 0x298
			 0x13d6e9e2, /// 0x29c
			 0x31ef121f, /// 0x2a0
			 0x56dc66e2, /// 0x2a4
			 0x635578a7, /// 0x2a8
			 0x9df84f5a, /// 0x2ac
			 0xc18150d9, /// 0x2b0
			 0xd92d5905, /// 0x2b4
			 0x95d7822c, /// 0x2b8
			 0xb990da2c, /// 0x2bc
			 0x634f7db4, /// 0x2c0
			 0xcc6ae681, /// 0x2c4
			 0x44beb846, /// 0x2c8
			 0xf03e09ec, /// 0x2cc
			 0x9b4eac67, /// 0x2d0
			 0x87ef9ac6, /// 0x2d4
			 0xab35ba00, /// 0x2d8
			 0x83e8b34f, /// 0x2dc
			 0x86b5f433, /// 0x2e0
			 0x4673c7f5, /// 0x2e4
			 0xbcbff89b, /// 0x2e8
			 0xa750f5f8, /// 0x2ec
			 0x7fde0595, /// 0x2f0
			 0x5014125f, /// 0x2f4
			 0x4a661ed0, /// 0x2f8
			 0x44126ca6, /// 0x2fc
			 0x70bba82a, /// 0x300
			 0x2f2a96f2, /// 0x304
			 0x8f435cbf, /// 0x308
			 0xe7c1c101, /// 0x30c
			 0x6950fee5, /// 0x310
			 0xe8fa4bbd, /// 0x314
			 0x3e153540, /// 0x318
			 0xc98f8fc2, /// 0x31c
			 0x1c8c4757, /// 0x320
			 0x31d1d8c8, /// 0x324
			 0x2282f9d2, /// 0x328
			 0x01c0804d, /// 0x32c
			 0x8bbd79b8, /// 0x330
			 0xa2c9b6f9, /// 0x334
			 0x00247586, /// 0x338
			 0xa98c7237, /// 0x33c
			 0xd29c7423, /// 0x340
			 0x772f2cdf, /// 0x344
			 0xd82aae42, /// 0x348
			 0x08af4ce5, /// 0x34c
			 0xedaffe32, /// 0x350
			 0x5ac6f24c, /// 0x354
			 0x21f13b37, /// 0x358
			 0xc9c1adf9, /// 0x35c
			 0x55a04a74, /// 0x360
			 0x9f3402ad, /// 0x364
			 0xa28aefde, /// 0x368
			 0xe6b0f302, /// 0x36c
			 0xb860fe00, /// 0x370
			 0x1ed3c079, /// 0x374
			 0xafe66f5f, /// 0x378
			 0x82281842, /// 0x37c
			 0x4b754255, /// 0x380
			 0x20b6172f, /// 0x384
			 0x134ecb90, /// 0x388
			 0x7672d224, /// 0x38c
			 0xf8fbe54b, /// 0x390
			 0xb8e1d891, /// 0x394
			 0x55b6fa0d, /// 0x398
			 0xfc33f5b9, /// 0x39c
			 0xaca7a3dc, /// 0x3a0
			 0x9862f22b, /// 0x3a4
			 0x38578cfc, /// 0x3a8
			 0x689a9c69, /// 0x3ac
			 0xad62c44f, /// 0x3b0
			 0x20534d2b, /// 0x3b4
			 0x2adaede0, /// 0x3b8
			 0x5a571219, /// 0x3bc
			 0xb38d07bd, /// 0x3c0
			 0x5093a9fb, /// 0x3c4
			 0x2de7030f, /// 0x3c8
			 0x57de40b2, /// 0x3cc
			 0x0f41d701, /// 0x3d0
			 0x59aa8c49, /// 0x3d4
			 0xecbac7c6, /// 0x3d8
			 0xd43f595e, /// 0x3dc
			 0xf2a00cd5, /// 0x3e0
			 0x34ad88e3, /// 0x3e4
			 0xa266623a, /// 0x3e8
			 0x3b63c5d5, /// 0x3ec
			 0x530a7666, /// 0x3f0
			 0x718d12c2, /// 0x3f4
			 0xbc70336f, /// 0x3f8
			 0xd38b7842, /// 0x3fc
			 0x08afa821, /// 0x400
			 0xc86f5585, /// 0x404
			 0xac3afc9c, /// 0x408
			 0x15cd22da, /// 0x40c
			 0x0bcc3b42, /// 0x410
			 0x6c924df1, /// 0x414
			 0x3b453b9d, /// 0x418
			 0x71609602, /// 0x41c
			 0xa4fd29a1, /// 0x420
			 0x7afa7a35, /// 0x424
			 0x02f4aa24, /// 0x428
			 0x62243f31, /// 0x42c
			 0x5e294f3b, /// 0x430
			 0x904859b2, /// 0x434
			 0xc72fc129, /// 0x438
			 0x4995bd28, /// 0x43c
			 0x1cb3a615, /// 0x440
			 0x2f8e7a7c, /// 0x444
			 0x15fcaef0, /// 0x448
			 0xa54f1a31, /// 0x44c
			 0x87681110, /// 0x450
			 0xa93de9dd, /// 0x454
			 0x0dfe69ce, /// 0x458
			 0xeb619797, /// 0x45c
			 0xef71af90, /// 0x460
			 0xa503ae07, /// 0x464
			 0x58e32696, /// 0x468
			 0xcc61e168, /// 0x46c
			 0x426a3ffb, /// 0x470
			 0xfbeeb315, /// 0x474
			 0xa6613cb6, /// 0x478
			 0x7da899ec, /// 0x47c
			 0x1ef39bd5, /// 0x480
			 0xed4e689b, /// 0x484
			 0x354f3df1, /// 0x488
			 0x84bab501, /// 0x48c
			 0x5cd76049, /// 0x490
			 0xb5646b2f, /// 0x494
			 0x22b693c9, /// 0x498
			 0xf9a2dbae, /// 0x49c
			 0x664a64a8, /// 0x4a0
			 0xbc8b79bd, /// 0x4a4
			 0xd8bffc6d, /// 0x4a8
			 0xd82f0639, /// 0x4ac
			 0x62f0017d, /// 0x4b0
			 0xe83c9923, /// 0x4b4
			 0xe928d6b0, /// 0x4b8
			 0xf197733f, /// 0x4bc
			 0x1213ed1c, /// 0x4c0
			 0x86b113e8, /// 0x4c4
			 0xea2228c1, /// 0x4c8
			 0xb200770a, /// 0x4cc
			 0x1ed95af4, /// 0x4d0
			 0xa7b3a9a4, /// 0x4d4
			 0xf1409e88, /// 0x4d8
			 0x1fade7a7, /// 0x4dc
			 0x93e8d748, /// 0x4e0
			 0xfa960c88, /// 0x4e4
			 0xea3f4381, /// 0x4e8
			 0x66310e92, /// 0x4ec
			 0x942d3558, /// 0x4f0
			 0x3afdbbe3, /// 0x4f4
			 0x5700c56b, /// 0x4f8
			 0x1684c8fe, /// 0x4fc
			 0x38efef17, /// 0x500
			 0xf2893fcd, /// 0x504
			 0xb2df1322, /// 0x508
			 0x519da70f, /// 0x50c
			 0xfc13e217, /// 0x510
			 0x91d6672e, /// 0x514
			 0xc4296f79, /// 0x518
			 0xa206fa7d, /// 0x51c
			 0x3b45b966, /// 0x520
			 0xe05d3bd0, /// 0x524
			 0x8e50d277, /// 0x528
			 0x1845cd5a, /// 0x52c
			 0x24718e24, /// 0x530
			 0x54ab8bac, /// 0x534
			 0xdbde9214, /// 0x538
			 0xf59bbc21, /// 0x53c
			 0x0ad2857c, /// 0x540
			 0x211aea19, /// 0x544
			 0x75541078, /// 0x548
			 0x1d3e5b0c, /// 0x54c
			 0x08db734e, /// 0x550
			 0x37c9a6ea, /// 0x554
			 0xa0fa4204, /// 0x558
			 0xea40f91c, /// 0x55c
			 0xd19e57ae, /// 0x560
			 0x70c68a3d, /// 0x564
			 0x05dc0d19, /// 0x568
			 0x6040f54b, /// 0x56c
			 0xa38c99bb, /// 0x570
			 0x85ae1922, /// 0x574
			 0x2f3cb83d, /// 0x578
			 0x4b92ff74, /// 0x57c
			 0x8bead2a3, /// 0x580
			 0x8c275da9, /// 0x584
			 0x24b24bd0, /// 0x588
			 0x4beb03c0, /// 0x58c
			 0x8c4a0fb9, /// 0x590
			 0x4e501409, /// 0x594
			 0x20c65ba8, /// 0x598
			 0xcb80d786, /// 0x59c
			 0x0a7bb665, /// 0x5a0
			 0xa7c9739f, /// 0x5a4
			 0x03cb0a88, /// 0x5a8
			 0xb0e9672f, /// 0x5ac
			 0x0cc6df2a, /// 0x5b0
			 0x0bc4b88c, /// 0x5b4
			 0xfa0677ad, /// 0x5b8
			 0x224bd873, /// 0x5bc
			 0x875b2a2c, /// 0x5c0
			 0x996b63b9, /// 0x5c4
			 0x376b084a, /// 0x5c8
			 0xd598492e, /// 0x5cc
			 0x61440d7c, /// 0x5d0
			 0x1f051e1d, /// 0x5d4
			 0x6dcb488f, /// 0x5d8
			 0xee3eda03, /// 0x5dc
			 0x56021ac0, /// 0x5e0
			 0xdea8cf6f, /// 0x5e4
			 0xbda45959, /// 0x5e8
			 0xfc42b9f3, /// 0x5ec
			 0xef3fd614, /// 0x5f0
			 0x2b02d5e8, /// 0x5f4
			 0x37b43dfc, /// 0x5f8
			 0xf4d4b462, /// 0x5fc
			 0x655c97ac, /// 0x600
			 0xb460df2b, /// 0x604
			 0xdac073f5, /// 0x608
			 0x7f30ba75, /// 0x60c
			 0x6855c042, /// 0x610
			 0x39f80294, /// 0x614
			 0x98e8f101, /// 0x618
			 0xdc8ac7de, /// 0x61c
			 0x6b05dc4e, /// 0x620
			 0xd9498760, /// 0x624
			 0x2ba05237, /// 0x628
			 0x702449c9, /// 0x62c
			 0xd6eb96c1, /// 0x630
			 0xdbfef84d, /// 0x634
			 0xde2bb808, /// 0x638
			 0x78f03149, /// 0x63c
			 0x59a7a0d0, /// 0x640
			 0x0e2d27da, /// 0x644
			 0xa4682d16, /// 0x648
			 0x602d68ab, /// 0x64c
			 0xc3ae4118, /// 0x650
			 0x2aed43db, /// 0x654
			 0x34683e3a, /// 0x658
			 0xa6198046, /// 0x65c
			 0x1e5c33fe, /// 0x660
			 0x6168ebcf, /// 0x664
			 0x36a7d3c3, /// 0x668
			 0x91f404e4, /// 0x66c
			 0x46db520c, /// 0x670
			 0xc17b155c, /// 0x674
			 0x3ac4bd6a, /// 0x678
			 0x5c6928a1, /// 0x67c
			 0xfccf4b96, /// 0x680
			 0xfa96a627, /// 0x684
			 0xde820328, /// 0x688
			 0x0728cc79, /// 0x68c
			 0x9b73b554, /// 0x690
			 0x1e42cda2, /// 0x694
			 0x7b67aeda, /// 0x698
			 0x6b86c6b1, /// 0x69c
			 0x623dcebd, /// 0x6a0
			 0x0aff7c97, /// 0x6a4
			 0xf46b570a, /// 0x6a8
			 0xd98c1a9c, /// 0x6ac
			 0xbd5127da, /// 0x6b0
			 0xfcc32478, /// 0x6b4
			 0x4deeb2c5, /// 0x6b8
			 0xb2626c6a, /// 0x6bc
			 0x6490d67f, /// 0x6c0
			 0x24678e68, /// 0x6c4
			 0x2bb1cba2, /// 0x6c8
			 0x73d75ebc, /// 0x6cc
			 0x0fc62750, /// 0x6d0
			 0x7211a274, /// 0x6d4
			 0x15656cc9, /// 0x6d8
			 0xf86957c0, /// 0x6dc
			 0xd63687b6, /// 0x6e0
			 0x6eb04fd6, /// 0x6e4
			 0x011f56eb, /// 0x6e8
			 0x9c517939, /// 0x6ec
			 0x0ae4c5c1, /// 0x6f0
			 0x724f3f78, /// 0x6f4
			 0xbd42596a, /// 0x6f8
			 0x18f6a9bd, /// 0x6fc
			 0xd5a0a9cf, /// 0x700
			 0xb0ecca14, /// 0x704
			 0x1d11555f, /// 0x708
			 0x817e99ed, /// 0x70c
			 0xe6c36d35, /// 0x710
			 0x3a1ae149, /// 0x714
			 0x038e4c19, /// 0x718
			 0x57bab14c, /// 0x71c
			 0x6f2bd499, /// 0x720
			 0xcc71f7d4, /// 0x724
			 0x0856480e, /// 0x728
			 0xb828fb15, /// 0x72c
			 0x05f4f9a2, /// 0x730
			 0x55889629, /// 0x734
			 0xdbcdd5e2, /// 0x738
			 0xa7f89b55, /// 0x73c
			 0x9a0170b0, /// 0x740
			 0x3860a562, /// 0x744
			 0xdfe35a75, /// 0x748
			 0xbb3129e4, /// 0x74c
			 0xa129aaf5, /// 0x750
			 0x6ea219f0, /// 0x754
			 0xd26c168b, /// 0x758
			 0xe0da0c95, /// 0x75c
			 0xf08bfdab, /// 0x760
			 0x8d52b908, /// 0x764
			 0xb486748a, /// 0x768
			 0x998004a5, /// 0x76c
			 0x75939b27, /// 0x770
			 0x4dd9348f, /// 0x774
			 0x1b3e782c, /// 0x778
			 0xc09f5a9d, /// 0x77c
			 0x6e786d6a, /// 0x780
			 0xd4babc47, /// 0x784
			 0x69bc9906, /// 0x788
			 0xf3c63b4a, /// 0x78c
			 0xc6a9c092, /// 0x790
			 0x94e60e24, /// 0x794
			 0x7e2b5169, /// 0x798
			 0x249206da, /// 0x79c
			 0xd7ad7878, /// 0x7a0
			 0xb3a55468, /// 0x7a4
			 0x31fdcc26, /// 0x7a8
			 0xec638092, /// 0x7ac
			 0xe3bc151d, /// 0x7b0
			 0xb04c2271, /// 0x7b4
			 0xa8233d04, /// 0x7b8
			 0xbce83911, /// 0x7bc
			 0x665a1379, /// 0x7c0
			 0xbcfc7555, /// 0x7c4
			 0x69c40659, /// 0x7c8
			 0x779de584, /// 0x7cc
			 0x13533dfd, /// 0x7d0
			 0x71200281, /// 0x7d4
			 0x66688fed, /// 0x7d8
			 0x48fb0f66, /// 0x7dc
			 0xa9671e88, /// 0x7e0
			 0xfeacf0b6, /// 0x7e4
			 0x1e080d74, /// 0x7e8
			 0xf5b6004c, /// 0x7ec
			 0x2b2efb00, /// 0x7f0
			 0xa13ccefb, /// 0x7f4
			 0x59dc1387, /// 0x7f8
			 0x8add7ef8, /// 0x7fc
			 0x7e84c27a, /// 0x800
			 0x4add4597, /// 0x804
			 0xed664999, /// 0x808
			 0xc69546cb, /// 0x80c
			 0xba775366, /// 0x810
			 0x5c0567c4, /// 0x814
			 0x309190cf, /// 0x818
			 0x2304ae40, /// 0x81c
			 0xaf3725a8, /// 0x820
			 0xb19588ee, /// 0x824
			 0x1550034d, /// 0x828
			 0x77bb616c, /// 0x82c
			 0x2649208d, /// 0x830
			 0x2752cbb9, /// 0x834
			 0x13c65e2f, /// 0x838
			 0x718cb7aa, /// 0x83c
			 0xfa8e182b, /// 0x840
			 0x3e82244b, /// 0x844
			 0x82685f9b, /// 0x848
			 0x04ed90fd, /// 0x84c
			 0x7cab4c6f, /// 0x850
			 0xddebcbe1, /// 0x854
			 0x31521063, /// 0x858
			 0x8a1f95aa, /// 0x85c
			 0xa5ea5b0a, /// 0x860
			 0xaaf67859, /// 0x864
			 0x48c8f8b8, /// 0x868
			 0xaad9d33f, /// 0x86c
			 0xfebc8538, /// 0x870
			 0xe5f281e8, /// 0x874
			 0x6404bab9, /// 0x878
			 0xc9449fcd, /// 0x87c
			 0x49639640, /// 0x880
			 0xcc8f6c89, /// 0x884
			 0xdb057d3d, /// 0x888
			 0xc76793f8, /// 0x88c
			 0x023fc1bc, /// 0x890
			 0xcaa2b03c, /// 0x894
			 0x4b859f63, /// 0x898
			 0xfd0aabb5, /// 0x89c
			 0x297bc1b1, /// 0x8a0
			 0xf113896f, /// 0x8a4
			 0xc1fcc0ad, /// 0x8a8
			 0xbf4f94f6, /// 0x8ac
			 0xf2bb936b, /// 0x8b0
			 0xb91af442, /// 0x8b4
			 0x6e9e5aa3, /// 0x8b8
			 0xd5a6b843, /// 0x8bc
			 0x705bb406, /// 0x8c0
			 0xcbf30cfa, /// 0x8c4
			 0x39097920, /// 0x8c8
			 0x2b5c3580, /// 0x8cc
			 0x3311abac, /// 0x8d0
			 0x09c9a64a, /// 0x8d4
			 0xca07caed, /// 0x8d8
			 0xe3a08b27, /// 0x8dc
			 0x9de9356f, /// 0x8e0
			 0x2d97ac2d, /// 0x8e4
			 0x114db8af, /// 0x8e8
			 0x8c983310, /// 0x8ec
			 0x8e8ab3e3, /// 0x8f0
			 0xefda5f53, /// 0x8f4
			 0x3a32d2fe, /// 0x8f8
			 0xa4e4dda4, /// 0x8fc
			 0x700dfa61, /// 0x900
			 0xd18b905a, /// 0x904
			 0xb6bc2af5, /// 0x908
			 0xc96f8757, /// 0x90c
			 0xc4562c98, /// 0x910
			 0x9d0b5151, /// 0x914
			 0x2fea791d, /// 0x918
			 0x6eb1af58, /// 0x91c
			 0xfd260677, /// 0x920
			 0xf5bc012c, /// 0x924
			 0xd19615f2, /// 0x928
			 0xab628a22, /// 0x92c
			 0x152db7f7, /// 0x930
			 0xbe00d172, /// 0x934
			 0x4e816813, /// 0x938
			 0x7be03f14, /// 0x93c
			 0xa9d51fff, /// 0x940
			 0x1882e097, /// 0x944
			 0x919a9826, /// 0x948
			 0xccdc8413, /// 0x94c
			 0xb441d61a, /// 0x950
			 0xc6e436dc, /// 0x954
			 0xfcd7956c, /// 0x958
			 0x3e3d7515, /// 0x95c
			 0x9a03206c, /// 0x960
			 0xf033e7c3, /// 0x964
			 0x17a73c6e, /// 0x968
			 0xed38e202, /// 0x96c
			 0x0e17ca9a, /// 0x970
			 0x6e3902fb, /// 0x974
			 0x466db261, /// 0x978
			 0xedfb9007, /// 0x97c
			 0x19e1e13c, /// 0x980
			 0x6db81cad, /// 0x984
			 0x7dd8771b, /// 0x988
			 0xf1fe4e35, /// 0x98c
			 0x65bd7764, /// 0x990
			 0x0ba85b4b, /// 0x994
			 0x2fc75bc5, /// 0x998
			 0x8b2b1688, /// 0x99c
			 0xf9987b4d, /// 0x9a0
			 0x30a8ab75, /// 0x9a4
			 0x842bcb1e, /// 0x9a8
			 0x08ceddad, /// 0x9ac
			 0x848d7ed3, /// 0x9b0
			 0x8e3e7820, /// 0x9b4
			 0x010c07e6, /// 0x9b8
			 0x32cb7192, /// 0x9bc
			 0x751785e9, /// 0x9c0
			 0xa7b01cf8, /// 0x9c4
			 0xcc16c22a, /// 0x9c8
			 0x81cb0b83, /// 0x9cc
			 0x4f4d0d08, /// 0x9d0
			 0x45df051f, /// 0x9d4
			 0x49938819, /// 0x9d8
			 0xceb13c87, /// 0x9dc
			 0xdc950ad2, /// 0x9e0
			 0x82496b2b, /// 0x9e4
			 0xa03be152, /// 0x9e8
			 0x9bc09d82, /// 0x9ec
			 0xc0925b76, /// 0x9f0
			 0x8e75d5fd, /// 0x9f4
			 0x526a04d9, /// 0x9f8
			 0xfb3d0c82, /// 0x9fc
			 0x883cd858, /// 0xa00
			 0x3e0e8a8b, /// 0xa04
			 0xb319cefa, /// 0xa08
			 0xf0ee4088, /// 0xa0c
			 0x6b5f4918, /// 0xa10
			 0x305b3ed2, /// 0xa14
			 0xc7f8529a, /// 0xa18
			 0xea286a10, /// 0xa1c
			 0x9cdf191b, /// 0xa20
			 0x185f765c, /// 0xa24
			 0x8249036c, /// 0xa28
			 0x63569cdd, /// 0xa2c
			 0xc81f981d, /// 0xa30
			 0x573ef957, /// 0xa34
			 0x0b1c2e92, /// 0xa38
			 0xfb6f1122, /// 0xa3c
			 0x14940085, /// 0xa40
			 0x77bc343a, /// 0xa44
			 0xa55f4c99, /// 0xa48
			 0x55a8e9be, /// 0xa4c
			 0x9f12b42a, /// 0xa50
			 0x890274d3, /// 0xa54
			 0x144027d6, /// 0xa58
			 0xcff51775, /// 0xa5c
			 0x8b76e38e, /// 0xa60
			 0x78f0c7de, /// 0xa64
			 0xa29653a3, /// 0xa68
			 0x993b21af, /// 0xa6c
			 0xdd3053a1, /// 0xa70
			 0xeec9fa25, /// 0xa74
			 0xe118a8d3, /// 0xa78
			 0x05809379, /// 0xa7c
			 0x047ddaa6, /// 0xa80
			 0xd738d5c6, /// 0xa84
			 0xd3e4aee9, /// 0xa88
			 0x6bafa840, /// 0xa8c
			 0x42e96040, /// 0xa90
			 0x569791b9, /// 0xa94
			 0x011c2ee0, /// 0xa98
			 0xd01b734f, /// 0xa9c
			 0x05d3328b, /// 0xaa0
			 0x3697491a, /// 0xaa4
			 0x895fc566, /// 0xaa8
			 0xa20481a6, /// 0xaac
			 0x76dc2244, /// 0xab0
			 0x12684aeb, /// 0xab4
			 0xd9ce72da, /// 0xab8
			 0xb7848ee9, /// 0xabc
			 0xa61a8f76, /// 0xac0
			 0x46867f09, /// 0xac4
			 0x9677568b, /// 0xac8
			 0x6418c64c, /// 0xacc
			 0xa03b5f73, /// 0xad0
			 0x89221ab1, /// 0xad4
			 0x4a6f8fe4, /// 0xad8
			 0x998b0667, /// 0xadc
			 0xed2ea3d3, /// 0xae0
			 0x9da875ae, /// 0xae4
			 0x7981e7fb, /// 0xae8
			 0xa6b98afe, /// 0xaec
			 0x88b22480, /// 0xaf0
			 0x6f3a9dcc, /// 0xaf4
			 0x6d562a77, /// 0xaf8
			 0xbbc74f52, /// 0xafc
			 0x60e41dc1, /// 0xb00
			 0xf7923264, /// 0xb04
			 0xdb927c29, /// 0xb08
			 0x3be292f8, /// 0xb0c
			 0x9fb7a20f, /// 0xb10
			 0x8003c32a, /// 0xb14
			 0xb259b75d, /// 0xb18
			 0xaa396761, /// 0xb1c
			 0x72b631d0, /// 0xb20
			 0x7f7d07a4, /// 0xb24
			 0x0ae4bcef, /// 0xb28
			 0x5bd41f0d, /// 0xb2c
			 0x8e26d536, /// 0xb30
			 0x2ddc6b93, /// 0xb34
			 0x22f5b091, /// 0xb38
			 0x1819bf32, /// 0xb3c
			 0xb8c7f4a7, /// 0xb40
			 0x3d61cd24, /// 0xb44
			 0xfd972276, /// 0xb48
			 0xe2730a0b, /// 0xb4c
			 0x75677d39, /// 0xb50
			 0x6a992452, /// 0xb54
			 0x9831ef89, /// 0xb58
			 0xb3fd370b, /// 0xb5c
			 0x2951c77a, /// 0xb60
			 0xcdd65ab9, /// 0xb64
			 0xfcfb344a, /// 0xb68
			 0x177b348e, /// 0xb6c
			 0xedf5ada6, /// 0xb70
			 0x1514950d, /// 0xb74
			 0xcfe369f3, /// 0xb78
			 0x4107e37e, /// 0xb7c
			 0x0edf66e2, /// 0xb80
			 0x61148b69, /// 0xb84
			 0xfe33992b, /// 0xb88
			 0x8b6db051, /// 0xb8c
			 0x9ff04cba, /// 0xb90
			 0x3d060336, /// 0xb94
			 0x7c34a23a, /// 0xb98
			 0xf878314f, /// 0xb9c
			 0x007197a6, /// 0xba0
			 0xbc868e71, /// 0xba4
			 0x4ffe6737, /// 0xba8
			 0x13b0879d, /// 0xbac
			 0xb432c5d0, /// 0xbb0
			 0xd89fff17, /// 0xbb4
			 0x630ca8b4, /// 0xbb8
			 0xca33cf7f, /// 0xbbc
			 0xb877c091, /// 0xbc0
			 0xf470555e, /// 0xbc4
			 0xac85b8f0, /// 0xbc8
			 0xf1e32bca, /// 0xbcc
			 0xd0d73137, /// 0xbd0
			 0xee7b4d53, /// 0xbd4
			 0x50aa5212, /// 0xbd8
			 0xcaead43a, /// 0xbdc
			 0x0958c425, /// 0xbe0
			 0xec38d079, /// 0xbe4
			 0x62536f99, /// 0xbe8
			 0x87175e50, /// 0xbec
			 0x1122e0bf, /// 0xbf0
			 0x9ca66c59, /// 0xbf4
			 0x4c9ff0df, /// 0xbf8
			 0x34bfedfd, /// 0xbfc
			 0xb1936ab8, /// 0xc00
			 0x52cda5ab, /// 0xc04
			 0x707bfdf7, /// 0xc08
			 0x4f228059, /// 0xc0c
			 0x9982e7ae, /// 0xc10
			 0x891780f3, /// 0xc14
			 0x1b67311b, /// 0xc18
			 0xd4816e7a, /// 0xc1c
			 0xdc0e181d, /// 0xc20
			 0xfd47d034, /// 0xc24
			 0x58210936, /// 0xc28
			 0xdb952ad2, /// 0xc2c
			 0x9ec8a3e4, /// 0xc30
			 0x1fc98485, /// 0xc34
			 0xe7e3d8bd, /// 0xc38
			 0x7ce7179f, /// 0xc3c
			 0xc9c884db, /// 0xc40
			 0x7413fe91, /// 0xc44
			 0xd6b1501f, /// 0xc48
			 0x94ac859c, /// 0xc4c
			 0xa4d648ce, /// 0xc50
			 0xdc1e784b, /// 0xc54
			 0x68907422, /// 0xc58
			 0xf8252d59, /// 0xc5c
			 0xcefb1edc, /// 0xc60
			 0xa7814fa4, /// 0xc64
			 0x054f3a2f, /// 0xc68
			 0xfb8a0cb5, /// 0xc6c
			 0xda247b35, /// 0xc70
			 0x411131cc, /// 0xc74
			 0x0132cff0, /// 0xc78
			 0x26d6e9a6, /// 0xc7c
			 0x0ae69908, /// 0xc80
			 0xc137125e, /// 0xc84
			 0xf56b7cb7, /// 0xc88
			 0xc6812c62, /// 0xc8c
			 0x19c66066, /// 0xc90
			 0x3c11b344, /// 0xc94
			 0x8e701782, /// 0xc98
			 0xaa1940bb, /// 0xc9c
			 0x0c9b986b, /// 0xca0
			 0x1d516163, /// 0xca4
			 0xb69f8bdb, /// 0xca8
			 0xab49aea8, /// 0xcac
			 0x89c75bf7, /// 0xcb0
			 0x8f93d467, /// 0xcb4
			 0x9010bd7f, /// 0xcb8
			 0x242e8c12, /// 0xcbc
			 0x1aaa4a87, /// 0xcc0
			 0xa3cac88f, /// 0xcc4
			 0x7d4ec087, /// 0xcc8
			 0xea1262ff, /// 0xccc
			 0x1f4771ed, /// 0xcd0
			 0xe3dc02b0, /// 0xcd4
			 0x6a738491, /// 0xcd8
			 0xa9090a15, /// 0xcdc
			 0xeb3783da, /// 0xce0
			 0x6769c18d, /// 0xce4
			 0x0664ff7e, /// 0xce8
			 0x6549dedf, /// 0xcec
			 0x59165c5f, /// 0xcf0
			 0xf1afe4f8, /// 0xcf4
			 0xc2702a53, /// 0xcf8
			 0x2a8e10f8, /// 0xcfc
			 0x51205297, /// 0xd00
			 0x29513ab9, /// 0xd04
			 0x45d44119, /// 0xd08
			 0xeb6673c9, /// 0xd0c
			 0xb71b7b6b, /// 0xd10
			 0xafb405b3, /// 0xd14
			 0xfe7b45c4, /// 0xd18
			 0x9d904118, /// 0xd1c
			 0x841e9ef3, /// 0xd20
			 0x0dfc8b8c, /// 0xd24
			 0x0a635de0, /// 0xd28
			 0x03844c09, /// 0xd2c
			 0x6ebc2135, /// 0xd30
			 0x26229cca, /// 0xd34
			 0x89bebf86, /// 0xd38
			 0x6847c979, /// 0xd3c
			 0x2a403b4b, /// 0xd40
			 0x322aff0e, /// 0xd44
			 0x72ad84cc, /// 0xd48
			 0x74adda37, /// 0xd4c
			 0x854c0976, /// 0xd50
			 0x448051e7, /// 0xd54
			 0xf9d7ba6a, /// 0xd58
			 0xda4da944, /// 0xd5c
			 0xdcfed5c2, /// 0xd60
			 0x6e72cad3, /// 0xd64
			 0x81b3f435, /// 0xd68
			 0xbc99be6f, /// 0xd6c
			 0xfa320097, /// 0xd70
			 0x742903bc, /// 0xd74
			 0xe5df87fc, /// 0xd78
			 0x5b956cbd, /// 0xd7c
			 0x0c2e589b, /// 0xd80
			 0x8d628f66, /// 0xd84
			 0xe0b53192, /// 0xd88
			 0x0be827f5, /// 0xd8c
			 0x9059dd7e, /// 0xd90
			 0xfee973a1, /// 0xd94
			 0xf0431b41, /// 0xd98
			 0x0e875bde, /// 0xd9c
			 0x7616811f, /// 0xda0
			 0xf991a66e, /// 0xda4
			 0x4d3e85d1, /// 0xda8
			 0xd6bb54fc, /// 0xdac
			 0x57da50ff, /// 0xdb0
			 0x19131fc9, /// 0xdb4
			 0xa2dbadb6, /// 0xdb8
			 0xa69a756c, /// 0xdbc
			 0x3d97f263, /// 0xdc0
			 0x8e24d10a, /// 0xdc4
			 0xe37046a2, /// 0xdc8
			 0x11964988, /// 0xdcc
			 0x9360a415, /// 0xdd0
			 0x55dc094d, /// 0xdd4
			 0x8e606ab9, /// 0xdd8
			 0x5aaf9b16, /// 0xddc
			 0x577c61f4, /// 0xde0
			 0x4c84ab92, /// 0xde4
			 0x7b0869e2, /// 0xde8
			 0xadbf6f8a, /// 0xdec
			 0x3261ef09, /// 0xdf0
			 0x096f9933, /// 0xdf4
			 0xccfb616c, /// 0xdf8
			 0xf292dca1, /// 0xdfc
			 0x38f71937, /// 0xe00
			 0xa9dda5bc, /// 0xe04
			 0x5782fcd5, /// 0xe08
			 0xf0298c3d, /// 0xe0c
			 0x8a003507, /// 0xe10
			 0x29870b95, /// 0xe14
			 0xd9ba3104, /// 0xe18
			 0x9a48aafb, /// 0xe1c
			 0x428b7280, /// 0xe20
			 0xe3671c4e, /// 0xe24
			 0xe93187ee, /// 0xe28
			 0x4969fb72, /// 0xe2c
			 0xdcdd73ae, /// 0xe30
			 0x66178787, /// 0xe34
			 0xe227e239, /// 0xe38
			 0x92a81010, /// 0xe3c
			 0x3f782242, /// 0xe40
			 0x39b6496d, /// 0xe44
			 0xa3e3500a, /// 0xe48
			 0x56db0dc5, /// 0xe4c
			 0xfe9584d8, /// 0xe50
			 0x4ca93f39, /// 0xe54
			 0xca9c877f, /// 0xe58
			 0x49f435fe, /// 0xe5c
			 0x702429dd, /// 0xe60
			 0xb516cfc7, /// 0xe64
			 0xe4de2832, /// 0xe68
			 0x04e1601e, /// 0xe6c
			 0xacf254ce, /// 0xe70
			 0xedc4ae52, /// 0xe74
			 0xab206854, /// 0xe78
			 0x0fd86e90, /// 0xe7c
			 0x913aa9c4, /// 0xe80
			 0x7b139d37, /// 0xe84
			 0xfd7fa3b3, /// 0xe88
			 0xe421c290, /// 0xe8c
			 0xe000f0e8, /// 0xe90
			 0x25be941e, /// 0xe94
			 0x50007bc6, /// 0xe98
			 0x493eb418, /// 0xe9c
			 0x22b8af30, /// 0xea0
			 0x3fe52401, /// 0xea4
			 0xf2acb372, /// 0xea8
			 0xef95bd64, /// 0xeac
			 0xe1550b13, /// 0xeb0
			 0xa9e2ff5c, /// 0xeb4
			 0x844f6970, /// 0xeb8
			 0x8f017f6c, /// 0xebc
			 0xa2441830, /// 0xec0
			 0x87bab0b8, /// 0xec4
			 0xcd11d5a9, /// 0xec8
			 0x045ae632, /// 0xecc
			 0x2f8bb7bc, /// 0xed0
			 0x251e7078, /// 0xed4
			 0x31ac2cdd, /// 0xed8
			 0x534672ea, /// 0xedc
			 0xb394f6d2, /// 0xee0
			 0xd65860bf, /// 0xee4
			 0xfe9d5e6a, /// 0xee8
			 0xbc0e21ff, /// 0xeec
			 0xe84f5fa0, /// 0xef0
			 0xf0bea747, /// 0xef4
			 0xe09bc051, /// 0xef8
			 0x663f435f, /// 0xefc
			 0x61e6b7f9, /// 0xf00
			 0xeabcef1e, /// 0xf04
			 0x9f85c66e, /// 0xf08
			 0x73e8aa6c, /// 0xf0c
			 0x8777a319, /// 0xf10
			 0xf8ec8c02, /// 0xf14
			 0xea36df8a, /// 0xf18
			 0xf9d28de8, /// 0xf1c
			 0xfcbf59c4, /// 0xf20
			 0xfffcfc22, /// 0xf24
			 0xd4c1cbfb, /// 0xf28
			 0x4a9722aa, /// 0xf2c
			 0xa88ec570, /// 0xf30
			 0x1e5a739b, /// 0xf34
			 0x156b87c2, /// 0xf38
			 0x2870874b, /// 0xf3c
			 0x7265167b, /// 0xf40
			 0xc9a88ad3, /// 0xf44
			 0xc05e3eb2, /// 0xf48
			 0x788460fe, /// 0xf4c
			 0xc19bfee8, /// 0xf50
			 0xc2ccc22a, /// 0xf54
			 0x0e8ba61f, /// 0xf58
			 0x14658424, /// 0xf5c
			 0x7e486679, /// 0xf60
			 0x4d69d4d4, /// 0xf64
			 0xd85ff5ef, /// 0xf68
			 0x3dde3624, /// 0xf6c
			 0x6db33486, /// 0xf70
			 0x3060d77a, /// 0xf74
			 0x0ae9363c, /// 0xf78
			 0xb048025b, /// 0xf7c
			 0x87f23050, /// 0xf80
			 0xf4824b64, /// 0xf84
			 0x67dfb4ac, /// 0xf88
			 0xbb192fea, /// 0xf8c
			 0x2ea2719f, /// 0xf90
			 0xa64a92e7, /// 0xf94
			 0x0e74bb52, /// 0xf98
			 0x9af61f82, /// 0xf9c
			 0x21f35d41, /// 0xfa0
			 0x87cda110, /// 0xfa4
			 0xa3722d0e, /// 0xfa8
			 0x32761ea0, /// 0xfac
			 0x07ab3554, /// 0xfb0
			 0x06d87a03, /// 0xfb4
			 0x831e8b9e, /// 0xfb8
			 0x54d34c3c, /// 0xfbc
			 0x3b4e979b, /// 0xfc0
			 0x6ba3440d, /// 0xfc4
			 0x55d21d2a, /// 0xfc8
			 0xa090f7b0, /// 0xfcc
			 0x9190ec5d, /// 0xfd0
			 0x78366409, /// 0xfd4
			 0x161b576c, /// 0xfd8
			 0xef8bb5b5, /// 0xfdc
			 0x4e16f800, /// 0xfe0
			 0xf5bc8059, /// 0xfe4
			 0x6ad9ed5f, /// 0xfe8
			 0xc69555e9, /// 0xfec
			 0xc18d33a9, /// 0xff0
			 0x355814ad, /// 0xff4
			 0x00b5cf47, /// 0xff8
			 0x345b11b3 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0650d9ef, /// 0x0
			 0x131e54bd, /// 0x4
			 0x810f7e59, /// 0x8
			 0xe7fc12de, /// 0xc
			 0x60f07cc7, /// 0x10
			 0x17013e01, /// 0x14
			 0xa243bee2, /// 0x18
			 0x44018662, /// 0x1c
			 0x1661bc3c, /// 0x20
			 0x1cf4344b, /// 0x24
			 0x94a77172, /// 0x28
			 0x57dbdfef, /// 0x2c
			 0xda91116f, /// 0x30
			 0x7d85b478, /// 0x34
			 0xb64d8457, /// 0x38
			 0xce6ab3b3, /// 0x3c
			 0xef46b646, /// 0x40
			 0x13390e52, /// 0x44
			 0x4ee18215, /// 0x48
			 0x26dc2ca0, /// 0x4c
			 0xe71d5ae8, /// 0x50
			 0x339bcd0b, /// 0x54
			 0x48f629e6, /// 0x58
			 0x6567c68b, /// 0x5c
			 0xfc0fed5b, /// 0x60
			 0x58911339, /// 0x64
			 0x74985388, /// 0x68
			 0x2dae65c5, /// 0x6c
			 0x2be4f485, /// 0x70
			 0x242902b3, /// 0x74
			 0x05f80558, /// 0x78
			 0x9ec4b906, /// 0x7c
			 0xb73b671f, /// 0x80
			 0x627ddfc8, /// 0x84
			 0xb263834b, /// 0x88
			 0x8a892dfe, /// 0x8c
			 0xf8f6f2b4, /// 0x90
			 0x4cf851fb, /// 0x94
			 0x2f458476, /// 0x98
			 0x0104e8a5, /// 0x9c
			 0xfd97312a, /// 0xa0
			 0xf23061cc, /// 0xa4
			 0x681d9997, /// 0xa8
			 0x3a3acd3d, /// 0xac
			 0x80a7dbf7, /// 0xb0
			 0x7e26860c, /// 0xb4
			 0xc6d7cb91, /// 0xb8
			 0xcf37515d, /// 0xbc
			 0xce567c35, /// 0xc0
			 0x818bcf59, /// 0xc4
			 0x70eb8056, /// 0xc8
			 0x26f25541, /// 0xcc
			 0xf00efbea, /// 0xd0
			 0x2df5c824, /// 0xd4
			 0xe519aef1, /// 0xd8
			 0x2279e2d7, /// 0xdc
			 0xde27d1a0, /// 0xe0
			 0xd3e964cc, /// 0xe4
			 0x89e37d96, /// 0xe8
			 0xccc8001a, /// 0xec
			 0xa1850f93, /// 0xf0
			 0x8f4d6792, /// 0xf4
			 0x4f077dd7, /// 0xf8
			 0x1dc02348, /// 0xfc
			 0xcd9577b0, /// 0x100
			 0xa0561fd2, /// 0x104
			 0xbb7afc2f, /// 0x108
			 0x1142ce0c, /// 0x10c
			 0xf9b69946, /// 0x110
			 0xfbdc3d97, /// 0x114
			 0x8a0b43e7, /// 0x118
			 0x0a083bd1, /// 0x11c
			 0x07d50da2, /// 0x120
			 0x62bb0880, /// 0x124
			 0x15df1098, /// 0x128
			 0xba356d60, /// 0x12c
			 0x6275dbcd, /// 0x130
			 0xf0c65fd2, /// 0x134
			 0x30a64e27, /// 0x138
			 0x49c363cb, /// 0x13c
			 0xf2f430c6, /// 0x140
			 0xafaab44f, /// 0x144
			 0x740a81da, /// 0x148
			 0x111083fb, /// 0x14c
			 0xe870d04a, /// 0x150
			 0xa77abc9a, /// 0x154
			 0xf838abcd, /// 0x158
			 0xa170ac65, /// 0x15c
			 0x884810d6, /// 0x160
			 0x9e453966, /// 0x164
			 0xa465aecc, /// 0x168
			 0x6fb65b32, /// 0x16c
			 0xbb2a1170, /// 0x170
			 0x48d2b705, /// 0x174
			 0xcb753ff7, /// 0x178
			 0x64d442b3, /// 0x17c
			 0x5a034096, /// 0x180
			 0x87d23797, /// 0x184
			 0x9dfeb004, /// 0x188
			 0xcf3a6d78, /// 0x18c
			 0x677e7bcf, /// 0x190
			 0x3f3a4ac6, /// 0x194
			 0xa31a7fec, /// 0x198
			 0xe86048e4, /// 0x19c
			 0x7cb676ab, /// 0x1a0
			 0x30a2dc42, /// 0x1a4
			 0x22a54ac6, /// 0x1a8
			 0x9638dcc3, /// 0x1ac
			 0x6539cdbd, /// 0x1b0
			 0xa7d7a33d, /// 0x1b4
			 0x1b0e5cf8, /// 0x1b8
			 0xb30f053b, /// 0x1bc
			 0x45f1d63d, /// 0x1c0
			 0x5fdee76d, /// 0x1c4
			 0x93ca3e83, /// 0x1c8
			 0x51a19ed4, /// 0x1cc
			 0xeaa22a0a, /// 0x1d0
			 0x1ea516a1, /// 0x1d4
			 0x60d31ccc, /// 0x1d8
			 0x55aa32e1, /// 0x1dc
			 0xc22f2b5a, /// 0x1e0
			 0x6bd0bbc0, /// 0x1e4
			 0x21467215, /// 0x1e8
			 0x188cc203, /// 0x1ec
			 0xe3fb4f84, /// 0x1f0
			 0xe902e402, /// 0x1f4
			 0x53333a5d, /// 0x1f8
			 0xf22684e8, /// 0x1fc
			 0x85fb5c6d, /// 0x200
			 0x20d53a0f, /// 0x204
			 0xbcc400de, /// 0x208
			 0x07f91ddc, /// 0x20c
			 0x81710d04, /// 0x210
			 0x4d38287e, /// 0x214
			 0xa29972ee, /// 0x218
			 0x5d5d99f7, /// 0x21c
			 0x63a66f91, /// 0x220
			 0xdfad4b10, /// 0x224
			 0x5c70b978, /// 0x228
			 0x0e265da1, /// 0x22c
			 0xc758ec33, /// 0x230
			 0xffdf787c, /// 0x234
			 0xb53403e6, /// 0x238
			 0x7485d8d3, /// 0x23c
			 0x6b7dffa1, /// 0x240
			 0xc14b48f6, /// 0x244
			 0xc37e8dc3, /// 0x248
			 0x8669cdca, /// 0x24c
			 0x5d373087, /// 0x250
			 0x173cae16, /// 0x254
			 0xa7241e05, /// 0x258
			 0x513c7f8a, /// 0x25c
			 0x83e6859e, /// 0x260
			 0xe74fffb1, /// 0x264
			 0xfbc1f633, /// 0x268
			 0x9fabdc11, /// 0x26c
			 0xf9ca79f8, /// 0x270
			 0x9e8aae71, /// 0x274
			 0xe80c505a, /// 0x278
			 0x3839c715, /// 0x27c
			 0xe9997624, /// 0x280
			 0x28e0b19f, /// 0x284
			 0x5c1e4f14, /// 0x288
			 0x3c2c1283, /// 0x28c
			 0x7515d2cd, /// 0x290
			 0x03dceead, /// 0x294
			 0xbb2d0a1a, /// 0x298
			 0x2fcfb01f, /// 0x29c
			 0xbe0c66f2, /// 0x2a0
			 0x4f3cb1ca, /// 0x2a4
			 0x32d1289b, /// 0x2a8
			 0xb0b9332b, /// 0x2ac
			 0x278ad0c2, /// 0x2b0
			 0x757439d8, /// 0x2b4
			 0xe2156c81, /// 0x2b8
			 0x86916de1, /// 0x2bc
			 0xc2e64401, /// 0x2c0
			 0xd9f6c151, /// 0x2c4
			 0x3cac455c, /// 0x2c8
			 0x3bf0aaa8, /// 0x2cc
			 0x09156848, /// 0x2d0
			 0x5de78a1b, /// 0x2d4
			 0x76ab103a, /// 0x2d8
			 0xfb9fe25d, /// 0x2dc
			 0x25bcac53, /// 0x2e0
			 0x1c38bd4e, /// 0x2e4
			 0xf04f48d9, /// 0x2e8
			 0xe58f3d1c, /// 0x2ec
			 0x78457893, /// 0x2f0
			 0x7b75136f, /// 0x2f4
			 0xfef61165, /// 0x2f8
			 0xd00c17f4, /// 0x2fc
			 0xc1d9e8ca, /// 0x300
			 0x5418ca45, /// 0x304
			 0xa0c93351, /// 0x308
			 0xc950dd76, /// 0x30c
			 0xecd53932, /// 0x310
			 0xda7f44bb, /// 0x314
			 0xc5982317, /// 0x318
			 0xe41ecc93, /// 0x31c
			 0x4d77391b, /// 0x320
			 0x10a768fd, /// 0x324
			 0x01275e3a, /// 0x328
			 0xbde7beb2, /// 0x32c
			 0x89ee0445, /// 0x330
			 0xd3ec2af9, /// 0x334
			 0x0ec4c4da, /// 0x338
			 0x4cc1c1d8, /// 0x33c
			 0x0e4ef542, /// 0x340
			 0xcc37c0f7, /// 0x344
			 0x12657d03, /// 0x348
			 0xff316d65, /// 0x34c
			 0x565668ac, /// 0x350
			 0xc04c6ecd, /// 0x354
			 0x5b6b99ea, /// 0x358
			 0x03c718ec, /// 0x35c
			 0xb33032d2, /// 0x360
			 0xb9e14184, /// 0x364
			 0xfb427ae7, /// 0x368
			 0x7d243e6d, /// 0x36c
			 0xb1e24d20, /// 0x370
			 0x25813efc, /// 0x374
			 0x47a46b0e, /// 0x378
			 0x94b94f2d, /// 0x37c
			 0x21138d6a, /// 0x380
			 0xb4448944, /// 0x384
			 0xbf289df4, /// 0x388
			 0x9ccd2438, /// 0x38c
			 0x96579787, /// 0x390
			 0xb01e7d1d, /// 0x394
			 0x117e690d, /// 0x398
			 0x7960e9e9, /// 0x39c
			 0x38b556f0, /// 0x3a0
			 0xc887004e, /// 0x3a4
			 0xb4d2acbc, /// 0x3a8
			 0xcb2bf6c4, /// 0x3ac
			 0xb4c35a7a, /// 0x3b0
			 0x665c7ed5, /// 0x3b4
			 0xc7c326ce, /// 0x3b8
			 0xbb98f04d, /// 0x3bc
			 0x2c625dfb, /// 0x3c0
			 0xff24bb6b, /// 0x3c4
			 0x72c7dc8f, /// 0x3c8
			 0xc9262d81, /// 0x3cc
			 0xd32015da, /// 0x3d0
			 0xc75dae13, /// 0x3d4
			 0x7a060c7f, /// 0x3d8
			 0x960f0b04, /// 0x3dc
			 0x9dd0a915, /// 0x3e0
			 0xd98056aa, /// 0x3e4
			 0xc87b9025, /// 0x3e8
			 0x6eabbc6a, /// 0x3ec
			 0xd6114dce, /// 0x3f0
			 0x885c418a, /// 0x3f4
			 0xf3889d8a, /// 0x3f8
			 0x7597c94c, /// 0x3fc
			 0x08a3f452, /// 0x400
			 0xc7d880bc, /// 0x404
			 0x66b2ac09, /// 0x408
			 0xa4237053, /// 0x40c
			 0xea214fc8, /// 0x410
			 0x187e3db2, /// 0x414
			 0xf4d52615, /// 0x418
			 0xfa9535e2, /// 0x41c
			 0x52d6b176, /// 0x420
			 0x375715b5, /// 0x424
			 0x01b6ec91, /// 0x428
			 0xdd3b00a5, /// 0x42c
			 0x6fce2493, /// 0x430
			 0xb2d86e0f, /// 0x434
			 0x4ba6b1eb, /// 0x438
			 0xa141523d, /// 0x43c
			 0x7f03a5eb, /// 0x440
			 0xb26b14fe, /// 0x444
			 0xc487a78a, /// 0x448
			 0x8eda0cb0, /// 0x44c
			 0x775ab044, /// 0x450
			 0x010404c2, /// 0x454
			 0x9915887d, /// 0x458
			 0x92f4476e, /// 0x45c
			 0x8fd1e739, /// 0x460
			 0x677cebb7, /// 0x464
			 0xf4d80994, /// 0x468
			 0x9c8ae7f8, /// 0x46c
			 0x0beb80c1, /// 0x470
			 0x73d21182, /// 0x474
			 0x7fee5ce5, /// 0x478
			 0xbfd69f75, /// 0x47c
			 0x0ae0bfab, /// 0x480
			 0x8f39056b, /// 0x484
			 0xa4af1caf, /// 0x488
			 0xc76eb061, /// 0x48c
			 0x8c89c3b2, /// 0x490
			 0xe415590a, /// 0x494
			 0xae16361e, /// 0x498
			 0xbd05ce11, /// 0x49c
			 0xcd941be4, /// 0x4a0
			 0x18df1a6d, /// 0x4a4
			 0x90f8052d, /// 0x4a8
			 0xcfa007e9, /// 0x4ac
			 0x67453d62, /// 0x4b0
			 0xc363d90f, /// 0x4b4
			 0x87739150, /// 0x4b8
			 0xd85c3838, /// 0x4bc
			 0x0a74cd17, /// 0x4c0
			 0x3c6cc9e9, /// 0x4c4
			 0x82847070, /// 0x4c8
			 0x2b3e126c, /// 0x4cc
			 0x48b1c345, /// 0x4d0
			 0x700a481a, /// 0x4d4
			 0xa027d915, /// 0x4d8
			 0x0c733988, /// 0x4dc
			 0x28292f6d, /// 0x4e0
			 0xbcfaa9b6, /// 0x4e4
			 0xeccbbb20, /// 0x4e8
			 0xd1c4a05e, /// 0x4ec
			 0x0a3ac836, /// 0x4f0
			 0x72957401, /// 0x4f4
			 0x6c6400b5, /// 0x4f8
			 0x1723411e, /// 0x4fc
			 0x97d13acc, /// 0x500
			 0x5122457a, /// 0x504
			 0x4efe4e59, /// 0x508
			 0x24c56ae2, /// 0x50c
			 0x6d55edad, /// 0x510
			 0xa9b3cd66, /// 0x514
			 0xb147b954, /// 0x518
			 0x89a499bc, /// 0x51c
			 0xd82576e7, /// 0x520
			 0xb1431104, /// 0x524
			 0x29c9179a, /// 0x528
			 0xfbc936bf, /// 0x52c
			 0x0f0d0a1a, /// 0x530
			 0xc09c08f6, /// 0x534
			 0xe03a126b, /// 0x538
			 0x00a75614, /// 0x53c
			 0xeaffb1c1, /// 0x540
			 0xf0b98d7d, /// 0x544
			 0x9ade9e9b, /// 0x548
			 0x5942feb5, /// 0x54c
			 0x0a694259, /// 0x550
			 0x1ca42bc0, /// 0x554
			 0x99253930, /// 0x558
			 0x8be4d354, /// 0x55c
			 0xf8775a76, /// 0x560
			 0xedd53298, /// 0x564
			 0x0abb02d1, /// 0x568
			 0x6eb72a2b, /// 0x56c
			 0xfe3b7f46, /// 0x570
			 0xe2efbb70, /// 0x574
			 0x4348ca7d, /// 0x578
			 0x1b3ee537, /// 0x57c
			 0xe8be8c10, /// 0x580
			 0xc1b9d373, /// 0x584
			 0xfdef4233, /// 0x588
			 0xbe0384a9, /// 0x58c
			 0x7b81425d, /// 0x590
			 0x4c015ce1, /// 0x594
			 0x84ede024, /// 0x598
			 0x552cc459, /// 0x59c
			 0xf30895c0, /// 0x5a0
			 0x8736fc21, /// 0x5a4
			 0x20e06287, /// 0x5a8
			 0xa5f47db2, /// 0x5ac
			 0xec026569, /// 0x5b0
			 0x773d5646, /// 0x5b4
			 0xc913cb7e, /// 0x5b8
			 0xa6a92b3e, /// 0x5bc
			 0x77cbbb0c, /// 0x5c0
			 0xa757fa0a, /// 0x5c4
			 0xc7b63fb7, /// 0x5c8
			 0xf4ec1b06, /// 0x5cc
			 0x737ffd99, /// 0x5d0
			 0x37d9af6b, /// 0x5d4
			 0x3c2e2975, /// 0x5d8
			 0x806d408f, /// 0x5dc
			 0xc4fb4ff4, /// 0x5e0
			 0x60f0da23, /// 0x5e4
			 0x20d4fbb9, /// 0x5e8
			 0x298390d6, /// 0x5ec
			 0x325241b4, /// 0x5f0
			 0xfec7bc33, /// 0x5f4
			 0x99d076c1, /// 0x5f8
			 0xdecb35d6, /// 0x5fc
			 0xcaac7d01, /// 0x600
			 0xab4f3ac7, /// 0x604
			 0x8f99ad05, /// 0x608
			 0x89425a4f, /// 0x60c
			 0x28abcca2, /// 0x610
			 0x450c599f, /// 0x614
			 0x1e7aeafe, /// 0x618
			 0xcde67ac5, /// 0x61c
			 0x29ec265c, /// 0x620
			 0xdcda8219, /// 0x624
			 0x1c3ac730, /// 0x628
			 0x3ab57278, /// 0x62c
			 0x21c7e3ae, /// 0x630
			 0xe655bae6, /// 0x634
			 0xae88a70d, /// 0x638
			 0x2dc4da01, /// 0x63c
			 0xbe2d5bc6, /// 0x640
			 0xe33baa4d, /// 0x644
			 0xefd7fd6f, /// 0x648
			 0x1b4db84c, /// 0x64c
			 0xe3dd0f9c, /// 0x650
			 0xaaaec2c3, /// 0x654
			 0x06bd8fed, /// 0x658
			 0x483dabdd, /// 0x65c
			 0xc57181b8, /// 0x660
			 0x964a0506, /// 0x664
			 0x741586db, /// 0x668
			 0x38389ef6, /// 0x66c
			 0x69215b57, /// 0x670
			 0xacd600ea, /// 0x674
			 0x46846173, /// 0x678
			 0x816ab8aa, /// 0x67c
			 0x6affa03d, /// 0x680
			 0x1452acd8, /// 0x684
			 0x19539b62, /// 0x688
			 0x5c32f2bf, /// 0x68c
			 0x0e214e01, /// 0x690
			 0x25b1b569, /// 0x694
			 0xc8a63687, /// 0x698
			 0xd78ed9d4, /// 0x69c
			 0xe197ab06, /// 0x6a0
			 0xb8516237, /// 0x6a4
			 0x835f8ea3, /// 0x6a8
			 0x5eacef39, /// 0x6ac
			 0x2f83420f, /// 0x6b0
			 0x1584ab52, /// 0x6b4
			 0x4cdadc34, /// 0x6b8
			 0x74a3889e, /// 0x6bc
			 0xe9a8c090, /// 0x6c0
			 0x710350b5, /// 0x6c4
			 0x9b524302, /// 0x6c8
			 0x7bbbf083, /// 0x6cc
			 0xb7222dfe, /// 0x6d0
			 0x058a80bc, /// 0x6d4
			 0xf28af328, /// 0x6d8
			 0xcf0098dc, /// 0x6dc
			 0x8f245be7, /// 0x6e0
			 0xcdc6d96e, /// 0x6e4
			 0xe30a41cb, /// 0x6e8
			 0x730f3c0c, /// 0x6ec
			 0x528be2f2, /// 0x6f0
			 0x6f52a623, /// 0x6f4
			 0x2b902c8d, /// 0x6f8
			 0x66634328, /// 0x6fc
			 0x0f3ebd94, /// 0x700
			 0xf294fcca, /// 0x704
			 0xab1dc251, /// 0x708
			 0x947f3929, /// 0x70c
			 0x2118eb2e, /// 0x710
			 0x8eea7bd9, /// 0x714
			 0x5058a6f8, /// 0x718
			 0x3202148a, /// 0x71c
			 0x007920c8, /// 0x720
			 0x6c78b9b0, /// 0x724
			 0xd2595ef6, /// 0x728
			 0x9e7b8a95, /// 0x72c
			 0xa192fa67, /// 0x730
			 0x6bcab5a8, /// 0x734
			 0xb06425d4, /// 0x738
			 0xd02dadb7, /// 0x73c
			 0x5381ead7, /// 0x740
			 0x4b63f622, /// 0x744
			 0x2904ee54, /// 0x748
			 0xe015073c, /// 0x74c
			 0x184867e3, /// 0x750
			 0x851521f5, /// 0x754
			 0xd4075d3e, /// 0x758
			 0xe0778d03, /// 0x75c
			 0x8d697a79, /// 0x760
			 0x784ba26d, /// 0x764
			 0xa487ff26, /// 0x768
			 0x53dfb31c, /// 0x76c
			 0xa5916d34, /// 0x770
			 0x6b05728a, /// 0x774
			 0x75abc4ec, /// 0x778
			 0x5c4420a7, /// 0x77c
			 0x2afa2c26, /// 0x780
			 0x4ce9e948, /// 0x784
			 0xf9a4483f, /// 0x788
			 0x1fe39c63, /// 0x78c
			 0xa31d556f, /// 0x790
			 0x8120b4bc, /// 0x794
			 0x389439df, /// 0x798
			 0x4485115f, /// 0x79c
			 0xebab02ad, /// 0x7a0
			 0xd92ff71b, /// 0x7a4
			 0xfdfd548f, /// 0x7a8
			 0xd63a8ad5, /// 0x7ac
			 0xd3b52dd4, /// 0x7b0
			 0x3ca0a8d6, /// 0x7b4
			 0xa837408f, /// 0x7b8
			 0x5e57172d, /// 0x7bc
			 0xf203a279, /// 0x7c0
			 0x50493d6d, /// 0x7c4
			 0x487c6d9a, /// 0x7c8
			 0xa5e06263, /// 0x7cc
			 0xc92d6719, /// 0x7d0
			 0xe2398ac4, /// 0x7d4
			 0x83e9635e, /// 0x7d8
			 0xc17e538b, /// 0x7dc
			 0xc8db71d6, /// 0x7e0
			 0x6efe4f59, /// 0x7e4
			 0xc6bd4558, /// 0x7e8
			 0x3e974391, /// 0x7ec
			 0x6e90b1c3, /// 0x7f0
			 0xeadc8d6f, /// 0x7f4
			 0xdf6549f5, /// 0x7f8
			 0x81b5deb9, /// 0x7fc
			 0x78dbbe44, /// 0x800
			 0xb62eef84, /// 0x804
			 0x8446e423, /// 0x808
			 0x0d4ec131, /// 0x80c
			 0x83f48eb8, /// 0x810
			 0xd42bf428, /// 0x814
			 0x74f3d974, /// 0x818
			 0x64f6e3a7, /// 0x81c
			 0x25c54ecf, /// 0x820
			 0xd3736f7a, /// 0x824
			 0x21ea6d2f, /// 0x828
			 0x6ee01c16, /// 0x82c
			 0xf21e7712, /// 0x830
			 0x17572b7c, /// 0x834
			 0x5ec8e698, /// 0x838
			 0x32cbc273, /// 0x83c
			 0x93301afb, /// 0x840
			 0x4337fe58, /// 0x844
			 0x00fe71be, /// 0x848
			 0xe51bf514, /// 0x84c
			 0xc27328af, /// 0x850
			 0xdcac1ea5, /// 0x854
			 0xaa019cf8, /// 0x858
			 0x0650d6c8, /// 0x85c
			 0x281515f5, /// 0x860
			 0xf2c16fa9, /// 0x864
			 0xe4525929, /// 0x868
			 0x9c7a7127, /// 0x86c
			 0x93fd1fe5, /// 0x870
			 0xa5c96045, /// 0x874
			 0x7ed3d0a2, /// 0x878
			 0x7d9a3117, /// 0x87c
			 0x30d78364, /// 0x880
			 0xa5f39bd3, /// 0x884
			 0x32160a48, /// 0x888
			 0x51848cf2, /// 0x88c
			 0x88fa908c, /// 0x890
			 0x5155fd54, /// 0x894
			 0xfff5ae1e, /// 0x898
			 0x6415c4c6, /// 0x89c
			 0xc65abd54, /// 0x8a0
			 0xfa9f9122, /// 0x8a4
			 0x1b19df59, /// 0x8a8
			 0x42131ed6, /// 0x8ac
			 0x63022304, /// 0x8b0
			 0xd99a9f1d, /// 0x8b4
			 0x9f7185cf, /// 0x8b8
			 0x12a8a80b, /// 0x8bc
			 0xbc0de0dc, /// 0x8c0
			 0xeed2582c, /// 0x8c4
			 0xb82d19fc, /// 0x8c8
			 0xfb1040a6, /// 0x8cc
			 0x87867cbc, /// 0x8d0
			 0x66820999, /// 0x8d4
			 0xf062a8cf, /// 0x8d8
			 0x8f913467, /// 0x8dc
			 0x687aa1ef, /// 0x8e0
			 0x063fe923, /// 0x8e4
			 0xc584567d, /// 0x8e8
			 0xd2934db6, /// 0x8ec
			 0xae37a205, /// 0x8f0
			 0x3e7fbb8b, /// 0x8f4
			 0x43a530b5, /// 0x8f8
			 0x69509768, /// 0x8fc
			 0x4431dc3f, /// 0x900
			 0x5cd3c06c, /// 0x904
			 0x466d1d62, /// 0x908
			 0xffda7721, /// 0x90c
			 0x4a8c117b, /// 0x910
			 0x0b1bb98b, /// 0x914
			 0xd234de3c, /// 0x918
			 0x2690ce37, /// 0x91c
			 0xb5fcce97, /// 0x920
			 0x096cd291, /// 0x924
			 0x660a0b0a, /// 0x928
			 0x5eea1b00, /// 0x92c
			 0x347d64d6, /// 0x930
			 0xe93d2dcb, /// 0x934
			 0x618f0252, /// 0x938
			 0xaa0e1547, /// 0x93c
			 0xf6bdd88b, /// 0x940
			 0xf4be24e4, /// 0x944
			 0x4a3f219a, /// 0x948
			 0xd8f57dc5, /// 0x94c
			 0xe400e4e0, /// 0x950
			 0x4914672d, /// 0x954
			 0x0e2a0f2f, /// 0x958
			 0xc2712507, /// 0x95c
			 0xce515bda, /// 0x960
			 0x893020b4, /// 0x964
			 0xb5aa9f87, /// 0x968
			 0xb7266996, /// 0x96c
			 0xf17db01f, /// 0x970
			 0xc5597bcb, /// 0x974
			 0xda1ab4ee, /// 0x978
			 0x7221a46d, /// 0x97c
			 0x84b95e82, /// 0x980
			 0xc7f7927d, /// 0x984
			 0x93b86ef7, /// 0x988
			 0xfb4dc435, /// 0x98c
			 0x45deadd8, /// 0x990
			 0x069b30dd, /// 0x994
			 0xafb1d5da, /// 0x998
			 0x9f89f262, /// 0x99c
			 0xc3b91d8e, /// 0x9a0
			 0x64451934, /// 0x9a4
			 0x129518b5, /// 0x9a8
			 0xde692b8d, /// 0x9ac
			 0x901d6191, /// 0x9b0
			 0x05a60606, /// 0x9b4
			 0x1d2493f1, /// 0x9b8
			 0xd843f077, /// 0x9bc
			 0xd47831c0, /// 0x9c0
			 0x3bb70c84, /// 0x9c4
			 0x9eaf239f, /// 0x9c8
			 0xf370c156, /// 0x9cc
			 0xd12cdab8, /// 0x9d0
			 0x5c3112dc, /// 0x9d4
			 0x7bd4fd7c, /// 0x9d8
			 0x95ec3dc4, /// 0x9dc
			 0x9ac824e2, /// 0x9e0
			 0x9a7c5495, /// 0x9e4
			 0x061ed250, /// 0x9e8
			 0xbc90cad0, /// 0x9ec
			 0xe270aa59, /// 0x9f0
			 0x0404d8a0, /// 0x9f4
			 0x67be1d15, /// 0x9f8
			 0xd56deadb, /// 0x9fc
			 0x0a6c1975, /// 0xa00
			 0x6d9a05b2, /// 0xa04
			 0xd0634eca, /// 0xa08
			 0xebebdff8, /// 0xa0c
			 0xa4ce5790, /// 0xa10
			 0xd822d8e2, /// 0xa14
			 0x28f2f939, /// 0xa18
			 0x9f1d3754, /// 0xa1c
			 0x548af322, /// 0xa20
			 0xe2897d91, /// 0xa24
			 0x28be77fc, /// 0xa28
			 0x5788947b, /// 0xa2c
			 0x77bc9434, /// 0xa30
			 0x0f44ea44, /// 0xa34
			 0x07718521, /// 0xa38
			 0x950b5f00, /// 0xa3c
			 0x2e50f4a5, /// 0xa40
			 0x590c9ecc, /// 0xa44
			 0x947a9fc0, /// 0xa48
			 0x137ee902, /// 0xa4c
			 0xd5e876aa, /// 0xa50
			 0x191293b8, /// 0xa54
			 0xfb1d9873, /// 0xa58
			 0x69d7bc23, /// 0xa5c
			 0x74152856, /// 0xa60
			 0xea8df0f1, /// 0xa64
			 0x84428597, /// 0xa68
			 0x82be5e2f, /// 0xa6c
			 0x112493de, /// 0xa70
			 0x77e83f4a, /// 0xa74
			 0xe1cecb34, /// 0xa78
			 0x442cdf12, /// 0xa7c
			 0xb363ccff, /// 0xa80
			 0x2f6d2d2a, /// 0xa84
			 0x2330d9ee, /// 0xa88
			 0x901793b9, /// 0xa8c
			 0x25290ba1, /// 0xa90
			 0x7c3a368e, /// 0xa94
			 0x8177fc0b, /// 0xa98
			 0x2804ad90, /// 0xa9c
			 0x5ce86755, /// 0xaa0
			 0x678731f7, /// 0xaa4
			 0xa855df65, /// 0xaa8
			 0xd3c643b7, /// 0xaac
			 0x75364625, /// 0xab0
			 0xdded3c95, /// 0xab4
			 0x5f5cd8a6, /// 0xab8
			 0x16e0526f, /// 0xabc
			 0x6b1af9ff, /// 0xac0
			 0x5b9775b6, /// 0xac4
			 0x7cfef47a, /// 0xac8
			 0x0ef07492, /// 0xacc
			 0x1862773b, /// 0xad0
			 0x27f6baaf, /// 0xad4
			 0x51d6816b, /// 0xad8
			 0x2cb988c2, /// 0xadc
			 0xbbe499c2, /// 0xae0
			 0xc4c36f7b, /// 0xae4
			 0x4e49138a, /// 0xae8
			 0x28c82718, /// 0xaec
			 0x32d4756a, /// 0xaf0
			 0x9839ebde, /// 0xaf4
			 0x8e222011, /// 0xaf8
			 0x572949f7, /// 0xafc
			 0x24744c44, /// 0xb00
			 0x33353741, /// 0xb04
			 0x184a0bb4, /// 0xb08
			 0x1bcad720, /// 0xb0c
			 0xfa6fa689, /// 0xb10
			 0x248ddaaa, /// 0xb14
			 0x95f520d0, /// 0xb18
			 0x7e51d5bf, /// 0xb1c
			 0x9b2f3c4e, /// 0xb20
			 0x2880df05, /// 0xb24
			 0xe79d0f08, /// 0xb28
			 0x7822999a, /// 0xb2c
			 0x5aece8ae, /// 0xb30
			 0x89f1be79, /// 0xb34
			 0x131e338a, /// 0xb38
			 0x3b141b12, /// 0xb3c
			 0x31167f4e, /// 0xb40
			 0x3bcc3791, /// 0xb44
			 0xfc64e536, /// 0xb48
			 0x6875e4e7, /// 0xb4c
			 0xce5504e0, /// 0xb50
			 0x0aa4ca8f, /// 0xb54
			 0xb519075e, /// 0xb58
			 0x9e093f52, /// 0xb5c
			 0x55b3515d, /// 0xb60
			 0xf5be8f38, /// 0xb64
			 0xb66dc685, /// 0xb68
			 0xd99d7aa7, /// 0xb6c
			 0xec1069ac, /// 0xb70
			 0x0ff4b6e3, /// 0xb74
			 0x32992b7f, /// 0xb78
			 0x139e2788, /// 0xb7c
			 0xb6b21417, /// 0xb80
			 0x321c7ada, /// 0xb84
			 0x7659396a, /// 0xb88
			 0xa48c146e, /// 0xb8c
			 0x3dcccca6, /// 0xb90
			 0xddd846e5, /// 0xb94
			 0x44cc6ead, /// 0xb98
			 0x03ae01a3, /// 0xb9c
			 0xce4212a8, /// 0xba0
			 0x08c715b2, /// 0xba4
			 0x27603abc, /// 0xba8
			 0x676b9773, /// 0xbac
			 0x5556738d, /// 0xbb0
			 0x228e33c5, /// 0xbb4
			 0x9366a46e, /// 0xbb8
			 0xc4424cfe, /// 0xbbc
			 0x561eaa3c, /// 0xbc0
			 0xba218753, /// 0xbc4
			 0xb29e57cd, /// 0xbc8
			 0xcf197a82, /// 0xbcc
			 0xf4e290ba, /// 0xbd0
			 0x145c58c4, /// 0xbd4
			 0x4d718f52, /// 0xbd8
			 0x27a15852, /// 0xbdc
			 0x4588c731, /// 0xbe0
			 0x27325d35, /// 0xbe4
			 0xc832dcdd, /// 0xbe8
			 0xcb314759, /// 0xbec
			 0x5ef03ace, /// 0xbf0
			 0xc8afc18a, /// 0xbf4
			 0x337f6141, /// 0xbf8
			 0x5cc59501, /// 0xbfc
			 0xd174d683, /// 0xc00
			 0xb2452029, /// 0xc04
			 0xb0b38234, /// 0xc08
			 0x32e26818, /// 0xc0c
			 0x7eb1b5f4, /// 0xc10
			 0xc21181b6, /// 0xc14
			 0x90e5a716, /// 0xc18
			 0xf063efff, /// 0xc1c
			 0xef4853d9, /// 0xc20
			 0x30c1566d, /// 0xc24
			 0x746328fa, /// 0xc28
			 0x361bece4, /// 0xc2c
			 0xe8d2c997, /// 0xc30
			 0x4bc8dfb1, /// 0xc34
			 0x810ec52c, /// 0xc38
			 0xfba95e31, /// 0xc3c
			 0x3988ae74, /// 0xc40
			 0xec098a11, /// 0xc44
			 0x68d16ee8, /// 0xc48
			 0xb907d743, /// 0xc4c
			 0x0c2394a1, /// 0xc50
			 0xa8d3459e, /// 0xc54
			 0x8acfefa4, /// 0xc58
			 0x3a7a8228, /// 0xc5c
			 0xf144834b, /// 0xc60
			 0x1ae43f5d, /// 0xc64
			 0x22d66e92, /// 0xc68
			 0xf30de395, /// 0xc6c
			 0x51958c3f, /// 0xc70
			 0x7a0f6882, /// 0xc74
			 0x43de8900, /// 0xc78
			 0xfa4ca34a, /// 0xc7c
			 0x0f3500b2, /// 0xc80
			 0x6302f580, /// 0xc84
			 0xe9887220, /// 0xc88
			 0xa542f562, /// 0xc8c
			 0xf419519f, /// 0xc90
			 0xb79fe1ba, /// 0xc94
			 0x0ec1c5f6, /// 0xc98
			 0x5ba4306d, /// 0xc9c
			 0xeb77ae88, /// 0xca0
			 0xeb32f18c, /// 0xca4
			 0x1e8181aa, /// 0xca8
			 0xedf130de, /// 0xcac
			 0x6e095842, /// 0xcb0
			 0x6072eef0, /// 0xcb4
			 0x847c9370, /// 0xcb8
			 0x29b5ac23, /// 0xcbc
			 0x2f0f9f6b, /// 0xcc0
			 0xd5796d0c, /// 0xcc4
			 0xdf313d77, /// 0xcc8
			 0x5e3e3375, /// 0xccc
			 0x141dbdbc, /// 0xcd0
			 0x461d5866, /// 0xcd4
			 0x8fe89dba, /// 0xcd8
			 0x0f1bb303, /// 0xcdc
			 0x1a6a209a, /// 0xce0
			 0x1e5ce919, /// 0xce4
			 0x73e93d9b, /// 0xce8
			 0x992160f5, /// 0xcec
			 0x8fd754be, /// 0xcf0
			 0x5c36ab53, /// 0xcf4
			 0x4f2e4986, /// 0xcf8
			 0xc54bf7fb, /// 0xcfc
			 0xc428f1f8, /// 0xd00
			 0x6a5f0300, /// 0xd04
			 0x992ac170, /// 0xd08
			 0xb491d77f, /// 0xd0c
			 0x0ffbf3a2, /// 0xd10
			 0xfdb239a2, /// 0xd14
			 0x5e7fea54, /// 0xd18
			 0xd17fd56d, /// 0xd1c
			 0x15e61cac, /// 0xd20
			 0xc7e9ee10, /// 0xd24
			 0x2b29f918, /// 0xd28
			 0x59dfe741, /// 0xd2c
			 0x61987254, /// 0xd30
			 0x7cf266ba, /// 0xd34
			 0x6573d7ae, /// 0xd38
			 0x69f13efe, /// 0xd3c
			 0xec289258, /// 0xd40
			 0x0cf462f1, /// 0xd44
			 0x7ec32ae5, /// 0xd48
			 0x78c584df, /// 0xd4c
			 0x8f16fa72, /// 0xd50
			 0xd858bb96, /// 0xd54
			 0xb8617152, /// 0xd58
			 0x09470bf7, /// 0xd5c
			 0xf56f2d83, /// 0xd60
			 0x13d8fe6a, /// 0xd64
			 0xdbfee71b, /// 0xd68
			 0xffa60864, /// 0xd6c
			 0x9806ff65, /// 0xd70
			 0x5c666207, /// 0xd74
			 0x85ee0dbe, /// 0xd78
			 0xb5bc46e6, /// 0xd7c
			 0xb293d6c2, /// 0xd80
			 0xe1a6323a, /// 0xd84
			 0xb4f51d02, /// 0xd88
			 0x7fe45647, /// 0xd8c
			 0x162f8260, /// 0xd90
			 0x2ccde9bc, /// 0xd94
			 0x4148903a, /// 0xd98
			 0x39f73df6, /// 0xd9c
			 0x6e0439f1, /// 0xda0
			 0xae1ddcf5, /// 0xda4
			 0xf39fa36c, /// 0xda8
			 0x108d007b, /// 0xdac
			 0xe11218e0, /// 0xdb0
			 0x2fe6c114, /// 0xdb4
			 0xeace34c0, /// 0xdb8
			 0x841ccc19, /// 0xdbc
			 0xbd6ec4e6, /// 0xdc0
			 0x4963d9f7, /// 0xdc4
			 0x8e17058e, /// 0xdc8
			 0x7633fe88, /// 0xdcc
			 0xb85e3496, /// 0xdd0
			 0x0e092277, /// 0xdd4
			 0xd6aebd18, /// 0xdd8
			 0xf2beca41, /// 0xddc
			 0x264627fa, /// 0xde0
			 0x7c10786b, /// 0xde4
			 0x2dc13976, /// 0xde8
			 0x2804a3e3, /// 0xdec
			 0x672d2d80, /// 0xdf0
			 0x7245c5c4, /// 0xdf4
			 0xf991dbc9, /// 0xdf8
			 0x57dcdaee, /// 0xdfc
			 0x6d17e3b9, /// 0xe00
			 0x1e40787d, /// 0xe04
			 0xae5f0f0c, /// 0xe08
			 0xcb034621, /// 0xe0c
			 0x7932243a, /// 0xe10
			 0x1d511c61, /// 0xe14
			 0xf9fbeaa1, /// 0xe18
			 0x5d944ac4, /// 0xe1c
			 0x092d01a3, /// 0xe20
			 0xd6f484b2, /// 0xe24
			 0xaa64c04b, /// 0xe28
			 0x2ee1d486, /// 0xe2c
			 0x7d18d128, /// 0xe30
			 0x65228fdb, /// 0xe34
			 0xfbc88864, /// 0xe38
			 0xee78bf48, /// 0xe3c
			 0xe5bef253, /// 0xe40
			 0x292c3350, /// 0xe44
			 0x703c2a97, /// 0xe48
			 0xa5f33897, /// 0xe4c
			 0xcb9632b6, /// 0xe50
			 0x09e54fa4, /// 0xe54
			 0x3b40b6c3, /// 0xe58
			 0xf47c43b1, /// 0xe5c
			 0x1f0639d2, /// 0xe60
			 0x6b922738, /// 0xe64
			 0x9cebc82b, /// 0xe68
			 0xa9a3628a, /// 0xe6c
			 0x62af7eff, /// 0xe70
			 0x02a9725e, /// 0xe74
			 0x8528cda1, /// 0xe78
			 0x38844573, /// 0xe7c
			 0x06a3d54a, /// 0xe80
			 0xb00d01b0, /// 0xe84
			 0xe03904e1, /// 0xe88
			 0x8e0e528f, /// 0xe8c
			 0xf7236823, /// 0xe90
			 0xefb33eca, /// 0xe94
			 0x06749e36, /// 0xe98
			 0xd153d7f6, /// 0xe9c
			 0xac880754, /// 0xea0
			 0xf4787cd6, /// 0xea4
			 0x8106e15a, /// 0xea8
			 0x48fd473c, /// 0xeac
			 0x8d1a2d30, /// 0xeb0
			 0x64a37c86, /// 0xeb4
			 0x90bc48b5, /// 0xeb8
			 0x5623fd02, /// 0xebc
			 0x1eae4983, /// 0xec0
			 0xe7aff9d1, /// 0xec4
			 0x57e86772, /// 0xec8
			 0x6e189a8d, /// 0xecc
			 0xe28db28b, /// 0xed0
			 0xf18aaa3f, /// 0xed4
			 0xb920752f, /// 0xed8
			 0x313f0bdb, /// 0xedc
			 0x0b2a69a2, /// 0xee0
			 0xdfde920f, /// 0xee4
			 0x1bec1769, /// 0xee8
			 0xf2a5a20f, /// 0xeec
			 0x9870ef37, /// 0xef0
			 0x7defbad9, /// 0xef4
			 0xef592719, /// 0xef8
			 0x1183579c, /// 0xefc
			 0x7bde7dbd, /// 0xf00
			 0xcff1795a, /// 0xf04
			 0xd4b8bdb1, /// 0xf08
			 0x005dea84, /// 0xf0c
			 0xaebed46b, /// 0xf10
			 0xfe1223dd, /// 0xf14
			 0xd5662946, /// 0xf18
			 0x2a39f526, /// 0xf1c
			 0x6e7ddebb, /// 0xf20
			 0x3d299c51, /// 0xf24
			 0xf7e6318e, /// 0xf28
			 0xf3c6410b, /// 0xf2c
			 0xd252a10b, /// 0xf30
			 0x85d364b5, /// 0xf34
			 0x5142a718, /// 0xf38
			 0x1a16ab3d, /// 0xf3c
			 0xf92f1cbf, /// 0xf40
			 0x910d0605, /// 0xf44
			 0xb25120af, /// 0xf48
			 0xf7193659, /// 0xf4c
			 0xac54fa9d, /// 0xf50
			 0x0d161c6b, /// 0xf54
			 0x4252ee55, /// 0xf58
			 0x4ca768c6, /// 0xf5c
			 0xe070add1, /// 0xf60
			 0x92da98cc, /// 0xf64
			 0xde5b162f, /// 0xf68
			 0x755fff2e, /// 0xf6c
			 0xd7b41e14, /// 0xf70
			 0xfc30abc8, /// 0xf74
			 0x921736ef, /// 0xf78
			 0x41805fe3, /// 0xf7c
			 0xf97dbfd9, /// 0xf80
			 0xb5c3686d, /// 0xf84
			 0x6d521be1, /// 0xf88
			 0x030f7771, /// 0xf8c
			 0x3a2335e2, /// 0xf90
			 0x6624f96f, /// 0xf94
			 0xb38d32c8, /// 0xf98
			 0xeca1de34, /// 0xf9c
			 0x1b76f0ff, /// 0xfa0
			 0xdac2b0a4, /// 0xfa4
			 0xc9bed400, /// 0xfa8
			 0xeab6aabd, /// 0xfac
			 0x00a713f7, /// 0xfb0
			 0xdea9947f, /// 0xfb4
			 0x011bcb9f, /// 0xfb8
			 0x189205c8, /// 0xfbc
			 0x4abe3de8, /// 0xfc0
			 0xa1dd8168, /// 0xfc4
			 0x8550091f, /// 0xfc8
			 0x796dbaa6, /// 0xfcc
			 0xb4cc16b1, /// 0xfd0
			 0x84b6193f, /// 0xfd4
			 0xf741789f, /// 0xfd8
			 0x0105e360, /// 0xfdc
			 0x3eda15fe, /// 0xfe0
			 0xbf0205e1, /// 0xfe4
			 0x5de82109, /// 0xfe8
			 0x8c3c85ff, /// 0xfec
			 0xb7b17cf8, /// 0xff0
			 0xb22331b3, /// 0xff4
			 0x5fd35d8e, /// 0xff8
			 0x9cceacc8 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00008
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0000c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0001c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00020
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c600000,                                                  // Leading 1s:                                 /// 00028
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00030
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00044
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0004c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00054
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0005c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00060
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00064
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00068
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00070
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00074
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00078
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0008c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00098
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0009c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d8
			 0x33333333,                                                  // 4 random values                             /// 000dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000e8
			 0xbf028f5c,                                                  // -0.51                                       /// 000ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00108
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0010c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c780000,                                                  // Leading 1s:                                 /// 00118
			 0x00011111,                                                  // 9.7958E-41                                  /// 0011c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00120
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00124
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00130
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x7f800000,                                                  // inf                                         /// 00138
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0013c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00148
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00150
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00154
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00158
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0015c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00168
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0016c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00170
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00174
			 0x3f028f5c,                                                  // 0.51                                        /// 00178
			 0xcb000000,                                                  // -8388608.0                                  /// 0017c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e000007,                                                  // Trailing 1s:                                /// 00184
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0018c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00198
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0019c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x33333333,                                                  // 4 random values                             /// 001a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000000,                                                  // zero                                        /// 001c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00200
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00204
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00208
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00210
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00214
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0021c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00220
			 0x80000000,                                                  // -zero                                       /// 00224
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0022c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00230
			 0x7f800000,                                                  // inf                                         /// 00234
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00248
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0024c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00250
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00258
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0025c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e000007,                                                  // Trailing 1s:                                /// 0026c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00274
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00278
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00284
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00298
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0029c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002f8
			 0x4b000000,                                                  // 8388608.0                                   /// 002fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00300
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00304
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00310
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0031c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00320
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00324
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00328
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0032c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00330
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00334
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00344
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0034c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00350
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00358
			 0x00000000,                                                  // zero                                        /// 0035c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00360
			 0x7f800000,                                                  // inf                                         /// 00364
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00368
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0036c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00370
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00378
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00380
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00384
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00388
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00394
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00398
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00408
			 0x80000000,                                                  // -zero                                       /// 0040c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00414
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00418
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00424
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0042c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0044c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00454
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00458
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00460
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00464
			 0x0c700000,                                                  // Leading 1s:                                 /// 00468
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00470
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00474
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0047c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00488
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0048c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0049c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004a4
			 0xff800000,                                                  // -inf                                        /// 004a8
			 0x33333333,                                                  // 4 random values                             /// 004ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00504
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00514
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00520
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00524
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00530
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00534
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00544
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0054c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00550
			 0x0c780000,                                                  // Leading 1s:                                 /// 00554
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00558
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0055c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00560
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0056c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00570
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00574
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0057c
			 0xff800000,                                                  // -inf                                        /// 00580
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00588
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0058c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00590
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a0
			 0x7f800000,                                                  // inf                                         /// 005a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00000000,                                                  // zero                                        /// 005ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00600
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00608
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0060c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00610
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00614
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00618
			 0x4b000000,                                                  // 8388608.0                                   /// 0061c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00620
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00624
			 0xbf028f5c,                                                  // -0.51                                       /// 00628
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00634
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00638
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00640
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00644
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00648
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0064c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0065c
			 0xcb000000,                                                  // -8388608.0                                  /// 00660
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00664
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00668
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00670
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00674
			 0x00000000,                                                  // zero                                        /// 00678
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0067c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00680
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0xff800000,                                                  // -inf                                        /// 00688
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0069c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00700
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00704
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0070c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00710
			 0x0e000001,                                                  // Trailing 1s:                                /// 00714
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000000,                                                  // zero                                        /// 0071c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00728
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0072c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00730
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00738
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0073c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00740
			 0x4b000000,                                                  // 8388608.0                                   /// 00744
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00748
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0074c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00750
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00754
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00758
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00760
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00764
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00768
			 0xbf028f5c,                                                  // -0.51                                       /// 0076c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00770
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00778
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0077c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00780
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00784
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00788
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0078c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00794
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00798
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0079c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00800
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00804
			 0x0c780000,                                                  // Leading 1s:                                 /// 00808
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0080c
			 0x4b000000,                                                  // 8388608.0                                   /// 00810
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00818
			 0x00000000,                                                  // zero                                        /// 0081c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00824
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00828
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x00000000,                                                  // zero                                        /// 0083c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00840
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00848
			 0x00000000,                                                  // zero                                        /// 0084c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00850
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00854
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00858
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0085c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00860
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00864
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0086c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00874
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00878
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00880
			 0x0e000003,                                                  // Trailing 1s:                                /// 00884
			 0x80011111,                                                  // -9.7958E-41                                 /// 00888
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0089c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x3f028f5c,                                                  // 0.51                                        /// 008bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e8
			 0x4b000000,                                                  // 8388608.0                                   /// 008ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00900
			 0x3f028f5c,                                                  // 0.51                                        /// 00904
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00908
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0090c
			 0x3f028f5c,                                                  // 0.51                                        /// 00910
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00914
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00918
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00924
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0092c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00938
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0093c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00944
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00948
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0094c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e000007,                                                  // Trailing 1s:                                /// 00954
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00958
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0095c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00960
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00964
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00968
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0096c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00974
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00978
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00980
			 0x0c700000,                                                  // Leading 1s:                                 /// 00984
			 0x7f800000,                                                  // inf                                         /// 00988
			 0x80011111,                                                  // -9.7958E-41                                 /// 0098c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00990
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00994
			 0x00011111,                                                  // 9.7958E-41                                  /// 00998
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0099c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009a4
			 0xff800000,                                                  // -inf                                        /// 009a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a98
			 0x7f800000,                                                  // inf                                         /// 00a9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ab8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00acc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ad0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00adc
			 0x00000000,                                                  // zero                                        /// 00ae0
			 0x7f800000,                                                  // inf                                         /// 00ae4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ae8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00af0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00af4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00afc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b04
			 0x7f800000,                                                  // inf                                         /// 00b08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b50
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x7f800000,                                                  // inf                                         /// 00b7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bb0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bdc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c14
			 0x7fc00001,                                                  // signaling NaN                               /// 00c18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000000,                                                  // -zero                                       /// 00c5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ccc
			 0x7f800000,                                                  // inf                                         /// 00cd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cf4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cfc
			 0xffc00001,                                                  // -signaling NaN                              /// 00d00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d08
			 0x3f028f5c,                                                  // 0.51                                        /// 00d0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d38
			 0x3f028f5c,                                                  // 0.51                                        /// 00d3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d50
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d9c
			 0xff800000,                                                  // -inf                                        /// 00da0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00db8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df4
			 0x80000000,                                                  // -zero                                       /// 00df8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dfc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e14
			 0xbf028f5c,                                                  // -0.51                                       /// 00e18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x7fc00001,                                                  // signaling NaN                               /// 00e74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e90
			 0x7fc00001,                                                  // signaling NaN                               /// 00e94
			 0x00000000,                                                  // zero                                        /// 00e98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ea0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eb8
			 0x80000000,                                                  // -zero                                       /// 00ebc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ec4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00edc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f54
			 0x33333333,                                                  // 4 random values                             /// 00f58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f70
			 0x4b000000,                                                  // 8388608.0                                   /// 00f74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00000000,                                                  // zero                                        /// 00f90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fa0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fb0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fcc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fe4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fe8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff8
			 0x0e00003f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00000
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xbf028f5c,                                                  // -0.51                                       /// 00010
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00014
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00018
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0001c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00024
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0002c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00034
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00038
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0003c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00044
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00048
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0004c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00054
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00064
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0006c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00070
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0007c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00080
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00084
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00088
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0008c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00090
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00094
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0009c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00100
			 0x0e000001,                                                  // Trailing 1s:                                /// 00104
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00110
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00118
			 0x00011111,                                                  // 9.7958E-41                                  /// 0011c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00120
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0e000001,                                                  // Trailing 1s:                                /// 00128
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0012c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00130
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00134
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00138
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0013c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00140
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00144
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00148
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00154
			 0xff800000,                                                  // -inf                                        /// 00158
			 0xffc00001,                                                  // -signaling NaN                              /// 0015c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00160
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00168
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00174
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0017c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00180
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0018c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00190
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00194
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00198
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0019c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001e0
			 0xff800000,                                                  // -inf                                        /// 001e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00200
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00204
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00208
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0020c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00218
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0021c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00220
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00224
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00228
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00230
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00238
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0023c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00240
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00244
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0024c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00254
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00264
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00270
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00274
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00278
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00280
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00284
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00290
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00294
			 0xffc00001,                                                  // -signaling NaN                              /// 00298
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b8
			 0x7f800000,                                                  // inf                                         /// 002bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002d4
			 0x7fc00001,                                                  // signaling NaN                               /// 002d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00300
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00304
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00310
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00314
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00318
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00320
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00328
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00330
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00334
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0033c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00340
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00344
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00348
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0034c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00350
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00354
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00358
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00360
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0036c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00378
			 0x7f800000,                                                  // inf                                         /// 0037c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00380
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0c700000,                                                  // Leading 1s:                                 /// 00388
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0038c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00390
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00398
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00400
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00410
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00414
			 0x00000000,                                                  // zero                                        /// 00418
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0041c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00420
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00424
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00428
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0042c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00430
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00434
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00438
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0043c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x55555555,                                                  // 4 random values                             /// 00444
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0044c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00450
			 0x00000000,                                                  // zero                                        /// 00454
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00458
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00460
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00464
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0046c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00470
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00478
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0047c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x00011111,                                                  // 9.7958E-41                                  /// 0048c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00494
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00498
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0049c
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00500
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00504
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00508
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0050c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00510
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00514
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00518
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00524
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00534
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0053c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00540
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00544
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0054c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00550
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00554
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00558
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0055c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00560
			 0x3f028f5c,                                                  // 0.51                                        /// 00564
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00568
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0056c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00570
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00578
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00580
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00588
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00594
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00598
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a4
			 0xff800000,                                                  // -inf                                        /// 005a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x7fc00001,                                                  // signaling NaN                               /// 005e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x7fc00001,                                                  // signaling NaN                               /// 00604
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00608
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0060c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00614
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00618
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0061c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00620
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00624
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00628
			 0x00011111,                                                  // 9.7958E-41                                  /// 0062c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00634
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00638
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00648
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0064c
			 0x4b000000,                                                  // 8388608.0                                   /// 00650
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00654
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00660
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0066c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x4b000000,                                                  // 8388608.0                                   /// 0067c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00680
			 0x80000000,                                                  // -zero                                       /// 00684
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00688
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0068c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00690
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00694
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00698
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0069c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006cc
			 0x00000000,                                                  // zero                                        /// 006d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x00000000,                                                  // zero                                        /// 006e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00704
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x00000000,                                                  // zero                                        /// 0070c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00710
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0071c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00724
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00728
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0072c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00730
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00734
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xcb000000,                                                  // -8388608.0                                  /// 00744
			 0x7fc00001,                                                  // signaling NaN                               /// 00748
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00750
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00758
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00760
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00770
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00774
			 0x0e000001,                                                  // Trailing 1s:                                /// 00778
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0077c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00780
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00788
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0078c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00790
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00798
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x7fc00001,                                                  // signaling NaN                               /// 007c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00800
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00808
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0080c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00810
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0081c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00824
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00828
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00834
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00838
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00840
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0e000003,                                                  // Trailing 1s:                                /// 0084c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00850
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xbf028f5c,                                                  // -0.51                                       /// 00858
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0085c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00864
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00868
			 0x0e000003,                                                  // Trailing 1s:                                /// 0086c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0xffc00001,                                                  // -signaling NaN                              /// 00874
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0087c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00888
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0088c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00890
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00894
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b8
			 0x55555555,                                                  // 4 random values                             /// 008bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00904
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0090c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00910
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00914
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0091c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00920
			 0xbf028f5c,                                                  // -0.51                                       /// 00924
			 0xffc00001,                                                  // -signaling NaN                              /// 00928
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0092c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00930
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00938
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0093c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00940
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00944
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00948
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00950
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00954
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00960
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00964
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00968
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00970
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00978
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0097c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00988
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0098c
			 0x4b000000,                                                  // 8388608.0                                   /// 00990
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0099c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0xcb000000,                                                  // -8388608.0                                  /// 009b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a04
			 0x3f028f5c,                                                  // 0.51                                        /// 00a08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a88
			 0xff800000,                                                  // -inf                                        /// 00a8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ab0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00abc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ac8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00acc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ad8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ae4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00af8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b68
			 0xff800000,                                                  // -inf                                        /// 00b6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bb8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bdc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c3c
			 0xff800000,                                                  // -inf                                        /// 00c40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ca0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cf8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d50
			 0x4b000000,                                                  // 8388608.0                                   /// 00d54
			 0x33333333,                                                  // 4 random values                             /// 00d58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xff800000,                                                  // -inf                                        /// 00d88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00da0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00db0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dbc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00dc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xff800000,                                                  // -inf                                        /// 00de8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00df0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0xcb000000,                                                  // -8388608.0                                  /// 00e04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e14
			 0x33333333,                                                  // 4 random values                             /// 00e18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e34
			 0x3f028f5c,                                                  // 0.51                                        /// 00e38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x55555555,                                                  // 4 random values                             /// 00eb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ef4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ef8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00efc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f78
			 0x7f800000,                                                  // inf                                         /// 00f7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f80
			 0xffc00001,                                                  // -signaling NaN                              /// 00f84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fb0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fdc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fe8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0xcb000000,                                                  // -8388608.0                                  /// 00ff0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff8
			 0x80000020                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x00000738,
			 0x0000040c,
			 0x00000110,
			 0x0000056c,
			 0x00000560,
			 0x000000d8,
			 0x00000734,
			 0x000003ac,

			 /// vpu register f2
			 0x41a80000,
			 0x41e00000,
			 0x42000000,
			 0x41f80000,
			 0x40000000,
			 0x41c80000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f3
			 0x42000000,
			 0x41980000,
			 0x41d80000,
			 0x41f00000,
			 0x41800000,
			 0x40400000,
			 0x40000000,
			 0x41980000,

			 /// vpu register f4
			 0x40e00000,
			 0x40a00000,
			 0x41f80000,
			 0x3f800000,
			 0x41d00000,
			 0x41500000,
			 0x41f00000,
			 0x41980000,

			 /// vpu register f5
			 0x41400000,
			 0x42000000,
			 0x3f800000,
			 0x41d80000,
			 0x40e00000,
			 0x41700000,
			 0x40800000,
			 0x41880000,

			 /// vpu register f6
			 0x40c00000,
			 0x40800000,
			 0x41f80000,
			 0x41c80000,
			 0x41400000,
			 0x40000000,
			 0x41980000,
			 0x41c00000,

			 /// vpu register f7
			 0x41800000,
			 0x40c00000,
			 0x40a00000,
			 0x41a00000,
			 0x41f00000,
			 0x41a80000,
			 0x41b00000,
			 0x41700000,

			 /// vpu register f8
			 0x41b00000,
			 0x41b00000,
			 0x40400000,
			 0x41100000,
			 0x41d80000,
			 0x41700000,
			 0x41c80000,
			 0x40400000,

			 /// vpu register f9
			 0x41980000,
			 0x41f00000,
			 0x41200000,
			 0x41880000,
			 0x41600000,
			 0x41700000,
			 0x41c00000,
			 0x41d00000,

			 /// vpu register f10
			 0x41800000,
			 0x41100000,
			 0x41c00000,
			 0x40a00000,
			 0x41a80000,
			 0x41e00000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f11
			 0x41100000,
			 0x41880000,
			 0x41200000,
			 0x41800000,
			 0x40000000,
			 0x41a00000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f12
			 0x41400000,
			 0x41600000,
			 0x42000000,
			 0x41000000,
			 0x3f800000,
			 0x41f00000,
			 0x41b00000,
			 0x41e00000,

			 /// vpu register f13
			 0x41c80000,
			 0x41500000,
			 0x41980000,
			 0x41c00000,
			 0x41f80000,
			 0x40e00000,
			 0x3f800000,
			 0x40a00000,

			 /// vpu register f14
			 0x41b80000,
			 0x40a00000,
			 0x41880000,
			 0x41300000,
			 0x40800000,
			 0x41b00000,
			 0x41a80000,
			 0x40000000,

			 /// vpu register f15
			 0x41c80000,
			 0x3f800000,
			 0x41b80000,
			 0x41300000,
			 0x41b00000,
			 0x3f800000,
			 0x41b00000,
			 0x41700000,

			 /// vpu register f16
			 0x41c80000,
			 0x41e80000,
			 0x40000000,
			 0x41b00000,
			 0x41980000,
			 0x41500000,
			 0x41600000,
			 0x41980000,

			 /// vpu register f17
			 0x41800000,
			 0x41b00000,
			 0x41800000,
			 0x41e80000,
			 0x41600000,
			 0x41d00000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f18
			 0x41f80000,
			 0x41300000,
			 0x41d00000,
			 0x41700000,
			 0x41300000,
			 0x41c00000,
			 0x41e00000,
			 0x41900000,

			 /// vpu register f19
			 0x41d00000,
			 0x41300000,
			 0x41980000,
			 0x41b00000,
			 0x41b00000,
			 0x41980000,
			 0x41b00000,
			 0x41c80000,

			 /// vpu register f20
			 0x41e00000,
			 0x40800000,
			 0x41300000,
			 0x41200000,
			 0x41980000,
			 0x41f80000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f21
			 0x41a00000,
			 0x41d80000,
			 0x41800000,
			 0x40800000,
			 0x41000000,
			 0x40400000,
			 0x40c00000,
			 0x41800000,

			 /// vpu register f22
			 0x41b00000,
			 0x40000000,
			 0x41400000,
			 0x41600000,
			 0x41c80000,
			 0x41e80000,
			 0x41200000,
			 0x42000000,

			 /// vpu register f23
			 0x41e80000,
			 0x41e00000,
			 0x41f00000,
			 0x40800000,
			 0x41b80000,
			 0x41e00000,
			 0x41100000,
			 0x41400000,

			 /// vpu register f24
			 0x41100000,
			 0x41600000,
			 0x40a00000,
			 0x41a80000,
			 0x41a80000,
			 0x41880000,
			 0x41700000,
			 0x41d80000,

			 /// vpu register f25
			 0x41800000,
			 0x41c00000,
			 0x41500000,
			 0x41c80000,
			 0x41980000,
			 0x40c00000,
			 0x41100000,
			 0x41a80000,

			 /// vpu register f26
			 0x40000000,
			 0x40400000,
			 0x41c00000,
			 0x41800000,
			 0x41d00000,
			 0x41800000,
			 0x41900000,
			 0x41c80000,

			 /// vpu register f27
			 0x41700000,
			 0x41400000,
			 0x41900000,
			 0x41b00000,
			 0x41500000,
			 0x41b80000,
			 0x41d80000,
			 0x41d80000,

			 /// vpu register f28
			 0x41900000,
			 0x41980000,
			 0x41800000,
			 0x41b80000,
			 0x41e00000,
			 0x41500000,
			 0x41a00000,
			 0x41900000,

			 /// vpu register f29
			 0x41e00000,
			 0x41e00000,
			 0x40000000,
			 0x41a00000,
			 0x41a80000,
			 0x41880000,
			 0x41b80000,
			 0x41900000,

			 /// vpu register f30
			 0x3f800000,
			 0x41100000,
			 0x41e00000,
			 0x41c80000,
			 0x41a80000,
			 0x41200000,
			 0x42000000,
			 0x41c00000,

			 /// vpu register f31
			 0x41600000,
			 0x41e00000,
			 0x41c00000,
			 0x41e80000,
			 0x41f80000,
			 0x41880000,
			 0x41600000,
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
		"mova.x.m x3\n"                                       ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x9\n"                                       ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x4\n"                                       ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x7\n"                                       ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x6\n"                                       ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x3\n"                                       ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.x.m x5\n"                                       ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
