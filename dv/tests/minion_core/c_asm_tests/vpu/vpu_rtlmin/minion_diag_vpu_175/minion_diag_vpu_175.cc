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
			 0xaa686b20, /// 0x0
			 0xbc333a5b, /// 0x4
			 0xe915d4f0, /// 0x8
			 0xed2c5a0f, /// 0xc
			 0xda899208, /// 0x10
			 0xe484d50c, /// 0x14
			 0xf67a49b1, /// 0x18
			 0xa1054088, /// 0x1c
			 0x9644434e, /// 0x20
			 0x616fe0a0, /// 0x24
			 0xee5ce77c, /// 0x28
			 0xd8afecd7, /// 0x2c
			 0x7e3a31a8, /// 0x30
			 0xa837e4dc, /// 0x34
			 0x7e25e385, /// 0x38
			 0x4243f262, /// 0x3c
			 0xbda414f7, /// 0x40
			 0xbf52c5ff, /// 0x44
			 0x96dc2f3e, /// 0x48
			 0xd4270f15, /// 0x4c
			 0xa8d095bc, /// 0x50
			 0xb2accd7b, /// 0x54
			 0xe6162bc9, /// 0x58
			 0x86beffa0, /// 0x5c
			 0x171685a1, /// 0x60
			 0x9d855fe7, /// 0x64
			 0x4cc788ba, /// 0x68
			 0xc808c01d, /// 0x6c
			 0x24870d9c, /// 0x70
			 0xb170c4a3, /// 0x74
			 0x48c152cd, /// 0x78
			 0x26a418bd, /// 0x7c
			 0x0f089343, /// 0x80
			 0x98369ba1, /// 0x84
			 0xdd67a37e, /// 0x88
			 0x5ec1ee93, /// 0x8c
			 0x496b2fa7, /// 0x90
			 0xd9509392, /// 0x94
			 0xc01a4226, /// 0x98
			 0xee2767f9, /// 0x9c
			 0xed42ac6d, /// 0xa0
			 0x99276d5f, /// 0xa4
			 0x5972735b, /// 0xa8
			 0x03788886, /// 0xac
			 0x6e2cc29c, /// 0xb0
			 0xa3a30942, /// 0xb4
			 0x9f3a0ad6, /// 0xb8
			 0x6d7e860b, /// 0xbc
			 0xd296fdae, /// 0xc0
			 0x1efc34df, /// 0xc4
			 0x42214b85, /// 0xc8
			 0xe4987716, /// 0xcc
			 0x5384eb6d, /// 0xd0
			 0xc3b9a054, /// 0xd4
			 0x4df5e99d, /// 0xd8
			 0x76cedeae, /// 0xdc
			 0x452241d5, /// 0xe0
			 0xbec1e3fe, /// 0xe4
			 0x0aa2686b, /// 0xe8
			 0xfbc74c49, /// 0xec
			 0xb50a216b, /// 0xf0
			 0xe249bf9c, /// 0xf4
			 0x1a51484f, /// 0xf8
			 0x321e9d55, /// 0xfc
			 0xbce856c6, /// 0x100
			 0xe39c2162, /// 0x104
			 0x1803b549, /// 0x108
			 0x41a1fa98, /// 0x10c
			 0xfc8c7079, /// 0x110
			 0x7c86bba5, /// 0x114
			 0xd132bc2b, /// 0x118
			 0x1754f65e, /// 0x11c
			 0xee97ae86, /// 0x120
			 0x25c4b836, /// 0x124
			 0x76cf508e, /// 0x128
			 0xf2aa2ffc, /// 0x12c
			 0xc4c4155e, /// 0x130
			 0xfc98c981, /// 0x134
			 0x4db7541d, /// 0x138
			 0xd6af77d5, /// 0x13c
			 0x43e3fe55, /// 0x140
			 0x72fdfd9f, /// 0x144
			 0x31b237ce, /// 0x148
			 0x281d62a6, /// 0x14c
			 0x0fb55d9d, /// 0x150
			 0xe837369e, /// 0x154
			 0xe8bc48b8, /// 0x158
			 0xee298b3c, /// 0x15c
			 0x0c6f3633, /// 0x160
			 0xcec1f722, /// 0x164
			 0x4e18c793, /// 0x168
			 0xecbeac9d, /// 0x16c
			 0x02683050, /// 0x170
			 0x7d0dcec1, /// 0x174
			 0x89b9657c, /// 0x178
			 0x1bce35dd, /// 0x17c
			 0x74a4f86c, /// 0x180
			 0x48fca8f8, /// 0x184
			 0x110e1ef0, /// 0x188
			 0xed405019, /// 0x18c
			 0x4f207083, /// 0x190
			 0xe6e7261b, /// 0x194
			 0x4a160ec7, /// 0x198
			 0x165f4caf, /// 0x19c
			 0x3e96fc94, /// 0x1a0
			 0x1a9b16a2, /// 0x1a4
			 0x3632526c, /// 0x1a8
			 0x7a53d5a7, /// 0x1ac
			 0x9935102b, /// 0x1b0
			 0x439552bc, /// 0x1b4
			 0x44643a23, /// 0x1b8
			 0x3ad14b08, /// 0x1bc
			 0x5dcaf8ca, /// 0x1c0
			 0xa16b4933, /// 0x1c4
			 0x0d5e9230, /// 0x1c8
			 0xf271033b, /// 0x1cc
			 0x0586785d, /// 0x1d0
			 0xa26981e6, /// 0x1d4
			 0x5083a936, /// 0x1d8
			 0x78372c59, /// 0x1dc
			 0x75f84f46, /// 0x1e0
			 0xa239ed02, /// 0x1e4
			 0x98fff353, /// 0x1e8
			 0x11371123, /// 0x1ec
			 0x1ed748b1, /// 0x1f0
			 0xdbe33801, /// 0x1f4
			 0x6239cd76, /// 0x1f8
			 0x9b107663, /// 0x1fc
			 0xb9b79638, /// 0x200
			 0xfe61e40c, /// 0x204
			 0xed35f246, /// 0x208
			 0x1c77c79a, /// 0x20c
			 0x9f210453, /// 0x210
			 0xa50c4054, /// 0x214
			 0x177bd095, /// 0x218
			 0xe5e21977, /// 0x21c
			 0xcf420bf5, /// 0x220
			 0x3c988068, /// 0x224
			 0x967d1b2c, /// 0x228
			 0xdfcb5200, /// 0x22c
			 0x2060b7f5, /// 0x230
			 0x227c85e8, /// 0x234
			 0x4e612f1e, /// 0x238
			 0x83c112ff, /// 0x23c
			 0x88ed53c9, /// 0x240
			 0x2059a38c, /// 0x244
			 0xe461c96d, /// 0x248
			 0x323f63c5, /// 0x24c
			 0xe19aab2f, /// 0x250
			 0x07ae9d7c, /// 0x254
			 0x92fea2ef, /// 0x258
			 0x3b430c6f, /// 0x25c
			 0x5bf6048e, /// 0x260
			 0x6397d461, /// 0x264
			 0x95aa866c, /// 0x268
			 0x3101f243, /// 0x26c
			 0x5bd043a4, /// 0x270
			 0x45f580c9, /// 0x274
			 0xae66dd96, /// 0x278
			 0x35731d2a, /// 0x27c
			 0x612d43f5, /// 0x280
			 0xdf014f63, /// 0x284
			 0x019da249, /// 0x288
			 0xdf2870e4, /// 0x28c
			 0x0ab7a5de, /// 0x290
			 0xfead039e, /// 0x294
			 0x8baba5b4, /// 0x298
			 0x0e4bb847, /// 0x29c
			 0xf773cdb2, /// 0x2a0
			 0x8aa57270, /// 0x2a4
			 0x12e08214, /// 0x2a8
			 0x574720d4, /// 0x2ac
			 0xe38edff1, /// 0x2b0
			 0x0b5e958b, /// 0x2b4
			 0xa85704c3, /// 0x2b8
			 0xc8925905, /// 0x2bc
			 0xb9f184c4, /// 0x2c0
			 0xb8cb59de, /// 0x2c4
			 0x59a7e4da, /// 0x2c8
			 0xe3e5c4c7, /// 0x2cc
			 0x6e27e9ca, /// 0x2d0
			 0xf90197f0, /// 0x2d4
			 0xd892ad9f, /// 0x2d8
			 0x84db4971, /// 0x2dc
			 0xdaf85adb, /// 0x2e0
			 0xe5e1be2f, /// 0x2e4
			 0x1d9baf7d, /// 0x2e8
			 0xda38f100, /// 0x2ec
			 0xaa5a60c0, /// 0x2f0
			 0xe704609b, /// 0x2f4
			 0x6f813f84, /// 0x2f8
			 0xcd989652, /// 0x2fc
			 0x7f967299, /// 0x300
			 0x15fac9d7, /// 0x304
			 0x3480de71, /// 0x308
			 0x6c0e0027, /// 0x30c
			 0x91b99584, /// 0x310
			 0x80585bfa, /// 0x314
			 0xcd4b8dd7, /// 0x318
			 0x0d45a1f1, /// 0x31c
			 0xf3674b79, /// 0x320
			 0xbcba22fa, /// 0x324
			 0x3ab27d5c, /// 0x328
			 0x5a903c8e, /// 0x32c
			 0x756cc38e, /// 0x330
			 0x947600d7, /// 0x334
			 0x64592aca, /// 0x338
			 0x37e86d96, /// 0x33c
			 0x9d167762, /// 0x340
			 0xd693571a, /// 0x344
			 0xcd4035eb, /// 0x348
			 0x15f9d094, /// 0x34c
			 0xcd58fe15, /// 0x350
			 0x099ec4ab, /// 0x354
			 0x0d0df8cb, /// 0x358
			 0xa5b63ac7, /// 0x35c
			 0xb18e6289, /// 0x360
			 0x1f065f87, /// 0x364
			 0xad2dfd66, /// 0x368
			 0xcbfc9e76, /// 0x36c
			 0xcd2874cd, /// 0x370
			 0xa8d17966, /// 0x374
			 0xd73e1690, /// 0x378
			 0x6a3915ed, /// 0x37c
			 0x22a8ab7d, /// 0x380
			 0x6624a9c8, /// 0x384
			 0xb1fb6b7f, /// 0x388
			 0xce00b594, /// 0x38c
			 0xab420a76, /// 0x390
			 0x3d399471, /// 0x394
			 0xb67f5b21, /// 0x398
			 0x1a7a7dee, /// 0x39c
			 0x7c54edcf, /// 0x3a0
			 0x5fa5eb23, /// 0x3a4
			 0x9369ba4d, /// 0x3a8
			 0x303313e3, /// 0x3ac
			 0x15c9d89d, /// 0x3b0
			 0x6a3ef987, /// 0x3b4
			 0x6ad88a6f, /// 0x3b8
			 0xaff8bb4c, /// 0x3bc
			 0xf5cfbb37, /// 0x3c0
			 0x09f02479, /// 0x3c4
			 0xd30e5dbd, /// 0x3c8
			 0x644ec1f9, /// 0x3cc
			 0xe9d25c19, /// 0x3d0
			 0x7cb9af00, /// 0x3d4
			 0x6913dec5, /// 0x3d8
			 0x5dc90998, /// 0x3dc
			 0x954535bd, /// 0x3e0
			 0x1060c0ab, /// 0x3e4
			 0x7444187b, /// 0x3e8
			 0x2a1d7ab8, /// 0x3ec
			 0x8bad5c53, /// 0x3f0
			 0xa20091c4, /// 0x3f4
			 0x668d6cb2, /// 0x3f8
			 0x0388c9d0, /// 0x3fc
			 0x558d81ab, /// 0x400
			 0x6d63855a, /// 0x404
			 0x27974424, /// 0x408
			 0x32353ee1, /// 0x40c
			 0x24745bfa, /// 0x410
			 0xe5d5121c, /// 0x414
			 0x51cf6051, /// 0x418
			 0xbff44cd4, /// 0x41c
			 0x1965d66a, /// 0x420
			 0xc5d9532a, /// 0x424
			 0x5de23961, /// 0x428
			 0xeff5162e, /// 0x42c
			 0x4aac85af, /// 0x430
			 0x1efbc6d8, /// 0x434
			 0x3bb849ee, /// 0x438
			 0xdd63ff99, /// 0x43c
			 0x7236cb27, /// 0x440
			 0x0e0e9342, /// 0x444
			 0xeae558bf, /// 0x448
			 0x63436c16, /// 0x44c
			 0x098805ba, /// 0x450
			 0x41b380f7, /// 0x454
			 0xd01a20cb, /// 0x458
			 0x146b7fda, /// 0x45c
			 0x7f9a86f6, /// 0x460
			 0x63eece27, /// 0x464
			 0xe2e82503, /// 0x468
			 0x99d12b0b, /// 0x46c
			 0xd6dfeccb, /// 0x470
			 0x19da473d, /// 0x474
			 0x0f7e64a7, /// 0x478
			 0x71016480, /// 0x47c
			 0xd3936aeb, /// 0x480
			 0x339bfc1c, /// 0x484
			 0xabe6bb18, /// 0x488
			 0xaa3f6f42, /// 0x48c
			 0xc05d0409, /// 0x490
			 0x54214c9c, /// 0x494
			 0x95959c31, /// 0x498
			 0x9aed9477, /// 0x49c
			 0x3f2da66a, /// 0x4a0
			 0xdfb94ecb, /// 0x4a4
			 0x9d079f9f, /// 0x4a8
			 0xf36acc4e, /// 0x4ac
			 0x117184ed, /// 0x4b0
			 0xe84bf492, /// 0x4b4
			 0x68cc4616, /// 0x4b8
			 0xb57ee374, /// 0x4bc
			 0xa8974923, /// 0x4c0
			 0x00c1ff02, /// 0x4c4
			 0x2d82cc7c, /// 0x4c8
			 0xfb083488, /// 0x4cc
			 0x07daeab3, /// 0x4d0
			 0x77bc55a6, /// 0x4d4
			 0xebdbafa0, /// 0x4d8
			 0x7d9101ea, /// 0x4dc
			 0x99a1fcee, /// 0x4e0
			 0x683c051d, /// 0x4e4
			 0xf527208e, /// 0x4e8
			 0xe25ce54d, /// 0x4ec
			 0x42794d54, /// 0x4f0
			 0x73a7a305, /// 0x4f4
			 0xa5a23f87, /// 0x4f8
			 0xcda19331, /// 0x4fc
			 0x8de09beb, /// 0x500
			 0x1833d17a, /// 0x504
			 0x0bd96cb4, /// 0x508
			 0x19fb47b5, /// 0x50c
			 0xfe04f5ad, /// 0x510
			 0xd747f8a4, /// 0x514
			 0x7a40ebb7, /// 0x518
			 0xfd522bb2, /// 0x51c
			 0x7d89f31a, /// 0x520
			 0xffe00faa, /// 0x524
			 0x1ad6af2f, /// 0x528
			 0xdd2c052f, /// 0x52c
			 0x65b1427b, /// 0x530
			 0x454769af, /// 0x534
			 0x10765d91, /// 0x538
			 0xc460ee10, /// 0x53c
			 0x42c4a010, /// 0x540
			 0x4db9ffe0, /// 0x544
			 0xbf5f797e, /// 0x548
			 0x055c0514, /// 0x54c
			 0x9b98f19f, /// 0x550
			 0xac86054c, /// 0x554
			 0xd97c3a38, /// 0x558
			 0xdc332ca8, /// 0x55c
			 0x49555ead, /// 0x560
			 0x6d96b68b, /// 0x564
			 0x1a17bb23, /// 0x568
			 0x9dc9da78, /// 0x56c
			 0xb0ee347b, /// 0x570
			 0x7623faa9, /// 0x574
			 0xc2d70424, /// 0x578
			 0x483fa1a1, /// 0x57c
			 0x87daebb7, /// 0x580
			 0x417a201f, /// 0x584
			 0xcc38f961, /// 0x588
			 0xaa68292e, /// 0x58c
			 0xe15b764e, /// 0x590
			 0x97746525, /// 0x594
			 0xf818e63a, /// 0x598
			 0x997b0e28, /// 0x59c
			 0x831fa729, /// 0x5a0
			 0x9274373a, /// 0x5a4
			 0x72e4bcfa, /// 0x5a8
			 0xc505be85, /// 0x5ac
			 0xd9c06cd1, /// 0x5b0
			 0x1baaaba0, /// 0x5b4
			 0x372067fe, /// 0x5b8
			 0xd600648f, /// 0x5bc
			 0x216a225f, /// 0x5c0
			 0xfcb186cb, /// 0x5c4
			 0xa5d35415, /// 0x5c8
			 0x487b1dae, /// 0x5cc
			 0x5da0e6c2, /// 0x5d0
			 0xb4391b80, /// 0x5d4
			 0xa03ae6f0, /// 0x5d8
			 0x23b3122a, /// 0x5dc
			 0xec61c6ce, /// 0x5e0
			 0x7e51ed17, /// 0x5e4
			 0xaed16e67, /// 0x5e8
			 0x3f0a759b, /// 0x5ec
			 0x1d094a40, /// 0x5f0
			 0xc83306e7, /// 0x5f4
			 0x571b44fa, /// 0x5f8
			 0x59e1e820, /// 0x5fc
			 0x3d83fe53, /// 0x600
			 0x2be4abe6, /// 0x604
			 0x35d2114d, /// 0x608
			 0x9c341780, /// 0x60c
			 0x540eb428, /// 0x610
			 0x90964eeb, /// 0x614
			 0x451bf4e8, /// 0x618
			 0x743067cd, /// 0x61c
			 0xef25dcbc, /// 0x620
			 0x75b14864, /// 0x624
			 0x5213c812, /// 0x628
			 0xfc0cce70, /// 0x62c
			 0x013ee064, /// 0x630
			 0x11bbe060, /// 0x634
			 0x9a365029, /// 0x638
			 0x6ad982e3, /// 0x63c
			 0x9ee5f5f0, /// 0x640
			 0x8c2af8a9, /// 0x644
			 0x39fd225e, /// 0x648
			 0x30ad01d5, /// 0x64c
			 0x0d81c532, /// 0x650
			 0x448de4c2, /// 0x654
			 0xeea75647, /// 0x658
			 0xeddaa7d8, /// 0x65c
			 0x79559b48, /// 0x660
			 0x8766f179, /// 0x664
			 0xeb5e31b3, /// 0x668
			 0x0615683a, /// 0x66c
			 0xf1198f55, /// 0x670
			 0xb26428b7, /// 0x674
			 0x2dfc6a1b, /// 0x678
			 0xf34e1d76, /// 0x67c
			 0x0aa5c60c, /// 0x680
			 0x32715a19, /// 0x684
			 0xd55eea8b, /// 0x688
			 0x77be7865, /// 0x68c
			 0x30c425d5, /// 0x690
			 0x703e6be7, /// 0x694
			 0xecb6c2aa, /// 0x698
			 0x35ed5801, /// 0x69c
			 0xb0ebba7e, /// 0x6a0
			 0xbe4e198a, /// 0x6a4
			 0x0cbc7f99, /// 0x6a8
			 0x21b38939, /// 0x6ac
			 0x233492a6, /// 0x6b0
			 0xfac0c9ef, /// 0x6b4
			 0x3fe05c3e, /// 0x6b8
			 0x0634e9f0, /// 0x6bc
			 0x0e4ea732, /// 0x6c0
			 0x6915cf23, /// 0x6c4
			 0xa67200c6, /// 0x6c8
			 0xcf941f77, /// 0x6cc
			 0x9d4391ad, /// 0x6d0
			 0x29874363, /// 0x6d4
			 0x901927a1, /// 0x6d8
			 0x0fc4067d, /// 0x6dc
			 0x849479f6, /// 0x6e0
			 0x09fdd018, /// 0x6e4
			 0x4e18c9c0, /// 0x6e8
			 0x1b4e56f5, /// 0x6ec
			 0x69022fd1, /// 0x6f0
			 0xb8a3c13c, /// 0x6f4
			 0xaa835e7d, /// 0x6f8
			 0x0e6b05b7, /// 0x6fc
			 0xe8724009, /// 0x700
			 0x755e045a, /// 0x704
			 0xf0fa571b, /// 0x708
			 0xeed9fe1c, /// 0x70c
			 0x6a862b29, /// 0x710
			 0x357b3d97, /// 0x714
			 0xbc2dcaf1, /// 0x718
			 0x30c93218, /// 0x71c
			 0x34f539ec, /// 0x720
			 0xc6ea6092, /// 0x724
			 0xb2a7e65e, /// 0x728
			 0x39f670b2, /// 0x72c
			 0xe96abab4, /// 0x730
			 0xf85cef23, /// 0x734
			 0xf60f2640, /// 0x738
			 0x28ce7869, /// 0x73c
			 0x07124050, /// 0x740
			 0xe8b45a76, /// 0x744
			 0x57eea22a, /// 0x748
			 0xf10f53ae, /// 0x74c
			 0xa94270a0, /// 0x750
			 0x4ab1a419, /// 0x754
			 0xcf66eaa9, /// 0x758
			 0xf21c1fa6, /// 0x75c
			 0xcd86a1bf, /// 0x760
			 0x43177409, /// 0x764
			 0xc18e618d, /// 0x768
			 0xa9b5f657, /// 0x76c
			 0xf4adc69e, /// 0x770
			 0xb7ae02fd, /// 0x774
			 0x701347be, /// 0x778
			 0xea003364, /// 0x77c
			 0x387b2716, /// 0x780
			 0x8a532241, /// 0x784
			 0xc4f834af, /// 0x788
			 0xa1f56023, /// 0x78c
			 0xda178d08, /// 0x790
			 0x9c7d1a07, /// 0x794
			 0x4add9b3a, /// 0x798
			 0x3094cc00, /// 0x79c
			 0xc36c060d, /// 0x7a0
			 0x6d94eae2, /// 0x7a4
			 0x6eb92030, /// 0x7a8
			 0xaa922d95, /// 0x7ac
			 0x14334066, /// 0x7b0
			 0x4afc5f45, /// 0x7b4
			 0x45ac1512, /// 0x7b8
			 0x7afa967d, /// 0x7bc
			 0x6ac01474, /// 0x7c0
			 0x011f5744, /// 0x7c4
			 0xd6868aef, /// 0x7c8
			 0x299b8a0e, /// 0x7cc
			 0xa3356286, /// 0x7d0
			 0x147b854f, /// 0x7d4
			 0xc020b8a1, /// 0x7d8
			 0x4a7d7605, /// 0x7dc
			 0xbbddd023, /// 0x7e0
			 0x9331a1ab, /// 0x7e4
			 0xb0572b26, /// 0x7e8
			 0xf91d4f7a, /// 0x7ec
			 0xd16cfd08, /// 0x7f0
			 0xac892a5e, /// 0x7f4
			 0x12acf1a3, /// 0x7f8
			 0x8e1d692c, /// 0x7fc
			 0xdaf3a48f, /// 0x800
			 0x9a9254a9, /// 0x804
			 0xde965389, /// 0x808
			 0xf4db097c, /// 0x80c
			 0xcdba603d, /// 0x810
			 0x4c47f569, /// 0x814
			 0x683e2076, /// 0x818
			 0xfc0b7788, /// 0x81c
			 0x90fba142, /// 0x820
			 0x1b820431, /// 0x824
			 0xa2f17fa6, /// 0x828
			 0xa0ad693c, /// 0x82c
			 0xfc3b9199, /// 0x830
			 0x2514c15c, /// 0x834
			 0x26835c88, /// 0x838
			 0x85f3b38a, /// 0x83c
			 0x475ea176, /// 0x840
			 0x1d784080, /// 0x844
			 0xb2116de4, /// 0x848
			 0x2acfdc5c, /// 0x84c
			 0x861ea868, /// 0x850
			 0xeb88a750, /// 0x854
			 0x136a6b7a, /// 0x858
			 0x3fd2c47a, /// 0x85c
			 0xe652149b, /// 0x860
			 0x0bc414da, /// 0x864
			 0xf4da4a37, /// 0x868
			 0x112e03ff, /// 0x86c
			 0x426754b8, /// 0x870
			 0x6c6ce52d, /// 0x874
			 0xc62f57ca, /// 0x878
			 0xa642f59e, /// 0x87c
			 0x01adeafd, /// 0x880
			 0xd2251eb8, /// 0x884
			 0xe90086cb, /// 0x888
			 0x5db41cc5, /// 0x88c
			 0xd15a4ac4, /// 0x890
			 0x9cac222a, /// 0x894
			 0x2a7cba84, /// 0x898
			 0xf8c54c80, /// 0x89c
			 0x07436bcc, /// 0x8a0
			 0xcb2501c7, /// 0x8a4
			 0x3b6f2bb5, /// 0x8a8
			 0x4af0f19a, /// 0x8ac
			 0x824737ae, /// 0x8b0
			 0xb86b1331, /// 0x8b4
			 0xee322dc6, /// 0x8b8
			 0x1b28d34c, /// 0x8bc
			 0x4d9cbdf2, /// 0x8c0
			 0x9b0b2c97, /// 0x8c4
			 0xed946647, /// 0x8c8
			 0x2d8ac716, /// 0x8cc
			 0xaeeaeb09, /// 0x8d0
			 0x4ec29164, /// 0x8d4
			 0x1839c472, /// 0x8d8
			 0xeda132ed, /// 0x8dc
			 0x1cc31c63, /// 0x8e0
			 0x62534639, /// 0x8e4
			 0x03f3ce87, /// 0x8e8
			 0xddad66ad, /// 0x8ec
			 0xfff52f60, /// 0x8f0
			 0xe84a879e, /// 0x8f4
			 0x4b5a7e61, /// 0x8f8
			 0xd353855b, /// 0x8fc
			 0x206da6ac, /// 0x900
			 0x75b5d86b, /// 0x904
			 0x6d720cc7, /// 0x908
			 0x81e1df59, /// 0x90c
			 0x42ad9b89, /// 0x910
			 0x0fb8de76, /// 0x914
			 0x9f9dd7e6, /// 0x918
			 0x805342d2, /// 0x91c
			 0xd647a3e0, /// 0x920
			 0xa1c413af, /// 0x924
			 0xe32e9df9, /// 0x928
			 0x9b941703, /// 0x92c
			 0x297981f2, /// 0x930
			 0x8801d0e9, /// 0x934
			 0xe86c3b4f, /// 0x938
			 0x62966f91, /// 0x93c
			 0x003ed826, /// 0x940
			 0x2cc7ce7c, /// 0x944
			 0x23328de4, /// 0x948
			 0xd7a4b54c, /// 0x94c
			 0xf1a66dee, /// 0x950
			 0x6530850c, /// 0x954
			 0x78e8edab, /// 0x958
			 0xf1951e72, /// 0x95c
			 0x612ce2f2, /// 0x960
			 0xca458b7d, /// 0x964
			 0x107e4b62, /// 0x968
			 0xf790ab8f, /// 0x96c
			 0x9b1342de, /// 0x970
			 0x854cd13d, /// 0x974
			 0x5ef35129, /// 0x978
			 0x3ae954c1, /// 0x97c
			 0x2fd33a61, /// 0x980
			 0x9a5c86b5, /// 0x984
			 0x919464f3, /// 0x988
			 0x2e35d829, /// 0x98c
			 0x796c7c26, /// 0x990
			 0xf85bc685, /// 0x994
			 0xc7da8a0f, /// 0x998
			 0x65364003, /// 0x99c
			 0x99b8099b, /// 0x9a0
			 0x6b5a64f7, /// 0x9a4
			 0x4eb34a2c, /// 0x9a8
			 0xfd136a46, /// 0x9ac
			 0x33cf25c1, /// 0x9b0
			 0x65f9523a, /// 0x9b4
			 0xad4404d9, /// 0x9b8
			 0x69c1aba7, /// 0x9bc
			 0x7304536c, /// 0x9c0
			 0x4c783176, /// 0x9c4
			 0x34713674, /// 0x9c8
			 0xbc064cd5, /// 0x9cc
			 0xfe3e92c4, /// 0x9d0
			 0x9d9e8f6b, /// 0x9d4
			 0x0b2b6e32, /// 0x9d8
			 0xb4451d29, /// 0x9dc
			 0x7b9203df, /// 0x9e0
			 0xc891ec4c, /// 0x9e4
			 0x372d940c, /// 0x9e8
			 0x1c03705c, /// 0x9ec
			 0x9042f0e6, /// 0x9f0
			 0xdcd6bfed, /// 0x9f4
			 0x352c3743, /// 0x9f8
			 0x5d412eb0, /// 0x9fc
			 0x82d456f1, /// 0xa00
			 0x33f8d533, /// 0xa04
			 0x0b057b0f, /// 0xa08
			 0x890e1606, /// 0xa0c
			 0x04836ad1, /// 0xa10
			 0x782873b9, /// 0xa14
			 0x864e09f6, /// 0xa18
			 0x90cbd9d0, /// 0xa1c
			 0x2c5abd07, /// 0xa20
			 0x62e81fb6, /// 0xa24
			 0x12607493, /// 0xa28
			 0x3d9d6c05, /// 0xa2c
			 0xf52f8e00, /// 0xa30
			 0xc3286f9d, /// 0xa34
			 0x103ee954, /// 0xa38
			 0x1bd8f7da, /// 0xa3c
			 0xbfb4cad3, /// 0xa40
			 0x6432b2d9, /// 0xa44
			 0xbccc84d9, /// 0xa48
			 0x1f4bb91c, /// 0xa4c
			 0x9785c876, /// 0xa50
			 0x7a88e83f, /// 0xa54
			 0x69266906, /// 0xa58
			 0xb3feb02b, /// 0xa5c
			 0x825b3756, /// 0xa60
			 0x48700778, /// 0xa64
			 0x7d1d2b4c, /// 0xa68
			 0xf5b15327, /// 0xa6c
			 0xbd6c5f98, /// 0xa70
			 0x3089e411, /// 0xa74
			 0x1234702b, /// 0xa78
			 0x8b43049c, /// 0xa7c
			 0xb209713d, /// 0xa80
			 0xff82d3e3, /// 0xa84
			 0xfb460ec2, /// 0xa88
			 0x4b3477c1, /// 0xa8c
			 0xd61eddbf, /// 0xa90
			 0x079c946f, /// 0xa94
			 0x6e682649, /// 0xa98
			 0x51c8c158, /// 0xa9c
			 0x0f483d12, /// 0xaa0
			 0x4bc8ef7b, /// 0xaa4
			 0x31496c81, /// 0xaa8
			 0x70bec501, /// 0xaac
			 0x9d51b003, /// 0xab0
			 0x89084908, /// 0xab4
			 0xe64cc642, /// 0xab8
			 0x0a56dcfe, /// 0xabc
			 0x4b1e866a, /// 0xac0
			 0x24bfaceb, /// 0xac4
			 0x3c5d4450, /// 0xac8
			 0x8c97f8dc, /// 0xacc
			 0x67981b67, /// 0xad0
			 0x79f83638, /// 0xad4
			 0xd0cdf449, /// 0xad8
			 0xa036c23f, /// 0xadc
			 0x2b66b015, /// 0xae0
			 0x0339cbef, /// 0xae4
			 0x84463678, /// 0xae8
			 0xe79a926e, /// 0xaec
			 0x20da509b, /// 0xaf0
			 0xf3f2a4b7, /// 0xaf4
			 0x7ddb1b5d, /// 0xaf8
			 0x5910d0b5, /// 0xafc
			 0xf8dd2128, /// 0xb00
			 0xa7340d51, /// 0xb04
			 0x69f89c16, /// 0xb08
			 0x18f32d69, /// 0xb0c
			 0x29db47e9, /// 0xb10
			 0x64951857, /// 0xb14
			 0x55688c5c, /// 0xb18
			 0x6d333332, /// 0xb1c
			 0x7538dd4f, /// 0xb20
			 0x8a693483, /// 0xb24
			 0xf31327b2, /// 0xb28
			 0x46675b78, /// 0xb2c
			 0xe4a9c381, /// 0xb30
			 0xc7b7f435, /// 0xb34
			 0xb183c804, /// 0xb38
			 0xc0fd38aa, /// 0xb3c
			 0x6cd80500, /// 0xb40
			 0x5d45deb3, /// 0xb44
			 0x2c070497, /// 0xb48
			 0xce72f24f, /// 0xb4c
			 0x2015a9bd, /// 0xb50
			 0x186ea06e, /// 0xb54
			 0xc4cc1d14, /// 0xb58
			 0x708df718, /// 0xb5c
			 0x62e59912, /// 0xb60
			 0xd7d6e941, /// 0xb64
			 0xff1f1ccd, /// 0xb68
			 0xe88e65c8, /// 0xb6c
			 0x1f18664c, /// 0xb70
			 0x6f759bce, /// 0xb74
			 0xd192e999, /// 0xb78
			 0x84be7e1f, /// 0xb7c
			 0x9c9638f7, /// 0xb80
			 0x83492b16, /// 0xb84
			 0xa2cedfe1, /// 0xb88
			 0xd75ace51, /// 0xb8c
			 0x44be9616, /// 0xb90
			 0xd1c663f0, /// 0xb94
			 0x7152c376, /// 0xb98
			 0x0c8f98af, /// 0xb9c
			 0x90a9a6be, /// 0xba0
			 0x388eb4ff, /// 0xba4
			 0x202b4641, /// 0xba8
			 0xb2599618, /// 0xbac
			 0xe159a7c9, /// 0xbb0
			 0xd7180a8f, /// 0xbb4
			 0xd834f3bd, /// 0xbb8
			 0x4e962a75, /// 0xbbc
			 0x67f49ca2, /// 0xbc0
			 0xb7ddc2b8, /// 0xbc4
			 0x4d4a925d, /// 0xbc8
			 0x8d017cb2, /// 0xbcc
			 0x79b18c08, /// 0xbd0
			 0x8b9c8a08, /// 0xbd4
			 0xbaa3e381, /// 0xbd8
			 0x48a97d16, /// 0xbdc
			 0xe5ce02c4, /// 0xbe0
			 0x83c643ba, /// 0xbe4
			 0x3f6d9ff9, /// 0xbe8
			 0xaf4a4762, /// 0xbec
			 0x92fb7fd5, /// 0xbf0
			 0x5282c9c3, /// 0xbf4
			 0x00efaa6f, /// 0xbf8
			 0xc700713e, /// 0xbfc
			 0x9c3aa66e, /// 0xc00
			 0x5092bb71, /// 0xc04
			 0x3ff163c2, /// 0xc08
			 0x1e209af5, /// 0xc0c
			 0x2b059c7a, /// 0xc10
			 0x8cc3cc33, /// 0xc14
			 0xe8bbcc99, /// 0xc18
			 0x60f13e3a, /// 0xc1c
			 0x709006d9, /// 0xc20
			 0x1f04b5df, /// 0xc24
			 0xf9014cd2, /// 0xc28
			 0x6340b833, /// 0xc2c
			 0xcf2e502c, /// 0xc30
			 0x2651f376, /// 0xc34
			 0x3700c349, /// 0xc38
			 0xa834e5fd, /// 0xc3c
			 0xa15deaf6, /// 0xc40
			 0xd13ca19e, /// 0xc44
			 0x043c3564, /// 0xc48
			 0xe2bd52a7, /// 0xc4c
			 0xa08b8363, /// 0xc50
			 0x58082897, /// 0xc54
			 0x2f865b91, /// 0xc58
			 0x624f4f4e, /// 0xc5c
			 0x5a43f718, /// 0xc60
			 0x2ba93ecd, /// 0xc64
			 0xa4a8cfdf, /// 0xc68
			 0x8c8638cb, /// 0xc6c
			 0x77dac845, /// 0xc70
			 0x8355e524, /// 0xc74
			 0x1d671ad8, /// 0xc78
			 0xcc322a4e, /// 0xc7c
			 0x741cc339, /// 0xc80
			 0x3f246ece, /// 0xc84
			 0xe0ac9716, /// 0xc88
			 0xb00dca6e, /// 0xc8c
			 0x989ea9c7, /// 0xc90
			 0xe551213d, /// 0xc94
			 0xa27ddea0, /// 0xc98
			 0xc51d92fe, /// 0xc9c
			 0x76f58e3e, /// 0xca0
			 0x9e7a2d8d, /// 0xca4
			 0x751fcd14, /// 0xca8
			 0xcabbc60a, /// 0xcac
			 0x57c00368, /// 0xcb0
			 0x6dd47d50, /// 0xcb4
			 0x4e7abb5e, /// 0xcb8
			 0xeed27686, /// 0xcbc
			 0x3ba8652f, /// 0xcc0
			 0xb7565e82, /// 0xcc4
			 0xc0c8be2c, /// 0xcc8
			 0xf9252090, /// 0xccc
			 0xf4a57e9a, /// 0xcd0
			 0xd821b86a, /// 0xcd4
			 0x1932efab, /// 0xcd8
			 0x2d956b62, /// 0xcdc
			 0x68e8172a, /// 0xce0
			 0x68de8711, /// 0xce4
			 0xdf350d3c, /// 0xce8
			 0x096b08a0, /// 0xcec
			 0xd1f7cbc0, /// 0xcf0
			 0x9b7516c7, /// 0xcf4
			 0xc5fbd15f, /// 0xcf8
			 0x1ce530f6, /// 0xcfc
			 0x9d30a739, /// 0xd00
			 0xb3e21a7b, /// 0xd04
			 0xc7fdc338, /// 0xd08
			 0xaef71b9b, /// 0xd0c
			 0x17660014, /// 0xd10
			 0x4eb7810c, /// 0xd14
			 0xc8ff72f0, /// 0xd18
			 0x607631ff, /// 0xd1c
			 0x71137cb3, /// 0xd20
			 0x92cec261, /// 0xd24
			 0x8e705af3, /// 0xd28
			 0x196d6a3d, /// 0xd2c
			 0x1e5da9a0, /// 0xd30
			 0x00450edc, /// 0xd34
			 0x0000e6fd, /// 0xd38
			 0xe7e97ab4, /// 0xd3c
			 0x6259a8dc, /// 0xd40
			 0x129727c9, /// 0xd44
			 0x2562d2c0, /// 0xd48
			 0x496fc8bd, /// 0xd4c
			 0x88923389, /// 0xd50
			 0x352f0bb4, /// 0xd54
			 0x06f5938a, /// 0xd58
			 0x522bcc05, /// 0xd5c
			 0xe45b1fd3, /// 0xd60
			 0x39ec187c, /// 0xd64
			 0x1207e8f7, /// 0xd68
			 0x0c4ac18f, /// 0xd6c
			 0x463d1eb3, /// 0xd70
			 0x21a0c7d0, /// 0xd74
			 0xf979d752, /// 0xd78
			 0x93e595a0, /// 0xd7c
			 0x0b66b90c, /// 0xd80
			 0x86493482, /// 0xd84
			 0x5f8d7537, /// 0xd88
			 0x1ddbfd46, /// 0xd8c
			 0xdcda27d9, /// 0xd90
			 0xbe806242, /// 0xd94
			 0x2932d8c3, /// 0xd98
			 0xe8f7d72e, /// 0xd9c
			 0x7f03268c, /// 0xda0
			 0x3706b5ff, /// 0xda4
			 0x524b7eb1, /// 0xda8
			 0x30a770a3, /// 0xdac
			 0x161b83ca, /// 0xdb0
			 0x7e765c66, /// 0xdb4
			 0x7012dd36, /// 0xdb8
			 0xf6c437d0, /// 0xdbc
			 0x7055dbd9, /// 0xdc0
			 0xcb91511f, /// 0xdc4
			 0xf0c6ffac, /// 0xdc8
			 0x51b12388, /// 0xdcc
			 0xd15fcb61, /// 0xdd0
			 0x8a716e8e, /// 0xdd4
			 0x5d5c5a0d, /// 0xdd8
			 0x2479ed89, /// 0xddc
			 0x2f24284d, /// 0xde0
			 0x04d67e3f, /// 0xde4
			 0x244c5cfb, /// 0xde8
			 0x35cecfdf, /// 0xdec
			 0xc2093581, /// 0xdf0
			 0x0fa434a9, /// 0xdf4
			 0x16cff6d7, /// 0xdf8
			 0xa859684c, /// 0xdfc
			 0xf22255b0, /// 0xe00
			 0x55d7681c, /// 0xe04
			 0x0ca11fa0, /// 0xe08
			 0x152e8376, /// 0xe0c
			 0xf5eb98c2, /// 0xe10
			 0xc22e0939, /// 0xe14
			 0xd7284ba2, /// 0xe18
			 0xdce46ca4, /// 0xe1c
			 0x9b610f7b, /// 0xe20
			 0x94b3568d, /// 0xe24
			 0x05f099f2, /// 0xe28
			 0x45f81115, /// 0xe2c
			 0xfab3c8f0, /// 0xe30
			 0x40145b20, /// 0xe34
			 0xe7864d4d, /// 0xe38
			 0x2c963966, /// 0xe3c
			 0x0bf44bd2, /// 0xe40
			 0x0e40cd81, /// 0xe44
			 0xc6869b61, /// 0xe48
			 0xea0a4880, /// 0xe4c
			 0x796c0710, /// 0xe50
			 0x9bb8a35d, /// 0xe54
			 0x3a737f6c, /// 0xe58
			 0x4c386ffc, /// 0xe5c
			 0x46c2f2a6, /// 0xe60
			 0x2eb2d23e, /// 0xe64
			 0x42dbce37, /// 0xe68
			 0x18a92846, /// 0xe6c
			 0x29f0d35a, /// 0xe70
			 0xf138082e, /// 0xe74
			 0xc096d312, /// 0xe78
			 0xbbb1be09, /// 0xe7c
			 0x58cc8238, /// 0xe80
			 0x7db77d95, /// 0xe84
			 0x7dd0ac3f, /// 0xe88
			 0xeb47e2e1, /// 0xe8c
			 0x5770064f, /// 0xe90
			 0x63d42e39, /// 0xe94
			 0x7fda20e5, /// 0xe98
			 0xce941db1, /// 0xe9c
			 0xf8aa5f56, /// 0xea0
			 0x532a0df7, /// 0xea4
			 0x5c917bb5, /// 0xea8
			 0xbb17c950, /// 0xeac
			 0x01d3dc98, /// 0xeb0
			 0xb198b831, /// 0xeb4
			 0x49d8fdf3, /// 0xeb8
			 0x5f8278f8, /// 0xebc
			 0x74d14f85, /// 0xec0
			 0xad17a821, /// 0xec4
			 0x6c18cf33, /// 0xec8
			 0x450f9e27, /// 0xecc
			 0xe3e7dd8e, /// 0xed0
			 0xf9bde807, /// 0xed4
			 0x85217dbe, /// 0xed8
			 0x9d481083, /// 0xedc
			 0x90b612da, /// 0xee0
			 0xc897ac83, /// 0xee4
			 0x05cffee4, /// 0xee8
			 0xdc443ce2, /// 0xeec
			 0xaf7f971d, /// 0xef0
			 0x6a88db16, /// 0xef4
			 0xa58a35e5, /// 0xef8
			 0xa1c9bc69, /// 0xefc
			 0xd5cc43c5, /// 0xf00
			 0x53e598ae, /// 0xf04
			 0x0582a053, /// 0xf08
			 0xa4d0fd4f, /// 0xf0c
			 0x74fdfc43, /// 0xf10
			 0x1f215b72, /// 0xf14
			 0x626ceedb, /// 0xf18
			 0x6628420b, /// 0xf1c
			 0xadb83aa3, /// 0xf20
			 0x9ca6fe31, /// 0xf24
			 0xd29c5b6e, /// 0xf28
			 0x9a89a06d, /// 0xf2c
			 0xb242c4dd, /// 0xf30
			 0x150399f9, /// 0xf34
			 0xab3a9efd, /// 0xf38
			 0x3dda2ec2, /// 0xf3c
			 0xa558b3bc, /// 0xf40
			 0x6ba4da58, /// 0xf44
			 0x8cfbc352, /// 0xf48
			 0xdb2eda80, /// 0xf4c
			 0x2b7fa085, /// 0xf50
			 0x45bd0836, /// 0xf54
			 0x4ffcd6ad, /// 0xf58
			 0x9e085283, /// 0xf5c
			 0xb537a27d, /// 0xf60
			 0xf77e079e, /// 0xf64
			 0x2c5b550a, /// 0xf68
			 0x529000a9, /// 0xf6c
			 0x05fc96d2, /// 0xf70
			 0x8207b528, /// 0xf74
			 0xdd84ac44, /// 0xf78
			 0x46abfc67, /// 0xf7c
			 0x913a4406, /// 0xf80
			 0x8bb3c5a4, /// 0xf84
			 0xbe4c84f6, /// 0xf88
			 0xa290a59a, /// 0xf8c
			 0xe2e2f9c2, /// 0xf90
			 0x8b105dc5, /// 0xf94
			 0xec72c64b, /// 0xf98
			 0x8d1ce432, /// 0xf9c
			 0x03ef636c, /// 0xfa0
			 0x81cbf5bc, /// 0xfa4
			 0xde14f752, /// 0xfa8
			 0xf1e2e98d, /// 0xfac
			 0x5d1ec1a8, /// 0xfb0
			 0xc12f4d32, /// 0xfb4
			 0x5fc1ec6f, /// 0xfb8
			 0xe37cb510, /// 0xfbc
			 0xf622e6f3, /// 0xfc0
			 0x2cfb4fc8, /// 0xfc4
			 0x520e7c37, /// 0xfc8
			 0xe07f72f7, /// 0xfcc
			 0x36ef2c30, /// 0xfd0
			 0x9ff2dd45, /// 0xfd4
			 0xb5aa1de2, /// 0xfd8
			 0x0756399b, /// 0xfdc
			 0xd72e0f9b, /// 0xfe0
			 0x5bc7b934, /// 0xfe4
			 0xcaf52f91, /// 0xfe8
			 0xca71c22c, /// 0xfec
			 0x6cd4fb2c, /// 0xff0
			 0xeaa6b2dc, /// 0xff4
			 0x54424fe2, /// 0xff8
			 0x056f4d71 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x633ef35b, /// 0x0
			 0xe8a1490d, /// 0x4
			 0xc186ab0d, /// 0x8
			 0x45a1ae1c, /// 0xc
			 0x066cdc55, /// 0x10
			 0xe498e84c, /// 0x14
			 0xd47b9a14, /// 0x18
			 0x41015ff3, /// 0x1c
			 0xe9ca6c41, /// 0x20
			 0x22c1d3e3, /// 0x24
			 0x9828adc4, /// 0x28
			 0x5a7ca1a7, /// 0x2c
			 0x90a8d97a, /// 0x30
			 0xfe308878, /// 0x34
			 0x0e9b055a, /// 0x38
			 0x6bef1cd1, /// 0x3c
			 0x3a4d3a35, /// 0x40
			 0x9f370ad2, /// 0x44
			 0x234e807d, /// 0x48
			 0x6eec0048, /// 0x4c
			 0x3b1b434f, /// 0x50
			 0xfe6aca96, /// 0x54
			 0x014637dc, /// 0x58
			 0xdc8cc3b4, /// 0x5c
			 0xa3d9f64c, /// 0x60
			 0x98132f77, /// 0x64
			 0xb0ae9ceb, /// 0x68
			 0x9fc85fe3, /// 0x6c
			 0xe06a7b9b, /// 0x70
			 0x5646aec0, /// 0x74
			 0xdcfbcd09, /// 0x78
			 0x0af09068, /// 0x7c
			 0x7ffd43a2, /// 0x80
			 0x57e981e5, /// 0x84
			 0x850b4dc7, /// 0x88
			 0x4c9d000d, /// 0x8c
			 0xdf360951, /// 0x90
			 0x9efa92ce, /// 0x94
			 0x866d59ee, /// 0x98
			 0x97ea015f, /// 0x9c
			 0x9e061fab, /// 0xa0
			 0x9550c062, /// 0xa4
			 0x8b2f82a7, /// 0xa8
			 0x4448cad8, /// 0xac
			 0x17344f89, /// 0xb0
			 0x15d36b95, /// 0xb4
			 0xf8437628, /// 0xb8
			 0x14d08600, /// 0xbc
			 0xc875515f, /// 0xc0
			 0x742addc5, /// 0xc4
			 0xbc55d6e0, /// 0xc8
			 0x03ddf30a, /// 0xcc
			 0xe0a41749, /// 0xd0
			 0x8d5d0422, /// 0xd4
			 0x09145e07, /// 0xd8
			 0x082f17b3, /// 0xdc
			 0x379829ef, /// 0xe0
			 0x9fb0bb74, /// 0xe4
			 0x90779de0, /// 0xe8
			 0x6da84de2, /// 0xec
			 0x2c1de5d2, /// 0xf0
			 0x74fdc767, /// 0xf4
			 0x1f94a382, /// 0xf8
			 0xf942eee6, /// 0xfc
			 0x6bfe6f3d, /// 0x100
			 0x17e52f8e, /// 0x104
			 0xaa4b45d1, /// 0x108
			 0x5af2bbc0, /// 0x10c
			 0xb1502d5b, /// 0x110
			 0x184dd8e7, /// 0x114
			 0xabc3c892, /// 0x118
			 0xf574d1ca, /// 0x11c
			 0xfe5aeb86, /// 0x120
			 0xc61ae343, /// 0x124
			 0x6ebe10b2, /// 0x128
			 0x2f7076b2, /// 0x12c
			 0x77f38f0a, /// 0x130
			 0x1c58379f, /// 0x134
			 0x827f7938, /// 0x138
			 0xf54c82ce, /// 0x13c
			 0x8e7a953c, /// 0x140
			 0x3c9c8e8b, /// 0x144
			 0x823cfc94, /// 0x148
			 0x4c4c5ec2, /// 0x14c
			 0xdd30f234, /// 0x150
			 0x84c9eb71, /// 0x154
			 0x6f5c963c, /// 0x158
			 0xaef6e123, /// 0x15c
			 0xb0015d52, /// 0x160
			 0x0c22f614, /// 0x164
			 0xda79cc83, /// 0x168
			 0x7284f47c, /// 0x16c
			 0x6c1268b4, /// 0x170
			 0x187a3949, /// 0x174
			 0x9698c89f, /// 0x178
			 0x02dfecb3, /// 0x17c
			 0x6fd7fab2, /// 0x180
			 0x140a45a1, /// 0x184
			 0x60348210, /// 0x188
			 0x2cc548ed, /// 0x18c
			 0x60940eac, /// 0x190
			 0x0c649b3b, /// 0x194
			 0x18e4dade, /// 0x198
			 0xdb40c8f4, /// 0x19c
			 0x1f04a4eb, /// 0x1a0
			 0x98f4016e, /// 0x1a4
			 0xbd23c733, /// 0x1a8
			 0xb5f7baa7, /// 0x1ac
			 0x22f30bd7, /// 0x1b0
			 0x184c216a, /// 0x1b4
			 0x1500021d, /// 0x1b8
			 0x35b9f809, /// 0x1bc
			 0xe2dcc91d, /// 0x1c0
			 0x34bd3c65, /// 0x1c4
			 0xe625196d, /// 0x1c8
			 0x4e344186, /// 0x1cc
			 0x05b5d964, /// 0x1d0
			 0xc77e96de, /// 0x1d4
			 0x60f40edc, /// 0x1d8
			 0x5603ac5a, /// 0x1dc
			 0xaf45056a, /// 0x1e0
			 0xc4ff9f0f, /// 0x1e4
			 0x5e90b215, /// 0x1e8
			 0x84ec7352, /// 0x1ec
			 0xec161f53, /// 0x1f0
			 0xfab9e70b, /// 0x1f4
			 0xb544df4d, /// 0x1f8
			 0xadc27b45, /// 0x1fc
			 0xad2eb9d2, /// 0x200
			 0x02886bee, /// 0x204
			 0xc5dda1f1, /// 0x208
			 0x02800862, /// 0x20c
			 0x292ad3a7, /// 0x210
			 0xb041b4f4, /// 0x214
			 0x2dbd89a7, /// 0x218
			 0x2485899d, /// 0x21c
			 0x6128fb81, /// 0x220
			 0x78883afd, /// 0x224
			 0x5ed0974e, /// 0x228
			 0x878ad174, /// 0x22c
			 0xf275167d, /// 0x230
			 0xd36acdb5, /// 0x234
			 0x51145a06, /// 0x238
			 0x5a0c610c, /// 0x23c
			 0x9819ccc2, /// 0x240
			 0xda745ecb, /// 0x244
			 0xf4fedefb, /// 0x248
			 0x91cd4f18, /// 0x24c
			 0x936ded19, /// 0x250
			 0x15034ab2, /// 0x254
			 0xd044ef9e, /// 0x258
			 0x3796bb8a, /// 0x25c
			 0x0394b108, /// 0x260
			 0xada71681, /// 0x264
			 0x3ee6a7c7, /// 0x268
			 0x8a6b1d5c, /// 0x26c
			 0xbf1b5c19, /// 0x270
			 0x6c511dcc, /// 0x274
			 0x524d332f, /// 0x278
			 0x63dd7769, /// 0x27c
			 0x5a669197, /// 0x280
			 0x9ef3647d, /// 0x284
			 0xd159bbcb, /// 0x288
			 0x6444e2a7, /// 0x28c
			 0xe9695c5e, /// 0x290
			 0x34c242d7, /// 0x294
			 0xea80afa1, /// 0x298
			 0x8e07faa7, /// 0x29c
			 0x0fbf2873, /// 0x2a0
			 0xf6636209, /// 0x2a4
			 0x906fd38b, /// 0x2a8
			 0xdad30ec3, /// 0x2ac
			 0x46a6ce5e, /// 0x2b0
			 0xe197bea2, /// 0x2b4
			 0x7a3f585a, /// 0x2b8
			 0x738092df, /// 0x2bc
			 0xf4fcecd2, /// 0x2c0
			 0x36283614, /// 0x2c4
			 0x01c9a056, /// 0x2c8
			 0x6eac0408, /// 0x2cc
			 0x9f114c10, /// 0x2d0
			 0x7facdb7a, /// 0x2d4
			 0xce8e51e9, /// 0x2d8
			 0x416bbca6, /// 0x2dc
			 0xe8978f67, /// 0x2e0
			 0x6b0f2476, /// 0x2e4
			 0xbd71f87b, /// 0x2e8
			 0x8c9171e3, /// 0x2ec
			 0xe80b82f0, /// 0x2f0
			 0x30600127, /// 0x2f4
			 0xd96b6bab, /// 0x2f8
			 0x8db8553f, /// 0x2fc
			 0x219facaf, /// 0x300
			 0x38a74978, /// 0x304
			 0x015665b2, /// 0x308
			 0x939b2e47, /// 0x30c
			 0x0702ec6b, /// 0x310
			 0xca7a99dd, /// 0x314
			 0xee52b2b6, /// 0x318
			 0x3585f0de, /// 0x31c
			 0xfe3ffe19, /// 0x320
			 0x05947e00, /// 0x324
			 0x88836fa0, /// 0x328
			 0xb46cbc6f, /// 0x32c
			 0x3f6a6df1, /// 0x330
			 0x4a7061f4, /// 0x334
			 0x78e8b06f, /// 0x338
			 0x445a17ef, /// 0x33c
			 0x7e457758, /// 0x340
			 0x47b69a75, /// 0x344
			 0x46aa7473, /// 0x348
			 0xba5b8bdd, /// 0x34c
			 0x665367b0, /// 0x350
			 0xf64ccf57, /// 0x354
			 0x549716b9, /// 0x358
			 0x239339b6, /// 0x35c
			 0x420c02c5, /// 0x360
			 0x6eee2c56, /// 0x364
			 0x7767bac6, /// 0x368
			 0xf25dedf8, /// 0x36c
			 0x814442c1, /// 0x370
			 0x54923452, /// 0x374
			 0xb8e2698f, /// 0x378
			 0x810eff93, /// 0x37c
			 0xcbc4c784, /// 0x380
			 0x359fc512, /// 0x384
			 0x2d3c4c3a, /// 0x388
			 0x4c8406e7, /// 0x38c
			 0x441e9d16, /// 0x390
			 0x14632fe2, /// 0x394
			 0x239ce322, /// 0x398
			 0x32338784, /// 0x39c
			 0xbbb0dbdd, /// 0x3a0
			 0xe327da0c, /// 0x3a4
			 0xd8b8e745, /// 0x3a8
			 0x0d9fbcf0, /// 0x3ac
			 0x3b6f952c, /// 0x3b0
			 0xaaf22e0b, /// 0x3b4
			 0x3c3ae53f, /// 0x3b8
			 0x4230d4ce, /// 0x3bc
			 0x140300e6, /// 0x3c0
			 0x2bd0585f, /// 0x3c4
			 0xfcd13951, /// 0x3c8
			 0x76145a53, /// 0x3cc
			 0x73412938, /// 0x3d0
			 0x636b36e3, /// 0x3d4
			 0x19a3a922, /// 0x3d8
			 0xe725baee, /// 0x3dc
			 0xfeee0e2c, /// 0x3e0
			 0x9cac0207, /// 0x3e4
			 0x0cce8b18, /// 0x3e8
			 0xc171bdb0, /// 0x3ec
			 0x5728898e, /// 0x3f0
			 0x15ab1cb8, /// 0x3f4
			 0xc425b4ec, /// 0x3f8
			 0xd9c036c9, /// 0x3fc
			 0xfb3bb6ad, /// 0x400
			 0x0fca673e, /// 0x404
			 0xbd8aece1, /// 0x408
			 0x1be9fe9c, /// 0x40c
			 0xbdefe75a, /// 0x410
			 0x14f836e6, /// 0x414
			 0x4e2569a0, /// 0x418
			 0x97064662, /// 0x41c
			 0x987695d9, /// 0x420
			 0x8f0e3331, /// 0x424
			 0xc5075b57, /// 0x428
			 0x8ab8dffc, /// 0x42c
			 0x8919138f, /// 0x430
			 0x5d10637e, /// 0x434
			 0x5460665b, /// 0x438
			 0x0a4dea77, /// 0x43c
			 0x335e3c46, /// 0x440
			 0xfd7aec4d, /// 0x444
			 0xd92316bb, /// 0x448
			 0x1a32260b, /// 0x44c
			 0xabd75673, /// 0x450
			 0x581f0c7b, /// 0x454
			 0x55a32730, /// 0x458
			 0x37a76f98, /// 0x45c
			 0x02d049e5, /// 0x460
			 0x392cceea, /// 0x464
			 0x0f6d4a02, /// 0x468
			 0x9de008f2, /// 0x46c
			 0x063035a4, /// 0x470
			 0xe7d4c10e, /// 0x474
			 0x671ce0df, /// 0x478
			 0x4fc8d757, /// 0x47c
			 0x03efec28, /// 0x480
			 0x379d93fb, /// 0x484
			 0x061aca0b, /// 0x488
			 0xf9f70c12, /// 0x48c
			 0x59eeb837, /// 0x490
			 0x0fe7ed38, /// 0x494
			 0x4b657c00, /// 0x498
			 0x9f9c8d11, /// 0x49c
			 0xa591e7bf, /// 0x4a0
			 0xfca6bdf7, /// 0x4a4
			 0xa409f007, /// 0x4a8
			 0x8a4b8b1d, /// 0x4ac
			 0x8cd52fa6, /// 0x4b0
			 0x45da7015, /// 0x4b4
			 0xa3d024fc, /// 0x4b8
			 0x99a4a537, /// 0x4bc
			 0x0c9c38a1, /// 0x4c0
			 0xd12efa9b, /// 0x4c4
			 0x6c6cac42, /// 0x4c8
			 0xc8e7f30f, /// 0x4cc
			 0x8bce1fbb, /// 0x4d0
			 0xb72e31dc, /// 0x4d4
			 0x2797d619, /// 0x4d8
			 0x1b532579, /// 0x4dc
			 0x40ad808e, /// 0x4e0
			 0x0e319463, /// 0x4e4
			 0xacd84c32, /// 0x4e8
			 0x9d60574a, /// 0x4ec
			 0x155c7a42, /// 0x4f0
			 0x4022becb, /// 0x4f4
			 0x9ca0168b, /// 0x4f8
			 0x51a8906a, /// 0x4fc
			 0xbca21fbc, /// 0x500
			 0xa9507d69, /// 0x504
			 0xdc87efd9, /// 0x508
			 0x232da635, /// 0x50c
			 0x6cd91032, /// 0x510
			 0x0b23f824, /// 0x514
			 0x4bd41205, /// 0x518
			 0x94032beb, /// 0x51c
			 0x66614257, /// 0x520
			 0x1764867d, /// 0x524
			 0xc3bdb04d, /// 0x528
			 0xa092e91a, /// 0x52c
			 0x2e94353e, /// 0x530
			 0x5be2cadb, /// 0x534
			 0x93fdf856, /// 0x538
			 0x0f16ba03, /// 0x53c
			 0x81385e7c, /// 0x540
			 0xd1e89218, /// 0x544
			 0xa89315ed, /// 0x548
			 0x193f5b9d, /// 0x54c
			 0x3760e16f, /// 0x550
			 0x2a1e3134, /// 0x554
			 0x91c10527, /// 0x558
			 0xfe7315d2, /// 0x55c
			 0x709bdad4, /// 0x560
			 0x8bdee75c, /// 0x564
			 0x6bcb9303, /// 0x568
			 0xbcb038b3, /// 0x56c
			 0x0bc70f5a, /// 0x570
			 0x02591236, /// 0x574
			 0xf7ede3db, /// 0x578
			 0xa2d8e97b, /// 0x57c
			 0xe7d43127, /// 0x580
			 0xb70932a0, /// 0x584
			 0x8123dbc0, /// 0x588
			 0xa79495f0, /// 0x58c
			 0xf80051ff, /// 0x590
			 0x57ceff56, /// 0x594
			 0x7d23c86d, /// 0x598
			 0x538dcfad, /// 0x59c
			 0x27238d18, /// 0x5a0
			 0x5cc90714, /// 0x5a4
			 0x7466738f, /// 0x5a8
			 0xc99431a5, /// 0x5ac
			 0x8d52471b, /// 0x5b0
			 0x36b2c3bb, /// 0x5b4
			 0x5ba67be8, /// 0x5b8
			 0xbb9be80f, /// 0x5bc
			 0x5d51a1b4, /// 0x5c0
			 0xdfb24d9f, /// 0x5c4
			 0x2e55fb31, /// 0x5c8
			 0xf01a4a77, /// 0x5cc
			 0x3f8982ef, /// 0x5d0
			 0xffface5f, /// 0x5d4
			 0x572e27cf, /// 0x5d8
			 0x4988ab50, /// 0x5dc
			 0xbe63b59a, /// 0x5e0
			 0x2e6e174b, /// 0x5e4
			 0x3ac568f6, /// 0x5e8
			 0xbd844720, /// 0x5ec
			 0x82133e46, /// 0x5f0
			 0x7b8845a7, /// 0x5f4
			 0xed32e78b, /// 0x5f8
			 0x56b5e60f, /// 0x5fc
			 0x0e5b3af8, /// 0x600
			 0xf0df9c28, /// 0x604
			 0xe54d4ee2, /// 0x608
			 0xa08bb8ac, /// 0x60c
			 0x54cbf7d6, /// 0x610
			 0x98ad0a0f, /// 0x614
			 0x6a4802df, /// 0x618
			 0x1b9c1aca, /// 0x61c
			 0x62d22140, /// 0x620
			 0x0b091274, /// 0x624
			 0xb646341d, /// 0x628
			 0x4b7ea840, /// 0x62c
			 0x45883ae6, /// 0x630
			 0xd846271c, /// 0x634
			 0xe25cb53c, /// 0x638
			 0x3fd4cb70, /// 0x63c
			 0x6ba891cf, /// 0x640
			 0x07d26247, /// 0x644
			 0xf1e2b0ff, /// 0x648
			 0x636758a1, /// 0x64c
			 0x9c1a7119, /// 0x650
			 0xc0df34fb, /// 0x654
			 0x278f4ee6, /// 0x658
			 0xbef73653, /// 0x65c
			 0xfc4e2fc2, /// 0x660
			 0xbd00b515, /// 0x664
			 0x83cb3a56, /// 0x668
			 0xa9b19c02, /// 0x66c
			 0x7f51701e, /// 0x670
			 0x3992f79c, /// 0x674
			 0x70681d80, /// 0x678
			 0xf328601c, /// 0x67c
			 0xf7c78092, /// 0x680
			 0xeb195238, /// 0x684
			 0x09919465, /// 0x688
			 0x27530502, /// 0x68c
			 0x01ab018d, /// 0x690
			 0xf9c79a83, /// 0x694
			 0xf860379b, /// 0x698
			 0x8e8a2a75, /// 0x69c
			 0x4b4d3aba, /// 0x6a0
			 0xde38d10f, /// 0x6a4
			 0x3669f551, /// 0x6a8
			 0xffcbf2a8, /// 0x6ac
			 0x50333d5b, /// 0x6b0
			 0x012df162, /// 0x6b4
			 0x805c6c7c, /// 0x6b8
			 0xf4ec1f7f, /// 0x6bc
			 0xc6cbc433, /// 0x6c0
			 0x6cc3c589, /// 0x6c4
			 0xc7c3fa25, /// 0x6c8
			 0xb66a5a8d, /// 0x6cc
			 0xe545971d, /// 0x6d0
			 0x2998c5b9, /// 0x6d4
			 0xfbb6b705, /// 0x6d8
			 0x626d640c, /// 0x6dc
			 0x36c407f6, /// 0x6e0
			 0x6c504882, /// 0x6e4
			 0xae432845, /// 0x6e8
			 0x28b4ad0f, /// 0x6ec
			 0x28e871d0, /// 0x6f0
			 0xc9101b67, /// 0x6f4
			 0x5d58454a, /// 0x6f8
			 0xfbc297dd, /// 0x6fc
			 0x1be30ef7, /// 0x700
			 0x15c953ea, /// 0x704
			 0xfa972327, /// 0x708
			 0x89f29d27, /// 0x70c
			 0x4ff559af, /// 0x710
			 0xf0507593, /// 0x714
			 0x49273fbe, /// 0x718
			 0x495b599b, /// 0x71c
			 0x7c8be454, /// 0x720
			 0xe733c738, /// 0x724
			 0x8859e7c8, /// 0x728
			 0x827d92e2, /// 0x72c
			 0xa968b0ac, /// 0x730
			 0x3fa89445, /// 0x734
			 0xd70edbab, /// 0x738
			 0xf90bf238, /// 0x73c
			 0x146e562f, /// 0x740
			 0xe917d9c4, /// 0x744
			 0xaa154773, /// 0x748
			 0xdb4c0712, /// 0x74c
			 0xcfb2a6db, /// 0x750
			 0x37c00603, /// 0x754
			 0x5b8074be, /// 0x758
			 0xec100e0b, /// 0x75c
			 0x585512bc, /// 0x760
			 0x444e1a7e, /// 0x764
			 0x401762b0, /// 0x768
			 0x8828eff6, /// 0x76c
			 0x5e10ca5f, /// 0x770
			 0x80f7f2d2, /// 0x774
			 0xd7ed6830, /// 0x778
			 0xad001378, /// 0x77c
			 0x9953273c, /// 0x780
			 0xce1e2ee2, /// 0x784
			 0xb24950a5, /// 0x788
			 0x270475a7, /// 0x78c
			 0xe71fe92d, /// 0x790
			 0xec83968e, /// 0x794
			 0x97e6db3d, /// 0x798
			 0x1a42dccf, /// 0x79c
			 0xb7e71de0, /// 0x7a0
			 0xc0d7e589, /// 0x7a4
			 0xe33e9b4c, /// 0x7a8
			 0xeb1ad32d, /// 0x7ac
			 0xed3fd444, /// 0x7b0
			 0xac8e24eb, /// 0x7b4
			 0xd2f1eb50, /// 0x7b8
			 0x43db38da, /// 0x7bc
			 0x1519bf54, /// 0x7c0
			 0xdafea753, /// 0x7c4
			 0x86d7ee58, /// 0x7c8
			 0xb9aa6d66, /// 0x7cc
			 0xea87c27a, /// 0x7d0
			 0xbb35a159, /// 0x7d4
			 0x78a6b15b, /// 0x7d8
			 0x1d94e1f0, /// 0x7dc
			 0x80fd1ea8, /// 0x7e0
			 0x32c96b31, /// 0x7e4
			 0x122c34d7, /// 0x7e8
			 0xad674c96, /// 0x7ec
			 0x5d4654f6, /// 0x7f0
			 0x7a033fcf, /// 0x7f4
			 0x4cd06ce6, /// 0x7f8
			 0xcd3c281a, /// 0x7fc
			 0x046c0f3f, /// 0x800
			 0xa7c3937e, /// 0x804
			 0x0c975c74, /// 0x808
			 0x6a500ea0, /// 0x80c
			 0x30565fe2, /// 0x810
			 0x13c05b54, /// 0x814
			 0x1777ee1f, /// 0x818
			 0xbee219b5, /// 0x81c
			 0xdcf9295b, /// 0x820
			 0x2d7254c5, /// 0x824
			 0x35debca2, /// 0x828
			 0x6b199815, /// 0x82c
			 0xec01d9b4, /// 0x830
			 0x7d7520d2, /// 0x834
			 0x88724124, /// 0x838
			 0xc85a8852, /// 0x83c
			 0xc1f4cb12, /// 0x840
			 0x69ef704b, /// 0x844
			 0x17143833, /// 0x848
			 0xbb1b9933, /// 0x84c
			 0xc89eeb32, /// 0x850
			 0xc4649276, /// 0x854
			 0xbbe08ff6, /// 0x858
			 0xe97a2643, /// 0x85c
			 0xa3d2c376, /// 0x860
			 0x8591e40c, /// 0x864
			 0x0203cb53, /// 0x868
			 0x55c1e2bf, /// 0x86c
			 0xac97c063, /// 0x870
			 0xad97acb8, /// 0x874
			 0x6e691811, /// 0x878
			 0x749b26d8, /// 0x87c
			 0x751dc741, /// 0x880
			 0x0b679752, /// 0x884
			 0x6a114c30, /// 0x888
			 0xcbbd0d97, /// 0x88c
			 0x90fef090, /// 0x890
			 0x1bc5de92, /// 0x894
			 0xbcbf43b3, /// 0x898
			 0xf472465a, /// 0x89c
			 0x7f337777, /// 0x8a0
			 0x92831375, /// 0x8a4
			 0x77846506, /// 0x8a8
			 0xa4723f8d, /// 0x8ac
			 0x50250dcb, /// 0x8b0
			 0x4ab62e1f, /// 0x8b4
			 0x99c8028a, /// 0x8b8
			 0xb71e05f2, /// 0x8bc
			 0xfd8c0c3b, /// 0x8c0
			 0x75dadd7d, /// 0x8c4
			 0xf2cbfdcb, /// 0x8c8
			 0xa221b856, /// 0x8cc
			 0xce2e3701, /// 0x8d0
			 0xfa2fe818, /// 0x8d4
			 0x80eb54cc, /// 0x8d8
			 0x70598574, /// 0x8dc
			 0x24c54251, /// 0x8e0
			 0x70592e4d, /// 0x8e4
			 0x4f6fbc1d, /// 0x8e8
			 0x4e59f915, /// 0x8ec
			 0x8a9fc8dd, /// 0x8f0
			 0x0cb259c6, /// 0x8f4
			 0xebfbdb77, /// 0x8f8
			 0xbd50761b, /// 0x8fc
			 0xc5368ae7, /// 0x900
			 0xa3af34f2, /// 0x904
			 0x26b01037, /// 0x908
			 0xf8b4281d, /// 0x90c
			 0xc4a7a8a8, /// 0x910
			 0x868ba48f, /// 0x914
			 0x16874b18, /// 0x918
			 0xfde4e382, /// 0x91c
			 0x39480ef4, /// 0x920
			 0x46dd568f, /// 0x924
			 0xf6b9f5a2, /// 0x928
			 0x3f3bde36, /// 0x92c
			 0x2c74c7a0, /// 0x930
			 0xdebdafa1, /// 0x934
			 0x62f0bda1, /// 0x938
			 0x6f43da33, /// 0x93c
			 0x318a1719, /// 0x940
			 0xd5bdae4e, /// 0x944
			 0x72337c9c, /// 0x948
			 0x4d9d61a2, /// 0x94c
			 0xf846b54d, /// 0x950
			 0x8835c313, /// 0x954
			 0x95ed42d6, /// 0x958
			 0x655b5263, /// 0x95c
			 0xa85236d6, /// 0x960
			 0x727d6bd4, /// 0x964
			 0xe9fde506, /// 0x968
			 0xed161588, /// 0x96c
			 0x927e1fc8, /// 0x970
			 0xbe351ff2, /// 0x974
			 0xa6ab802f, /// 0x978
			 0xf3555415, /// 0x97c
			 0x243b98d6, /// 0x980
			 0x2c494cd0, /// 0x984
			 0x7c3e5c93, /// 0x988
			 0xa612baa7, /// 0x98c
			 0xe2b0ac49, /// 0x990
			 0x298873c0, /// 0x994
			 0xa9fd856b, /// 0x998
			 0xf3575881, /// 0x99c
			 0x85f19028, /// 0x9a0
			 0xc025c2ec, /// 0x9a4
			 0x0a81d990, /// 0x9a8
			 0xf342debe, /// 0x9ac
			 0x406bbd77, /// 0x9b0
			 0xe64181c2, /// 0x9b4
			 0x54c61ac1, /// 0x9b8
			 0xa5e732ef, /// 0x9bc
			 0x79b7a115, /// 0x9c0
			 0xf7731ead, /// 0x9c4
			 0x9907faa7, /// 0x9c8
			 0xf047f129, /// 0x9cc
			 0x90117fe0, /// 0x9d0
			 0xae7a1331, /// 0x9d4
			 0x2e98c29e, /// 0x9d8
			 0x515f64cd, /// 0x9dc
			 0xb82934cc, /// 0x9e0
			 0x96330be3, /// 0x9e4
			 0xaafcaf05, /// 0x9e8
			 0x5f278439, /// 0x9ec
			 0x0fb81552, /// 0x9f0
			 0x882d9a53, /// 0x9f4
			 0x780a1561, /// 0x9f8
			 0xdc35dd9e, /// 0x9fc
			 0xe37a1969, /// 0xa00
			 0x8144d438, /// 0xa04
			 0x4e1b03ae, /// 0xa08
			 0xbaebe416, /// 0xa0c
			 0x2ac7f92d, /// 0xa10
			 0xf768c726, /// 0xa14
			 0x7eb3f405, /// 0xa18
			 0x4261989e, /// 0xa1c
			 0xe3a212f9, /// 0xa20
			 0x93f30f70, /// 0xa24
			 0x789a75f0, /// 0xa28
			 0xd9b319c6, /// 0xa2c
			 0x1e412cc1, /// 0xa30
			 0x6ffb1262, /// 0xa34
			 0x7095bbfa, /// 0xa38
			 0x0381adee, /// 0xa3c
			 0xbc3540e2, /// 0xa40
			 0x118b7fdf, /// 0xa44
			 0x6b54e74f, /// 0xa48
			 0xeda3104e, /// 0xa4c
			 0x1674749c, /// 0xa50
			 0x477af60d, /// 0xa54
			 0x4f628bc0, /// 0xa58
			 0xccab4544, /// 0xa5c
			 0x64b23c7e, /// 0xa60
			 0x65118ba8, /// 0xa64
			 0x5eb1ae39, /// 0xa68
			 0x07eb99c1, /// 0xa6c
			 0x5ec87ce8, /// 0xa70
			 0x24d01dc7, /// 0xa74
			 0xd98c0429, /// 0xa78
			 0xdc99f019, /// 0xa7c
			 0x8cfcc1e9, /// 0xa80
			 0xbac304c2, /// 0xa84
			 0xfe7dd48b, /// 0xa88
			 0x10eb004d, /// 0xa8c
			 0xf89557d4, /// 0xa90
			 0x252a4f6b, /// 0xa94
			 0x42185630, /// 0xa98
			 0x11574c8d, /// 0xa9c
			 0xede68285, /// 0xaa0
			 0x9441fb6c, /// 0xaa4
			 0x7b0065a5, /// 0xaa8
			 0xba5165de, /// 0xaac
			 0x9214efae, /// 0xab0
			 0xddfbdef7, /// 0xab4
			 0x30df8bfd, /// 0xab8
			 0xe3a8c570, /// 0xabc
			 0x6610f0da, /// 0xac0
			 0x31bf1941, /// 0xac4
			 0x56d33c86, /// 0xac8
			 0x6725f365, /// 0xacc
			 0x67303b8e, /// 0xad0
			 0xf1beaec2, /// 0xad4
			 0x20fb7440, /// 0xad8
			 0xc7dfea80, /// 0xadc
			 0x6c11343b, /// 0xae0
			 0xd71bc8e6, /// 0xae4
			 0xd5012639, /// 0xae8
			 0x29a03dea, /// 0xaec
			 0xe5239950, /// 0xaf0
			 0xaf4d8e29, /// 0xaf4
			 0x1e227e61, /// 0xaf8
			 0x4f8bdbea, /// 0xafc
			 0xc20ea46b, /// 0xb00
			 0x3f863e72, /// 0xb04
			 0x3b759ea1, /// 0xb08
			 0xdc64b67a, /// 0xb0c
			 0xa07d2d42, /// 0xb10
			 0x44447ad7, /// 0xb14
			 0x9e6166e3, /// 0xb18
			 0x40e679f2, /// 0xb1c
			 0x723cf168, /// 0xb20
			 0x3e233bee, /// 0xb24
			 0x33312ea5, /// 0xb28
			 0x6cea6e5e, /// 0xb2c
			 0x0fb219d4, /// 0xb30
			 0x4a0fe2c4, /// 0xb34
			 0xc0a93e6a, /// 0xb38
			 0xf577c7ac, /// 0xb3c
			 0x4fdeaad4, /// 0xb40
			 0x58fa9173, /// 0xb44
			 0xc9d4e9c2, /// 0xb48
			 0x64f66580, /// 0xb4c
			 0xe9eaf8b5, /// 0xb50
			 0x3c8b7a13, /// 0xb54
			 0x4f54499a, /// 0xb58
			 0x58687ca0, /// 0xb5c
			 0xc4e5fc27, /// 0xb60
			 0xe3f7c47f, /// 0xb64
			 0x0bf7acfd, /// 0xb68
			 0x69f9665f, /// 0xb6c
			 0xd023cb7d, /// 0xb70
			 0x8b9a0573, /// 0xb74
			 0xaaec4688, /// 0xb78
			 0x8eda9388, /// 0xb7c
			 0x2cbd8145, /// 0xb80
			 0x3078cbc6, /// 0xb84
			 0x6cadf331, /// 0xb88
			 0xef21bc81, /// 0xb8c
			 0x46b8bc3c, /// 0xb90
			 0x12f802bb, /// 0xb94
			 0xa56ad5e9, /// 0xb98
			 0xc681a949, /// 0xb9c
			 0x1f80ae71, /// 0xba0
			 0x5dcfe5d8, /// 0xba4
			 0x1589710f, /// 0xba8
			 0x685ab89a, /// 0xbac
			 0x75ecfa7b, /// 0xbb0
			 0x76167d4b, /// 0xbb4
			 0xfd6b8ac9, /// 0xbb8
			 0x73ff755c, /// 0xbbc
			 0xddd35ea8, /// 0xbc0
			 0xd75f0c73, /// 0xbc4
			 0xa76faa49, /// 0xbc8
			 0xf8d742e3, /// 0xbcc
			 0x02f810bc, /// 0xbd0
			 0xa322c7b8, /// 0xbd4
			 0xfa2c6229, /// 0xbd8
			 0xe19da945, /// 0xbdc
			 0x5d2d7371, /// 0xbe0
			 0x3ca424e3, /// 0xbe4
			 0x42dac635, /// 0xbe8
			 0xb5c8cff7, /// 0xbec
			 0x314bebd2, /// 0xbf0
			 0x173d96ae, /// 0xbf4
			 0xc743dcbb, /// 0xbf8
			 0x54070bf4, /// 0xbfc
			 0xf9064754, /// 0xc00
			 0x53a85afd, /// 0xc04
			 0x03ac8228, /// 0xc08
			 0x53829642, /// 0xc0c
			 0x004f7b99, /// 0xc10
			 0x14eeba3b, /// 0xc14
			 0xbee0dd78, /// 0xc18
			 0x555b7927, /// 0xc1c
			 0x4949ebbd, /// 0xc20
			 0xf7cef643, /// 0xc24
			 0x098f95b3, /// 0xc28
			 0x59b6c7fd, /// 0xc2c
			 0x64d8f478, /// 0xc30
			 0x908eddab, /// 0xc34
			 0x9c855a7a, /// 0xc38
			 0xc3734670, /// 0xc3c
			 0xf3dc00a5, /// 0xc40
			 0xeae9f58a, /// 0xc44
			 0x33527e51, /// 0xc48
			 0x77239b58, /// 0xc4c
			 0x79b87f77, /// 0xc50
			 0xecffcea7, /// 0xc54
			 0x1c985965, /// 0xc58
			 0x0d150af0, /// 0xc5c
			 0xb1b7ae42, /// 0xc60
			 0xb2be0381, /// 0xc64
			 0x7eff7f7d, /// 0xc68
			 0xa0fa10a5, /// 0xc6c
			 0x585802ab, /// 0xc70
			 0x96e3bb72, /// 0xc74
			 0x14917148, /// 0xc78
			 0x2bec7cce, /// 0xc7c
			 0x46791c7b, /// 0xc80
			 0x2b298cf9, /// 0xc84
			 0x88d9bff6, /// 0xc88
			 0x7461e327, /// 0xc8c
			 0x9584af37, /// 0xc90
			 0xbfa9dec6, /// 0xc94
			 0x199bbb92, /// 0xc98
			 0x9d4d5e49, /// 0xc9c
			 0x3608f0e8, /// 0xca0
			 0xd29cc61b, /// 0xca4
			 0xfa7e646c, /// 0xca8
			 0xd170d113, /// 0xcac
			 0x69bf4446, /// 0xcb0
			 0xa98ba162, /// 0xcb4
			 0xfe128359, /// 0xcb8
			 0xd6e43fa6, /// 0xcbc
			 0xc33826d9, /// 0xcc0
			 0x2b142e74, /// 0xcc4
			 0xf892c38e, /// 0xcc8
			 0x7ef8f010, /// 0xccc
			 0xea0dd149, /// 0xcd0
			 0x16c491fb, /// 0xcd4
			 0x257e7254, /// 0xcd8
			 0x593bb215, /// 0xcdc
			 0x7b4cf899, /// 0xce0
			 0xd60f53af, /// 0xce4
			 0x77d433f2, /// 0xce8
			 0x42b98cf8, /// 0xcec
			 0x4975891b, /// 0xcf0
			 0xe453cb19, /// 0xcf4
			 0x5ae27c3a, /// 0xcf8
			 0xe390ddcb, /// 0xcfc
			 0xb6b5a1cf, /// 0xd00
			 0x17df4252, /// 0xd04
			 0xfa68a198, /// 0xd08
			 0x0b30a900, /// 0xd0c
			 0xab768b39, /// 0xd10
			 0x81b256a8, /// 0xd14
			 0xce31b6f4, /// 0xd18
			 0x1f24f43a, /// 0xd1c
			 0x6e9dc574, /// 0xd20
			 0x17c58bb2, /// 0xd24
			 0x8e0ebcc1, /// 0xd28
			 0xde184f60, /// 0xd2c
			 0x3ec17310, /// 0xd30
			 0x32224eb4, /// 0xd34
			 0x4acb3f5e, /// 0xd38
			 0xa5f5f839, /// 0xd3c
			 0x16121a77, /// 0xd40
			 0x0f647709, /// 0xd44
			 0x028ac919, /// 0xd48
			 0x45b76e78, /// 0xd4c
			 0x30ba6a46, /// 0xd50
			 0xcf41ca83, /// 0xd54
			 0xe5ea2b6b, /// 0xd58
			 0x5263d06c, /// 0xd5c
			 0x779d5edd, /// 0xd60
			 0xe195951a, /// 0xd64
			 0xecd70156, /// 0xd68
			 0x3a94a9d8, /// 0xd6c
			 0x55136347, /// 0xd70
			 0xfdfb5987, /// 0xd74
			 0x4cf38d16, /// 0xd78
			 0xeadb5633, /// 0xd7c
			 0xa48f6c34, /// 0xd80
			 0xb5f5dfed, /// 0xd84
			 0xab589093, /// 0xd88
			 0xa0232dd1, /// 0xd8c
			 0x71308763, /// 0xd90
			 0x3e2ea67a, /// 0xd94
			 0x2b9d443c, /// 0xd98
			 0xab363852, /// 0xd9c
			 0x8e180b2c, /// 0xda0
			 0x489b738e, /// 0xda4
			 0x9f13a304, /// 0xda8
			 0xeea8cd2a, /// 0xdac
			 0x021e99c2, /// 0xdb0
			 0xc19cc42c, /// 0xdb4
			 0x1afed1e2, /// 0xdb8
			 0xbd0348f3, /// 0xdbc
			 0xf8b371ed, /// 0xdc0
			 0x0f6e19ce, /// 0xdc4
			 0x1753808e, /// 0xdc8
			 0xd4f02a6f, /// 0xdcc
			 0x58d9d559, /// 0xdd0
			 0x1a204a15, /// 0xdd4
			 0xfff634fa, /// 0xdd8
			 0xefed87e5, /// 0xddc
			 0x30793e00, /// 0xde0
			 0x54f0989d, /// 0xde4
			 0x2bd6d2b1, /// 0xde8
			 0x30259c70, /// 0xdec
			 0xd9982fee, /// 0xdf0
			 0xf226dbed, /// 0xdf4
			 0x4407b5b7, /// 0xdf8
			 0xec1eccdd, /// 0xdfc
			 0xac2e7227, /// 0xe00
			 0xa8c3e9eb, /// 0xe04
			 0xe24487ce, /// 0xe08
			 0x9cabcf1e, /// 0xe0c
			 0xdb46b4ef, /// 0xe10
			 0x3618152f, /// 0xe14
			 0xffdef7cb, /// 0xe18
			 0xe93315da, /// 0xe1c
			 0xb4935113, /// 0xe20
			 0xa8c0f6f7, /// 0xe24
			 0x159dd65d, /// 0xe28
			 0x5eb5eae0, /// 0xe2c
			 0x083a44fd, /// 0xe30
			 0xb2632a3d, /// 0xe34
			 0x842aeea8, /// 0xe38
			 0xb4d3d0e3, /// 0xe3c
			 0xa7d5267b, /// 0xe40
			 0xb0dc36b3, /// 0xe44
			 0x9690f0e8, /// 0xe48
			 0xf86e569c, /// 0xe4c
			 0x638ad53c, /// 0xe50
			 0x41594402, /// 0xe54
			 0x5d32859e, /// 0xe58
			 0x287c36ab, /// 0xe5c
			 0x2ca11ed2, /// 0xe60
			 0x3b7d918f, /// 0xe64
			 0x341d00fb, /// 0xe68
			 0xdad20376, /// 0xe6c
			 0xffff2f40, /// 0xe70
			 0x05cc80c7, /// 0xe74
			 0xc0f19402, /// 0xe78
			 0xfea5799d, /// 0xe7c
			 0xaa00f936, /// 0xe80
			 0x6fdd4e96, /// 0xe84
			 0x92cffc0c, /// 0xe88
			 0xd80b5fc5, /// 0xe8c
			 0x81345c73, /// 0xe90
			 0x49838a5f, /// 0xe94
			 0xed762e0b, /// 0xe98
			 0x172e3e7e, /// 0xe9c
			 0xe57eb84d, /// 0xea0
			 0x826c03f9, /// 0xea4
			 0xe12e6322, /// 0xea8
			 0xea7242b1, /// 0xeac
			 0x142b19aa, /// 0xeb0
			 0xa4880a55, /// 0xeb4
			 0x9ec18c79, /// 0xeb8
			 0x904e33fe, /// 0xebc
			 0xc55d9ba3, /// 0xec0
			 0x4f4a6188, /// 0xec4
			 0xdac14388, /// 0xec8
			 0xbce9aea6, /// 0xecc
			 0xd4c41e54, /// 0xed0
			 0x4266b373, /// 0xed4
			 0xffae1d85, /// 0xed8
			 0x6454d9b4, /// 0xedc
			 0x6a68db16, /// 0xee0
			 0x709950a5, /// 0xee4
			 0xe9f3f305, /// 0xee8
			 0xa87c2a29, /// 0xeec
			 0xa1391724, /// 0xef0
			 0x0cc7c931, /// 0xef4
			 0x6ebfb9af, /// 0xef8
			 0x564a8904, /// 0xefc
			 0xe390bd61, /// 0xf00
			 0x1bc89d77, /// 0xf04
			 0x70a37ee1, /// 0xf08
			 0x63b85d97, /// 0xf0c
			 0x787d54ba, /// 0xf10
			 0x8a0fc3c4, /// 0xf14
			 0xfd38964d, /// 0xf18
			 0x32166325, /// 0xf1c
			 0x0bbd2d21, /// 0xf20
			 0x5f814740, /// 0xf24
			 0x51a99fae, /// 0xf28
			 0x855e7e67, /// 0xf2c
			 0xb4be6a00, /// 0xf30
			 0x521c841b, /// 0xf34
			 0x9295958a, /// 0xf38
			 0xb19ff309, /// 0xf3c
			 0xcfc6bd3b, /// 0xf40
			 0x4bd8febf, /// 0xf44
			 0xe5cd332c, /// 0xf48
			 0xaf126983, /// 0xf4c
			 0xb1cb421b, /// 0xf50
			 0x8501f1b4, /// 0xf54
			 0x3777386d, /// 0xf58
			 0xe56bbeb7, /// 0xf5c
			 0xe720ab08, /// 0xf60
			 0xa59e6dbf, /// 0xf64
			 0xefe21dac, /// 0xf68
			 0xd4ca60f4, /// 0xf6c
			 0xcdb0a288, /// 0xf70
			 0x75d49ec8, /// 0xf74
			 0x85011005, /// 0xf78
			 0x58af76b4, /// 0xf7c
			 0x9eebe798, /// 0xf80
			 0xb85fe9da, /// 0xf84
			 0x3700fc66, /// 0xf88
			 0xbf349057, /// 0xf8c
			 0xbbe56c29, /// 0xf90
			 0xe0b6c931, /// 0xf94
			 0xe9e6dcf9, /// 0xf98
			 0xb1e12ed3, /// 0xf9c
			 0x5714a2ca, /// 0xfa0
			 0x5ae3843b, /// 0xfa4
			 0x367822cd, /// 0xfa8
			 0xb440afa6, /// 0xfac
			 0x5e5dfb26, /// 0xfb0
			 0x499219e3, /// 0xfb4
			 0x6f3ef806, /// 0xfb8
			 0xd801d78b, /// 0xfbc
			 0xddee9f35, /// 0xfc0
			 0x42bc0fa2, /// 0xfc4
			 0x43d7ea98, /// 0xfc8
			 0x6a815a7c, /// 0xfcc
			 0xf8dbbac6, /// 0xfd0
			 0x50d49bfc, /// 0xfd4
			 0x840bac9e, /// 0xfd8
			 0x0683b9fe, /// 0xfdc
			 0x72eeffe4, /// 0xfe0
			 0x16e301d4, /// 0xfe4
			 0x61e7cf22, /// 0xfe8
			 0xaadc09cf, /// 0xfec
			 0x6355c36a, /// 0xff0
			 0x489bc308, /// 0xff4
			 0x0e529faa, /// 0xff8
			 0x39dd84db /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00000
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00004
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00010
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00018
			 0x0e000001,                                                  // Trailing 1s:                                /// 0001c
			 0xffc00001,                                                  // -signaling NaN                              /// 00020
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00024
			 0x33333333,                                                  // 4 random values                             /// 00028
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00034
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0003c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00040
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00044
			 0x7f800000,                                                  // inf                                         /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00054
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0005c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00060
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000000,                                                  // zero                                        /// 00070
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00078
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00080
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00084
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00088
			 0x0c700000,                                                  // Leading 1s:                                 /// 0008c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00090
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x55555555,                                                  // 4 random values                             /// 000dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e000007,                                                  // Trailing 1s:                                /// 00108
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0010c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00110
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00114
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00118
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00120
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00124
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00128
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x7f800000,                                                  // inf                                         /// 00130
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00134
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0013c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00140
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00144
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00148
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00150
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00154
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0015c
			 0x33333333,                                                  // 4 random values                             /// 00160
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00164
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00170
			 0xff800000,                                                  // -inf                                        /// 00174
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00178
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00184
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00188
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0018c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00190
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00194
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00198
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0019c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00000000,                                                  // zero                                        /// 001dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00204
			 0x00000000,                                                  // zero                                        /// 00208
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00210
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xcb000000,                                                  // -8388608.0                                  /// 00218
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0021c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00224
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00228
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00238
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00244
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00248
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0024c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00254
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0025c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00260
			 0x55555555,                                                  // 4 random values                             /// 00264
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00268
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00274
			 0x7fc00001,                                                  // signaling NaN                               /// 00278
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00280
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00284
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00288
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0028c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00290
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00308
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0030c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00310
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00314
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0031c
			 0x80000000,                                                  // -zero                                       /// 00320
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00324
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00328
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e000003,                                                  // Trailing 1s:                                /// 00338
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0033c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00340
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00344
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00348
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00358
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0035c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00360
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0036c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00370
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00378
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00380
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00384
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00388
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00394
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00398
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0039c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00400
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00408
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0040c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00410
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00414
			 0x33333333,                                                  // 4 random values                             /// 00418
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0041c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00428
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0042c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00438
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0043c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00440
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00444
			 0x0c700000,                                                  // Leading 1s:                                 /// 00448
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00450
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00454
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00458
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00460
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00464
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x7f800000,                                                  // inf                                         /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00470
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0047c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00488
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00490
			 0x80000000,                                                  // -zero                                       /// 00494
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0049c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00500
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0050c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00510
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00514
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00518
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00520
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00524
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x7fc00001,                                                  // signaling NaN                               /// 00534
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0053c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00548
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0054c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00550
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00554
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00558
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0xffc00001,                                                  // -signaling NaN                              /// 00560
			 0xffc00001,                                                  // -signaling NaN                              /// 00564
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00568
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0xbf028f5c,                                                  // -0.51                                       /// 00574
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0057c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00580
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00584
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00588
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0058c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00590
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0059c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e4
			 0xbf028f5c,                                                  // -0.51                                       /// 005e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 005f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00600
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00604
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00608
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0060c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00610
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00614
			 0x0c600000,                                                  // Leading 1s:                                 /// 00618
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0061c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00620
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00624
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00628
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0062c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00630
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00638
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00640
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00644
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00648
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00654
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0065c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00660
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00664
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0066c
			 0x00000000,                                                  // zero                                        /// 00670
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00678
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00680
			 0xbf028f5c,                                                  // -0.51                                       /// 00684
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00688
			 0xbf028f5c,                                                  // -0.51                                       /// 0068c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00690
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00698
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00700
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00708
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0070c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00714
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0071c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00720
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00724
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00728
			 0x80000000,                                                  // -zero                                       /// 0072c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00734
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0073c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00744
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0074c
			 0x33333333,                                                  // 4 random values                             /// 00750
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00754
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00758
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0075c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00760
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00764
			 0x80000000,                                                  // -zero                                       /// 00768
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00770
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00774
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00778
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0077c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00780
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00784
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0078c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00790
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00794
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00798
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00804
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00808
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00810
			 0x55555555,                                                  // 4 random values                             /// 00814
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00818
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00824
			 0x0c400000,                                                  // Leading 1s:                                 /// 00828
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0082c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00834
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0x7f800000,                                                  // inf                                         /// 00840
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00844
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00848
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0084c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x0c700000,                                                  // Leading 1s:                                 /// 00854
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00858
			 0x00000000,                                                  // zero                                        /// 0085c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00860
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00864
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0086c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00878
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00880
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00884
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00888
			 0x0e000003,                                                  // Trailing 1s:                                /// 0088c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00894
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00898
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00900
			 0x33333333,                                                  // 4 random values                             /// 00904
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00908
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00910
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00914
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0091c
			 0x55555555,                                                  // 4 random values                             /// 00920
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00924
			 0x00000000,                                                  // zero                                        /// 00928
			 0x33333333,                                                  // 4 random values                             /// 0092c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00948
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00954
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00958
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0095c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00964
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00974
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00978
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00980
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00984
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00988
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0098c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00000000,                                                  // zero                                        /// 00994
			 0x0c600000,                                                  // Leading 1s:                                 /// 00998
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009c8
			 0x7f800000,                                                  // inf                                         /// 009cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x4b000000,                                                  // 8388608.0                                   /// 00a48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a7c
			 0x7f800000,                                                  // inf                                         /// 00a80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00000000,                                                  // zero                                        /// 00a90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00aa4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ac0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ac8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad0
			 0x33333333,                                                  // 4 random values                             /// 00ad4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00adc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ae0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ae8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af0
			 0x33333333,                                                  // 4 random values                             /// 00af4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00afc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x55555555,                                                  // 4 random values                             /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b30
			 0x7f800000,                                                  // inf                                         /// 00b34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x00000000,                                                  // zero                                        /// 00b40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ba0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bbc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bf0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x80000000,                                                  // -zero                                       /// 00c44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c5c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ca4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cb8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ccc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cd0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cdc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ce8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cf4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cfc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00000000,                                                  // zero                                        /// 00d84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d98
			 0x80000000,                                                  // -zero                                       /// 00d9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db0
			 0x7fc00001,                                                  // signaling NaN                               /// 00db4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ddc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00df0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00df4
			 0x4b000000,                                                  // 8388608.0                                   /// 00df8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e20
			 0x33333333,                                                  // 4 random values                             /// 00e24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e30
			 0x3f028f5c,                                                  // 0.51                                        /// 00e34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ea4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ec4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ed4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ed8
			 0x4b000000,                                                  // 8388608.0                                   /// 00edc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ee4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ee8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef4
			 0x55555555,                                                  // 4 random values                             /// 00ef8
			 0xffc00001,                                                  // -signaling NaN                              /// 00efc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0xff800000,                                                  // -inf                                        /// 00f0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xff800000,                                                  // -inf                                        /// 00f18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x7f800000,                                                  // inf                                         /// 00f24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f34
			 0x3f028f5c,                                                  // 0.51                                        /// 00f38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x55555555,                                                  // 4 random values                             /// 00f5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fa8
			 0x55555555,                                                  // 4 random values                             /// 00fac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fe0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ff8
			 0x80800000                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00000
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00004
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00008
			 0x0c600000,                                                  // Leading 1s:                                 /// 0000c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x80000000,                                                  // -zero                                       /// 00014
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x3f028f5c,                                                  // 0.51                                        /// 00020
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00024
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00034
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0003c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00040
			 0xcb000000,                                                  // -8388608.0                                  /// 00044
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00048
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0004c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00050
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00058
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0005c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00060
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0006c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00070
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00074
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00078
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00080
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00084
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00088
			 0x80011111,                                                  // -9.7958E-41                                 /// 0008c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00094
			 0x4b000000,                                                  // 8388608.0                                   /// 00098
			 0x33333333,                                                  // 4 random values                             /// 0009c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b0
			 0xcb000000,                                                  // -8388608.0                                  /// 000b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e0
			 0x7fc00001,                                                  // signaling NaN                               /// 000e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00100
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00104
			 0x33333333,                                                  // 4 random values                             /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00110
			 0x0e000001,                                                  // Trailing 1s:                                /// 00114
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00118
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0011c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00120
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0012c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00130
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00134
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0c600000,                                                  // Leading 1s:                                 /// 00144
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0014c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00150
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00158
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0015c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c700000,                                                  // Leading 1s:                                 /// 00164
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0016c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00174
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00178
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00184
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00188
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00194
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00198
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0019c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c4
			 0x4b000000,                                                  // 8388608.0                                   /// 001c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001dc
			 0xcb000000,                                                  // -8388608.0                                  /// 001e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00200
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00204
			 0x0c780000,                                                  // Leading 1s:                                 /// 00208
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00214
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00220
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00224
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0c700000,                                                  // Leading 1s:                                 /// 0022c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00230
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00234
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0023c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00244
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00248
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0024c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00264
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00268
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00270
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00278
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00284
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00288
			 0x00011111,                                                  // 9.7958E-41                                  /// 0028c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00290
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x7f800000,                                                  // inf                                         /// 002b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00300
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00304
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00308
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0030c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00310
			 0x0c780000,                                                  // Leading 1s:                                 /// 00314
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00318
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0031c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00328
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00334
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00348
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00350
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00354
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0035c
			 0x55555555,                                                  // 4 random values                             /// 00360
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0036c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00370
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00374
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00388
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0038c
			 0xcb000000,                                                  // -8388608.0                                  /// 00390
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00400
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00404
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0041c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00420
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00424
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00428
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0042c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00434
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00440
			 0x0c600000,                                                  // Leading 1s:                                 /// 00444
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00458
			 0x0c780000,                                                  // Leading 1s:                                 /// 0045c
			 0x55555555,                                                  // 4 random values                             /// 00460
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00468
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00474
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00478
			 0x0e000007,                                                  // Trailing 1s:                                /// 0047c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00480
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00498
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0xbf028f5c,                                                  // -0.51                                       /// 004b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 004e4
			 0x55555555,                                                  // 4 random values                             /// 004e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f8
			 0x55555555,                                                  // 4 random values                             /// 004fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x4b000000,                                                  // 8388608.0                                   /// 00508
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0050c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00510
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00514
			 0x7fc00001,                                                  // signaling NaN                               /// 00518
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00520
			 0x80000000,                                                  // -zero                                       /// 00524
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00528
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00534
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00540
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0c780000,                                                  // Leading 1s:                                 /// 0054c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00550
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00558
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0055c
			 0x3f028f5c,                                                  // 0.51                                        /// 00560
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00564
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00568
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0056c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00574
			 0x0e000003,                                                  // Trailing 1s:                                /// 00578
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0057c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00580
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00584
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00588
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00598
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0059c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x3f028f5c,                                                  // 0.51                                        /// 005e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00600
			 0x80000000,                                                  // -zero                                       /// 00604
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00610
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00614
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0061c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00624
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00628
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0062c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00630
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00638
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0063c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00640
			 0x0e000001,                                                  // Trailing 1s:                                /// 00644
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00650
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00658
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0065c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00660
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00664
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00668
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00674
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00678
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00684
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00688
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0068c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00690
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00694
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00698
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0069c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00700
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00704
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00708
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00714
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00718
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00720
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00724
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00730
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00734
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00738
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0073c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00744
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00748
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00750
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00758
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0075c
			 0x3f028f5c,                                                  // 0.51                                        /// 00760
			 0x33333333,                                                  // 4 random values                             /// 00764
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00768
			 0x0e000003,                                                  // Trailing 1s:                                /// 0076c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00774
			 0x80000000,                                                  // -zero                                       /// 00778
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0077c
			 0x7f800000,                                                  // inf                                         /// 00780
			 0x0e000001,                                                  // Trailing 1s:                                /// 00784
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00788
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00794
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0xbf028f5c,                                                  // -0.51                                       /// 007bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00800
			 0x0c700000,                                                  // Leading 1s:                                 /// 00804
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00808
			 0x55555555,                                                  // 4 random values                             /// 0080c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00810
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00814
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00818
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0081c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00820
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00824
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00828
			 0x0c400000,                                                  // Leading 1s:                                 /// 0082c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00838
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00844
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00848
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00850
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00854
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0085c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x3f028f5c,                                                  // 0.51                                        /// 00870
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00874
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00878
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00880
			 0xcb000000,                                                  // -8388608.0                                  /// 00884
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00890
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00894
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00898
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0089c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x4b000000,                                                  // 8388608.0                                   /// 008c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00900
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00904
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00908
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0xffc00001,                                                  // -signaling NaN                              /// 00910
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00918
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0091c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x55555555,                                                  // 4 random values                             /// 0092c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00938
			 0xffc00001,                                                  // -signaling NaN                              /// 0093c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00940
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00944
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0094c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00950
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00960
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00968
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0096c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00974
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0097c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00980
			 0x0e000007,                                                  // Trailing 1s:                                /// 00984
			 0x0c700000,                                                  // Leading 1s:                                 /// 00988
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00990
			 0x00011111,                                                  // 9.7958E-41                                  /// 00994
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00998
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0099c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a34
			 0x7fc00001,                                                  // signaling NaN                               /// 00a38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xffc00001,                                                  // -signaling NaN                              /// 00a44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00abc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ac8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00acc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00adc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ae0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00af8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b10
			 0xff800000,                                                  // -inf                                        /// 00b14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x4b000000,                                                  // 8388608.0                                   /// 00b98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ba0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ba8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bcc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bd8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be0
			 0xffc00001,                                                  // -signaling NaN                              /// 00be4
			 0x33333333,                                                  // 4 random values                             /// 00be8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bfc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0xcb000000,                                                  // -8388608.0                                  /// 00c3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x55555555,                                                  // 4 random values                             /// 00c60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c64
			 0xbf028f5c,                                                  // -0.51                                       /// 00c68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c74
			 0x55555555,                                                  // 4 random values                             /// 00c78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00000000,                                                  // zero                                        /// 00c94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cb0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cb4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ce4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cf0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d20
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d68
			 0xff800000,                                                  // -inf                                        /// 00d6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d88
			 0xbf028f5c,                                                  // -0.51                                       /// 00d8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da0
			 0xcb000000,                                                  // -8388608.0                                  /// 00da4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00db0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x33333333,                                                  // 4 random values                             /// 00dc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dd8
			 0x55555555,                                                  // 4 random values                             /// 00ddc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00de0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dfc
			 0x7f800000,                                                  // inf                                         /// 00e00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e50
			 0x4b000000,                                                  // 8388608.0                                   /// 00e54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e5c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ea4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ea8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eb0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ebc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x7f800000,                                                  // inf                                         /// 00ec8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ecc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ed0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ed4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00edc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00eec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f08
			 0x4b000000,                                                  // 8388608.0                                   /// 00f0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fa4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fc8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fe4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0xff800000                                                  // -inf                                        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xe0010af0, /// 0x0
			 0xd4b144b6, /// 0x4
			 0x7bb56126, /// 0x8
			 0x4ebb2352, /// 0xc
			 0xd3f45071, /// 0x10
			 0x73e743fd, /// 0x14
			 0x93c4efd6, /// 0x18
			 0x293f6828, /// 0x1c
			 0xa9d4f855, /// 0x20
			 0x34047ecd, /// 0x24
			 0xe18ce9e9, /// 0x28
			 0x9fb5eabe, /// 0x2c
			 0xc9caec35, /// 0x30
			 0xcbdd704e, /// 0x34
			 0xa6fa34c0, /// 0x38
			 0x7e04e496, /// 0x3c
			 0xeaafbaa6, /// 0x40
			 0xa0cbb105, /// 0x44
			 0x0b00d6a4, /// 0x48
			 0x5820a803, /// 0x4c
			 0x8d95bc0b, /// 0x50
			 0x6914e19f, /// 0x54
			 0x1722cce7, /// 0x58
			 0xe7445f58, /// 0x5c
			 0xcc20139a, /// 0x60
			 0x516b3b02, /// 0x64
			 0xa9f70441, /// 0x68
			 0x4f1b3cc9, /// 0x6c
			 0x70a3e1d7, /// 0x70
			 0xb3f222ac, /// 0x74
			 0x6e1c8387, /// 0x78
			 0xfd01a873, /// 0x7c
			 0x765e5506, /// 0x80
			 0x6519aa7c, /// 0x84
			 0x1ae29015, /// 0x88
			 0xa473b428, /// 0x8c
			 0xb5b8c50f, /// 0x90
			 0x7341e50e, /// 0x94
			 0x27e92d4e, /// 0x98
			 0xc5e1e226, /// 0x9c
			 0x30436100, /// 0xa0
			 0x5100491c, /// 0xa4
			 0x229c7107, /// 0xa8
			 0xc1d131e6, /// 0xac
			 0x4a462e3b, /// 0xb0
			 0x19ffb382, /// 0xb4
			 0xbb1a1903, /// 0xb8
			 0x8f4ebcfb, /// 0xbc
			 0xca075e5f, /// 0xc0
			 0x96627d10, /// 0xc4
			 0xb3b8363b, /// 0xc8
			 0x5e0c861e, /// 0xcc
			 0xf91f3252, /// 0xd0
			 0xc47053c9, /// 0xd4
			 0x26aeadb9, /// 0xd8
			 0x77f98176, /// 0xdc
			 0x023d9ccd, /// 0xe0
			 0x681833ba, /// 0xe4
			 0xa3816cf1, /// 0xe8
			 0xa94a3ab3, /// 0xec
			 0xbbc3197f, /// 0xf0
			 0x5abaf0a2, /// 0xf4
			 0xa8e5cf06, /// 0xf8
			 0xca67cf85, /// 0xfc
			 0x0f964f0a, /// 0x100
			 0x67203692, /// 0x104
			 0xb61a6ebe, /// 0x108
			 0xdc7f2ab4, /// 0x10c
			 0x8d89d883, /// 0x110
			 0x058efd14, /// 0x114
			 0xe1336290, /// 0x118
			 0x960b434e, /// 0x11c
			 0x76d7c44a, /// 0x120
			 0xe775c82f, /// 0x124
			 0xdf1c0565, /// 0x128
			 0xe5578f62, /// 0x12c
			 0x46ed5d6f, /// 0x130
			 0xd6e91934, /// 0x134
			 0xc2e2b08f, /// 0x138
			 0x66896ee7, /// 0x13c
			 0xf558ed74, /// 0x140
			 0x7b0886f6, /// 0x144
			 0x901eeec3, /// 0x148
			 0x8f0397e9, /// 0x14c
			 0xbf285aed, /// 0x150
			 0xeb456295, /// 0x154
			 0xac0a090f, /// 0x158
			 0x445823eb, /// 0x15c
			 0x92c912e3, /// 0x160
			 0x22fd4305, /// 0x164
			 0xcdf5fc07, /// 0x168
			 0x668403c4, /// 0x16c
			 0xeb94b262, /// 0x170
			 0xd0ca1432, /// 0x174
			 0xf88c893f, /// 0x178
			 0xd98b4daf, /// 0x17c
			 0x12b14ea9, /// 0x180
			 0x2d18ec2d, /// 0x184
			 0x8de0f1e4, /// 0x188
			 0x4baeb13c, /// 0x18c
			 0x4b916ef1, /// 0x190
			 0xff178b74, /// 0x194
			 0x42c5b403, /// 0x198
			 0xbe94a29d, /// 0x19c
			 0x5ad3a1a1, /// 0x1a0
			 0xb82aec23, /// 0x1a4
			 0x709515cd, /// 0x1a8
			 0x9e3a213b, /// 0x1ac
			 0x38d137ce, /// 0x1b0
			 0xfb85f5dd, /// 0x1b4
			 0xb4e4e3b8, /// 0x1b8
			 0x53b43df2, /// 0x1bc
			 0x1626d074, /// 0x1c0
			 0xef7d0ddd, /// 0x1c4
			 0x89ab77a3, /// 0x1c8
			 0xfee3301c, /// 0x1cc
			 0x4c496d0e, /// 0x1d0
			 0xb4d4fa78, /// 0x1d4
			 0x715f31d0, /// 0x1d8
			 0xdb35c286, /// 0x1dc
			 0x5d8eb98b, /// 0x1e0
			 0xf29efdef, /// 0x1e4
			 0xf0e0c36e, /// 0x1e8
			 0x7ed729bc, /// 0x1ec
			 0xd2633a2f, /// 0x1f0
			 0xbafbbecb, /// 0x1f4
			 0xd3146ef4, /// 0x1f8
			 0x85fac7b6, /// 0x1fc
			 0x09bed4a6, /// 0x200
			 0x23d5fa46, /// 0x204
			 0x4bbff19b, /// 0x208
			 0xf4fa7111, /// 0x20c
			 0x9460c16c, /// 0x210
			 0x8af3d4b5, /// 0x214
			 0x3d9630d0, /// 0x218
			 0x9772952a, /// 0x21c
			 0xa9fc1e60, /// 0x220
			 0x809be1ad, /// 0x224
			 0x0c13c7b0, /// 0x228
			 0xa39f2445, /// 0x22c
			 0x37360f09, /// 0x230
			 0xd69c10ef, /// 0x234
			 0x683331d6, /// 0x238
			 0xa4e4114f, /// 0x23c
			 0x7da32be7, /// 0x240
			 0xb8f4368f, /// 0x244
			 0x1997a87a, /// 0x248
			 0x16587c17, /// 0x24c
			 0x4311058a, /// 0x250
			 0x2e480474, /// 0x254
			 0xa8ba505b, /// 0x258
			 0xb3d59e35, /// 0x25c
			 0xe9779a1b, /// 0x260
			 0x2af51ea3, /// 0x264
			 0x17b75db3, /// 0x268
			 0x661e59fc, /// 0x26c
			 0x18eaa861, /// 0x270
			 0x31da22fd, /// 0x274
			 0x38166a77, /// 0x278
			 0x7c4e9fb5, /// 0x27c
			 0x0cfc67e6, /// 0x280
			 0x3445aaa3, /// 0x284
			 0x7d4d63b1, /// 0x288
			 0x8d8796da, /// 0x28c
			 0xa2ca492f, /// 0x290
			 0xe3b09652, /// 0x294
			 0x387e4ac0, /// 0x298
			 0xa52090b7, /// 0x29c
			 0x0cfd2d44, /// 0x2a0
			 0xf5694a99, /// 0x2a4
			 0xab9c0583, /// 0x2a8
			 0x31aac47b, /// 0x2ac
			 0x851a1830, /// 0x2b0
			 0xaa26d703, /// 0x2b4
			 0x5501d20b, /// 0x2b8
			 0x02a50eb9, /// 0x2bc
			 0x9ffc996e, /// 0x2c0
			 0x91b06fe9, /// 0x2c4
			 0x126084ed, /// 0x2c8
			 0x2c14a744, /// 0x2cc
			 0x46939c50, /// 0x2d0
			 0x2ad0acfe, /// 0x2d4
			 0x4b4b1a44, /// 0x2d8
			 0x62843fb3, /// 0x2dc
			 0x9daa8bbe, /// 0x2e0
			 0xe427410c, /// 0x2e4
			 0x7ca861d5, /// 0x2e8
			 0x4fb57bf9, /// 0x2ec
			 0x8ed00102, /// 0x2f0
			 0x169a0825, /// 0x2f4
			 0x239e2ca6, /// 0x2f8
			 0xcb06f990, /// 0x2fc
			 0xf4385cb3, /// 0x300
			 0xbbd9405e, /// 0x304
			 0xc44cf1b7, /// 0x308
			 0x485d0fb1, /// 0x30c
			 0x50ae9d8c, /// 0x310
			 0xee184b1c, /// 0x314
			 0x4708fb14, /// 0x318
			 0x71c9db5c, /// 0x31c
			 0x89478fe0, /// 0x320
			 0x64603532, /// 0x324
			 0x2cb5a10f, /// 0x328
			 0xe411240b, /// 0x32c
			 0x64d95b43, /// 0x330
			 0xa1ee9d84, /// 0x334
			 0x7170f219, /// 0x338
			 0x72f05859, /// 0x33c
			 0xd0480aab, /// 0x340
			 0x9481a833, /// 0x344
			 0x2ec3547e, /// 0x348
			 0x67d8023c, /// 0x34c
			 0xe910f9c3, /// 0x350
			 0x6025ae49, /// 0x354
			 0xf79bde1e, /// 0x358
			 0xeb77281d, /// 0x35c
			 0xefb0b440, /// 0x360
			 0x602f605d, /// 0x364
			 0x16fb1767, /// 0x368
			 0x15d9bda7, /// 0x36c
			 0xa2ae6c9d, /// 0x370
			 0x94f71079, /// 0x374
			 0x3b031371, /// 0x378
			 0xb2f7e661, /// 0x37c
			 0xaab653f9, /// 0x380
			 0x7b6e4dc7, /// 0x384
			 0x201f5a72, /// 0x388
			 0x733db670, /// 0x38c
			 0x7e720eb3, /// 0x390
			 0x468859a1, /// 0x394
			 0x4ffeff3d, /// 0x398
			 0xe21ca092, /// 0x39c
			 0xa01da2aa, /// 0x3a0
			 0x6b296529, /// 0x3a4
			 0xfc4fa93c, /// 0x3a8
			 0x905fd432, /// 0x3ac
			 0x4b39e870, /// 0x3b0
			 0xbf76803e, /// 0x3b4
			 0xbacfea0e, /// 0x3b8
			 0xaf7871d7, /// 0x3bc
			 0xef0a1ff9, /// 0x3c0
			 0x2e777e3e, /// 0x3c4
			 0xb8d04c71, /// 0x3c8
			 0x458252fd, /// 0x3cc
			 0xbda67834, /// 0x3d0
			 0x83a669a5, /// 0x3d4
			 0xca189434, /// 0x3d8
			 0x9e4fcb9f, /// 0x3dc
			 0x26a23f8a, /// 0x3e0
			 0x28b8a324, /// 0x3e4
			 0x53f69180, /// 0x3e8
			 0x1c796530, /// 0x3ec
			 0x83961d08, /// 0x3f0
			 0xaddde5ad, /// 0x3f4
			 0xd87aba41, /// 0x3f8
			 0x8c1f0a9c, /// 0x3fc
			 0x55d35dd3, /// 0x400
			 0xdc581228, /// 0x404
			 0xee3e7b63, /// 0x408
			 0x2ce7c5d1, /// 0x40c
			 0x5d2bae90, /// 0x410
			 0x9d614da5, /// 0x414
			 0x5e0d9919, /// 0x418
			 0x32d77181, /// 0x41c
			 0xb916ff7d, /// 0x420
			 0x8cf78fe3, /// 0x424
			 0x338a0e88, /// 0x428
			 0xb0b8661b, /// 0x42c
			 0x5e4acfbe, /// 0x430
			 0x17dd3ccc, /// 0x434
			 0x068f8025, /// 0x438
			 0x2d86d6a5, /// 0x43c
			 0x9785a37c, /// 0x440
			 0x2a729d44, /// 0x444
			 0x01e135aa, /// 0x448
			 0x230306df, /// 0x44c
			 0x296a5412, /// 0x450
			 0x00093bad, /// 0x454
			 0x31c65b8b, /// 0x458
			 0x7546f36d, /// 0x45c
			 0x7cd34660, /// 0x460
			 0xd78b587e, /// 0x464
			 0x65935335, /// 0x468
			 0xfe52dde2, /// 0x46c
			 0x3c621e85, /// 0x470
			 0x330f45ab, /// 0x474
			 0x1a7fdfd1, /// 0x478
			 0x09c225a9, /// 0x47c
			 0xaabce544, /// 0x480
			 0xf244eed3, /// 0x484
			 0x0bac5802, /// 0x488
			 0xce9080b7, /// 0x48c
			 0x05af57cc, /// 0x490
			 0x909e4dc3, /// 0x494
			 0x98a54137, /// 0x498
			 0x64fe3787, /// 0x49c
			 0x5acc07aa, /// 0x4a0
			 0xa9db2011, /// 0x4a4
			 0xddb4134c, /// 0x4a8
			 0xb3f11343, /// 0x4ac
			 0x701e726d, /// 0x4b0
			 0xf48a467b, /// 0x4b4
			 0xc2c30c22, /// 0x4b8
			 0x91455dc1, /// 0x4bc
			 0x13800c93, /// 0x4c0
			 0xbee3c020, /// 0x4c4
			 0xb8f4b620, /// 0x4c8
			 0xe860c577, /// 0x4cc
			 0x329f03b6, /// 0x4d0
			 0x656e634d, /// 0x4d4
			 0x10100425, /// 0x4d8
			 0x091535f6, /// 0x4dc
			 0x2499bb81, /// 0x4e0
			 0xb207e901, /// 0x4e4
			 0xc59a8ffe, /// 0x4e8
			 0x00036b6f, /// 0x4ec
			 0xfa2bc66a, /// 0x4f0
			 0x26cef3ec, /// 0x4f4
			 0x7eaa923b, /// 0x4f8
			 0x1b6564b3, /// 0x4fc
			 0x017cbee2, /// 0x500
			 0xe280e621, /// 0x504
			 0x9627fb8f, /// 0x508
			 0xe64d6dc0, /// 0x50c
			 0x5826c460, /// 0x510
			 0x31dd7ce7, /// 0x514
			 0xf2b521c5, /// 0x518
			 0xff37d778, /// 0x51c
			 0x5ed7aa18, /// 0x520
			 0x1dd7b066, /// 0x524
			 0x30fb3739, /// 0x528
			 0x8b5fcc3b, /// 0x52c
			 0xf24a8f86, /// 0x530
			 0x095cca08, /// 0x534
			 0xf213f751, /// 0x538
			 0x26d0c771, /// 0x53c
			 0xa1844d45, /// 0x540
			 0x6612ecaa, /// 0x544
			 0xc0901c5c, /// 0x548
			 0x1a8760bf, /// 0x54c
			 0xa8b808e4, /// 0x550
			 0x9da566af, /// 0x554
			 0xa3508924, /// 0x558
			 0x778a57b8, /// 0x55c
			 0x9a8a3b6c, /// 0x560
			 0x6d2520ba, /// 0x564
			 0x4c5fcbb9, /// 0x568
			 0xd3deebb0, /// 0x56c
			 0xf68b5f34, /// 0x570
			 0x72ffa432, /// 0x574
			 0xceed2dff, /// 0x578
			 0x43610b7b, /// 0x57c
			 0x3512a2ef, /// 0x580
			 0x138ce4d2, /// 0x584
			 0x9e56dea5, /// 0x588
			 0x03f016ba, /// 0x58c
			 0xfb3c6230, /// 0x590
			 0x7e6e45f0, /// 0x594
			 0x2a4d4bbe, /// 0x598
			 0x89ca3eb5, /// 0x59c
			 0xe08fbb27, /// 0x5a0
			 0xf46adad1, /// 0x5a4
			 0xb6e4baba, /// 0x5a8
			 0xb113f24e, /// 0x5ac
			 0x1ae95bbd, /// 0x5b0
			 0xf72b7fa1, /// 0x5b4
			 0x5f635e6e, /// 0x5b8
			 0x3d12d855, /// 0x5bc
			 0x0984d5aa, /// 0x5c0
			 0x844b6ba3, /// 0x5c4
			 0x4a221d81, /// 0x5c8
			 0x3bfa23fb, /// 0x5cc
			 0x2925bdf1, /// 0x5d0
			 0x4486db63, /// 0x5d4
			 0xd5d7861f, /// 0x5d8
			 0xf45d976d, /// 0x5dc
			 0xc25918a8, /// 0x5e0
			 0x7bc6af71, /// 0x5e4
			 0xe04d3f01, /// 0x5e8
			 0x1421d7f0, /// 0x5ec
			 0xdd031bf7, /// 0x5f0
			 0x280cb434, /// 0x5f4
			 0xad01193c, /// 0x5f8
			 0x62f8b891, /// 0x5fc
			 0x54f488e0, /// 0x600
			 0x3f2ba8e6, /// 0x604
			 0xd8510d6c, /// 0x608
			 0x909c8ff5, /// 0x60c
			 0xfb59d7ff, /// 0x610
			 0x35e3b280, /// 0x614
			 0x4aa10139, /// 0x618
			 0x6d0c6af7, /// 0x61c
			 0xb1adfdfd, /// 0x620
			 0xa8cc1e51, /// 0x624
			 0x24d29d2a, /// 0x628
			 0x8f4ce2cf, /// 0x62c
			 0x4b3139de, /// 0x630
			 0xd08421bf, /// 0x634
			 0xdeac5e07, /// 0x638
			 0xfe6ce4ae, /// 0x63c
			 0x2077b1b6, /// 0x640
			 0x13c008cb, /// 0x644
			 0x7230c05a, /// 0x648
			 0xfe800c3d, /// 0x64c
			 0x3ea835df, /// 0x650
			 0xafc04fec, /// 0x654
			 0xa6f65a90, /// 0x658
			 0xb8f4af8f, /// 0x65c
			 0xc5ecebdd, /// 0x660
			 0x1bfd78dc, /// 0x664
			 0x8ece801b, /// 0x668
			 0xe13b8015, /// 0x66c
			 0x0005287f, /// 0x670
			 0x103b7efa, /// 0x674
			 0x35bc798e, /// 0x678
			 0x95c00f4e, /// 0x67c
			 0x56ec0d3d, /// 0x680
			 0xf81ca02e, /// 0x684
			 0xd7886635, /// 0x688
			 0x5184b59e, /// 0x68c
			 0xc14e3830, /// 0x690
			 0x4b2e60d2, /// 0x694
			 0xf35b6967, /// 0x698
			 0x5278579a, /// 0x69c
			 0xae3900a4, /// 0x6a0
			 0x2a903c59, /// 0x6a4
			 0x1305115c, /// 0x6a8
			 0xe7a5b914, /// 0x6ac
			 0xd142ad02, /// 0x6b0
			 0x7e15e1a8, /// 0x6b4
			 0x31932f9e, /// 0x6b8
			 0x9f593bdc, /// 0x6bc
			 0x763a8b18, /// 0x6c0
			 0x8ee3bd9e, /// 0x6c4
			 0xd33c18e0, /// 0x6c8
			 0xa9e7ab47, /// 0x6cc
			 0x9d2a5bd5, /// 0x6d0
			 0x2902f200, /// 0x6d4
			 0x331b0349, /// 0x6d8
			 0x1638db45, /// 0x6dc
			 0xb843320b, /// 0x6e0
			 0xcb0727fa, /// 0x6e4
			 0x69f06709, /// 0x6e8
			 0xfeb44e78, /// 0x6ec
			 0x2c8a08a3, /// 0x6f0
			 0xeb3f574c, /// 0x6f4
			 0xd00fd564, /// 0x6f8
			 0x96119f24, /// 0x6fc
			 0x9f770b76, /// 0x700
			 0x5b3c47d8, /// 0x704
			 0x704e982b, /// 0x708
			 0x5b6aa6bf, /// 0x70c
			 0xd94bfb7d, /// 0x710
			 0xc3eddab0, /// 0x714
			 0xa2cd91d5, /// 0x718
			 0x21f4a98e, /// 0x71c
			 0x24cc7f54, /// 0x720
			 0xfba2b6b2, /// 0x724
			 0xc4458211, /// 0x728
			 0x5da56841, /// 0x72c
			 0x15a1d553, /// 0x730
			 0xd3ffae92, /// 0x734
			 0xd9992095, /// 0x738
			 0xe2246e83, /// 0x73c
			 0xb597b350, /// 0x740
			 0x4085b024, /// 0x744
			 0xca9b6b31, /// 0x748
			 0xce38ab48, /// 0x74c
			 0x4877792a, /// 0x750
			 0xca6d7840, /// 0x754
			 0x723d0614, /// 0x758
			 0xceb71777, /// 0x75c
			 0x09f34b9a, /// 0x760
			 0x7d8a56ab, /// 0x764
			 0x93f54663, /// 0x768
			 0x505bf647, /// 0x76c
			 0x20cf91ff, /// 0x770
			 0x44a50b51, /// 0x774
			 0x8dbc47b8, /// 0x778
			 0xa4f89e90, /// 0x77c
			 0x340fff07, /// 0x780
			 0x0c02660f, /// 0x784
			 0x11043e0c, /// 0x788
			 0xb5c9c25b, /// 0x78c
			 0x450fdce0, /// 0x790
			 0xa758c6b8, /// 0x794
			 0x36465f78, /// 0x798
			 0x8c0afb04, /// 0x79c
			 0xb361c056, /// 0x7a0
			 0x60489f02, /// 0x7a4
			 0x395f1ada, /// 0x7a8
			 0xdede0c5d, /// 0x7ac
			 0x3b579e92, /// 0x7b0
			 0x1cebed88, /// 0x7b4
			 0xb3cc35b2, /// 0x7b8
			 0x1fef3f61, /// 0x7bc
			 0xf051b6ef, /// 0x7c0
			 0xe9d5c888, /// 0x7c4
			 0x5a7fbfb9, /// 0x7c8
			 0x85302855, /// 0x7cc
			 0x466fe290, /// 0x7d0
			 0x7b82e481, /// 0x7d4
			 0xbeefea8c, /// 0x7d8
			 0x928ecf72, /// 0x7dc
			 0x6dd542ac, /// 0x7e0
			 0x0513238e, /// 0x7e4
			 0x96907f23, /// 0x7e8
			 0x42aa0166, /// 0x7ec
			 0x4521218e, /// 0x7f0
			 0x23429af8, /// 0x7f4
			 0x06dde680, /// 0x7f8
			 0xc18fe921, /// 0x7fc
			 0x5ef170be, /// 0x800
			 0xae5d69bd, /// 0x804
			 0xd4ccb1a8, /// 0x808
			 0x8c0b5cae, /// 0x80c
			 0xaabc0caf, /// 0x810
			 0xa9a18ace, /// 0x814
			 0x709a1b80, /// 0x818
			 0xdfcd1eb5, /// 0x81c
			 0x5ba331ca, /// 0x820
			 0xb8b13747, /// 0x824
			 0xd606978c, /// 0x828
			 0xcdda7ac7, /// 0x82c
			 0x84579934, /// 0x830
			 0x6406e3a6, /// 0x834
			 0x87c5c8df, /// 0x838
			 0x9fead5e8, /// 0x83c
			 0xe90ba20a, /// 0x840
			 0x7fbc8697, /// 0x844
			 0x8cba9137, /// 0x848
			 0xf50be138, /// 0x84c
			 0xaca6c910, /// 0x850
			 0x96a7803d, /// 0x854
			 0x2d8262f3, /// 0x858
			 0xf67ffc06, /// 0x85c
			 0x022609f3, /// 0x860
			 0x0a291983, /// 0x864
			 0x0fca59da, /// 0x868
			 0xcea2ed2f, /// 0x86c
			 0xc158e884, /// 0x870
			 0xa1db6ef0, /// 0x874
			 0x3709da9d, /// 0x878
			 0x8228017f, /// 0x87c
			 0x4eb55d33, /// 0x880
			 0x1a84d0b3, /// 0x884
			 0x8d4c4acd, /// 0x888
			 0x14af93f8, /// 0x88c
			 0xd0292210, /// 0x890
			 0x37270eaa, /// 0x894
			 0x48b70870, /// 0x898
			 0x369f156c, /// 0x89c
			 0x67c5e760, /// 0x8a0
			 0xaff98766, /// 0x8a4
			 0x4bdda543, /// 0x8a8
			 0x9ccae2bf, /// 0x8ac
			 0xf4ae0cec, /// 0x8b0
			 0xff733e46, /// 0x8b4
			 0x93ae0a6b, /// 0x8b8
			 0x60b0ce34, /// 0x8bc
			 0xdb0fa905, /// 0x8c0
			 0x0b758a43, /// 0x8c4
			 0xe387fae0, /// 0x8c8
			 0x4c9733a5, /// 0x8cc
			 0x1b723de4, /// 0x8d0
			 0x29482ab8, /// 0x8d4
			 0xb40c7530, /// 0x8d8
			 0xf71dc62c, /// 0x8dc
			 0xb64e2758, /// 0x8e0
			 0xe0401003, /// 0x8e4
			 0xe66abd28, /// 0x8e8
			 0x19ab9dd1, /// 0x8ec
			 0x191a6352, /// 0x8f0
			 0xb39e95b6, /// 0x8f4
			 0xa30613b4, /// 0x8f8
			 0xd9ccec34, /// 0x8fc
			 0xd0594f8f, /// 0x900
			 0xfaf294ce, /// 0x904
			 0xa23c3a5c, /// 0x908
			 0x1cfacd46, /// 0x90c
			 0x96a26c71, /// 0x910
			 0x22e8ade0, /// 0x914
			 0x79504042, /// 0x918
			 0xce4e735c, /// 0x91c
			 0xa299cbd5, /// 0x920
			 0x3822884e, /// 0x924
			 0xbc7085ec, /// 0x928
			 0x545f6c99, /// 0x92c
			 0xf2bf859e, /// 0x930
			 0xdbe977ff, /// 0x934
			 0xad911964, /// 0x938
			 0x99e83402, /// 0x93c
			 0xece2f050, /// 0x940
			 0xdefc8033, /// 0x944
			 0xbc65313f, /// 0x948
			 0xd3710493, /// 0x94c
			 0x4910f7fe, /// 0x950
			 0xd02187be, /// 0x954
			 0x7c91aa99, /// 0x958
			 0x6ac6d463, /// 0x95c
			 0xf70f91b1, /// 0x960
			 0xaaad7ff2, /// 0x964
			 0xedb4abdd, /// 0x968
			 0x20860aa7, /// 0x96c
			 0x1c445794, /// 0x970
			 0x93f9eaf1, /// 0x974
			 0x36df73da, /// 0x978
			 0xaeb1586e, /// 0x97c
			 0x276954ad, /// 0x980
			 0xfe3fa26e, /// 0x984
			 0xac6eb912, /// 0x988
			 0xf000a31c, /// 0x98c
			 0x9039a48d, /// 0x990
			 0xd92c5cc1, /// 0x994
			 0xf97a5083, /// 0x998
			 0xb86f50d7, /// 0x99c
			 0xc8ea3910, /// 0x9a0
			 0xf74b77c4, /// 0x9a4
			 0xf2f3921d, /// 0x9a8
			 0x49348983, /// 0x9ac
			 0x13219636, /// 0x9b0
			 0x2f49c3d0, /// 0x9b4
			 0x8606a19c, /// 0x9b8
			 0x6654176c, /// 0x9bc
			 0x221ed76e, /// 0x9c0
			 0xfe9f44e2, /// 0x9c4
			 0x6b11b6d3, /// 0x9c8
			 0xa8b79f81, /// 0x9cc
			 0x8766cb24, /// 0x9d0
			 0x6c65144e, /// 0x9d4
			 0x3e0bdc5b, /// 0x9d8
			 0x6c642ac0, /// 0x9dc
			 0xc9a781b8, /// 0x9e0
			 0xd24db702, /// 0x9e4
			 0x8ad4601b, /// 0x9e8
			 0xb2f8af0d, /// 0x9ec
			 0xb0aa1e8e, /// 0x9f0
			 0x208277eb, /// 0x9f4
			 0x8d492083, /// 0x9f8
			 0xa0112a1a, /// 0x9fc
			 0x587db8f0, /// 0xa00
			 0xea503d41, /// 0xa04
			 0x35abfa76, /// 0xa08
			 0x668acb9c, /// 0xa0c
			 0x1155e712, /// 0xa10
			 0x1a8a5f8c, /// 0xa14
			 0xf92e6a1a, /// 0xa18
			 0x648679e3, /// 0xa1c
			 0xfa3c5d97, /// 0xa20
			 0x2d63ac36, /// 0xa24
			 0xe5b28e98, /// 0xa28
			 0xbe2c91eb, /// 0xa2c
			 0x29cd9b13, /// 0xa30
			 0x0f3f2639, /// 0xa34
			 0xda5b3400, /// 0xa38
			 0x3c35b2c7, /// 0xa3c
			 0x11408c95, /// 0xa40
			 0xb52d50ff, /// 0xa44
			 0x2dc80e7c, /// 0xa48
			 0x875b512c, /// 0xa4c
			 0xd40037fa, /// 0xa50
			 0x8ed9c126, /// 0xa54
			 0x779aa527, /// 0xa58
			 0xab87335c, /// 0xa5c
			 0xbe416c8b, /// 0xa60
			 0x8800313e, /// 0xa64
			 0x67769518, /// 0xa68
			 0xf2ae0f6e, /// 0xa6c
			 0x46a3796e, /// 0xa70
			 0x80a5ba52, /// 0xa74
			 0x2097189e, /// 0xa78
			 0x2f768e1d, /// 0xa7c
			 0x9594af76, /// 0xa80
			 0xb4e975ab, /// 0xa84
			 0x61c919c7, /// 0xa88
			 0xd5937487, /// 0xa8c
			 0x66e251a3, /// 0xa90
			 0x78fb2f8b, /// 0xa94
			 0x04342e7c, /// 0xa98
			 0xab277c21, /// 0xa9c
			 0xe6297cea, /// 0xaa0
			 0x5dbed551, /// 0xaa4
			 0x0f597d47, /// 0xaa8
			 0xdf7e7383, /// 0xaac
			 0x6757d9bd, /// 0xab0
			 0x48555780, /// 0xab4
			 0x1d3b48b9, /// 0xab8
			 0xe4d9fc93, /// 0xabc
			 0x2c9afbae, /// 0xac0
			 0xcf2277a6, /// 0xac4
			 0x7ac4c0c5, /// 0xac8
			 0x58ec7b98, /// 0xacc
			 0xb6ad81d9, /// 0xad0
			 0x4d19cc62, /// 0xad4
			 0x9f52c957, /// 0xad8
			 0x1e81ec40, /// 0xadc
			 0xa5996f77, /// 0xae0
			 0x1cc6e95a, /// 0xae4
			 0x76adcc1c, /// 0xae8
			 0x7ca0981e, /// 0xaec
			 0xa319638f, /// 0xaf0
			 0x206851e2, /// 0xaf4
			 0x64df7d2a, /// 0xaf8
			 0x75a448b2, /// 0xafc
			 0xf0c7ff5a, /// 0xb00
			 0x7f22606c, /// 0xb04
			 0x840cfb18, /// 0xb08
			 0x4e036888, /// 0xb0c
			 0x30e54502, /// 0xb10
			 0x9bc84fca, /// 0xb14
			 0x0a38cb61, /// 0xb18
			 0x8f9b2ad9, /// 0xb1c
			 0xb634736c, /// 0xb20
			 0x226c4395, /// 0xb24
			 0x5e37d3e3, /// 0xb28
			 0xa3a12689, /// 0xb2c
			 0x77843cb4, /// 0xb30
			 0x7f10bf8c, /// 0xb34
			 0x4c529236, /// 0xb38
			 0x1a03365f, /// 0xb3c
			 0xae7c5711, /// 0xb40
			 0xed4ad6a6, /// 0xb44
			 0xce9faf81, /// 0xb48
			 0xa78d5707, /// 0xb4c
			 0x3609ba8c, /// 0xb50
			 0x9257d340, /// 0xb54
			 0xf2f05efc, /// 0xb58
			 0x6384627a, /// 0xb5c
			 0x2607200c, /// 0xb60
			 0x7f5a0b75, /// 0xb64
			 0x1268a553, /// 0xb68
			 0xd8e2fbff, /// 0xb6c
			 0x36207164, /// 0xb70
			 0x62602c51, /// 0xb74
			 0x73d539a5, /// 0xb78
			 0x26e75dc4, /// 0xb7c
			 0xa938c700, /// 0xb80
			 0x53a60d3c, /// 0xb84
			 0x8d0f6af0, /// 0xb88
			 0x8d3971d7, /// 0xb8c
			 0xca7dee9b, /// 0xb90
			 0x0b02a43d, /// 0xb94
			 0x3b6cc8c9, /// 0xb98
			 0x1d03d842, /// 0xb9c
			 0x495c9a89, /// 0xba0
			 0x59ce2618, /// 0xba4
			 0x7f56a477, /// 0xba8
			 0x8c4a1fb8, /// 0xbac
			 0x3ecd2412, /// 0xbb0
			 0x31233f1e, /// 0xbb4
			 0x0f6a824c, /// 0xbb8
			 0xbc5ab608, /// 0xbbc
			 0x5c7b0a75, /// 0xbc0
			 0x252310a8, /// 0xbc4
			 0x93a3303c, /// 0xbc8
			 0x73d437cb, /// 0xbcc
			 0xed0931fd, /// 0xbd0
			 0x99c0f92c, /// 0xbd4
			 0xbe93b9a8, /// 0xbd8
			 0x4f4844cb, /// 0xbdc
			 0x4d29a0e3, /// 0xbe0
			 0xa5814cfb, /// 0xbe4
			 0xe4ccb510, /// 0xbe8
			 0x4a3b6786, /// 0xbec
			 0x9269c7d0, /// 0xbf0
			 0xca106bb7, /// 0xbf4
			 0x396a849f, /// 0xbf8
			 0xedd444f7, /// 0xbfc
			 0x7206c814, /// 0xc00
			 0x827cdc0d, /// 0xc04
			 0x150f234c, /// 0xc08
			 0xf3f5e255, /// 0xc0c
			 0xa7fa5430, /// 0xc10
			 0x8ac01643, /// 0xc14
			 0xd6d8a3b5, /// 0xc18
			 0x6d7f4569, /// 0xc1c
			 0x5fb0475c, /// 0xc20
			 0x9eff6ac7, /// 0xc24
			 0xfe61ac75, /// 0xc28
			 0x7f87c4c0, /// 0xc2c
			 0x140249e2, /// 0xc30
			 0xbad9a0fa, /// 0xc34
			 0x76e21f1b, /// 0xc38
			 0x0a34be80, /// 0xc3c
			 0x82eb5bbe, /// 0xc40
			 0xc77a2250, /// 0xc44
			 0xaa03a11f, /// 0xc48
			 0x9d56b638, /// 0xc4c
			 0x19105c44, /// 0xc50
			 0x5bb64ac7, /// 0xc54
			 0x4546a337, /// 0xc58
			 0x2ba38996, /// 0xc5c
			 0xd6ea2e7a, /// 0xc60
			 0x44ae5b38, /// 0xc64
			 0x01875d9e, /// 0xc68
			 0x4cc84977, /// 0xc6c
			 0x2434c88e, /// 0xc70
			 0x35ae2054, /// 0xc74
			 0x2290cb75, /// 0xc78
			 0x5c884258, /// 0xc7c
			 0xf33bf62d, /// 0xc80
			 0x0fec083d, /// 0xc84
			 0x011b4aa7, /// 0xc88
			 0x7469f3a6, /// 0xc8c
			 0xb9384a32, /// 0xc90
			 0x6912955f, /// 0xc94
			 0xd204add3, /// 0xc98
			 0x3d644317, /// 0xc9c
			 0xc5655fc8, /// 0xca0
			 0x65f36f9c, /// 0xca4
			 0x99a3544d, /// 0xca8
			 0x9155a3f7, /// 0xcac
			 0x2178a1ef, /// 0xcb0
			 0x4454abe8, /// 0xcb4
			 0x37ca524f, /// 0xcb8
			 0x4e236914, /// 0xcbc
			 0x85e06cb1, /// 0xcc0
			 0xf3eb8d9e, /// 0xcc4
			 0x483e272a, /// 0xcc8
			 0x3a19a86c, /// 0xccc
			 0x6ae29e16, /// 0xcd0
			 0x068e4b3b, /// 0xcd4
			 0xa18b2059, /// 0xcd8
			 0xceb06fd4, /// 0xcdc
			 0x8efa8a57, /// 0xce0
			 0x8b18b128, /// 0xce4
			 0xb2ff932e, /// 0xce8
			 0x10a7b37d, /// 0xcec
			 0x65795547, /// 0xcf0
			 0x869af66d, /// 0xcf4
			 0x83b91c6e, /// 0xcf8
			 0x43d198b6, /// 0xcfc
			 0xdebbe759, /// 0xd00
			 0xd6fa3628, /// 0xd04
			 0x36656d2f, /// 0xd08
			 0x2abb60de, /// 0xd0c
			 0xc606d4d7, /// 0xd10
			 0x8a38c1d5, /// 0xd14
			 0x036e026f, /// 0xd18
			 0x5eb54951, /// 0xd1c
			 0x105feffd, /// 0xd20
			 0x440a16e6, /// 0xd24
			 0x37bafb20, /// 0xd28
			 0x16d7c5ca, /// 0xd2c
			 0xe43815b3, /// 0xd30
			 0x511d8980, /// 0xd34
			 0xa180f004, /// 0xd38
			 0xb98eda36, /// 0xd3c
			 0x5a69d35b, /// 0xd40
			 0x42367e0a, /// 0xd44
			 0x75303afd, /// 0xd48
			 0xbf311942, /// 0xd4c
			 0x8028ed69, /// 0xd50
			 0x57e645bd, /// 0xd54
			 0xe8374b54, /// 0xd58
			 0xbdd0800f, /// 0xd5c
			 0xb3c1d29d, /// 0xd60
			 0xb5b2db56, /// 0xd64
			 0x826bb77d, /// 0xd68
			 0xfff2278a, /// 0xd6c
			 0x0b4841b4, /// 0xd70
			 0xcaeb4329, /// 0xd74
			 0x1c3e0b4a, /// 0xd78
			 0x5ad8f1a9, /// 0xd7c
			 0xfe3fdf26, /// 0xd80
			 0x19acd41e, /// 0xd84
			 0x46a576bf, /// 0xd88
			 0x5b4a3cff, /// 0xd8c
			 0x563504e3, /// 0xd90
			 0xd5f8d0c6, /// 0xd94
			 0x47c2c440, /// 0xd98
			 0x0f8b4747, /// 0xd9c
			 0x3d432c16, /// 0xda0
			 0x8a579467, /// 0xda4
			 0x92d35683, /// 0xda8
			 0x3429f526, /// 0xdac
			 0xf82a5093, /// 0xdb0
			 0xc231a6e8, /// 0xdb4
			 0x73291f32, /// 0xdb8
			 0x2cbd5883, /// 0xdbc
			 0xfb6ff5a8, /// 0xdc0
			 0xb69f9ac3, /// 0xdc4
			 0x610e0799, /// 0xdc8
			 0x08df2ab5, /// 0xdcc
			 0x3d69e669, /// 0xdd0
			 0x145e79e8, /// 0xdd4
			 0x34b632d8, /// 0xdd8
			 0x2b3ab663, /// 0xddc
			 0x2c30c12f, /// 0xde0
			 0x006be812, /// 0xde4
			 0x3959c65e, /// 0xde8
			 0x13ffe957, /// 0xdec
			 0x115fe018, /// 0xdf0
			 0x4004a8ab, /// 0xdf4
			 0x1bdab9fd, /// 0xdf8
			 0xe365225a, /// 0xdfc
			 0xe834d237, /// 0xe00
			 0x0c5b944a, /// 0xe04
			 0xb911d86f, /// 0xe08
			 0x9ad9faf8, /// 0xe0c
			 0xe78f1c44, /// 0xe10
			 0xd5870618, /// 0xe14
			 0x08910b64, /// 0xe18
			 0x6979887c, /// 0xe1c
			 0x96a41c3d, /// 0xe20
			 0x1275055b, /// 0xe24
			 0xb27a1fb6, /// 0xe28
			 0x5c5262cf, /// 0xe2c
			 0x6e741846, /// 0xe30
			 0x95bd5fca, /// 0xe34
			 0x980f1de7, /// 0xe38
			 0x9a11c0fc, /// 0xe3c
			 0x894b61d0, /// 0xe40
			 0x3e7985fe, /// 0xe44
			 0xb3b8fe6a, /// 0xe48
			 0xfaeeaa0c, /// 0xe4c
			 0x2d2ba626, /// 0xe50
			 0xe3fb64cb, /// 0xe54
			 0x15e58a84, /// 0xe58
			 0xb0524738, /// 0xe5c
			 0x7b6c31a5, /// 0xe60
			 0x632ad68a, /// 0xe64
			 0xfd55be1c, /// 0xe68
			 0xea7367ab, /// 0xe6c
			 0x80469c4d, /// 0xe70
			 0xfedf1722, /// 0xe74
			 0xf9f5470b, /// 0xe78
			 0x641e61bf, /// 0xe7c
			 0x1150e223, /// 0xe80
			 0x351fc2d0, /// 0xe84
			 0xec295be1, /// 0xe88
			 0xc79672e6, /// 0xe8c
			 0xf893957c, /// 0xe90
			 0x35d4680e, /// 0xe94
			 0x3ad2005f, /// 0xe98
			 0x22f76852, /// 0xe9c
			 0xb9892826, /// 0xea0
			 0xb3ceddc4, /// 0xea4
			 0x6a447c91, /// 0xea8
			 0x09fd80eb, /// 0xeac
			 0x4df0aa2e, /// 0xeb0
			 0x6cec4ccc, /// 0xeb4
			 0xfc06ef45, /// 0xeb8
			 0x409b27f9, /// 0xebc
			 0xc9da1541, /// 0xec0
			 0x15d92c56, /// 0xec4
			 0x4afd3a92, /// 0xec8
			 0x674b5361, /// 0xecc
			 0xc8aff823, /// 0xed0
			 0x3283a8cf, /// 0xed4
			 0x8c65dbf6, /// 0xed8
			 0x2ca31b5a, /// 0xedc
			 0x0cc24d02, /// 0xee0
			 0x40fa1c92, /// 0xee4
			 0x0d6f2cc0, /// 0xee8
			 0xb6e3d60e, /// 0xeec
			 0xbed7b233, /// 0xef0
			 0x2538d87b, /// 0xef4
			 0x3797a7f5, /// 0xef8
			 0xf7fbdc50, /// 0xefc
			 0xb9a7f991, /// 0xf00
			 0xc6d45d31, /// 0xf04
			 0x4506de60, /// 0xf08
			 0xf479d662, /// 0xf0c
			 0x3f90a03f, /// 0xf10
			 0x4f93cec1, /// 0xf14
			 0x5d53cb02, /// 0xf18
			 0x7bf0eec0, /// 0xf1c
			 0xfaa87900, /// 0xf20
			 0x53089017, /// 0xf24
			 0x407b0d71, /// 0xf28
			 0x9a26a013, /// 0xf2c
			 0xcd16a221, /// 0xf30
			 0xb39c3c11, /// 0xf34
			 0x7cccb070, /// 0xf38
			 0x70a1ab51, /// 0xf3c
			 0x27813b38, /// 0xf40
			 0x12c91939, /// 0xf44
			 0x07b4b3b0, /// 0xf48
			 0xc3ea9fb1, /// 0xf4c
			 0x931ae8fc, /// 0xf50
			 0xc09a5713, /// 0xf54
			 0xdf7c12df, /// 0xf58
			 0x1bbb621d, /// 0xf5c
			 0xf981a776, /// 0xf60
			 0xa84a6f24, /// 0xf64
			 0xadfd0206, /// 0xf68
			 0x955fc47f, /// 0xf6c
			 0x7455eca0, /// 0xf70
			 0xcb302848, /// 0xf74
			 0x1b079851, /// 0xf78
			 0xb989b627, /// 0xf7c
			 0x69299dfb, /// 0xf80
			 0x54b26676, /// 0xf84
			 0x6caf0a07, /// 0xf88
			 0x24f227bf, /// 0xf8c
			 0xee0431ed, /// 0xf90
			 0x62992e3a, /// 0xf94
			 0x27223436, /// 0xf98
			 0x77ad2d16, /// 0xf9c
			 0x10e91d9c, /// 0xfa0
			 0x72f82209, /// 0xfa4
			 0xccaae15d, /// 0xfa8
			 0x9546e897, /// 0xfac
			 0x52c99fa7, /// 0xfb0
			 0xa12701d6, /// 0xfb4
			 0x17248fa4, /// 0xfb8
			 0xcac963a0, /// 0xfbc
			 0xde0573ff, /// 0xfc0
			 0xe9c6f36b, /// 0xfc4
			 0x4716e1d3, /// 0xfc8
			 0x9bd973cf, /// 0xfcc
			 0x930e6ca4, /// 0xfd0
			 0xcd083365, /// 0xfd4
			 0x07117d1b, /// 0xfd8
			 0x6f52cc14, /// 0xfdc
			 0x08ce0267, /// 0xfe0
			 0xdcad78aa, /// 0xfe4
			 0x0f2e3abc, /// 0xfe8
			 0x8d49ab9c, /// 0xfec
			 0xb40b30f6, /// 0xff0
			 0xcf92829e, /// 0xff4
			 0xd3ae29d9, /// 0xff8
			 0x635f141e /// last
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
			 0x00000318,
			 0x00000218,
			 0x000007c4,
			 0x00000768,
			 0x0000075c,
			 0x0000047c,
			 0x00000408,
			 0x00000504,

			 /// vpu register f2
			 0x41000000,
			 0x41d00000,
			 0x41b80000,
			 0x3f800000,
			 0x41c80000,
			 0x41a00000,
			 0x41400000,
			 0x41d00000,

			 /// vpu register f3
			 0x41300000,
			 0x41f00000,
			 0x41b00000,
			 0x41600000,
			 0x41e00000,
			 0x41880000,
			 0x41a00000,
			 0x41880000,

			 /// vpu register f4
			 0x41a80000,
			 0x41b00000,
			 0x41a00000,
			 0x41b80000,
			 0x41b00000,
			 0x41d00000,
			 0x40400000,
			 0x3f800000,

			 /// vpu register f5
			 0x41f80000,
			 0x41700000,
			 0x41300000,
			 0x41900000,
			 0x41d00000,
			 0x41600000,
			 0x41900000,
			 0x41000000,

			 /// vpu register f6
			 0x41c00000,
			 0x3f800000,
			 0x41a00000,
			 0x41980000,
			 0x41c80000,
			 0x41000000,
			 0x41600000,
			 0x41f80000,

			 /// vpu register f7
			 0x40000000,
			 0x41f00000,
			 0x41000000,
			 0x40400000,
			 0x41c00000,
			 0x41600000,
			 0x40400000,
			 0x41e80000,

			 /// vpu register f8
			 0x41f80000,
			 0x40e00000,
			 0x41500000,
			 0x41200000,
			 0x40400000,
			 0x41a00000,
			 0x41400000,
			 0x41e80000,

			 /// vpu register f9
			 0x41880000,
			 0x40800000,
			 0x41e00000,
			 0x41700000,
			 0x42000000,
			 0x41f00000,
			 0x41980000,
			 0x41b80000,

			 /// vpu register f10
			 0x41700000,
			 0x41000000,
			 0x41900000,
			 0x41d80000,
			 0x41100000,
			 0x40800000,
			 0x40a00000,
			 0x41500000,

			 /// vpu register f11
			 0x41900000,
			 0x41880000,
			 0x41900000,
			 0x40a00000,
			 0x41f80000,
			 0x41c80000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f12
			 0x42000000,
			 0x41300000,
			 0x41a80000,
			 0x41700000,
			 0x41700000,
			 0x40800000,
			 0x41c00000,
			 0x41200000,

			 /// vpu register f13
			 0x41600000,
			 0x41300000,
			 0x40c00000,
			 0x41f00000,
			 0x40e00000,
			 0x41600000,
			 0x41d80000,
			 0x41c00000,

			 /// vpu register f14
			 0x41f00000,
			 0x41a00000,
			 0x41400000,
			 0x41f00000,
			 0x41300000,
			 0x40800000,
			 0x40400000,
			 0x42000000,

			 /// vpu register f15
			 0x40400000,
			 0x41f80000,
			 0x41d80000,
			 0x41500000,
			 0x41e00000,
			 0x41600000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f16
			 0x40000000,
			 0x41700000,
			 0x41a80000,
			 0x41400000,
			 0x41d80000,
			 0x41d00000,
			 0x41d00000,
			 0x41400000,

			 /// vpu register f17
			 0x3f800000,
			 0x40c00000,
			 0x41300000,
			 0x3f800000,
			 0x41b00000,
			 0x41b00000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f18
			 0x41b80000,
			 0x40400000,
			 0x41c80000,
			 0x40a00000,
			 0x42000000,
			 0x41f00000,
			 0x41c00000,
			 0x41d80000,

			 /// vpu register f19
			 0x41c00000,
			 0x41a80000,
			 0x41200000,
			 0x41600000,
			 0x41e00000,
			 0x41d80000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f20
			 0x41880000,
			 0x41a00000,
			 0x41800000,
			 0x41800000,
			 0x41700000,
			 0x40e00000,
			 0x40800000,
			 0x41600000,

			 /// vpu register f21
			 0x42000000,
			 0x41100000,
			 0x41d00000,
			 0x41500000,
			 0x41000000,
			 0x41a00000,
			 0x41980000,
			 0x41a00000,

			 /// vpu register f22
			 0x40000000,
			 0x41200000,
			 0x41e00000,
			 0x42000000,
			 0x41f80000,
			 0x3f800000,
			 0x41a00000,
			 0x41c80000,

			 /// vpu register f23
			 0x41f00000,
			 0x41c80000,
			 0x41c00000,
			 0x41800000,
			 0x41600000,
			 0x41a00000,
			 0x42000000,
			 0x41e80000,

			 /// vpu register f24
			 0x40a00000,
			 0x40c00000,
			 0x41500000,
			 0x41b80000,
			 0x41c80000,
			 0x41b80000,
			 0x41f80000,
			 0x41300000,

			 /// vpu register f25
			 0x41800000,
			 0x40e00000,
			 0x40800000,
			 0x41d80000,
			 0x41b00000,
			 0x3f800000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f26
			 0x40a00000,
			 0x41c00000,
			 0x41500000,
			 0x41200000,
			 0x41000000,
			 0x40000000,
			 0x41600000,
			 0x3f800000,

			 /// vpu register f27
			 0x40a00000,
			 0x41f80000,
			 0x41b00000,
			 0x41100000,
			 0x41300000,
			 0x41100000,
			 0x40400000,
			 0x41500000,

			 /// vpu register f28
			 0x41400000,
			 0x41f00000,
			 0x41a80000,
			 0x41600000,
			 0x41880000,
			 0x40000000,
			 0x41f80000,
			 0x40000000,

			 /// vpu register f29
			 0x41f00000,
			 0x41e80000,
			 0x41100000,
			 0x41500000,
			 0x40a00000,
			 0x41a80000,
			 0x41100000,
			 0x42000000,

			 /// vpu register f30
			 0x41d80000,
			 0x41400000,
			 0x41c80000,
			 0x41000000,
			 0x40000000,
			 0x41800000,
			 0x41d80000,
			 0x41b80000,

			 /// vpu register f31
			 0x41700000,
			 0x41b00000,
			 0x41d00000,
			 0x41700000,
			 0x40400000,
			 0x41c00000,
			 0x40c00000,
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
		"famoandg.pi f30, f1 (x14)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f27, f1 (x11)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f25, f1 (x13)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f12, f1 (x14)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f7, f1 (x12)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f15, f1 (x14)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f20, f1 (x14)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f10, f1 (x11)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f10, f1 (x15)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x13)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f29, f1 (x15)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f19, f1 (x11)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f21, f1 (x13)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f21, f1 (x11)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f13, f1 (x14)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f15, f1 (x12)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f8, f1 (x14)\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f7, f1 (x15)\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f30, f1 (x15)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f29, f1 (x11)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f13, f1 (x15)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f3, f1 (x13)\n"                          ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f17, f1 (x13)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x11)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f20, f1 (x12)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f4, f1 (x14)\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f7, f1 (x12)\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x13)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f27, f1 (x12)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f6, f1 (x14)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f5, f1 (x14)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f20, f1 (x14)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f30, f1 (x15)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f23, f1 (x13)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f11, f1 (x14)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f10, f1 (x12)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f25, f1 (x14)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f28, f1 (x12)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f19, f1 (x11)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f19, f1 (x13)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f17, f1 (x15)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f17, f1 (x11)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f24, f1 (x15)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f5, f1 (x11)\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f13, f1 (x14)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f28, f1 (x15)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f6, f1 (x13)\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f18, f1 (x12)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f26, f1 (x15)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f30, f1 (x12)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f6, f1 (x13)\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f17, f1 (x14)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f28, f1 (x12)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f7, f1 (x13)\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f2, f1 (x12)\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x14)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f13, f1 (x13)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f29, f1 (x12)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f17, f1 (x12)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f26, f1 (x15)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f10, f1 (x13)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f18, f1 (x15)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f19, f1 (x12)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f29, f1 (x15)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f22, f1 (x13)\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f5, f1 (x12)\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f27, f1 (x13)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f22, f1 (x14)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f4, f1 (x13)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f5, f1 (x14)\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f8, f1 (x14)\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f13, f1 (x11)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f23, f1 (x15)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f21, f1 (x15)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f6, f1 (x12)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f27, f1 (x14)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f5, f1 (x12)\n"                          ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f18, f1 (x15)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x11)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f27, f1 (x11)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f17, f1 (x13)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f30, f1 (x15)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f18, f1 (x13)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f8, f1 (x14)\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f10, f1 (x13)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f25, f1 (x12)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f19, f1 (x12)\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f27, f1 (x12)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x12)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f4, f1 (x13)\n"                          ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f19, f1 (x11)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f24, f1 (x14)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f10, f1 (x11)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x11)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f22, f1 (x14)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f25, f1 (x15)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f22, f1 (x14)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f9, f1 (x11)\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoandg.pi f14, f1 (x13)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
