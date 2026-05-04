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
			 0xc6cb8127, /// 0x0
			 0x02957082, /// 0x4
			 0x826ada25, /// 0x8
			 0xae1b9d14, /// 0xc
			 0x3a326b93, /// 0x10
			 0x2e71debf, /// 0x14
			 0xe8690b1f, /// 0x18
			 0x04417c39, /// 0x1c
			 0x39ac2b03, /// 0x20
			 0x0cbf071a, /// 0x24
			 0x47a06865, /// 0x28
			 0x3a9c5aa3, /// 0x2c
			 0xce6f33d4, /// 0x30
			 0xa9bda369, /// 0x34
			 0xa641c764, /// 0x38
			 0x280fbbd8, /// 0x3c
			 0xaf7fc855, /// 0x40
			 0x30d4f80e, /// 0x44
			 0x0bc98510, /// 0x48
			 0xecc70705, /// 0x4c
			 0x4cf5e061, /// 0x50
			 0xf65a8c09, /// 0x54
			 0xff02e5c7, /// 0x58
			 0xc49c0f17, /// 0x5c
			 0x1d9e952e, /// 0x60
			 0xfeec00f2, /// 0x64
			 0xa7da9592, /// 0x68
			 0x35f49d87, /// 0x6c
			 0xe29e784d, /// 0x70
			 0x4210ce69, /// 0x74
			 0x42dd0e7b, /// 0x78
			 0x4017d388, /// 0x7c
			 0x89a08bdd, /// 0x80
			 0x8efe2bf0, /// 0x84
			 0xfb2b2fa8, /// 0x88
			 0x204ef35c, /// 0x8c
			 0x860153e3, /// 0x90
			 0xa17b7d85, /// 0x94
			 0x0b23ab2f, /// 0x98
			 0x169045eb, /// 0x9c
			 0x4d93e88c, /// 0xa0
			 0x26da322b, /// 0xa4
			 0x9c01198d, /// 0xa8
			 0x9074caa7, /// 0xac
			 0xf87d3e66, /// 0xb0
			 0xa820ca4b, /// 0xb4
			 0x346b4f46, /// 0xb8
			 0x2b055cb5, /// 0xbc
			 0x9a05f911, /// 0xc0
			 0xf88eb127, /// 0xc4
			 0xa496f08b, /// 0xc8
			 0xe0457d5a, /// 0xcc
			 0x2d0afe30, /// 0xd0
			 0xf5a91e00, /// 0xd4
			 0x39238514, /// 0xd8
			 0x847fe059, /// 0xdc
			 0x07376e6b, /// 0xe0
			 0x32fc15c3, /// 0xe4
			 0x73903cd6, /// 0xe8
			 0xfa9a043e, /// 0xec
			 0xf0d1e027, /// 0xf0
			 0xf1d420b0, /// 0xf4
			 0xd9656dcf, /// 0xf8
			 0x63a8d204, /// 0xfc
			 0xcbb777e6, /// 0x100
			 0x9fffdca5, /// 0x104
			 0x8cd4f8ad, /// 0x108
			 0xd78570d4, /// 0x10c
			 0xe0f6e706, /// 0x110
			 0x38dee4de, /// 0x114
			 0x50e2042d, /// 0x118
			 0xf1cf7679, /// 0x11c
			 0x69f4396c, /// 0x120
			 0x8e2c5a89, /// 0x124
			 0x977cdb4d, /// 0x128
			 0x945ffab2, /// 0x12c
			 0x0b63d059, /// 0x130
			 0xacd3463a, /// 0x134
			 0x15551912, /// 0x138
			 0x3bcbf1a9, /// 0x13c
			 0x18e7aa68, /// 0x140
			 0x8529f740, /// 0x144
			 0x168ba11d, /// 0x148
			 0xef288323, /// 0x14c
			 0xb7916112, /// 0x150
			 0x2ea9cdfa, /// 0x154
			 0xf90352d7, /// 0x158
			 0x7d1d47c3, /// 0x15c
			 0x8ccccb4f, /// 0x160
			 0xf2fcf66b, /// 0x164
			 0x0b934b66, /// 0x168
			 0xbcb9a973, /// 0x16c
			 0xa82f85c1, /// 0x170
			 0x35663199, /// 0x174
			 0xe4a583e3, /// 0x178
			 0x2431926c, /// 0x17c
			 0x12d2e106, /// 0x180
			 0x52a37391, /// 0x184
			 0x08d5aac7, /// 0x188
			 0x5b03dda3, /// 0x18c
			 0xb7f9b611, /// 0x190
			 0xb97732b0, /// 0x194
			 0x6e3a400a, /// 0x198
			 0x6d309213, /// 0x19c
			 0x5e28d669, /// 0x1a0
			 0xf777fa2a, /// 0x1a4
			 0xa27c7434, /// 0x1a8
			 0x3a10308a, /// 0x1ac
			 0x79f7445d, /// 0x1b0
			 0x28c3174d, /// 0x1b4
			 0xdc45ecdc, /// 0x1b8
			 0x850a37e2, /// 0x1bc
			 0x560a229a, /// 0x1c0
			 0x61cecbd7, /// 0x1c4
			 0xda231ec5, /// 0x1c8
			 0x74e6023a, /// 0x1cc
			 0x719a6671, /// 0x1d0
			 0x5f03b51d, /// 0x1d4
			 0x671aca50, /// 0x1d8
			 0x31197b2d, /// 0x1dc
			 0xb5a09dc2, /// 0x1e0
			 0xa9c64508, /// 0x1e4
			 0x10eac48d, /// 0x1e8
			 0xbf388eac, /// 0x1ec
			 0x88ec6a6a, /// 0x1f0
			 0x8a3559e9, /// 0x1f4
			 0x702289bb, /// 0x1f8
			 0xfed6e9cf, /// 0x1fc
			 0x04b3c6c3, /// 0x200
			 0x9922ce07, /// 0x204
			 0xb5d26933, /// 0x208
			 0x3534154a, /// 0x20c
			 0x8d574113, /// 0x210
			 0x10e1babe, /// 0x214
			 0xc8eb7335, /// 0x218
			 0x74f5ed10, /// 0x21c
			 0x7d4cfe54, /// 0x220
			 0xc6afd023, /// 0x224
			 0x54a5d16a, /// 0x228
			 0xaac95489, /// 0x22c
			 0x8666a231, /// 0x230
			 0x3fd679b8, /// 0x234
			 0x46b801cf, /// 0x238
			 0x5505ab1c, /// 0x23c
			 0x3345384a, /// 0x240
			 0x4ee0eb7d, /// 0x244
			 0x9014b381, /// 0x248
			 0x5193fdb3, /// 0x24c
			 0xf44962d2, /// 0x250
			 0xca28e5c9, /// 0x254
			 0x2a768d44, /// 0x258
			 0xaec89dce, /// 0x25c
			 0x4cf35647, /// 0x260
			 0x1f48aad5, /// 0x264
			 0x57eb30ec, /// 0x268
			 0xf478b493, /// 0x26c
			 0xb4bde400, /// 0x270
			 0x56859c8a, /// 0x274
			 0x4dfcbbd5, /// 0x278
			 0x9b6e2bd2, /// 0x27c
			 0x69d46ff6, /// 0x280
			 0xae502f22, /// 0x284
			 0xb6c8c904, /// 0x288
			 0xea83770f, /// 0x28c
			 0x6434a8ab, /// 0x290
			 0xa3550b12, /// 0x294
			 0x90b2f342, /// 0x298
			 0x14e5480d, /// 0x29c
			 0x768b89eb, /// 0x2a0
			 0xadd1aa8a, /// 0x2a4
			 0x8293c859, /// 0x2a8
			 0x99d8a835, /// 0x2ac
			 0x5f9d3a19, /// 0x2b0
			 0x908c82a0, /// 0x2b4
			 0xaa6886c4, /// 0x2b8
			 0xdf4894b0, /// 0x2bc
			 0x25da2125, /// 0x2c0
			 0xb046e704, /// 0x2c4
			 0xe3716207, /// 0x2c8
			 0x7b4c0e51, /// 0x2cc
			 0x140615ce, /// 0x2d0
			 0x3d42e34f, /// 0x2d4
			 0x2f9d2a11, /// 0x2d8
			 0xbb111cd7, /// 0x2dc
			 0x9866d432, /// 0x2e0
			 0xa29ff396, /// 0x2e4
			 0xf6de0b55, /// 0x2e8
			 0x69387bdb, /// 0x2ec
			 0x416f314f, /// 0x2f0
			 0x21976d91, /// 0x2f4
			 0x53837de0, /// 0x2f8
			 0x21c38df1, /// 0x2fc
			 0x565dd4b3, /// 0x300
			 0x087e8a41, /// 0x304
			 0xff322b7c, /// 0x308
			 0xced62aed, /// 0x30c
			 0x3f9226b7, /// 0x310
			 0x39ceef5d, /// 0x314
			 0x31b72a20, /// 0x318
			 0x836ca021, /// 0x31c
			 0x79026bc4, /// 0x320
			 0x697b4663, /// 0x324
			 0x61f46afe, /// 0x328
			 0xbbad8131, /// 0x32c
			 0x690daf66, /// 0x330
			 0x4c9e65aa, /// 0x334
			 0x7cad6167, /// 0x338
			 0xfdf61ae4, /// 0x33c
			 0x80fd7de6, /// 0x340
			 0x83372a67, /// 0x344
			 0x9d4a6780, /// 0x348
			 0xaadac25b, /// 0x34c
			 0x3ec10713, /// 0x350
			 0xab02a926, /// 0x354
			 0x1d468936, /// 0x358
			 0x2d3b8212, /// 0x35c
			 0x0d0fcad7, /// 0x360
			 0xe63efd60, /// 0x364
			 0x16e31a72, /// 0x368
			 0xac0dbee5, /// 0x36c
			 0x45175626, /// 0x370
			 0x506dd505, /// 0x374
			 0x8a9de1c9, /// 0x378
			 0x0977027c, /// 0x37c
			 0x2152d1f0, /// 0x380
			 0x0e3e6cd1, /// 0x384
			 0xa5ad13b1, /// 0x388
			 0x0797d993, /// 0x38c
			 0x1e61a7b1, /// 0x390
			 0x959047cb, /// 0x394
			 0xe54e2a99, /// 0x398
			 0x5b2f046c, /// 0x39c
			 0xe81a7792, /// 0x3a0
			 0xd525214f, /// 0x3a4
			 0x9aa6c5a8, /// 0x3a8
			 0xaf6555fe, /// 0x3ac
			 0x3cb3c553, /// 0x3b0
			 0x5835a910, /// 0x3b4
			 0x73bdce7d, /// 0x3b8
			 0xcb2092df, /// 0x3bc
			 0x21ed1ccb, /// 0x3c0
			 0xafcf5a29, /// 0x3c4
			 0x769dcac2, /// 0x3c8
			 0x9b2e0ff4, /// 0x3cc
			 0x222f217a, /// 0x3d0
			 0xdb7237bf, /// 0x3d4
			 0xd34983cf, /// 0x3d8
			 0x77a5eb78, /// 0x3dc
			 0x84a3e621, /// 0x3e0
			 0xe57bbe33, /// 0x3e4
			 0xbe1365f7, /// 0x3e8
			 0xb2ebc34c, /// 0x3ec
			 0x174f2f90, /// 0x3f0
			 0x91bca121, /// 0x3f4
			 0xe31a6687, /// 0x3f8
			 0x0a22c427, /// 0x3fc
			 0xc84668b0, /// 0x400
			 0x17a7abab, /// 0x404
			 0x30fc897e, /// 0x408
			 0x4ee184ee, /// 0x40c
			 0x3c71cc4d, /// 0x410
			 0x529844f8, /// 0x414
			 0x027b68f7, /// 0x418
			 0x96864a50, /// 0x41c
			 0x450f0009, /// 0x420
			 0x14fc70c1, /// 0x424
			 0x340027eb, /// 0x428
			 0x3b4abbad, /// 0x42c
			 0x909de813, /// 0x430
			 0x5f94385e, /// 0x434
			 0x4ec28bcf, /// 0x438
			 0xec8ceb55, /// 0x43c
			 0xc2c9b9fa, /// 0x440
			 0x690f2669, /// 0x444
			 0xdf1ffaf5, /// 0x448
			 0x7573e88d, /// 0x44c
			 0x186ea616, /// 0x450
			 0x0c6c11c4, /// 0x454
			 0x0deed80e, /// 0x458
			 0x49a70c13, /// 0x45c
			 0xbbadd7ba, /// 0x460
			 0x9730bf6c, /// 0x464
			 0xe3b2b299, /// 0x468
			 0x55f6c1e8, /// 0x46c
			 0x20e785e6, /// 0x470
			 0x7d835796, /// 0x474
			 0x78351f25, /// 0x478
			 0xdb40b443, /// 0x47c
			 0xe18416de, /// 0x480
			 0x08041928, /// 0x484
			 0xa0a181bc, /// 0x488
			 0xb3ac8c69, /// 0x48c
			 0xfe3ea189, /// 0x490
			 0x06bfb2d8, /// 0x494
			 0x037edcf6, /// 0x498
			 0xbc0e1423, /// 0x49c
			 0x92192a95, /// 0x4a0
			 0xd40a7dfb, /// 0x4a4
			 0x4e059cef, /// 0x4a8
			 0x13f25e05, /// 0x4ac
			 0x578448c0, /// 0x4b0
			 0xc4fc2528, /// 0x4b4
			 0x08dc1284, /// 0x4b8
			 0xd0790ec0, /// 0x4bc
			 0x4af7bd8b, /// 0x4c0
			 0xfaf7ba89, /// 0x4c4
			 0x1c735638, /// 0x4c8
			 0x7218ca4f, /// 0x4cc
			 0x3b3e25ca, /// 0x4d0
			 0xa3355112, /// 0x4d4
			 0x8c3ae4a2, /// 0x4d8
			 0xc4b60919, /// 0x4dc
			 0x8f62ef01, /// 0x4e0
			 0xe03b9272, /// 0x4e4
			 0xda0b628e, /// 0x4e8
			 0x6e4d5515, /// 0x4ec
			 0x48d6cf8a, /// 0x4f0
			 0x41b421c9, /// 0x4f4
			 0xb222e50b, /// 0x4f8
			 0x6898a0cc, /// 0x4fc
			 0xa76f8b45, /// 0x500
			 0x25a9a3f5, /// 0x504
			 0x8155810c, /// 0x508
			 0x37c53c21, /// 0x50c
			 0x4ac49994, /// 0x510
			 0x476c0f1b, /// 0x514
			 0x10a13a7f, /// 0x518
			 0x5718f290, /// 0x51c
			 0x8a763cce, /// 0x520
			 0x155b5480, /// 0x524
			 0xa3b9b396, /// 0x528
			 0x0c73a66e, /// 0x52c
			 0xdd8ba7db, /// 0x530
			 0xb75bf0a7, /// 0x534
			 0xda580a42, /// 0x538
			 0xdbc70c7e, /// 0x53c
			 0xc5334176, /// 0x540
			 0x68e39d6d, /// 0x544
			 0xdd84a2e9, /// 0x548
			 0xd757027a, /// 0x54c
			 0x85af5a26, /// 0x550
			 0x4b8503f5, /// 0x554
			 0x9ea1b4f1, /// 0x558
			 0xc9c574d4, /// 0x55c
			 0xa6fe3275, /// 0x560
			 0x40512af7, /// 0x564
			 0x0854e9e8, /// 0x568
			 0x260c108a, /// 0x56c
			 0xe463f4e3, /// 0x570
			 0xfdb2c527, /// 0x574
			 0xa777459a, /// 0x578
			 0x72863699, /// 0x57c
			 0x4ee87e4c, /// 0x580
			 0xfc638fbe, /// 0x584
			 0x7a7adfce, /// 0x588
			 0x37ea8a3e, /// 0x58c
			 0xd8d11a48, /// 0x590
			 0x21957f51, /// 0x594
			 0xe4f37612, /// 0x598
			 0x715a8d1f, /// 0x59c
			 0x729cc3bf, /// 0x5a0
			 0x118bed86, /// 0x5a4
			 0x391ca83c, /// 0x5a8
			 0x81c6d00f, /// 0x5ac
			 0xb761a9e1, /// 0x5b0
			 0x8867621d, /// 0x5b4
			 0x5be2fc12, /// 0x5b8
			 0x05a3674d, /// 0x5bc
			 0xb1bbd0b5, /// 0x5c0
			 0xb7e6cada, /// 0x5c4
			 0xf318bcbc, /// 0x5c8
			 0x3637d90a, /// 0x5cc
			 0x0cb1a7e3, /// 0x5d0
			 0x04ab482c, /// 0x5d4
			 0xa0be3428, /// 0x5d8
			 0x275b7227, /// 0x5dc
			 0x81c57a60, /// 0x5e0
			 0xa8fc7287, /// 0x5e4
			 0xab3a4601, /// 0x5e8
			 0x5e4a7266, /// 0x5ec
			 0x8cc195bd, /// 0x5f0
			 0x44985eda, /// 0x5f4
			 0xb6e17bac, /// 0x5f8
			 0x52acaab2, /// 0x5fc
			 0x70d69c0b, /// 0x600
			 0x344b88a4, /// 0x604
			 0xc4ce5905, /// 0x608
			 0x5c2b0a9b, /// 0x60c
			 0x0c68a2b1, /// 0x610
			 0xa56076e8, /// 0x614
			 0xb4ee24db, /// 0x618
			 0xfcb44591, /// 0x61c
			 0x7120555c, /// 0x620
			 0x02bb3c8e, /// 0x624
			 0x47cf806a, /// 0x628
			 0xd3402373, /// 0x62c
			 0x86357890, /// 0x630
			 0xc5db8fff, /// 0x634
			 0xb1dff592, /// 0x638
			 0xaa53cd12, /// 0x63c
			 0x0e215e76, /// 0x640
			 0x5a2bac97, /// 0x644
			 0xc200e7f8, /// 0x648
			 0xd4c82701, /// 0x64c
			 0x04ae2ab1, /// 0x650
			 0x0ee035d4, /// 0x654
			 0x78fd5ec5, /// 0x658
			 0x2440fe8d, /// 0x65c
			 0xa60d536b, /// 0x660
			 0xe33b5393, /// 0x664
			 0x386ba1b9, /// 0x668
			 0x27becbf5, /// 0x66c
			 0x58005dd0, /// 0x670
			 0x0c567839, /// 0x674
			 0x98f22182, /// 0x678
			 0x10b362c2, /// 0x67c
			 0xc65ce6a3, /// 0x680
			 0x2f88b473, /// 0x684
			 0xed07d5b1, /// 0x688
			 0x3fff899e, /// 0x68c
			 0xa733977c, /// 0x690
			 0x4a028e56, /// 0x694
			 0xb34da362, /// 0x698
			 0xbe6839bd, /// 0x69c
			 0xf0171021, /// 0x6a0
			 0x60a6a416, /// 0x6a4
			 0xcd8dfedd, /// 0x6a8
			 0xbb036aca, /// 0x6ac
			 0xd135b190, /// 0x6b0
			 0xb045e1a7, /// 0x6b4
			 0x92982a93, /// 0x6b8
			 0x1595a712, /// 0x6bc
			 0xb74620b3, /// 0x6c0
			 0x85777385, /// 0x6c4
			 0x387181d6, /// 0x6c8
			 0x3e3ac960, /// 0x6cc
			 0xf47fa510, /// 0x6d0
			 0xd2c02841, /// 0x6d4
			 0xfd824ca7, /// 0x6d8
			 0xb47e3936, /// 0x6dc
			 0x1a3affc7, /// 0x6e0
			 0xed40e802, /// 0x6e4
			 0xcc9c75a8, /// 0x6e8
			 0xed1d497a, /// 0x6ec
			 0xe8c5dcc2, /// 0x6f0
			 0xdc985c6a, /// 0x6f4
			 0xbed259cd, /// 0x6f8
			 0x5b2cd265, /// 0x6fc
			 0x0add7eb7, /// 0x700
			 0xceafd327, /// 0x704
			 0x61f5008c, /// 0x708
			 0xd82d47a5, /// 0x70c
			 0xe704df5a, /// 0x710
			 0x131d12dc, /// 0x714
			 0x529c35dd, /// 0x718
			 0x56fcf43f, /// 0x71c
			 0x5a20b43d, /// 0x720
			 0xa9fcd894, /// 0x724
			 0x1d7f541d, /// 0x728
			 0x73f57792, /// 0x72c
			 0x87121785, /// 0x730
			 0x96ebbfcf, /// 0x734
			 0x9acfa725, /// 0x738
			 0x6ae02dd3, /// 0x73c
			 0x933c7f7c, /// 0x740
			 0x19448715, /// 0x744
			 0x00605df7, /// 0x748
			 0xd9dc7532, /// 0x74c
			 0xf92d3f3e, /// 0x750
			 0x530f8d09, /// 0x754
			 0x907ac8c1, /// 0x758
			 0x61a7d5af, /// 0x75c
			 0x4d1e8716, /// 0x760
			 0xb27d8246, /// 0x764
			 0xe0102454, /// 0x768
			 0xec679422, /// 0x76c
			 0xc6d11a90, /// 0x770
			 0xbb4c284a, /// 0x774
			 0x0e6db257, /// 0x778
			 0x56037d44, /// 0x77c
			 0xceb21304, /// 0x780
			 0xe682c127, /// 0x784
			 0x38f8c0df, /// 0x788
			 0x08a1aed3, /// 0x78c
			 0xb119c521, /// 0x790
			 0x8ff8068e, /// 0x794
			 0x3046bb8a, /// 0x798
			 0x244693c9, /// 0x79c
			 0xf9a4706d, /// 0x7a0
			 0xb65f246f, /// 0x7a4
			 0x35e89620, /// 0x7a8
			 0x5679032a, /// 0x7ac
			 0x686244eb, /// 0x7b0
			 0xaa33ab06, /// 0x7b4
			 0xaadfe917, /// 0x7b8
			 0xa93deb3f, /// 0x7bc
			 0x42d74909, /// 0x7c0
			 0xc798256d, /// 0x7c4
			 0x253015b8, /// 0x7c8
			 0x6a8684d5, /// 0x7cc
			 0x18cd1515, /// 0x7d0
			 0x24961535, /// 0x7d4
			 0x144dd3fc, /// 0x7d8
			 0x70e4f9d2, /// 0x7dc
			 0x3506c0c0, /// 0x7e0
			 0xf7e85e41, /// 0x7e4
			 0x0001f178, /// 0x7e8
			 0xd8a012e8, /// 0x7ec
			 0x22d35578, /// 0x7f0
			 0x7061ba29, /// 0x7f4
			 0xe4cebdfa, /// 0x7f8
			 0xcfd1d62a, /// 0x7fc
			 0xf1bdebcd, /// 0x800
			 0xd78c1c67, /// 0x804
			 0xf3cd7c14, /// 0x808
			 0xde59d9d5, /// 0x80c
			 0xb5ca6249, /// 0x810
			 0x7cb8b40f, /// 0x814
			 0x98ca0038, /// 0x818
			 0x6ab35393, /// 0x81c
			 0xe5f4c68d, /// 0x820
			 0xda63bba8, /// 0x824
			 0x267f497c, /// 0x828
			 0x5791de67, /// 0x82c
			 0x55d3548a, /// 0x830
			 0xcef3c157, /// 0x834
			 0x6e7c4df3, /// 0x838
			 0x6f111557, /// 0x83c
			 0x15d85cb6, /// 0x840
			 0xf235676a, /// 0x844
			 0x94ccf692, /// 0x848
			 0xcfd1eacb, /// 0x84c
			 0xdadc1152, /// 0x850
			 0xa2021d08, /// 0x854
			 0x380638e5, /// 0x858
			 0xa4f54dbc, /// 0x85c
			 0x609ac08a, /// 0x860
			 0xeba94fa3, /// 0x864
			 0xace49825, /// 0x868
			 0x156b9d32, /// 0x86c
			 0x824676d9, /// 0x870
			 0xeda0d6d9, /// 0x874
			 0xc9a66cd0, /// 0x878
			 0x96f13236, /// 0x87c
			 0xb82a95af, /// 0x880
			 0xd2f9bb6c, /// 0x884
			 0x1eaabde5, /// 0x888
			 0x0c175725, /// 0x88c
			 0x62ab9064, /// 0x890
			 0xc5f120b2, /// 0x894
			 0x1671f42a, /// 0x898
			 0x44d74947, /// 0x89c
			 0x8527150b, /// 0x8a0
			 0x82799e5c, /// 0x8a4
			 0x67394cea, /// 0x8a8
			 0xdca0671b, /// 0x8ac
			 0xbdf25ba0, /// 0x8b0
			 0x110469ff, /// 0x8b4
			 0x64c0291a, /// 0x8b8
			 0xb783083c, /// 0x8bc
			 0x9e973edb, /// 0x8c0
			 0x3eed5b1b, /// 0x8c4
			 0x78f5133f, /// 0x8c8
			 0x03314f61, /// 0x8cc
			 0x14f1be47, /// 0x8d0
			 0x3a7e4314, /// 0x8d4
			 0x1ffe2fe1, /// 0x8d8
			 0x5e3acdea, /// 0x8dc
			 0xbdbaaf02, /// 0x8e0
			 0x468dd50d, /// 0x8e4
			 0x0e5cfbbe, /// 0x8e8
			 0x74a74665, /// 0x8ec
			 0xcc958591, /// 0x8f0
			 0x1f1ba26f, /// 0x8f4
			 0x66e39bd0, /// 0x8f8
			 0xfce136eb, /// 0x8fc
			 0xfaa7a7b5, /// 0x900
			 0xba1e4fe7, /// 0x904
			 0x8e616bbc, /// 0x908
			 0xd25cae2e, /// 0x90c
			 0x4544fa53, /// 0x910
			 0xb88e77e7, /// 0x914
			 0x95175b4c, /// 0x918
			 0x226d2cfd, /// 0x91c
			 0x8d580c55, /// 0x920
			 0xa3291e2a, /// 0x924
			 0xb43502d2, /// 0x928
			 0xcfc3f6ef, /// 0x92c
			 0x71bd2de1, /// 0x930
			 0x11d76696, /// 0x934
			 0xa8615970, /// 0x938
			 0x595fe7a8, /// 0x93c
			 0xc6fb44a2, /// 0x940
			 0xe192c093, /// 0x944
			 0x2bb215ce, /// 0x948
			 0x470d6fee, /// 0x94c
			 0xf80e951a, /// 0x950
			 0xc2f64eff, /// 0x954
			 0xa36448c3, /// 0x958
			 0xe4da4b25, /// 0x95c
			 0x58357e7e, /// 0x960
			 0x23533e49, /// 0x964
			 0xac1a6681, /// 0x968
			 0x7c0b3e19, /// 0x96c
			 0xc34b0271, /// 0x970
			 0xddf14f95, /// 0x974
			 0xc9d38cd9, /// 0x978
			 0xd15204bf, /// 0x97c
			 0xafbee7e3, /// 0x980
			 0x07208d99, /// 0x984
			 0x99489912, /// 0x988
			 0xbfd1b991, /// 0x98c
			 0xe65a81b4, /// 0x990
			 0xd847ce0f, /// 0x994
			 0xda8cfb24, /// 0x998
			 0x15979911, /// 0x99c
			 0x9c39834b, /// 0x9a0
			 0x9196c0aa, /// 0x9a4
			 0xf3ca9d53, /// 0x9a8
			 0x2accd0b9, /// 0x9ac
			 0x5686ffa4, /// 0x9b0
			 0xa19e2acd, /// 0x9b4
			 0x1eced958, /// 0x9b8
			 0xd9330214, /// 0x9bc
			 0x771bafbe, /// 0x9c0
			 0x56b821a4, /// 0x9c4
			 0x154b92d8, /// 0x9c8
			 0x55a4085f, /// 0x9cc
			 0x808b0823, /// 0x9d0
			 0xf7a1734c, /// 0x9d4
			 0xbda5bcd1, /// 0x9d8
			 0x19c357ac, /// 0x9dc
			 0xf61644c3, /// 0x9e0
			 0xeaa06644, /// 0x9e4
			 0xae7f91d8, /// 0x9e8
			 0xd57d4de8, /// 0x9ec
			 0x4641a1ce, /// 0x9f0
			 0xd1705d97, /// 0x9f4
			 0x47a093b4, /// 0x9f8
			 0xa83f0124, /// 0x9fc
			 0xb0138f99, /// 0xa00
			 0x7e5ad096, /// 0xa04
			 0x26024500, /// 0xa08
			 0xb5585ea5, /// 0xa0c
			 0x0d4af528, /// 0xa10
			 0x053f2e98, /// 0xa14
			 0x726721b0, /// 0xa18
			 0x2ed7fbc5, /// 0xa1c
			 0x25819ced, /// 0xa20
			 0xe600815d, /// 0xa24
			 0x4345493d, /// 0xa28
			 0x6d5c4afc, /// 0xa2c
			 0x41a6f869, /// 0xa30
			 0x2434b134, /// 0xa34
			 0x53467664, /// 0xa38
			 0xf838f639, /// 0xa3c
			 0xd541aa21, /// 0xa40
			 0x767b734f, /// 0xa44
			 0x0df897a7, /// 0xa48
			 0xdc28ecd6, /// 0xa4c
			 0x116d47cc, /// 0xa50
			 0x578b2f7f, /// 0xa54
			 0xaee394a5, /// 0xa58
			 0xb6b1f06a, /// 0xa5c
			 0x0b65bbb8, /// 0xa60
			 0x3115cdb4, /// 0xa64
			 0xd28d9ccd, /// 0xa68
			 0x04ba9934, /// 0xa6c
			 0x78cadb63, /// 0xa70
			 0xbfa18ea1, /// 0xa74
			 0x1609cbab, /// 0xa78
			 0x80c330f7, /// 0xa7c
			 0xaf4d1ee3, /// 0xa80
			 0x56aa0359, /// 0xa84
			 0xaedd4ff7, /// 0xa88
			 0x63b6d05c, /// 0xa8c
			 0xbfbd9eff, /// 0xa90
			 0x4cb096b3, /// 0xa94
			 0x3ed8a9d9, /// 0xa98
			 0x8899c88d, /// 0xa9c
			 0x4e81959b, /// 0xaa0
			 0xfb5108c9, /// 0xaa4
			 0xb357302e, /// 0xaa8
			 0xf84d9658, /// 0xaac
			 0x282d52d7, /// 0xab0
			 0xdecdfd99, /// 0xab4
			 0x43f4e9bc, /// 0xab8
			 0x1d39f1de, /// 0xabc
			 0x3bd32593, /// 0xac0
			 0x9be8c4b4, /// 0xac4
			 0xa29a95f4, /// 0xac8
			 0x50d4c66c, /// 0xacc
			 0x076b9554, /// 0xad0
			 0x8d49cbca, /// 0xad4
			 0x4b53e6cd, /// 0xad8
			 0x64482338, /// 0xadc
			 0xb86be110, /// 0xae0
			 0x7e7fef62, /// 0xae4
			 0xd9ba546f, /// 0xae8
			 0xaa9955dd, /// 0xaec
			 0x27700660, /// 0xaf0
			 0x5f6afd05, /// 0xaf4
			 0x84ff93ea, /// 0xaf8
			 0x81cf0c14, /// 0xafc
			 0x608a3dd3, /// 0xb00
			 0x3fd4d057, /// 0xb04
			 0x41b1544c, /// 0xb08
			 0xe04e1c4e, /// 0xb0c
			 0xe4bb3610, /// 0xb10
			 0xa5368d73, /// 0xb14
			 0x505fc8ce, /// 0xb18
			 0xc805f496, /// 0xb1c
			 0x03f57cf2, /// 0xb20
			 0xb1d96e33, /// 0xb24
			 0xcc8c6f8e, /// 0xb28
			 0x825ce16c, /// 0xb2c
			 0x3e4d4224, /// 0xb30
			 0x8abd6be1, /// 0xb34
			 0xc80a424c, /// 0xb38
			 0xeffe2e94, /// 0xb3c
			 0x48fbd837, /// 0xb40
			 0xe8e88784, /// 0xb44
			 0x8a9a3588, /// 0xb48
			 0xa1f61f3f, /// 0xb4c
			 0xf985e535, /// 0xb50
			 0x432c7311, /// 0xb54
			 0xdef85dfd, /// 0xb58
			 0x5e90959a, /// 0xb5c
			 0x02fa061a, /// 0xb60
			 0x2dc53b15, /// 0xb64
			 0x0f492d77, /// 0xb68
			 0x1ef7d5a8, /// 0xb6c
			 0x1b556ce8, /// 0xb70
			 0xf60eb271, /// 0xb74
			 0x7a49610a, /// 0xb78
			 0x3a376efd, /// 0xb7c
			 0x3e547c1e, /// 0xb80
			 0x948eaf70, /// 0xb84
			 0x58823871, /// 0xb88
			 0x281da35c, /// 0xb8c
			 0xf42f0d52, /// 0xb90
			 0xf8dfba1f, /// 0xb94
			 0x4e207a32, /// 0xb98
			 0x1f291da8, /// 0xb9c
			 0x9c41b13e, /// 0xba0
			 0x399cb630, /// 0xba4
			 0xf454c5b4, /// 0xba8
			 0xd34451d8, /// 0xbac
			 0x4b95964c, /// 0xbb0
			 0x50947887, /// 0xbb4
			 0xaf90b838, /// 0xbb8
			 0xfbb87d15, /// 0xbbc
			 0xf8c8bfc8, /// 0xbc0
			 0xf76e8256, /// 0xbc4
			 0xfec74a6f, /// 0xbc8
			 0xf4e4a1b9, /// 0xbcc
			 0x8ca647f4, /// 0xbd0
			 0x70dd12fa, /// 0xbd4
			 0x38945928, /// 0xbd8
			 0x53c9c668, /// 0xbdc
			 0x722fec42, /// 0xbe0
			 0x5a6bc407, /// 0xbe4
			 0xe93a7616, /// 0xbe8
			 0x6d453f30, /// 0xbec
			 0x1562a587, /// 0xbf0
			 0xb56b7dcd, /// 0xbf4
			 0x209d1e62, /// 0xbf8
			 0xcb793990, /// 0xbfc
			 0x588c5b6a, /// 0xc00
			 0xcedd0415, /// 0xc04
			 0xcdd4d84b, /// 0xc08
			 0x06cf240f, /// 0xc0c
			 0xcd246b0d, /// 0xc10
			 0xe6e3fd5f, /// 0xc14
			 0x8064e120, /// 0xc18
			 0x80fad89e, /// 0xc1c
			 0x606460fb, /// 0xc20
			 0xb92b4ae2, /// 0xc24
			 0xbe31ce02, /// 0xc28
			 0x4b21bc39, /// 0xc2c
			 0x6afb26de, /// 0xc30
			 0xb3cfa864, /// 0xc34
			 0xab8926fb, /// 0xc38
			 0x34f1ce3c, /// 0xc3c
			 0x37df53a8, /// 0xc40
			 0x574ecd85, /// 0xc44
			 0x4248eb44, /// 0xc48
			 0x9bd49c17, /// 0xc4c
			 0x211ee491, /// 0xc50
			 0xe026f56c, /// 0xc54
			 0xb326928f, /// 0xc58
			 0x96c3762e, /// 0xc5c
			 0x5e61a9a8, /// 0xc60
			 0x4ac3af3b, /// 0xc64
			 0x06871884, /// 0xc68
			 0x69399b66, /// 0xc6c
			 0xe1e87109, /// 0xc70
			 0xe0bc6567, /// 0xc74
			 0x7d6e9b6a, /// 0xc78
			 0xdce052e0, /// 0xc7c
			 0xaeab7bc0, /// 0xc80
			 0x02f51229, /// 0xc84
			 0xdadef272, /// 0xc88
			 0x86debae0, /// 0xc8c
			 0x09d7233e, /// 0xc90
			 0x1507ee6c, /// 0xc94
			 0xaa3cc19c, /// 0xc98
			 0xa4c215a3, /// 0xc9c
			 0xa7c50eb9, /// 0xca0
			 0x7915eecf, /// 0xca4
			 0x39b0c84a, /// 0xca8
			 0x76453949, /// 0xcac
			 0xfd8a7a45, /// 0xcb0
			 0xa5185896, /// 0xcb4
			 0x05851543, /// 0xcb8
			 0xf4ab7be5, /// 0xcbc
			 0x46d853cb, /// 0xcc0
			 0x637c67f8, /// 0xcc4
			 0x237930ae, /// 0xcc8
			 0x08d48584, /// 0xccc
			 0x71ef6e7f, /// 0xcd0
			 0x706c0cdf, /// 0xcd4
			 0x9256dd12, /// 0xcd8
			 0x8afbaf8e, /// 0xcdc
			 0xdaea3b7a, /// 0xce0
			 0xf2a6cf6d, /// 0xce4
			 0x8319afe5, /// 0xce8
			 0xa2158c9b, /// 0xcec
			 0x3441335a, /// 0xcf0
			 0xf362126f, /// 0xcf4
			 0xf8213cbe, /// 0xcf8
			 0xae2066af, /// 0xcfc
			 0xfa2355a4, /// 0xd00
			 0x487008b4, /// 0xd04
			 0xf751eb7f, /// 0xd08
			 0xbd08c773, /// 0xd0c
			 0x41b7fd03, /// 0xd10
			 0xa9eb3622, /// 0xd14
			 0x0f5cf2e2, /// 0xd18
			 0x29724be5, /// 0xd1c
			 0x4356ab38, /// 0xd20
			 0x816a4eda, /// 0xd24
			 0x55b7157f, /// 0xd28
			 0xa5f22b2c, /// 0xd2c
			 0x6f47b80d, /// 0xd30
			 0x30b659ce, /// 0xd34
			 0xedaa4545, /// 0xd38
			 0x4c0c02a9, /// 0xd3c
			 0x92ba787d, /// 0xd40
			 0xbe28f2ce, /// 0xd44
			 0x14b2768b, /// 0xd48
			 0x2a488783, /// 0xd4c
			 0x5d4a7b63, /// 0xd50
			 0x6427daec, /// 0xd54
			 0x85b4120d, /// 0xd58
			 0x3dc22958, /// 0xd5c
			 0xf9763504, /// 0xd60
			 0x5368f7e7, /// 0xd64
			 0xc19a5e98, /// 0xd68
			 0xb247c67c, /// 0xd6c
			 0xc297a45d, /// 0xd70
			 0xe58e3e27, /// 0xd74
			 0xbaf006fc, /// 0xd78
			 0x62227d64, /// 0xd7c
			 0xefecf64a, /// 0xd80
			 0x1d5c51bd, /// 0xd84
			 0x5977e4bc, /// 0xd88
			 0x6d12aaad, /// 0xd8c
			 0x902dcc4e, /// 0xd90
			 0xa980adcb, /// 0xd94
			 0xe2979099, /// 0xd98
			 0xaa37971e, /// 0xd9c
			 0x4f41dbc1, /// 0xda0
			 0x7a1efe87, /// 0xda4
			 0x20b4f0e4, /// 0xda8
			 0x98921792, /// 0xdac
			 0x9d5786ba, /// 0xdb0
			 0x77aebfc0, /// 0xdb4
			 0x1437437b, /// 0xdb8
			 0xf76c5df3, /// 0xdbc
			 0x2bf70e61, /// 0xdc0
			 0x68c93d9e, /// 0xdc4
			 0x14ee3eb6, /// 0xdc8
			 0x533369b5, /// 0xdcc
			 0x30b06466, /// 0xdd0
			 0xe7271cd6, /// 0xdd4
			 0xf01fc644, /// 0xdd8
			 0x4917e6df, /// 0xddc
			 0x0ad76c20, /// 0xde0
			 0xa8d0e655, /// 0xde4
			 0x1888ace9, /// 0xde8
			 0xc56733f0, /// 0xdec
			 0x2d72d62d, /// 0xdf0
			 0x409a5b9d, /// 0xdf4
			 0x184586a9, /// 0xdf8
			 0x11eb8b6d, /// 0xdfc
			 0xd8e8c419, /// 0xe00
			 0x807dcd9d, /// 0xe04
			 0x54e273e9, /// 0xe08
			 0x34944173, /// 0xe0c
			 0xa2ae797a, /// 0xe10
			 0x2cde452d, /// 0xe14
			 0xa86fdd55, /// 0xe18
			 0x15919005, /// 0xe1c
			 0x9aed7073, /// 0xe20
			 0x7d53654a, /// 0xe24
			 0xcc4c966f, /// 0xe28
			 0x56e5c6bd, /// 0xe2c
			 0xf0fbbac3, /// 0xe30
			 0x79cd66cb, /// 0xe34
			 0x1b69ad5f, /// 0xe38
			 0x8af091f9, /// 0xe3c
			 0x3a4f1550, /// 0xe40
			 0xc89dd3d9, /// 0xe44
			 0x5696a684, /// 0xe48
			 0x36405ee1, /// 0xe4c
			 0xf1e2d8fb, /// 0xe50
			 0xde65f14f, /// 0xe54
			 0xb4e12499, /// 0xe58
			 0x87058fdb, /// 0xe5c
			 0x81eb6888, /// 0xe60
			 0x1fbbab54, /// 0xe64
			 0x4d10e8a1, /// 0xe68
			 0xff97bef3, /// 0xe6c
			 0x47eef8ed, /// 0xe70
			 0x9950d8ba, /// 0xe74
			 0x9653a7f8, /// 0xe78
			 0x3414ff1f, /// 0xe7c
			 0x71df8be3, /// 0xe80
			 0x1d4811a2, /// 0xe84
			 0x02a04c10, /// 0xe88
			 0x8a0d76e1, /// 0xe8c
			 0xfa534e20, /// 0xe90
			 0x40ef3f5e, /// 0xe94
			 0xe85eb084, /// 0xe98
			 0x98c1183c, /// 0xe9c
			 0x1ba5be80, /// 0xea0
			 0x0f29f3fa, /// 0xea4
			 0xf1984a01, /// 0xea8
			 0x461db1a7, /// 0xeac
			 0xd5289cf0, /// 0xeb0
			 0xbf1a14cf, /// 0xeb4
			 0x5d98e4bd, /// 0xeb8
			 0x83b41929, /// 0xebc
			 0x7ff56e82, /// 0xec0
			 0x08e4322d, /// 0xec4
			 0x852c9da4, /// 0xec8
			 0xea5fc760, /// 0xecc
			 0xdd6245a1, /// 0xed0
			 0x35facbd7, /// 0xed4
			 0x82db1c1a, /// 0xed8
			 0x79f3da46, /// 0xedc
			 0x3c93199c, /// 0xee0
			 0x830540a2, /// 0xee4
			 0xcce52886, /// 0xee8
			 0x87282c58, /// 0xeec
			 0x760dbd4f, /// 0xef0
			 0x29219023, /// 0xef4
			 0x421b055d, /// 0xef8
			 0x2420da67, /// 0xefc
			 0xe23335a2, /// 0xf00
			 0xce196262, /// 0xf04
			 0xfdd1b7c4, /// 0xf08
			 0x33adfb00, /// 0xf0c
			 0x25f34205, /// 0xf10
			 0x860c94a0, /// 0xf14
			 0x0d741468, /// 0xf18
			 0x77cf95a5, /// 0xf1c
			 0xe173ab5a, /// 0xf20
			 0x7a928193, /// 0xf24
			 0xad6d13db, /// 0xf28
			 0x6ec8bddf, /// 0xf2c
			 0x23797329, /// 0xf30
			 0xd31ef769, /// 0xf34
			 0x5c6a1bb9, /// 0xf38
			 0xb180964d, /// 0xf3c
			 0x56b28a9c, /// 0xf40
			 0xd1ea3d3c, /// 0xf44
			 0x6b3b888c, /// 0xf48
			 0x4134dc8f, /// 0xf4c
			 0x7f306b4c, /// 0xf50
			 0xd50856e2, /// 0xf54
			 0xfc639ccb, /// 0xf58
			 0x7f13efc8, /// 0xf5c
			 0x6e245106, /// 0xf60
			 0xea6cc142, /// 0xf64
			 0x8e4232c7, /// 0xf68
			 0x5c37963e, /// 0xf6c
			 0x783587ee, /// 0xf70
			 0xc02a00f2, /// 0xf74
			 0x32747d45, /// 0xf78
			 0x36dbb354, /// 0xf7c
			 0xb3e95e3c, /// 0xf80
			 0x418f3e56, /// 0xf84
			 0xf8e829d0, /// 0xf88
			 0x97537214, /// 0xf8c
			 0xc1f5a46b, /// 0xf90
			 0x1939b1e9, /// 0xf94
			 0x9a938b13, /// 0xf98
			 0xd872317d, /// 0xf9c
			 0x35b1bddd, /// 0xfa0
			 0xb4eb5338, /// 0xfa4
			 0xe7709a8c, /// 0xfa8
			 0x8f562733, /// 0xfac
			 0x3d7ae1aa, /// 0xfb0
			 0x98acc5de, /// 0xfb4
			 0x389f8b37, /// 0xfb8
			 0x8b60c7a4, /// 0xfbc
			 0x58382829, /// 0xfc0
			 0x0657744d, /// 0xfc4
			 0xd2aa52ff, /// 0xfc8
			 0xae59df2b, /// 0xfcc
			 0xb3198646, /// 0xfd0
			 0x0047eae3, /// 0xfd4
			 0x69d2456f, /// 0xfd8
			 0xddd3594f, /// 0xfdc
			 0x56613846, /// 0xfe0
			 0xbd0d4ea8, /// 0xfe4
			 0xac747aca, /// 0xfe8
			 0x26f16c6a, /// 0xfec
			 0x3c90db6f, /// 0xff0
			 0xcec16233, /// 0xff4
			 0x00a9101a, /// 0xff8
			 0xc00344dd /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00000
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00010
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00014
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00028
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0002c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00030
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00040
			 0x0c600000,                                                  // Leading 1s:                                 /// 00044
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff800000,                                                  // -inf                                        /// 0004c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00050
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00054
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00058
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0005c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00064
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00068
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0006c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00078
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0007c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00084
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0008c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00094
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00098
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00104
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0010c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00110
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c780000,                                                  // Leading 1s:                                 /// 0011c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00120
			 0xcb000000,                                                  // -8388608.0                                  /// 00124
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00128
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00130
			 0x0e000003,                                                  // Trailing 1s:                                /// 00134
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00138
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0013c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00140
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00144
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00148
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00150
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00154
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e000007,                                                  // Trailing 1s:                                /// 0015c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00160
			 0xbf028f5c,                                                  // -0.51                                       /// 00164
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00168
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00170
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00178
			 0x0e000001,                                                  // Trailing 1s:                                /// 0017c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00180
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00184
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00190
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00194
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0019c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00200
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00204
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0020c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c780000,                                                  // Leading 1s:                                 /// 00214
			 0x4b000000,                                                  // 8388608.0                                   /// 00218
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0021c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00224
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00228
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0022c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00230
			 0x0c400000,                                                  // Leading 1s:                                 /// 00234
			 0xcb000000,                                                  // -8388608.0                                  /// 00238
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00240
			 0x4b000000,                                                  // 8388608.0                                   /// 00244
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00250
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00254
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00258
			 0x3f028f5c,                                                  // 0.51                                        /// 0025c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00260
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00264
			 0x0e000001,                                                  // Trailing 1s:                                /// 00268
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0026c
			 0x33333333,                                                  // 4 random values                             /// 00270
			 0xbf028f5c,                                                  // -0.51                                       /// 00274
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x4b000000,                                                  // 8388608.0                                   /// 0027c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00280
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00288
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0028c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00290
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00294
			 0xbf028f5c,                                                  // -0.51                                       /// 00298
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ac
			 0x00000000,                                                  // zero                                        /// 002b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 002e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002ec
			 0x80000000,                                                  // -zero                                       /// 002f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00300
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80011111,                                                  // -9.7958E-41                                 /// 00308
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0030c
			 0xcb000000,                                                  // -8388608.0                                  /// 00310
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00318
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0031c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x7f800000,                                                  // inf                                         /// 00328
			 0x33333333,                                                  // 4 random values                             /// 0032c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00334
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00338
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00344
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00348
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0034c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00354
			 0x00011111,                                                  // 9.7958E-41                                  /// 00358
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00360
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00364
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00368
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0036c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00370
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00378
			 0x0c700000,                                                  // Leading 1s:                                 /// 0037c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00380
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00388
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0038c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00390
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00394
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00398
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003a0
			 0x00000000,                                                  // zero                                        /// 003a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00400
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00408
			 0x0c780000,                                                  // Leading 1s:                                 /// 0040c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00410
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0041c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00420
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00424
			 0x80011111,                                                  // -9.7958E-41                                 /// 00428
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0042c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00434
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00440
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00448
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0044c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xff800000,                                                  // -inf                                        /// 00454
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00458
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0045c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00460
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0046c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00470
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00474
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00478
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0047c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00488
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x0e000007,                                                  // Trailing 1s:                                /// 0049c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xffc00001,                                                  // -signaling NaN                              /// 004f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0050c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x4b000000,                                                  // 8388608.0                                   /// 00514
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0051c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00528
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0052c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x33333333,                                                  // 4 random values                             /// 00538
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0053c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x3f028f5c,                                                  // 0.51                                        /// 00550
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00554
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0c600000,                                                  // Leading 1s:                                 /// 0055c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00560
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00564
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00568
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00570
			 0x00011111,                                                  // 9.7958E-41                                  /// 00574
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00584
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00588
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0058c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00590
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00598
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0059c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005d4
			 0xbf028f5c,                                                  // -0.51                                       /// 005d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00604
			 0xcb000000,                                                  // -8388608.0                                  /// 00608
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0060c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00610
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00614
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0061c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00620
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00628
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00634
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00638
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0063c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00640
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00644
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00650
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00654
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00658
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00660
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00664
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0066c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00670
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00674
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0067c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00680
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00688
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0068c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00690
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00694
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00698
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0069c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00708
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0070c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00710
			 0x0c400000,                                                  // Leading 1s:                                 /// 00714
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00720
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00724
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00728
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xcb000000,                                                  // -8388608.0                                  /// 00730
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00738
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x7f800000,                                                  // inf                                         /// 00748
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00750
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00758
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00760
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00764
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00768
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00770
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00774
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00778
			 0x0c780000,                                                  // Leading 1s:                                 /// 0077c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00784
			 0xff800000,                                                  // -inf                                        /// 00788
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0078c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xffc00001,                                                  // -signaling NaN                              /// 00798
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0079c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00804
			 0x0c400000,                                                  // Leading 1s:                                 /// 00808
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0081c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00820
			 0xbf028f5c,                                                  // -0.51                                       /// 00824
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00828
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00840
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00844
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00848
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00850
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00858
			 0x0c780000,                                                  // Leading 1s:                                 /// 0085c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00868
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00870
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00878
			 0x7fc00001,                                                  // signaling NaN                               /// 0087c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00888
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0088c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00890
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00894
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0089c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008cc
			 0x7f800000,                                                  // inf                                         /// 008d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00908
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0090c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00910
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00918
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00924
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00928
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0092c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0093c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00950
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0095c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00960
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00964
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00968
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0096c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00970
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0097c
			 0x00000000,                                                  // zero                                        /// 00980
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00990
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00994
			 0x7f800000,                                                  // inf                                         /// 00998
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x7fc00001,                                                  // signaling NaN                               /// 009f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00000000,                                                  // zero                                        /// 00a18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a54
			 0x80000000,                                                  // -zero                                       /// 00a58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a78
			 0x55555555,                                                  // 4 random values                             /// 00a7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aa4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00aac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ac8
			 0xcb000000,                                                  // -8388608.0                                  /// 00acc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ad0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00af0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00af4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00afc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b1c
			 0x00000000,                                                  // zero                                        /// 00b20
			 0x00000000,                                                  // zero                                        /// 00b24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ba4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf4
			 0x55555555,                                                  // 4 random values                             /// 00bf8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c24
			 0x00000000,                                                  // zero                                        /// 00c28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c58
			 0x80000000,                                                  // -zero                                       /// 00c5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c94
			 0xcb000000,                                                  // -8388608.0                                  /// 00c98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ca8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cf8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cfc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000000,                                                  // -zero                                       /// 00d18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x33333333,                                                  // 4 random values                             /// 00d2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00db8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ddc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00df0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e78
			 0xffc00001,                                                  // -signaling NaN                              /// 00e7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x7fc00001,                                                  // signaling NaN                               /// 00e9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb0
			 0x7f800000,                                                  // inf                                         /// 00eb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ebc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ecc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00edc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00efc
			 0x33333333,                                                  // 4 random values                             /// 00f00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f48
			 0x7f800000,                                                  // inf                                         /// 00f4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f90
			 0xbf028f5c,                                                  // -0.51                                       /// 00f94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fe4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ff0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ff8
			 0x00001000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xf570c6df, /// 0x0
			 0x7b594023, /// 0x4
			 0x05cf77e1, /// 0x8
			 0xe1c53f63, /// 0xc
			 0x699cd423, /// 0x10
			 0x2159005d, /// 0x14
			 0x98702ff8, /// 0x18
			 0x8554e3c4, /// 0x1c
			 0xf5067e69, /// 0x20
			 0x0529d7f0, /// 0x24
			 0xcd264bf2, /// 0x28
			 0x10385e18, /// 0x2c
			 0x85c829d0, /// 0x30
			 0x9ea0c498, /// 0x34
			 0xd676e090, /// 0x38
			 0x4064961a, /// 0x3c
			 0x49e6ebec, /// 0x40
			 0x076d374b, /// 0x44
			 0x98b33f31, /// 0x48
			 0x55c01cb0, /// 0x4c
			 0xbccbb51c, /// 0x50
			 0x737b4999, /// 0x54
			 0x4e1fe4b1, /// 0x58
			 0xa67ae3f0, /// 0x5c
			 0x0dbb6107, /// 0x60
			 0xda66a041, /// 0x64
			 0x752e2d1a, /// 0x68
			 0x6e5173e4, /// 0x6c
			 0xec6260b9, /// 0x70
			 0x764a4015, /// 0x74
			 0x36cba9da, /// 0x78
			 0xcb5d84be, /// 0x7c
			 0x008dd79e, /// 0x80
			 0xa86e9065, /// 0x84
			 0xf7317b62, /// 0x88
			 0xe88c508d, /// 0x8c
			 0x4c515237, /// 0x90
			 0xc6ada889, /// 0x94
			 0x0211f6cd, /// 0x98
			 0x6be92f1d, /// 0x9c
			 0x51568be3, /// 0xa0
			 0x509a2fab, /// 0xa4
			 0x632946a0, /// 0xa8
			 0x321442ff, /// 0xac
			 0xac628f0b, /// 0xb0
			 0xf8cee590, /// 0xb4
			 0xa2c1f43a, /// 0xb8
			 0x3be918bf, /// 0xbc
			 0xe0a2034b, /// 0xc0
			 0xe02bc58d, /// 0xc4
			 0x1b2bd466, /// 0xc8
			 0x38c85132, /// 0xcc
			 0x4e5ec21d, /// 0xd0
			 0xf68c6648, /// 0xd4
			 0x8c548c50, /// 0xd8
			 0xb512cc05, /// 0xdc
			 0xc45bd0b5, /// 0xe0
			 0x57b08ac9, /// 0xe4
			 0x4bbad129, /// 0xe8
			 0x6f1904b9, /// 0xec
			 0x49cf3ffb, /// 0xf0
			 0xcbaf91b2, /// 0xf4
			 0x0f336605, /// 0xf8
			 0x0c6a4b61, /// 0xfc
			 0x84766503, /// 0x100
			 0x3f30ddab, /// 0x104
			 0xa6853a13, /// 0x108
			 0xc31e5b3f, /// 0x10c
			 0xdcd0750c, /// 0x110
			 0x8e5c3622, /// 0x114
			 0x57a9dec8, /// 0x118
			 0x6204269c, /// 0x11c
			 0x4d8c8ec7, /// 0x120
			 0x3cf14a9d, /// 0x124
			 0xc13db1a6, /// 0x128
			 0x0dce5ed5, /// 0x12c
			 0x78eb6975, /// 0x130
			 0x04d4f04c, /// 0x134
			 0x4db049d9, /// 0x138
			 0xaba2560e, /// 0x13c
			 0x5167b720, /// 0x140
			 0xa6fd0733, /// 0x144
			 0xa7e2fe82, /// 0x148
			 0x1467fa0c, /// 0x14c
			 0x096d88c7, /// 0x150
			 0x6e8eefc2, /// 0x154
			 0xd3509433, /// 0x158
			 0xdb866e90, /// 0x15c
			 0xfbd861f9, /// 0x160
			 0xf4a3fd9c, /// 0x164
			 0x1f6aee0e, /// 0x168
			 0x213fefe8, /// 0x16c
			 0xe88564d4, /// 0x170
			 0xa7afb455, /// 0x174
			 0xb09d2d06, /// 0x178
			 0xaef03072, /// 0x17c
			 0x2494c2d5, /// 0x180
			 0x3c1cbb51, /// 0x184
			 0x8315b9b9, /// 0x188
			 0xc5f9a968, /// 0x18c
			 0xf416d2bf, /// 0x190
			 0x4c2edc55, /// 0x194
			 0xe35f0772, /// 0x198
			 0x5bfdad9d, /// 0x19c
			 0xa5681764, /// 0x1a0
			 0xdb626e52, /// 0x1a4
			 0x3788a1a2, /// 0x1a8
			 0x6d0e09a4, /// 0x1ac
			 0x1dd2019b, /// 0x1b0
			 0xab29885e, /// 0x1b4
			 0x55993ee4, /// 0x1b8
			 0xc42dae0e, /// 0x1bc
			 0xd5d6fa83, /// 0x1c0
			 0x1e193db9, /// 0x1c4
			 0x8406239f, /// 0x1c8
			 0x5eb1352c, /// 0x1cc
			 0x49f416b0, /// 0x1d0
			 0x6ce3e4d8, /// 0x1d4
			 0xc2b1f405, /// 0x1d8
			 0xd178d101, /// 0x1dc
			 0x550d65a8, /// 0x1e0
			 0x401502a1, /// 0x1e4
			 0xc529c4cd, /// 0x1e8
			 0x20f6e33a, /// 0x1ec
			 0x4ea50ddb, /// 0x1f0
			 0xb911f0fb, /// 0x1f4
			 0xf65634f2, /// 0x1f8
			 0x2d0688aa, /// 0x1fc
			 0xe45b1b80, /// 0x200
			 0xbda549c5, /// 0x204
			 0x2cd64212, /// 0x208
			 0x6d3204ea, /// 0x20c
			 0x6d021721, /// 0x210
			 0xf40f3b60, /// 0x214
			 0xa8469605, /// 0x218
			 0x1693d930, /// 0x21c
			 0x2529c70b, /// 0x220
			 0x8cfcbbd4, /// 0x224
			 0xbdcb79f6, /// 0x228
			 0x9e8cb515, /// 0x22c
			 0x655907d4, /// 0x230
			 0xbbd0365f, /// 0x234
			 0x4f2eebff, /// 0x238
			 0x091d0019, /// 0x23c
			 0xc893126d, /// 0x240
			 0x1e3fb479, /// 0x244
			 0x0d0bb166, /// 0x248
			 0xdeaee478, /// 0x24c
			 0xc0977f58, /// 0x250
			 0x45fcf57e, /// 0x254
			 0xa4024a6c, /// 0x258
			 0x85ce96e8, /// 0x25c
			 0xd3906e8d, /// 0x260
			 0xa725a4b2, /// 0x264
			 0x29ed80ad, /// 0x268
			 0x8ccb8f04, /// 0x26c
			 0x4f429918, /// 0x270
			 0x97a0317e, /// 0x274
			 0x4cb183bd, /// 0x278
			 0x29677b40, /// 0x27c
			 0x968684b1, /// 0x280
			 0xb57e04a0, /// 0x284
			 0xab69330e, /// 0x288
			 0xa3683cc5, /// 0x28c
			 0xf9121455, /// 0x290
			 0x8932944f, /// 0x294
			 0x151a8295, /// 0x298
			 0x2f9f4566, /// 0x29c
			 0xec528ea5, /// 0x2a0
			 0x1226e232, /// 0x2a4
			 0xc87c0d5b, /// 0x2a8
			 0xdb5f2c09, /// 0x2ac
			 0x985e1458, /// 0x2b0
			 0x18ad99e9, /// 0x2b4
			 0x36e8eaa8, /// 0x2b8
			 0x79439cd1, /// 0x2bc
			 0x24371856, /// 0x2c0
			 0x26a4524a, /// 0x2c4
			 0xf3110ea5, /// 0x2c8
			 0x929b5891, /// 0x2cc
			 0x0b378d16, /// 0x2d0
			 0x97494d52, /// 0x2d4
			 0x76571893, /// 0x2d8
			 0xe721fda9, /// 0x2dc
			 0x77a40a85, /// 0x2e0
			 0x9385049e, /// 0x2e4
			 0x0775afc2, /// 0x2e8
			 0x5623df2f, /// 0x2ec
			 0x65209799, /// 0x2f0
			 0xbd93fb7d, /// 0x2f4
			 0xbf5bbaf1, /// 0x2f8
			 0xbc6df191, /// 0x2fc
			 0x82cc03a8, /// 0x300
			 0x389c5076, /// 0x304
			 0x1e3b2da9, /// 0x308
			 0x7d05e6be, /// 0x30c
			 0xfa4799e9, /// 0x310
			 0x7c2a7135, /// 0x314
			 0xf406935c, /// 0x318
			 0x7309e774, /// 0x31c
			 0xd150a577, /// 0x320
			 0x0bbe9b27, /// 0x324
			 0x01098c3b, /// 0x328
			 0xe0e48110, /// 0x32c
			 0x087e00a4, /// 0x330
			 0x496c9124, /// 0x334
			 0xd198a602, /// 0x338
			 0x04196b16, /// 0x33c
			 0x344f98e2, /// 0x340
			 0xabbf06b1, /// 0x344
			 0x6fccdc29, /// 0x348
			 0x13974088, /// 0x34c
			 0x159e4a2b, /// 0x350
			 0x5cc7a062, /// 0x354
			 0x80889fa9, /// 0x358
			 0xaa8d2236, /// 0x35c
			 0xd3dc998e, /// 0x360
			 0x597b113f, /// 0x364
			 0xbcab67df, /// 0x368
			 0x04b99511, /// 0x36c
			 0x0e369313, /// 0x370
			 0x35a34203, /// 0x374
			 0x78c13bcb, /// 0x378
			 0x4c5ab78f, /// 0x37c
			 0x2e84eef7, /// 0x380
			 0xbf4dba93, /// 0x384
			 0x60a74dc4, /// 0x388
			 0x79ac8feb, /// 0x38c
			 0x76c8f5f0, /// 0x390
			 0xf48af6b2, /// 0x394
			 0x3b018b13, /// 0x398
			 0xc10ddb33, /// 0x39c
			 0xd79dcd90, /// 0x3a0
			 0xba819d89, /// 0x3a4
			 0x15c39daa, /// 0x3a8
			 0x8e769e12, /// 0x3ac
			 0xe4edde69, /// 0x3b0
			 0xc4bd785a, /// 0x3b4
			 0x72f017e9, /// 0x3b8
			 0xe1ee26bc, /// 0x3bc
			 0x8c8e67ff, /// 0x3c0
			 0x9a6fedf7, /// 0x3c4
			 0x2528f5b5, /// 0x3c8
			 0x5ab17d54, /// 0x3cc
			 0x2a9e9dc1, /// 0x3d0
			 0x0b5cbd1f, /// 0x3d4
			 0x152d1d31, /// 0x3d8
			 0x367410ef, /// 0x3dc
			 0x6cbddf4e, /// 0x3e0
			 0x96694f65, /// 0x3e4
			 0x80bf45d0, /// 0x3e8
			 0xbab77086, /// 0x3ec
			 0xe70c223b, /// 0x3f0
			 0x2d7054b0, /// 0x3f4
			 0x73a4b9a1, /// 0x3f8
			 0xd0fee112, /// 0x3fc
			 0x98a65a4d, /// 0x400
			 0xa523b353, /// 0x404
			 0x50e314ff, /// 0x408
			 0x5383b820, /// 0x40c
			 0xe0f44f99, /// 0x410
			 0x0d8f8574, /// 0x414
			 0x8a5886a2, /// 0x418
			 0x219dab78, /// 0x41c
			 0xea82a614, /// 0x420
			 0x8199295a, /// 0x424
			 0x7367a707, /// 0x428
			 0x7b5e327e, /// 0x42c
			 0xde2a9c88, /// 0x430
			 0x13aecd1a, /// 0x434
			 0xad3f746b, /// 0x438
			 0x89a1d8e3, /// 0x43c
			 0x80a4acd2, /// 0x440
			 0x8c8b4796, /// 0x444
			 0xd1faced9, /// 0x448
			 0x7b1c9c61, /// 0x44c
			 0x592840d3, /// 0x450
			 0x5eecd45d, /// 0x454
			 0x5515e6f6, /// 0x458
			 0x02806e9a, /// 0x45c
			 0xa82edc58, /// 0x460
			 0x8e71926d, /// 0x464
			 0x7f3c2ca0, /// 0x468
			 0xb89ba2c3, /// 0x46c
			 0xa65af980, /// 0x470
			 0x2083f186, /// 0x474
			 0xecc92f2d, /// 0x478
			 0x92894c5f, /// 0x47c
			 0x72705adc, /// 0x480
			 0x9a603ee4, /// 0x484
			 0x8b980484, /// 0x488
			 0xef783b99, /// 0x48c
			 0xf29b0520, /// 0x490
			 0xb4230d44, /// 0x494
			 0x76c472df, /// 0x498
			 0x2ea42493, /// 0x49c
			 0xcd59dd38, /// 0x4a0
			 0xbabcc6cd, /// 0x4a4
			 0x2b711c5b, /// 0x4a8
			 0xe9ed8f75, /// 0x4ac
			 0x34efb39f, /// 0x4b0
			 0xd624be60, /// 0x4b4
			 0xe40c2eea, /// 0x4b8
			 0x92baf0ec, /// 0x4bc
			 0x036930f1, /// 0x4c0
			 0x803cecc2, /// 0x4c4
			 0x81f6d7e2, /// 0x4c8
			 0x74c23d1b, /// 0x4cc
			 0xdd5e36e0, /// 0x4d0
			 0xff71b568, /// 0x4d4
			 0x2089b57a, /// 0x4d8
			 0xb23825d5, /// 0x4dc
			 0x100048a6, /// 0x4e0
			 0x87fd61c5, /// 0x4e4
			 0x4d4816bb, /// 0x4e8
			 0x9dd3b531, /// 0x4ec
			 0xf5741760, /// 0x4f0
			 0x67c49873, /// 0x4f4
			 0x6724033a, /// 0x4f8
			 0x9eb06021, /// 0x4fc
			 0xa77fd9ea, /// 0x500
			 0x52904809, /// 0x504
			 0xa861048d, /// 0x508
			 0x556999a8, /// 0x50c
			 0x4e38117c, /// 0x510
			 0xa1fbfe2b, /// 0x514
			 0xdf287a00, /// 0x518
			 0xfa487b3b, /// 0x51c
			 0x31b64ba0, /// 0x520
			 0xee28ff10, /// 0x524
			 0x19ceed8d, /// 0x528
			 0xc2260f9d, /// 0x52c
			 0xcaa8982f, /// 0x530
			 0x1735460e, /// 0x534
			 0x961935a0, /// 0x538
			 0x2d815cad, /// 0x53c
			 0x34dfa7fb, /// 0x540
			 0x3cf2a28c, /// 0x544
			 0xb741b2c9, /// 0x548
			 0xdfd1967a, /// 0x54c
			 0x89be92ad, /// 0x550
			 0xa0beadd1, /// 0x554
			 0x0f7d468a, /// 0x558
			 0x248077b9, /// 0x55c
			 0x7ae4865a, /// 0x560
			 0xb6c99fc6, /// 0x564
			 0x77314205, /// 0x568
			 0x765743bc, /// 0x56c
			 0x3150ef1d, /// 0x570
			 0xcaefc800, /// 0x574
			 0x15a586f9, /// 0x578
			 0x132d455f, /// 0x57c
			 0x9517d1b7, /// 0x580
			 0x4c519407, /// 0x584
			 0xa2fd24d0, /// 0x588
			 0x296dca92, /// 0x58c
			 0x89df178d, /// 0x590
			 0x83b97d1c, /// 0x594
			 0x42d3c072, /// 0x598
			 0xd6b81576, /// 0x59c
			 0xf830ad37, /// 0x5a0
			 0x11ba4814, /// 0x5a4
			 0x58b73a60, /// 0x5a8
			 0xac0ffdde, /// 0x5ac
			 0x59795b57, /// 0x5b0
			 0xd2e33568, /// 0x5b4
			 0x30d90e19, /// 0x5b8
			 0x6cf05d6c, /// 0x5bc
			 0xc14c2163, /// 0x5c0
			 0x81eb5f1e, /// 0x5c4
			 0x14135d70, /// 0x5c8
			 0x484e5129, /// 0x5cc
			 0xbdaf549e, /// 0x5d0
			 0x4037b111, /// 0x5d4
			 0xec63097d, /// 0x5d8
			 0x045c0340, /// 0x5dc
			 0xb5a913cf, /// 0x5e0
			 0x1d3d4acd, /// 0x5e4
			 0xe0f258f9, /// 0x5e8
			 0x320b45c8, /// 0x5ec
			 0x67ac29e8, /// 0x5f0
			 0x07032b25, /// 0x5f4
			 0xebe6c6d8, /// 0x5f8
			 0x36755b31, /// 0x5fc
			 0xcaec28fc, /// 0x600
			 0xee4a020a, /// 0x604
			 0x0372bcc0, /// 0x608
			 0xade04e8e, /// 0x60c
			 0x632d47f5, /// 0x610
			 0x8af79cf7, /// 0x614
			 0xdb03edbe, /// 0x618
			 0x2bba927f, /// 0x61c
			 0x2cb2d23c, /// 0x620
			 0xa82ff284, /// 0x624
			 0xc04a2cca, /// 0x628
			 0xa9b17858, /// 0x62c
			 0xc7e0c945, /// 0x630
			 0xaf10e1aa, /// 0x634
			 0x2feeb62f, /// 0x638
			 0x95992245, /// 0x63c
			 0xf009d053, /// 0x640
			 0xf4996271, /// 0x644
			 0xa3bcd521, /// 0x648
			 0x33bb1c2a, /// 0x64c
			 0x0cd27310, /// 0x650
			 0xe6c58ce4, /// 0x654
			 0xc60b245c, /// 0x658
			 0xd2d2fe39, /// 0x65c
			 0x14d7e8f9, /// 0x660
			 0xff587856, /// 0x664
			 0xa9901ef3, /// 0x668
			 0xc8ad7821, /// 0x66c
			 0xc54bfb35, /// 0x670
			 0x18462cb4, /// 0x674
			 0x9462382e, /// 0x678
			 0x5f6296e4, /// 0x67c
			 0xbb3fd8d8, /// 0x680
			 0x830cc257, /// 0x684
			 0xcfc44d56, /// 0x688
			 0x34811748, /// 0x68c
			 0x34bc9436, /// 0x690
			 0x337f3a80, /// 0x694
			 0xf9c52c8c, /// 0x698
			 0x797083ca, /// 0x69c
			 0x199f6e60, /// 0x6a0
			 0x07db659c, /// 0x6a4
			 0x6c7ca82b, /// 0x6a8
			 0xfec80694, /// 0x6ac
			 0x6b758a0f, /// 0x6b0
			 0xdd356599, /// 0x6b4
			 0xb8d199af, /// 0x6b8
			 0xcf427115, /// 0x6bc
			 0xf2373b9d, /// 0x6c0
			 0xad8f4a5d, /// 0x6c4
			 0xe12d0ff3, /// 0x6c8
			 0xe4a2bb9d, /// 0x6cc
			 0x178350f5, /// 0x6d0
			 0x81626689, /// 0x6d4
			 0x57d931d5, /// 0x6d8
			 0x42db7816, /// 0x6dc
			 0x887dc1ca, /// 0x6e0
			 0xf3f3eaf1, /// 0x6e4
			 0x490b61cc, /// 0x6e8
			 0x2e3adcfd, /// 0x6ec
			 0x8a89b034, /// 0x6f0
			 0x66c72557, /// 0x6f4
			 0x60331dbd, /// 0x6f8
			 0xb883ffb9, /// 0x6fc
			 0xd21a2502, /// 0x700
			 0x179d4cc4, /// 0x704
			 0x6188d9f1, /// 0x708
			 0x9e915889, /// 0x70c
			 0x10122734, /// 0x710
			 0x6d40587f, /// 0x714
			 0x12022e92, /// 0x718
			 0x424b20e5, /// 0x71c
			 0x9f375162, /// 0x720
			 0xf760551e, /// 0x724
			 0xb950fcca, /// 0x728
			 0xb8586b29, /// 0x72c
			 0x705d05cd, /// 0x730
			 0x8663f02f, /// 0x734
			 0x2d49c993, /// 0x738
			 0xef38b741, /// 0x73c
			 0x3059e135, /// 0x740
			 0x179d8e7c, /// 0x744
			 0x93541014, /// 0x748
			 0xf75bb43c, /// 0x74c
			 0xc056a9e7, /// 0x750
			 0xe709cfc9, /// 0x754
			 0x7b54b1ac, /// 0x758
			 0x2a745ecd, /// 0x75c
			 0x1e9c1cbf, /// 0x760
			 0x4a241a96, /// 0x764
			 0x56db5be4, /// 0x768
			 0x2300e54b, /// 0x76c
			 0x83b56f6f, /// 0x770
			 0x7b1620dd, /// 0x774
			 0x5a44bdce, /// 0x778
			 0xd2353fd2, /// 0x77c
			 0x36e37505, /// 0x780
			 0x615bc26c, /// 0x784
			 0x58a55be1, /// 0x788
			 0xed379170, /// 0x78c
			 0x9081d771, /// 0x790
			 0xa249e89a, /// 0x794
			 0xb8ecbaee, /// 0x798
			 0x16829559, /// 0x79c
			 0x22e313f3, /// 0x7a0
			 0x28d95c32, /// 0x7a4
			 0x06b30644, /// 0x7a8
			 0xa0a57bdc, /// 0x7ac
			 0x07358e04, /// 0x7b0
			 0xa44e11b1, /// 0x7b4
			 0x4e1ad81f, /// 0x7b8
			 0x39a9bb09, /// 0x7bc
			 0x5e63ee02, /// 0x7c0
			 0xa3bdd68d, /// 0x7c4
			 0xf730dfee, /// 0x7c8
			 0x7339262e, /// 0x7cc
			 0x5c14beb6, /// 0x7d0
			 0xec1c5d29, /// 0x7d4
			 0x976ea183, /// 0x7d8
			 0x23042d0e, /// 0x7dc
			 0xa6f4bbf2, /// 0x7e0
			 0x5206b88d, /// 0x7e4
			 0x502d195d, /// 0x7e8
			 0xab13e9eb, /// 0x7ec
			 0x9ce83a13, /// 0x7f0
			 0xbd600a67, /// 0x7f4
			 0x6ed25f73, /// 0x7f8
			 0x95d81288, /// 0x7fc
			 0x3c73b9d9, /// 0x800
			 0x19c449a8, /// 0x804
			 0x3cb08f7e, /// 0x808
			 0x4f076b5f, /// 0x80c
			 0x576c2d40, /// 0x810
			 0xb3fbe21b, /// 0x814
			 0x916b8f14, /// 0x818
			 0x7a7c11f6, /// 0x81c
			 0x52936c47, /// 0x820
			 0xe238f46c, /// 0x824
			 0xeff3a0f0, /// 0x828
			 0xc9184da3, /// 0x82c
			 0xb4ab469b, /// 0x830
			 0x559f5e7c, /// 0x834
			 0x49d8aeca, /// 0x838
			 0x020f412f, /// 0x83c
			 0xc039bc4e, /// 0x840
			 0x56caf01f, /// 0x844
			 0xe50008b4, /// 0x848
			 0xd9c08dad, /// 0x84c
			 0x9196bc1d, /// 0x850
			 0xd8dfdaef, /// 0x854
			 0xce5f1ff6, /// 0x858
			 0x63a30c6f, /// 0x85c
			 0x6ff5d42d, /// 0x860
			 0x1f57a563, /// 0x864
			 0x0a8ed43c, /// 0x868
			 0x2014cc2e, /// 0x86c
			 0x68a2c6a7, /// 0x870
			 0x2a8106cb, /// 0x874
			 0xbfb877e1, /// 0x878
			 0x6ea48a86, /// 0x87c
			 0x7260b564, /// 0x880
			 0xdd5e5efd, /// 0x884
			 0x3193cf81, /// 0x888
			 0xab535a21, /// 0x88c
			 0x0b301da9, /// 0x890
			 0x2574d7b7, /// 0x894
			 0x3c7d250c, /// 0x898
			 0xaaa1eb5d, /// 0x89c
			 0xeacb70a3, /// 0x8a0
			 0x654d3f58, /// 0x8a4
			 0x39e4a543, /// 0x8a8
			 0x2fa07249, /// 0x8ac
			 0x8185064b, /// 0x8b0
			 0x194a2f2e, /// 0x8b4
			 0xb049f16e, /// 0x8b8
			 0x62881959, /// 0x8bc
			 0x91969f8c, /// 0x8c0
			 0x7cddc8a3, /// 0x8c4
			 0xc8f75dfa, /// 0x8c8
			 0xfbff4486, /// 0x8cc
			 0x4e549fc5, /// 0x8d0
			 0x167e96a0, /// 0x8d4
			 0x5986a8f3, /// 0x8d8
			 0x98ba10d7, /// 0x8dc
			 0x691531d4, /// 0x8e0
			 0xaaf48762, /// 0x8e4
			 0x880bbf92, /// 0x8e8
			 0x7772b30f, /// 0x8ec
			 0x0b1f0e6f, /// 0x8f0
			 0xf86ef717, /// 0x8f4
			 0x54af29b5, /// 0x8f8
			 0xed920adc, /// 0x8fc
			 0xae82a229, /// 0x900
			 0xf19a0342, /// 0x904
			 0xc0c6210a, /// 0x908
			 0x1196c156, /// 0x90c
			 0x436d9e71, /// 0x910
			 0x125d0af0, /// 0x914
			 0xbef8f324, /// 0x918
			 0x2a42c047, /// 0x91c
			 0x396d5f4b, /// 0x920
			 0x28c8c788, /// 0x924
			 0xbbf55426, /// 0x928
			 0xf8aeaf68, /// 0x92c
			 0xe373a524, /// 0x930
			 0x6b41183b, /// 0x934
			 0xf4c59a0f, /// 0x938
			 0xc49d8878, /// 0x93c
			 0xea8d43af, /// 0x940
			 0x2099b239, /// 0x944
			 0xa0c0827b, /// 0x948
			 0x6c52247f, /// 0x94c
			 0x7bc1d4a3, /// 0x950
			 0x35e84c3d, /// 0x954
			 0x3e47711d, /// 0x958
			 0x8e1c30c0, /// 0x95c
			 0x937083db, /// 0x960
			 0x9138e053, /// 0x964
			 0x80fd3336, /// 0x968
			 0x558fbf46, /// 0x96c
			 0x3d51156f, /// 0x970
			 0x101b47b1, /// 0x974
			 0xb4c66fdf, /// 0x978
			 0x9b9a9a3f, /// 0x97c
			 0xc1f9b374, /// 0x980
			 0xb40a54d0, /// 0x984
			 0x26412031, /// 0x988
			 0xc885f352, /// 0x98c
			 0xf02463c4, /// 0x990
			 0x84fbbe68, /// 0x994
			 0x41bbf46c, /// 0x998
			 0x76522652, /// 0x99c
			 0x7185cf9e, /// 0x9a0
			 0x29fcd189, /// 0x9a4
			 0xc9221794, /// 0x9a8
			 0x897ef30e, /// 0x9ac
			 0xc2b84a65, /// 0x9b0
			 0xfaf51583, /// 0x9b4
			 0x59c395aa, /// 0x9b8
			 0x283c50c5, /// 0x9bc
			 0x3ed957e3, /// 0x9c0
			 0x4775d5e5, /// 0x9c4
			 0x3657a734, /// 0x9c8
			 0x32bbcc79, /// 0x9cc
			 0x3e7019c5, /// 0x9d0
			 0x971e6914, /// 0x9d4
			 0xc9749ddd, /// 0x9d8
			 0x361b26b6, /// 0x9dc
			 0xf5dfdcad, /// 0x9e0
			 0x0493bd3d, /// 0x9e4
			 0xcc8ec8a7, /// 0x9e8
			 0x494a878b, /// 0x9ec
			 0x5fda5d58, /// 0x9f0
			 0x1a2d5845, /// 0x9f4
			 0x51cd720c, /// 0x9f8
			 0x8d820ae5, /// 0x9fc
			 0x215b1b10, /// 0xa00
			 0x868f31e1, /// 0xa04
			 0xbfd00d98, /// 0xa08
			 0xf57206a2, /// 0xa0c
			 0x338eee36, /// 0xa10
			 0x5c20d652, /// 0xa14
			 0x0c19ecce, /// 0xa18
			 0xa4af5c83, /// 0xa1c
			 0x8a48305a, /// 0xa20
			 0xcea3aa0c, /// 0xa24
			 0x7cc1f36d, /// 0xa28
			 0x71a8da8c, /// 0xa2c
			 0x961fc8e6, /// 0xa30
			 0xfe8bfe9e, /// 0xa34
			 0xc3b8bc47, /// 0xa38
			 0x6bc13343, /// 0xa3c
			 0xd7b49334, /// 0xa40
			 0x106c4bc8, /// 0xa44
			 0xda9fbb7c, /// 0xa48
			 0x00afef2c, /// 0xa4c
			 0x7499f13b, /// 0xa50
			 0xe5e2865b, /// 0xa54
			 0x218d21af, /// 0xa58
			 0x9ec82842, /// 0xa5c
			 0x856a031c, /// 0xa60
			 0xa634e812, /// 0xa64
			 0xc0c679f4, /// 0xa68
			 0x0d41d432, /// 0xa6c
			 0x1ee160cf, /// 0xa70
			 0x344df628, /// 0xa74
			 0xbd925cce, /// 0xa78
			 0xefbd70ed, /// 0xa7c
			 0x27a5449b, /// 0xa80
			 0xf04bf2c6, /// 0xa84
			 0x7f052bb0, /// 0xa88
			 0x1289ee45, /// 0xa8c
			 0x3806e1ad, /// 0xa90
			 0x25449e78, /// 0xa94
			 0xbbe97478, /// 0xa98
			 0x38345fbf, /// 0xa9c
			 0x6bcdb1b8, /// 0xaa0
			 0xee40d3d5, /// 0xaa4
			 0xa2ce3e10, /// 0xaa8
			 0x3b252dae, /// 0xaac
			 0x595bec1b, /// 0xab0
			 0x5e0832a7, /// 0xab4
			 0xcd13b6ae, /// 0xab8
			 0xac0ceec5, /// 0xabc
			 0x6b0a034a, /// 0xac0
			 0xdab6c1aa, /// 0xac4
			 0x8cf7140d, /// 0xac8
			 0xfd0c21f1, /// 0xacc
			 0x364bd444, /// 0xad0
			 0x4752aa7f, /// 0xad4
			 0x717ba0ab, /// 0xad8
			 0x8b14dbcf, /// 0xadc
			 0xda72488e, /// 0xae0
			 0xa2234465, /// 0xae4
			 0x16eb9800, /// 0xae8
			 0x08e0dce5, /// 0xaec
			 0x3cd0c747, /// 0xaf0
			 0xe08b895c, /// 0xaf4
			 0xdd5346ac, /// 0xaf8
			 0x86c8df5b, /// 0xafc
			 0x291074b6, /// 0xb00
			 0x7cccd93c, /// 0xb04
			 0x078d5052, /// 0xb08
			 0xa935312a, /// 0xb0c
			 0x2ef47dcf, /// 0xb10
			 0x95b2d178, /// 0xb14
			 0x35076dc6, /// 0xb18
			 0x68c5d6a8, /// 0xb1c
			 0x2b731f98, /// 0xb20
			 0xa3026093, /// 0xb24
			 0x1a5f3eed, /// 0xb28
			 0x2565ce6e, /// 0xb2c
			 0x732a0a5b, /// 0xb30
			 0x77a03858, /// 0xb34
			 0x0ff63878, /// 0xb38
			 0xbee6a46c, /// 0xb3c
			 0x7567d121, /// 0xb40
			 0xcda78bc4, /// 0xb44
			 0x99141b45, /// 0xb48
			 0x6200a407, /// 0xb4c
			 0x370f5865, /// 0xb50
			 0xd2a5eacf, /// 0xb54
			 0x848552bd, /// 0xb58
			 0xc54a912c, /// 0xb5c
			 0x4297001a, /// 0xb60
			 0x45e0323c, /// 0xb64
			 0x0dc8f3af, /// 0xb68
			 0x0c31b1bd, /// 0xb6c
			 0xd053215b, /// 0xb70
			 0x79951d9f, /// 0xb74
			 0x6c1284dc, /// 0xb78
			 0x7d90bba2, /// 0xb7c
			 0xa4d843a9, /// 0xb80
			 0x99d0a394, /// 0xb84
			 0x580e0c82, /// 0xb88
			 0xe1adeeab, /// 0xb8c
			 0x59402cb4, /// 0xb90
			 0xa88c79fd, /// 0xb94
			 0xdaefbfd2, /// 0xb98
			 0xfedb7169, /// 0xb9c
			 0xa8877ddc, /// 0xba0
			 0xd5e34a24, /// 0xba4
			 0xfeefbaf2, /// 0xba8
			 0xb5ba072c, /// 0xbac
			 0x2243ec2c, /// 0xbb0
			 0xb6a1e1dc, /// 0xbb4
			 0x69ec06a3, /// 0xbb8
			 0xe559992c, /// 0xbbc
			 0xbcfe1454, /// 0xbc0
			 0xe451457a, /// 0xbc4
			 0x0b47e50c, /// 0xbc8
			 0x8c515847, /// 0xbcc
			 0xafa6b26b, /// 0xbd0
			 0xddf16260, /// 0xbd4
			 0xd979e3f9, /// 0xbd8
			 0x9db1dbfc, /// 0xbdc
			 0xb101d8b1, /// 0xbe0
			 0x7612c1aa, /// 0xbe4
			 0x6a37816a, /// 0xbe8
			 0xa7c921d0, /// 0xbec
			 0x26b1137b, /// 0xbf0
			 0x92696669, /// 0xbf4
			 0x0cb3ec54, /// 0xbf8
			 0xbd45f8b2, /// 0xbfc
			 0x860bce85, /// 0xc00
			 0x3e7bebe2, /// 0xc04
			 0x2de21700, /// 0xc08
			 0x05df14be, /// 0xc0c
			 0xfe6a9d77, /// 0xc10
			 0x78c2523d, /// 0xc14
			 0x2702997a, /// 0xc18
			 0xc61a475a, /// 0xc1c
			 0xa4236caa, /// 0xc20
			 0x14592a97, /// 0xc24
			 0x8cd2e329, /// 0xc28
			 0x2ddd9239, /// 0xc2c
			 0xbc925328, /// 0xc30
			 0x16f95312, /// 0xc34
			 0x3c48384b, /// 0xc38
			 0xae94e718, /// 0xc3c
			 0x1c03cb01, /// 0xc40
			 0xa5dfd5d8, /// 0xc44
			 0x4220fc87, /// 0xc48
			 0xbb0cb9fd, /// 0xc4c
			 0x225a49d8, /// 0xc50
			 0xce6130fa, /// 0xc54
			 0x38a9489f, /// 0xc58
			 0x06e67b83, /// 0xc5c
			 0x7974983e, /// 0xc60
			 0x0177214f, /// 0xc64
			 0x3c568b61, /// 0xc68
			 0x48637e82, /// 0xc6c
			 0x1350f502, /// 0xc70
			 0xe283b085, /// 0xc74
			 0x71e4ce65, /// 0xc78
			 0x53082cec, /// 0xc7c
			 0xdef06ded, /// 0xc80
			 0x344f28d8, /// 0xc84
			 0x4f952ac1, /// 0xc88
			 0x7ef2656b, /// 0xc8c
			 0x9f14f905, /// 0xc90
			 0x321214f9, /// 0xc94
			 0xdd1e4f4d, /// 0xc98
			 0xb7731977, /// 0xc9c
			 0x8c05557f, /// 0xca0
			 0x12babf20, /// 0xca4
			 0x013385be, /// 0xca8
			 0x346ce8ad, /// 0xcac
			 0x06c99a11, /// 0xcb0
			 0xfb945a32, /// 0xcb4
			 0x80fa7c5d, /// 0xcb8
			 0x76bac88e, /// 0xcbc
			 0x19a81a7f, /// 0xcc0
			 0xb8b06241, /// 0xcc4
			 0xd221c089, /// 0xcc8
			 0xe6f8028d, /// 0xccc
			 0x9c4d925a, /// 0xcd0
			 0x2b30d27a, /// 0xcd4
			 0xe5a31e8a, /// 0xcd8
			 0x6c9385d1, /// 0xcdc
			 0xc049c498, /// 0xce0
			 0x9958330b, /// 0xce4
			 0x814d9cef, /// 0xce8
			 0x1b9afaa1, /// 0xcec
			 0x1e774968, /// 0xcf0
			 0x499f521b, /// 0xcf4
			 0x342d29ca, /// 0xcf8
			 0x3215a8f0, /// 0xcfc
			 0xafbcd6d1, /// 0xd00
			 0x3d48d2d7, /// 0xd04
			 0x1b6a01b5, /// 0xd08
			 0xc6cfec1d, /// 0xd0c
			 0x15083724, /// 0xd10
			 0xb0d37f59, /// 0xd14
			 0x5a87eedb, /// 0xd18
			 0x48b7ef95, /// 0xd1c
			 0x6f2de1f0, /// 0xd20
			 0xce6e6e96, /// 0xd24
			 0x2aaa6334, /// 0xd28
			 0xf7f4cd6b, /// 0xd2c
			 0x68aaaea8, /// 0xd30
			 0xf8d554f2, /// 0xd34
			 0xffcceff8, /// 0xd38
			 0x38ef467d, /// 0xd3c
			 0x2fc3334a, /// 0xd40
			 0x7eff36be, /// 0xd44
			 0x5b24a9cf, /// 0xd48
			 0xf5a33114, /// 0xd4c
			 0x3c646350, /// 0xd50
			 0xdf27a144, /// 0xd54
			 0xde345ca0, /// 0xd58
			 0x5a5dcf29, /// 0xd5c
			 0xa7d03b9d, /// 0xd60
			 0x43a94435, /// 0xd64
			 0x09f0019d, /// 0xd68
			 0x90d1fcc7, /// 0xd6c
			 0x64a46fa6, /// 0xd70
			 0xf4ae3807, /// 0xd74
			 0xdd1c1450, /// 0xd78
			 0xf40a3a52, /// 0xd7c
			 0xdbd26c84, /// 0xd80
			 0x46b80def, /// 0xd84
			 0x692c4406, /// 0xd88
			 0xf46e8af3, /// 0xd8c
			 0xc6ea090f, /// 0xd90
			 0x26c75383, /// 0xd94
			 0x649dd519, /// 0xd98
			 0xb15821bc, /// 0xd9c
			 0xd8f1ba91, /// 0xda0
			 0x5319553b, /// 0xda4
			 0x2998fe2f, /// 0xda8
			 0x0b7d3858, /// 0xdac
			 0x298de1ea, /// 0xdb0
			 0x8b4a4d1d, /// 0xdb4
			 0x23e656aa, /// 0xdb8
			 0x7fd6fd48, /// 0xdbc
			 0x21582a44, /// 0xdc0
			 0xb3a1ed41, /// 0xdc4
			 0x0a1e3a7f, /// 0xdc8
			 0xa24c13e3, /// 0xdcc
			 0xc2ac94be, /// 0xdd0
			 0x3e5ffddf, /// 0xdd4
			 0x786a2c8a, /// 0xdd8
			 0x12676b13, /// 0xddc
			 0x57952006, /// 0xde0
			 0x5f404cc3, /// 0xde4
			 0x26e3d6b5, /// 0xde8
			 0xc6b1d676, /// 0xdec
			 0xd5ff0657, /// 0xdf0
			 0x802130c5, /// 0xdf4
			 0x2e3bdfa5, /// 0xdf8
			 0x26ac9719, /// 0xdfc
			 0xfea6584e, /// 0xe00
			 0xf26e27ab, /// 0xe04
			 0x78fc6cb5, /// 0xe08
			 0x19dd42cf, /// 0xe0c
			 0x06b9254c, /// 0xe10
			 0x8193dfa7, /// 0xe14
			 0x85f41dc0, /// 0xe18
			 0x89ebfade, /// 0xe1c
			 0x04a26db1, /// 0xe20
			 0xebde5747, /// 0xe24
			 0xae520574, /// 0xe28
			 0x18f6f771, /// 0xe2c
			 0xd5726ef9, /// 0xe30
			 0x9f20ade4, /// 0xe34
			 0x95550629, /// 0xe38
			 0xbf0662d0, /// 0xe3c
			 0xe389a142, /// 0xe40
			 0x54264ea8, /// 0xe44
			 0x83ed66c1, /// 0xe48
			 0x0cfdf18d, /// 0xe4c
			 0xeda690c7, /// 0xe50
			 0x631927c5, /// 0xe54
			 0x6bd42a0e, /// 0xe58
			 0x0e468559, /// 0xe5c
			 0x32c64c52, /// 0xe60
			 0xab239917, /// 0xe64
			 0x9fee14dd, /// 0xe68
			 0xd211c1ac, /// 0xe6c
			 0xbb525e6e, /// 0xe70
			 0x75757daa, /// 0xe74
			 0x744cbc47, /// 0xe78
			 0xb7657859, /// 0xe7c
			 0x59336bb9, /// 0xe80
			 0xdb2b301c, /// 0xe84
			 0x035428dd, /// 0xe88
			 0xdd34a0cd, /// 0xe8c
			 0xc9b4326d, /// 0xe90
			 0x5c97497b, /// 0xe94
			 0x46b20500, /// 0xe98
			 0xc29465c1, /// 0xe9c
			 0x300f6ddd, /// 0xea0
			 0xef43f080, /// 0xea4
			 0xbf9b17c0, /// 0xea8
			 0x58fa4805, /// 0xeac
			 0xdbc985fe, /// 0xeb0
			 0xcfb1689e, /// 0xeb4
			 0x362dae1b, /// 0xeb8
			 0x29dea51c, /// 0xebc
			 0xb863e23c, /// 0xec0
			 0x8340cab6, /// 0xec4
			 0x30f90348, /// 0xec8
			 0x79bafd18, /// 0xecc
			 0x9fec6cd4, /// 0xed0
			 0x64c7b7d5, /// 0xed4
			 0x1ac5e77b, /// 0xed8
			 0x5a1107e9, /// 0xedc
			 0xd66e1672, /// 0xee0
			 0xfca69ab5, /// 0xee4
			 0xe5be5a2a, /// 0xee8
			 0xb2686a8f, /// 0xeec
			 0x058b12fa, /// 0xef0
			 0x1b3400ea, /// 0xef4
			 0x81b4b9e6, /// 0xef8
			 0x00850a0a, /// 0xefc
			 0x75d621d6, /// 0xf00
			 0xfc30ce28, /// 0xf04
			 0x27108048, /// 0xf08
			 0xe6116071, /// 0xf0c
			 0x58e47d25, /// 0xf10
			 0xe3700361, /// 0xf14
			 0x9cf746c9, /// 0xf18
			 0x49cf5704, /// 0xf1c
			 0x96858e93, /// 0xf20
			 0x924d32d3, /// 0xf24
			 0x7d3ac4dc, /// 0xf28
			 0xf98ad447, /// 0xf2c
			 0x1c442c10, /// 0xf30
			 0x845430d7, /// 0xf34
			 0xcaf1401b, /// 0xf38
			 0x5ab2d222, /// 0xf3c
			 0x441b34e5, /// 0xf40
			 0x2320e11d, /// 0xf44
			 0xe0b31f36, /// 0xf48
			 0x6a8eacbb, /// 0xf4c
			 0xc40b264a, /// 0xf50
			 0xc1239a91, /// 0xf54
			 0x39adb6b5, /// 0xf58
			 0x7af03a0e, /// 0xf5c
			 0x27eae1b5, /// 0xf60
			 0xac93fed4, /// 0xf64
			 0x3bb04fb0, /// 0xf68
			 0xc1091ebc, /// 0xf6c
			 0xe6bd27e2, /// 0xf70
			 0xa55803ad, /// 0xf74
			 0x40b711bf, /// 0xf78
			 0xafd5b588, /// 0xf7c
			 0xb9916d59, /// 0xf80
			 0xd328aff0, /// 0xf84
			 0x169026b0, /// 0xf88
			 0x40653426, /// 0xf8c
			 0xdeaae2aa, /// 0xf90
			 0x01febb28, /// 0xf94
			 0x3bd3b09e, /// 0xf98
			 0xf65e3692, /// 0xf9c
			 0xbb690d23, /// 0xfa0
			 0x320c32f4, /// 0xfa4
			 0x7db57e8d, /// 0xfa8
			 0xa7f7c51c, /// 0xfac
			 0x56f70231, /// 0xfb0
			 0x7d5ffb01, /// 0xfb4
			 0x30a782d2, /// 0xfb8
			 0xcfab1020, /// 0xfbc
			 0x0f2d1b74, /// 0xfc0
			 0xb5de481a, /// 0xfc4
			 0x6e6d7ab9, /// 0xfc8
			 0x8b17832e, /// 0xfcc
			 0x5220a1f3, /// 0xfd0
			 0x9e5b7b31, /// 0xfd4
			 0x01c77fb8, /// 0xfd8
			 0x324530c7, /// 0xfdc
			 0x9d85235e, /// 0xfe0
			 0xfe361faf, /// 0xfe4
			 0x4a678169, /// 0xfe8
			 0xd5854f21, /// 0xfec
			 0x5f282d9f, /// 0xff0
			 0xd17bfdac, /// 0xff4
			 0xb1bc556d, /// 0xff8
			 0x4fdaf1e4 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00011111,                                                  // 9.7958E-41                                  /// 00000
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00004
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00008
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0000c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00010
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00014
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00018
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00020
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00024
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0002c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xffc00001,                                                  // -signaling NaN                              /// 00034
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0003c
			 0x3f028f5c,                                                  // 0.51                                        /// 00040
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x7f800000,                                                  // inf                                         /// 00048
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0004c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00050
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00054
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0005c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00060
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x7fc00001,                                                  // signaling NaN                               /// 00074
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0007c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00088
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0008c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0e000003,                                                  // Trailing 1s:                                /// 0009c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x7fc00001,                                                  // signaling NaN                               /// 000bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x7fc00001,                                                  // signaling NaN                               /// 000d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000ec
			 0x33333333,                                                  // 4 random values                             /// 000f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x7fc00001,                                                  // signaling NaN                               /// 00104
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00108
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0010c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00120
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00124
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00128
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00134
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00138
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0013c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00140
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00144
			 0x55555555,                                                  // 4 random values                             /// 00148
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x55555555,                                                  // 4 random values                             /// 00150
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xbf028f5c,                                                  // -0.51                                       /// 00158
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00160
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0016c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00170
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00174
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00178
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00180
			 0xcb000000,                                                  // -8388608.0                                  /// 00184
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00190
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00194
			 0x0c400000,                                                  // Leading 1s:                                 /// 00198
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0019c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001d4
			 0xffc00001,                                                  // -signaling NaN                              /// 001d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001e0
			 0x3f028f5c,                                                  // 0.51                                        /// 001e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00200
			 0xffc00001,                                                  // -signaling NaN                              /// 00204
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0020c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00210
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00214
			 0x0c400000,                                                  // Leading 1s:                                 /// 00218
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00220
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00228
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00230
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00234
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00238
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00240
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00244
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0024c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00250
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0025c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00264
			 0x4b000000,                                                  // 8388608.0                                   /// 00268
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0026c
			 0xbf028f5c,                                                  // -0.51                                       /// 00270
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00274
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x33333333,                                                  // 4 random values                             /// 00284
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00288
			 0x80011111,                                                  // -9.7958E-41                                 /// 0028c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00290
			 0xcb000000,                                                  // -8388608.0                                  /// 00294
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00298
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0029c
			 0x80000000,                                                  // -zero                                       /// 002a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x55555555,                                                  // 4 random values                             /// 002bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0xcb000000,                                                  // -8388608.0                                  /// 002d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x7fc00001,                                                  // signaling NaN                               /// 002e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f4
			 0xcb000000,                                                  // -8388608.0                                  /// 002f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00304
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00308
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0030c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00310
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80011111,                                                  // -9.7958E-41                                 /// 0031c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00320
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00324
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00328
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0032c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00334
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0033c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00340
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0034c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00358
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00364
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0036c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00378
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00388
			 0x0c780000,                                                  // Leading 1s:                                 /// 0038c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00394
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00000000,                                                  // zero                                        /// 0039c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003a0
			 0x7f800000,                                                  // inf                                         /// 003a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x55555555,                                                  // 4 random values                             /// 003b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d0
			 0x33333333,                                                  // 4 random values                             /// 003d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x55555555,                                                  // 4 random values                             /// 003f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00400
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00408
			 0x0e000003,                                                  // Trailing 1s:                                /// 0040c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00414
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00420
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00424
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x7f800000,                                                  // inf                                         /// 0042c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00430
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00434
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00438
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00440
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00444
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00450
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00454
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00458
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00460
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00468
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00470
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00478
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0047c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00484
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0xffc00001,                                                  // -signaling NaN                              /// 0048c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00498
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x7fc00001,                                                  // signaling NaN                               /// 004f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00500
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00504
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00508
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00510
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00514
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00518
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0051c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00530
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0053c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00540
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00544
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0054c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00554
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00558
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0055c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c780000,                                                  // Leading 1s:                                 /// 00564
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00568
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0056c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00574
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0057c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00580
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00584
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00588
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0058c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00590
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00598
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0059c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d8
			 0xcb000000,                                                  // -8388608.0                                  /// 005dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00600
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00604
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00608
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00610
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00618
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0061c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00620
			 0x00000000,                                                  // zero                                        /// 00624
			 0x0e000007,                                                  // Trailing 1s:                                /// 00628
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0062c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00644
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00648
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0064c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00650
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0c400000,                                                  // Leading 1s:                                 /// 0065c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00660
			 0x7fc00001,                                                  // signaling NaN                               /// 00664
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0066c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00674
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00688
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0068c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00694
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00698
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00700
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00704
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0070c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00718
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0071c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0e000003,                                                  // Trailing 1s:                                /// 00728
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00730
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00734
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0073c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00740
			 0x3f028f5c,                                                  // 0.51                                        /// 00744
			 0x80011111,                                                  // -9.7958E-41                                 /// 00748
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0074c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00750
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00754
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00764
			 0x55555555,                                                  // 4 random values                             /// 00768
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00770
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00774
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0xff800000,                                                  // -inf                                        /// 00780
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00788
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0078c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0079c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b0
			 0x3f028f5c,                                                  // 0.51                                        /// 007b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f0
			 0xcb000000,                                                  // -8388608.0                                  /// 007f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00800
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00808
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0080c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00810
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0081c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00820
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00824
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00828
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0082c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00830
			 0x0c400000,                                                  // Leading 1s:                                 /// 00834
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00838
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00840
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00844
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00850
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00854
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0085c
			 0x3f028f5c,                                                  // 0.51                                        /// 00860
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00864
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00868
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00870
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00874
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00878
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00880
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00888
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0088c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00890
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00894
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00898
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xffc00001,                                                  // -signaling NaN                              /// 008ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008dc
			 0x33333333,                                                  // 4 random values                             /// 008e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00900
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00908
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0090c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00914
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00918
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00920
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00930
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00934
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00938
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00940
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00944
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00948
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0094c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00954
			 0x0c780000,                                                  // Leading 1s:                                 /// 00958
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00964
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00968
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0096c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00974
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x0c400000,                                                  // Leading 1s:                                 /// 0097c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00980
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00984
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00988
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00990
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00998
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0099c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a4
			 0x4b000000,                                                  // 8388608.0                                   /// 009a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x4b000000,                                                  // 8388608.0                                   /// 009cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x55555555,                                                  // 4 random values                             /// 00a10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ab0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ab8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ac0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00acc
			 0x80000000,                                                  // -zero                                       /// 00ad0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ad4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00afc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b10
			 0xbf028f5c,                                                  // -0.51                                       /// 00b14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x33333333,                                                  // 4 random values                             /// 00b54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xffc00001,                                                  // -signaling NaN                              /// 00b6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b74
			 0xff800000,                                                  // -inf                                        /// 00b78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00be0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00be8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x4b000000,                                                  // 8388608.0                                   /// 00c1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c58
			 0x4b000000,                                                  // 8388608.0                                   /// 00c5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c98
			 0x00000000,                                                  // zero                                        /// 00c9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ca0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cb8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ccc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cec
			 0x3f028f5c,                                                  // 0.51                                        /// 00cf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cfc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d38
			 0xff800000,                                                  // -inf                                        /// 00d3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d5c
			 0x00000000,                                                  // zero                                        /// 00d60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0xbf028f5c,                                                  // -0.51                                       /// 00db0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ddc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dec
			 0xff800000,                                                  // -inf                                        /// 00df0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000000,                                                  // -zero                                       /// 00df8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0xff800000,                                                  // -inf                                        /// 00e74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ea4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ea8
			 0xffc00001,                                                  // -signaling NaN                              /// 00eac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ec4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ecc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00edc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ef8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0xbf028f5c,                                                  // -0.51                                       /// 00f4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0e07ffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00000
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00018
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0001c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00020
			 0x33333333,                                                  // 4 random values                             /// 00024
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00028
			 0x0e000001,                                                  // Trailing 1s:                                /// 0002c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00030
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00034
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00038
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0003c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00044
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00048
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0004c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00054
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0005c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00060
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00064
			 0x0c700000,                                                  // Leading 1s:                                 /// 00068
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00070
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00078
			 0x0e000007,                                                  // Trailing 1s:                                /// 0007c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0008c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00094
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d4
			 0x80000000,                                                  // -zero                                       /// 000d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00100
			 0x0c400000,                                                  // Leading 1s:                                 /// 00104
			 0x0c600000,                                                  // Leading 1s:                                 /// 00108
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0010c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00110
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00114
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00118
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00120
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0012c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00130
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00134
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00140
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00144
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00148
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00150
			 0x0c780000,                                                  // Leading 1s:                                 /// 00154
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00158
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x7fc00001,                                                  // signaling NaN                               /// 00164
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00170
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00178
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00184
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00188
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0018c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00190
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001ac
			 0x00000000,                                                  // zero                                        /// 001b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001c8
			 0x7f800000,                                                  // inf                                         /// 001cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e0
			 0x80000000,                                                  // -zero                                       /// 001e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00200
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0020c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00210
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x0c400000,                                                  // Leading 1s:                                 /// 00218
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00220
			 0x7fc00001,                                                  // signaling NaN                               /// 00224
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00228
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00230
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00238
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0023c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00240
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00248
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00254
			 0x7f800000,                                                  // inf                                         /// 00258
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0025c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00260
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00268
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00274
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00278
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00284
			 0xff800000,                                                  // -inf                                        /// 00288
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00290
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00294
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00298
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0029c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c0
			 0x7f800000,                                                  // inf                                         /// 002c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002e8
			 0x00000000,                                                  // zero                                        /// 002ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00304
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0030c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0c700000,                                                  // Leading 1s:                                 /// 0031c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00324
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00328
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0032c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00330
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00338
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00340
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00350
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00354
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0035c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x33333333,                                                  // 4 random values                             /// 00364
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00368
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00370
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0c600000,                                                  // Leading 1s:                                 /// 00384
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0038c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00390
			 0x0e000007,                                                  // Trailing 1s:                                /// 00394
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00398
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0039c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0xffc00001,                                                  // -signaling NaN                              /// 003a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00400
			 0x7fc00001,                                                  // signaling NaN                               /// 00404
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00408
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00410
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00414
			 0x0c700000,                                                  // Leading 1s:                                 /// 00418
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00420
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00424
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00428
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0042c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00430
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00438
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0043c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00440
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00444
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00448
			 0x0e000003,                                                  // Trailing 1s:                                /// 0044c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00450
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00454
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00460
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00464
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00470
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0047c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00480
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00484
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00488
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x7f800000,                                                  // inf                                         /// 00490
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00494
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00498
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0e000007,                                                  // Trailing 1s:                                /// 00504
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00508
			 0xffc00001,                                                  // -signaling NaN                              /// 0050c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00514
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00518
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0051c
			 0x7f800000,                                                  // inf                                         /// 00520
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00524
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00528
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0052c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00530
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00538
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0053c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00540
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00544
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00550
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00554
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0055c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00560
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00564
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00568
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00574
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00578
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0057c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00580
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00584
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0058c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00590
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x3f028f5c,                                                  // 0.51                                        /// 00598
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a4
			 0x4b000000,                                                  // 8388608.0                                   /// 005a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00604
			 0x0c780000,                                                  // Leading 1s:                                 /// 00608
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00614
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00618
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0061c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00624
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0062c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00638
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00644
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00648
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0064c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0065c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x3f028f5c,                                                  // 0.51                                        /// 00664
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00670
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00674
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0067c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00680
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00684
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00688
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00690
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00694
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00700
			 0xcb000000,                                                  // -8388608.0                                  /// 00704
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00708
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00710
			 0x0c700000,                                                  // Leading 1s:                                 /// 00714
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00718
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0071c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00724
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0072c
			 0xbf028f5c,                                                  // -0.51                                       /// 00730
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00738
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0073c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00744
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00748
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0074c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00754
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00758
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0075c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00764
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00768
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0076c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00770
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00774
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00778
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0077c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00780
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00784
			 0x0c780000,                                                  // Leading 1s:                                 /// 00788
			 0xff800000,                                                  // -inf                                        /// 0078c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00794
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00798
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0079c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80000000,                                                  // -zero                                       /// 007bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0xcb000000,                                                  // -8388608.0                                  /// 007f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00800
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00804
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00808
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0080c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00820
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00824
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0082c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80011111,                                                  // -9.7958E-41                                 /// 00834
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00838
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0083c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00848
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00854
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0085c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xcb000000,                                                  // -8388608.0                                  /// 00868
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0086c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00878
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0087c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00880
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00884
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00888
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00890
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00900
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00908
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00914
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00918
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0091c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00920
			 0x0c400000,                                                  // Leading 1s:                                 /// 00924
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00934
			 0x33333333,                                                  // 4 random values                             /// 00938
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00940
			 0x0e000007,                                                  // Trailing 1s:                                /// 00944
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00948
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0094c
			 0x7f800000,                                                  // inf                                         /// 00950
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00954
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00958
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0095c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00960
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00964
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00968
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0096c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00970
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x55555555,                                                  // 4 random values                             /// 00984
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00988
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00994
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0099c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80000000,                                                  // -zero                                       /// 009a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b8
			 0x4b000000,                                                  // 8388608.0                                   /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009cc
			 0x3f028f5c,                                                  // 0.51                                        /// 009d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80000000,                                                  // -zero                                       /// 00a94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xbf028f5c,                                                  // -0.51                                       /// 00aac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x3f028f5c,                                                  // 0.51                                        /// 00abc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00adc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x3f028f5c,                                                  // 0.51                                        /// 00b00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ba4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ba8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bdc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00be0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00be4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00be8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xbf028f5c,                                                  // -0.51                                       /// 00c34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c60
			 0x00000000,                                                  // zero                                        /// 00c64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ca4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ca8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cd4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cf0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d00
			 0x55555555,                                                  // 4 random values                             /// 00d04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d30
			 0x7f800000,                                                  // inf                                         /// 00d34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e50
			 0x4b000000,                                                  // 8388608.0                                   /// 00e54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x33333333,                                                  // 4 random values                             /// 00eb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ec0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ecc
			 0xff800000,                                                  // -inf                                        /// 00ed0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ed4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ed8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00edc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ee8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xffc00001,                                                  // -signaling NaN                              /// 00f00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x7f800000,                                                  // inf                                         /// 00f28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f78
			 0x7fc00001,                                                  // signaling NaN                               /// 00f7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f98
			 0xffc00001,                                                  // -signaling NaN                              /// 00f9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fa4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fec
			 0x55555555,                                                  // 4 random values                             /// 00ff0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ff4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x8f7ffffe                                                  // all bit of mantissa set upto -3ulp          /// last
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
			 0x00000274,
			 0x000001f8,
			 0x0000010c,
			 0x00000368,
			 0x000004a0,
			 0x000006cc,
			 0x0000065c,
			 0x00000218,

			 /// vpu register f2
			 0x40c00000,
			 0x41880000,
			 0x41f00000,
			 0x41900000,
			 0x40a00000,
			 0x41880000,
			 0x41300000,
			 0x41f80000,

			 /// vpu register f3
			 0x41500000,
			 0x41300000,
			 0x41700000,
			 0x41000000,
			 0x41b00000,
			 0x41a80000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f4
			 0x40c00000,
			 0x3f800000,
			 0x41980000,
			 0x41200000,
			 0x41600000,
			 0x42000000,
			 0x41a00000,
			 0x41f00000,

			 /// vpu register f5
			 0x42000000,
			 0x40a00000,
			 0x41b00000,
			 0x40e00000,
			 0x41d80000,
			 0x41e80000,
			 0x41a00000,
			 0x41e00000,

			 /// vpu register f6
			 0x41300000,
			 0x41880000,
			 0x41500000,
			 0x41e00000,
			 0x41700000,
			 0x41d80000,
			 0x41e00000,
			 0x41f00000,

			 /// vpu register f7
			 0x41500000,
			 0x41800000,
			 0x41600000,
			 0x41a80000,
			 0x41200000,
			 0x41e80000,
			 0x41900000,
			 0x41800000,

			 /// vpu register f8
			 0x41e00000,
			 0x41e80000,
			 0x41880000,
			 0x41b00000,
			 0x41d80000,
			 0x41900000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f9
			 0x41200000,
			 0x41d80000,
			 0x41800000,
			 0x40800000,
			 0x41c80000,
			 0x41a80000,
			 0x41a00000,
			 0x41b00000,

			 /// vpu register f10
			 0x41c00000,
			 0x41a00000,
			 0x40800000,
			 0x42000000,
			 0x41900000,
			 0x3f800000,
			 0x41e00000,
			 0x41a80000,

			 /// vpu register f11
			 0x41000000,
			 0x42000000,
			 0x41880000,
			 0x41900000,
			 0x42000000,
			 0x41880000,
			 0x42000000,
			 0x41e80000,

			 /// vpu register f12
			 0x41700000,
			 0x42000000,
			 0x41500000,
			 0x41880000,
			 0x41a80000,
			 0x41e00000,
			 0x41600000,
			 0x40a00000,

			 /// vpu register f13
			 0x41980000,
			 0x41d00000,
			 0x41e80000,
			 0x40000000,
			 0x3f800000,
			 0x41a00000,
			 0x41500000,
			 0x41900000,

			 /// vpu register f14
			 0x41200000,
			 0x41c00000,
			 0x41c80000,
			 0x41b00000,
			 0x40400000,
			 0x40c00000,
			 0x41e80000,
			 0x41600000,

			 /// vpu register f15
			 0x40800000,
			 0x40c00000,
			 0x41c00000,
			 0x40e00000,
			 0x40e00000,
			 0x40800000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f16
			 0x41700000,
			 0x41700000,
			 0x41f00000,
			 0x41c00000,
			 0x41300000,
			 0x41c80000,
			 0x40000000,
			 0x41600000,

			 /// vpu register f17
			 0x41500000,
			 0x41880000,
			 0x41700000,
			 0x40800000,
			 0x41a80000,
			 0x40c00000,
			 0x41500000,
			 0x41900000,

			 /// vpu register f18
			 0x40a00000,
			 0x41c00000,
			 0x41980000,
			 0x41700000,
			 0x41f80000,
			 0x40a00000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f19
			 0x41f80000,
			 0x40400000,
			 0x40a00000,
			 0x41e00000,
			 0x41000000,
			 0x40000000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f20
			 0x41a80000,
			 0x41c00000,
			 0x41a00000,
			 0x41a00000,
			 0x40800000,
			 0x41f00000,
			 0x41c80000,
			 0x41d00000,

			 /// vpu register f21
			 0x41e00000,
			 0x41e80000,
			 0x41b80000,
			 0x41a80000,
			 0x41e00000,
			 0x41600000,
			 0x40a00000,
			 0x41b00000,

			 /// vpu register f22
			 0x40400000,
			 0x41600000,
			 0x40a00000,
			 0x40800000,
			 0x40000000,
			 0x41880000,
			 0x41100000,
			 0x41f80000,

			 /// vpu register f23
			 0x41200000,
			 0x41d80000,
			 0x41900000,
			 0x40800000,
			 0x40800000,
			 0x41980000,
			 0x40000000,
			 0x41e00000,

			 /// vpu register f24
			 0x41100000,
			 0x41800000,
			 0x41d80000,
			 0x41c00000,
			 0x41600000,
			 0x41200000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f25
			 0x40800000,
			 0x41900000,
			 0x41500000,
			 0x42000000,
			 0x41c80000,
			 0x41f80000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f26
			 0x41500000,
			 0x41a80000,
			 0x41400000,
			 0x41400000,
			 0x40e00000,
			 0x40000000,
			 0x41c00000,
			 0x41e00000,

			 /// vpu register f27
			 0x41400000,
			 0x40000000,
			 0x41980000,
			 0x41c80000,
			 0x41a80000,
			 0x41f00000,
			 0x41a00000,
			 0x41f00000,

			 /// vpu register f28
			 0x40000000,
			 0x41e80000,
			 0x41c00000,
			 0x40800000,
			 0x40400000,
			 0x40000000,
			 0x41f00000,
			 0x41800000,

			 /// vpu register f29
			 0x41400000,
			 0x40a00000,
			 0x41e00000,
			 0x41c00000,
			 0x41e00000,
			 0x41600000,
			 0x41a00000,
			 0x40c00000,

			 /// vpu register f30
			 0x41c80000,
			 0x41a00000,
			 0x41a80000,
			 0x41d00000,
			 0x41600000,
			 0x40c00000,
			 0x3f800000,
			 0x41c00000,

			 /// vpu register f31
			 0x40000000,
			 0x42000000,
			 0x41400000,
			 0x41c00000,
			 0x41c80000,
			 0x41880000,
			 0x41900000,
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
		"fsub.ps f16, f28, f23, rdn\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f20, f25, rdn\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f8, f1, f30, dyn\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f14, f24, f8, rup\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f27, f9, f3, rmm\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f28, f20, f12, rdn\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f5, f11, f18, rdn\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f17, f11, f23, rtz\n"                        ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f20, f29, f7, rtz\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f1, f7, rne\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f26, f28, f12, rdn\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f15, f19, f29, rmm\n"                        ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f14, f4, rdn\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f5, f22, f19, rup\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f20, f6, f8, rup\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f16, f22, f19, dyn\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f25, f23, f23, rtz\n"                        ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f27, f6, f24, rtz\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f28, f24, rtz\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f28, f24, f29, rne\n"                        ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f28, f1, f20, rmm\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f19, f5, f11, rdn\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f5, f15, f28, rup\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f6, f22, f8, rtz\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f10, f27, f11, dyn\n"                        ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f3, f25, f0, rne\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f6, f11, f2, rne\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f29, f26, f24, rdn\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f15, f14, f22, rmm\n"                        ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f2, f30, rup\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f19, f22, f26, rne\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f13, f0, rtz\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f20, f30, f1, rdn\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f5, f23, f5, dyn\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f29, f6, f11, rmm\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f20, f24, f4, rne\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f21, f22, f28, rtz\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f10, f1, f20, rtz\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f22, f23, f13, rdn\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f3, f20, rne\n"                          ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f3, f30, f29, dyn\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f3, f0, rtz\n"                           ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f6, f28, f6, rtz\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f5, f26, f4, rdn\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f27, f6, f16, rdn\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f23, f7, f7, rup\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f24, f22, rtz\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f20, f12, f4, rne\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f22, f7, f13, dyn\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f30, f5, f2, rmm\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f10, f2, rup\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f19, f6, f6, rmm\n"                          ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f2, f26, f10, rtz\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f16, f24, f0, rup\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f26, f15, f18, rdn\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f19, f6, f24, rne\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f25, f24, rdn\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f6, f3, f15, dyn\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f7, f21, f26, rne\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f19, f2, f0, rne\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f23, f17, rdn\n"                        ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f8, f7, f25, rne\n"                          ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f22, f22, rdn\n"                        ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f2, f23, f12, rmm\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f4, f22, f14, rne\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f18, f1, f12, rtz\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f19, f25, f4, rtz\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f24, f16, f17, rmm\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f6, f4, f28, rne\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f28, f1, rup\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f12, f15, f30, rtz\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f27, f27, f4, rup\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f4, f19, f24, dyn\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f8, f29, rmm\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f17, f8, f4, rdn\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f11, f14, f0, rmm\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f9, f24, rup\n"                          ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f6, f17, rdn\n"                          ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f27, f5, f12, rup\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f11, f19, f1, rdn\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f17, f5, f17, rdn\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f9, f2, f25, rne\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f30, f3, f6, rtz\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f15, f19, f7, dyn\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f14, f13, rup\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f27, f23, f24, rtz\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f13, f10, f6, rdn\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f22, f17, f15, dyn\n"                        ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f25, f5, f21, rmm\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f14, f7, f19, rup\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f4, f24, f7, rup\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f10, f28, f27, rne\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f25, f14, f30, rne\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f21, f12, f0, rdn\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f7, f28, f7, rmm\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f29, f28, f22, rne\n"                        ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f25, f28, f28, rmm\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f14, f10, f3, rtz\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.ps f6, f7, f28, rdn\n"                          ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
