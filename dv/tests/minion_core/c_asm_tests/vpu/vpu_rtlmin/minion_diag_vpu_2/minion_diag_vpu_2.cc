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
			 0x3a2e482a, /// 0x0
			 0x58e138fb, /// 0x4
			 0x4ff4bb9e, /// 0x8
			 0x4d20c6b1, /// 0xc
			 0x735db916, /// 0x10
			 0x30781b5f, /// 0x14
			 0xaf6755ef, /// 0x18
			 0x62ddd008, /// 0x1c
			 0x97a52e61, /// 0x20
			 0x4bb305e5, /// 0x24
			 0xb9e8f8cf, /// 0x28
			 0x07b261cd, /// 0x2c
			 0x44c309a6, /// 0x30
			 0xe770c4ac, /// 0x34
			 0xd2ab102c, /// 0x38
			 0xb67b54e2, /// 0x3c
			 0xc4841e43, /// 0x40
			 0x9483ab47, /// 0x44
			 0x58eeb2b3, /// 0x48
			 0xb30fd7f6, /// 0x4c
			 0x87675532, /// 0x50
			 0xab61b1f2, /// 0x54
			 0x037ba057, /// 0x58
			 0x997fa1ad, /// 0x5c
			 0x0b85ccbe, /// 0x60
			 0xa08f7773, /// 0x64
			 0x085e7bae, /// 0x68
			 0xf4d1f6dd, /// 0x6c
			 0x481451b0, /// 0x70
			 0x66349db1, /// 0x74
			 0xb3b8f141, /// 0x78
			 0x93ef9bb5, /// 0x7c
			 0x95023747, /// 0x80
			 0xc4feab22, /// 0x84
			 0x2334791f, /// 0x88
			 0x5c3c46a1, /// 0x8c
			 0xb09425aa, /// 0x90
			 0xa7915d2a, /// 0x94
			 0x607998e6, /// 0x98
			 0x7e07cd29, /// 0x9c
			 0x91198df0, /// 0xa0
			 0x6891bd68, /// 0xa4
			 0x5015d7ee, /// 0xa8
			 0xc7e0060d, /// 0xac
			 0xab0ef368, /// 0xb0
			 0x6cdee491, /// 0xb4
			 0x38748d70, /// 0xb8
			 0xfbd48971, /// 0xbc
			 0x98600a60, /// 0xc0
			 0x19811d24, /// 0xc4
			 0x8d9153f8, /// 0xc8
			 0xa4bd3aaf, /// 0xcc
			 0x33beec0f, /// 0xd0
			 0xf1a146be, /// 0xd4
			 0x6d676b2f, /// 0xd8
			 0x065395ba, /// 0xdc
			 0xdbc6b7f8, /// 0xe0
			 0x2bdf7602, /// 0xe4
			 0xb40bdcf5, /// 0xe8
			 0xd63d9217, /// 0xec
			 0xa349ef45, /// 0xf0
			 0x185cb8cd, /// 0xf4
			 0xf17b6ea2, /// 0xf8
			 0x310fd73a, /// 0xfc
			 0x6dd6042b, /// 0x100
			 0x3c2114c2, /// 0x104
			 0xb4cac471, /// 0x108
			 0x3a6b421a, /// 0x10c
			 0x2294db78, /// 0x110
			 0x5ce09ea8, /// 0x114
			 0x568410a1, /// 0x118
			 0xd7e5b499, /// 0x11c
			 0x4d984d0a, /// 0x120
			 0x8bbc3d20, /// 0x124
			 0x82fc3fed, /// 0x128
			 0x2699e717, /// 0x12c
			 0x7da52434, /// 0x130
			 0x924fa617, /// 0x134
			 0x3168605d, /// 0x138
			 0x62016aa8, /// 0x13c
			 0x5c4e0777, /// 0x140
			 0x8548b233, /// 0x144
			 0xbb47617a, /// 0x148
			 0x38e6f401, /// 0x14c
			 0x4c18a1d1, /// 0x150
			 0x2e81323d, /// 0x154
			 0xbda4ee11, /// 0x158
			 0x30a791d2, /// 0x15c
			 0xfbb3c938, /// 0x160
			 0x4ee19d84, /// 0x164
			 0x2efc8631, /// 0x168
			 0x4576bc6f, /// 0x16c
			 0x2841c182, /// 0x170
			 0x5635fa7d, /// 0x174
			 0x147bc73e, /// 0x178
			 0x53cce504, /// 0x17c
			 0xdec9c7ec, /// 0x180
			 0x4b48d368, /// 0x184
			 0x93530ee5, /// 0x188
			 0x2266f46c, /// 0x18c
			 0x3d09c74e, /// 0x190
			 0x9da9c202, /// 0x194
			 0x6730b16e, /// 0x198
			 0xca560088, /// 0x19c
			 0x413293d9, /// 0x1a0
			 0x92d346ef, /// 0x1a4
			 0x0d9f0b33, /// 0x1a8
			 0x1a95aaed, /// 0x1ac
			 0x974aee96, /// 0x1b0
			 0x12bf9436, /// 0x1b4
			 0x7e05604a, /// 0x1b8
			 0x82870cef, /// 0x1bc
			 0x2ea9c364, /// 0x1c0
			 0x5bb92aec, /// 0x1c4
			 0x561e6d01, /// 0x1c8
			 0x947e2532, /// 0x1cc
			 0x7bee26cd, /// 0x1d0
			 0xe5200c5a, /// 0x1d4
			 0xb0a2a055, /// 0x1d8
			 0xc25a27c8, /// 0x1dc
			 0x4ad04638, /// 0x1e0
			 0x98441b9a, /// 0x1e4
			 0x3bdd12ea, /// 0x1e8
			 0x80022192, /// 0x1ec
			 0x6886e173, /// 0x1f0
			 0x7edfebb2, /// 0x1f4
			 0x3af40598, /// 0x1f8
			 0xf86c49bd, /// 0x1fc
			 0x6ed40620, /// 0x200
			 0xa19d8c65, /// 0x204
			 0x9d12d589, /// 0x208
			 0xc5507c3b, /// 0x20c
			 0xfb22bdbf, /// 0x210
			 0x5af42ad5, /// 0x214
			 0x9534d30f, /// 0x218
			 0x5070751d, /// 0x21c
			 0x72f00663, /// 0x220
			 0x4fa9f1cb, /// 0x224
			 0x7763430d, /// 0x228
			 0xe9ef05b3, /// 0x22c
			 0x5666ef9e, /// 0x230
			 0xee8efe03, /// 0x234
			 0x33d060d2, /// 0x238
			 0x79cde3d3, /// 0x23c
			 0x19af6463, /// 0x240
			 0xa2e3cb9c, /// 0x244
			 0xfb56e437, /// 0x248
			 0x7b8cc32a, /// 0x24c
			 0xfac7d9e1, /// 0x250
			 0x24d5204e, /// 0x254
			 0xea8f4081, /// 0x258
			 0x5996f146, /// 0x25c
			 0x1f24ecf6, /// 0x260
			 0x53de50d6, /// 0x264
			 0xe8f1cb01, /// 0x268
			 0x1298e11e, /// 0x26c
			 0x46537d92, /// 0x270
			 0xafdd4735, /// 0x274
			 0x8b1d432a, /// 0x278
			 0x89a1a2be, /// 0x27c
			 0x15698fa1, /// 0x280
			 0x217f1ef8, /// 0x284
			 0x4a1a5d3e, /// 0x288
			 0x178d91e2, /// 0x28c
			 0x97c21ffc, /// 0x290
			 0x95b3ddcc, /// 0x294
			 0x09110cdf, /// 0x298
			 0x681227d5, /// 0x29c
			 0xd755fdc9, /// 0x2a0
			 0x9b1a163b, /// 0x2a4
			 0x50541f33, /// 0x2a8
			 0xe4172bc1, /// 0x2ac
			 0xe1eee208, /// 0x2b0
			 0xd52e604f, /// 0x2b4
			 0x04ff2f91, /// 0x2b8
			 0x8d47f31d, /// 0x2bc
			 0x5bfd9c0d, /// 0x2c0
			 0x96cb609c, /// 0x2c4
			 0xdb8ac6fc, /// 0x2c8
			 0x8d19dcb8, /// 0x2cc
			 0x2bb41233, /// 0x2d0
			 0x6195ddc4, /// 0x2d4
			 0x57f2d6b4, /// 0x2d8
			 0x3a0aa418, /// 0x2dc
			 0x89034402, /// 0x2e0
			 0x107c3ede, /// 0x2e4
			 0xe65512ab, /// 0x2e8
			 0x8488920b, /// 0x2ec
			 0x86fc7a48, /// 0x2f0
			 0x9ce5a366, /// 0x2f4
			 0xf880a546, /// 0x2f8
			 0xd88a177e, /// 0x2fc
			 0x76f216a4, /// 0x300
			 0x9b9c774f, /// 0x304
			 0x10f1945b, /// 0x308
			 0x1c80d7bf, /// 0x30c
			 0x6037ef6d, /// 0x310
			 0xd3e7dbbb, /// 0x314
			 0xc7ce74a5, /// 0x318
			 0xbbe2c496, /// 0x31c
			 0xe3bb1418, /// 0x320
			 0x7ecf4a8f, /// 0x324
			 0xdd1c12a7, /// 0x328
			 0x570a54f7, /// 0x32c
			 0x81174f1a, /// 0x330
			 0xb1ac7811, /// 0x334
			 0x09cc66cc, /// 0x338
			 0x5bf87902, /// 0x33c
			 0xb169955b, /// 0x340
			 0xf6a4b92e, /// 0x344
			 0xc572002a, /// 0x348
			 0x7187e3a4, /// 0x34c
			 0x88da2505, /// 0x350
			 0xcea0e27a, /// 0x354
			 0x1649216a, /// 0x358
			 0x9fa575d1, /// 0x35c
			 0x600287ea, /// 0x360
			 0x0e80edb2, /// 0x364
			 0x5eebfb7b, /// 0x368
			 0x05177430, /// 0x36c
			 0xdce7bca1, /// 0x370
			 0xb9a6adf9, /// 0x374
			 0x14a0529b, /// 0x378
			 0x4c8b8036, /// 0x37c
			 0xbd0e9328, /// 0x380
			 0x21b63111, /// 0x384
			 0xb765065c, /// 0x388
			 0xa1b6d443, /// 0x38c
			 0x4a248e3a, /// 0x390
			 0xc7d00af8, /// 0x394
			 0x27b47de3, /// 0x398
			 0x77008491, /// 0x39c
			 0x8d51e2e3, /// 0x3a0
			 0x95372029, /// 0x3a4
			 0xe6b9645d, /// 0x3a8
			 0x6f533f5f, /// 0x3ac
			 0x64cbdd2a, /// 0x3b0
			 0xe2e811eb, /// 0x3b4
			 0x5a6e7114, /// 0x3b8
			 0xbcabaad4, /// 0x3bc
			 0x5413491c, /// 0x3c0
			 0x00c1b00e, /// 0x3c4
			 0xbc10dcc3, /// 0x3c8
			 0xa3695184, /// 0x3cc
			 0xd2b88a0d, /// 0x3d0
			 0x7d90a08e, /// 0x3d4
			 0x27c68ab0, /// 0x3d8
			 0x684c1374, /// 0x3dc
			 0x9df96bcf, /// 0x3e0
			 0xdc154ba4, /// 0x3e4
			 0x87cc3a94, /// 0x3e8
			 0x02bb83af, /// 0x3ec
			 0xa9c0f121, /// 0x3f0
			 0xef5a87c4, /// 0x3f4
			 0x54b0c9ac, /// 0x3f8
			 0x094cb4bc, /// 0x3fc
			 0x1dedf46e, /// 0x400
			 0xc52e7642, /// 0x404
			 0x02e5ab50, /// 0x408
			 0xd0832078, /// 0x40c
			 0xe54d383d, /// 0x410
			 0x0fac2ff7, /// 0x414
			 0x31365fe8, /// 0x418
			 0x88e961de, /// 0x41c
			 0x5138686d, /// 0x420
			 0x845e82f4, /// 0x424
			 0x49b3d72e, /// 0x428
			 0x6f451ed2, /// 0x42c
			 0x954b8fae, /// 0x430
			 0x0777b80d, /// 0x434
			 0x7af2573b, /// 0x438
			 0x51334b95, /// 0x43c
			 0x8d3718c5, /// 0x440
			 0x3931f9a0, /// 0x444
			 0xe17c8e77, /// 0x448
			 0xd392b7ac, /// 0x44c
			 0xc29a38fb, /// 0x450
			 0xf07585ae, /// 0x454
			 0x6ce97af9, /// 0x458
			 0xf31e322f, /// 0x45c
			 0xa86888ea, /// 0x460
			 0xd8ae1f56, /// 0x464
			 0x7d2da622, /// 0x468
			 0xdc53df59, /// 0x46c
			 0x088b6926, /// 0x470
			 0x46968a87, /// 0x474
			 0x00ee6675, /// 0x478
			 0xcb716912, /// 0x47c
			 0x3c5bb3f7, /// 0x480
			 0xf46de7aa, /// 0x484
			 0x8aca019a, /// 0x488
			 0x83489d3b, /// 0x48c
			 0x96d1dd30, /// 0x490
			 0x122ecb31, /// 0x494
			 0x3391b637, /// 0x498
			 0xda2b2432, /// 0x49c
			 0x72ee042c, /// 0x4a0
			 0x639f5716, /// 0x4a4
			 0x72f8d04c, /// 0x4a8
			 0x02edeeaa, /// 0x4ac
			 0x7e5e8512, /// 0x4b0
			 0x35c9ba3c, /// 0x4b4
			 0x8cc32fcc, /// 0x4b8
			 0x84e2c27d, /// 0x4bc
			 0xcc332db7, /// 0x4c0
			 0x81a6e840, /// 0x4c4
			 0xb1a6ebb1, /// 0x4c8
			 0x2eb61d39, /// 0x4cc
			 0x36372d72, /// 0x4d0
			 0xe87e60e6, /// 0x4d4
			 0x52821944, /// 0x4d8
			 0x1cd2f7ff, /// 0x4dc
			 0x99981b5a, /// 0x4e0
			 0x696672cc, /// 0x4e4
			 0x80a06d36, /// 0x4e8
			 0x6e5f46f6, /// 0x4ec
			 0x463708c1, /// 0x4f0
			 0xad2d8c1f, /// 0x4f4
			 0xf2690cfc, /// 0x4f8
			 0x58b6afec, /// 0x4fc
			 0x1de41c85, /// 0x500
			 0x6315c692, /// 0x504
			 0x03f56f70, /// 0x508
			 0x0b51072d, /// 0x50c
			 0xd23bc818, /// 0x510
			 0xbe6d712b, /// 0x514
			 0x67c3f3f9, /// 0x518
			 0xe3d17752, /// 0x51c
			 0x4dbc8c51, /// 0x520
			 0x22c9923a, /// 0x524
			 0xbd77b6cf, /// 0x528
			 0xa0a91a49, /// 0x52c
			 0xd0a7ed4a, /// 0x530
			 0x3599b126, /// 0x534
			 0xb79e3e7e, /// 0x538
			 0x0d6b8837, /// 0x53c
			 0xcb50736e, /// 0x540
			 0xed464b50, /// 0x544
			 0x6372fdb8, /// 0x548
			 0x61bc0bfd, /// 0x54c
			 0xab1496fb, /// 0x550
			 0xab8a09e5, /// 0x554
			 0x7be8a456, /// 0x558
			 0xd12ecd12, /// 0x55c
			 0x88bba847, /// 0x560
			 0xb1c8bc49, /// 0x564
			 0x6d1e5c42, /// 0x568
			 0x155858c8, /// 0x56c
			 0x24464f1b, /// 0x570
			 0x8e191d20, /// 0x574
			 0xb1a8a0af, /// 0x578
			 0xf5f5fee0, /// 0x57c
			 0xf8bcf1c6, /// 0x580
			 0x2335d38a, /// 0x584
			 0x87af7185, /// 0x588
			 0x4908fd8e, /// 0x58c
			 0x8a308e77, /// 0x590
			 0xf4b4ac8a, /// 0x594
			 0x421c714f, /// 0x598
			 0xb7fb0572, /// 0x59c
			 0x6b3fef77, /// 0x5a0
			 0xe79b156b, /// 0x5a4
			 0xa901136b, /// 0x5a8
			 0x97caf908, /// 0x5ac
			 0x7d271f4f, /// 0x5b0
			 0xd9e7569d, /// 0x5b4
			 0xc71073ce, /// 0x5b8
			 0x508e5ea0, /// 0x5bc
			 0xbb80d151, /// 0x5c0
			 0xace85073, /// 0x5c4
			 0xbeef8c2b, /// 0x5c8
			 0x09f12004, /// 0x5cc
			 0x668c580a, /// 0x5d0
			 0x94ce8f96, /// 0x5d4
			 0xbe9bbc79, /// 0x5d8
			 0x2eb4c46d, /// 0x5dc
			 0x6f800848, /// 0x5e0
			 0x257c5de3, /// 0x5e4
			 0x22fdc32f, /// 0x5e8
			 0xc5e36aad, /// 0x5ec
			 0xcafd2a8e, /// 0x5f0
			 0xa32ffc42, /// 0x5f4
			 0x6de18675, /// 0x5f8
			 0x6caf0137, /// 0x5fc
			 0xde5a4435, /// 0x600
			 0x156a5289, /// 0x604
			 0xb35e590c, /// 0x608
			 0x911ef9e9, /// 0x60c
			 0x11fd1bd1, /// 0x610
			 0x7f52300f, /// 0x614
			 0xdd051fcc, /// 0x618
			 0xc86e31a9, /// 0x61c
			 0x1a5dae67, /// 0x620
			 0x9dbe8d68, /// 0x624
			 0xa28c0043, /// 0x628
			 0xb84727d6, /// 0x62c
			 0x37675d6f, /// 0x630
			 0xa50c41c6, /// 0x634
			 0xede3e5ee, /// 0x638
			 0xc811c078, /// 0x63c
			 0x3e7eb255, /// 0x640
			 0x91e4e6a4, /// 0x644
			 0x2b13d76e, /// 0x648
			 0xfbf852ac, /// 0x64c
			 0xccdc6f0d, /// 0x650
			 0x2065b799, /// 0x654
			 0xb0312fd7, /// 0x658
			 0x67fe60cc, /// 0x65c
			 0x9b191eb4, /// 0x660
			 0xe0695284, /// 0x664
			 0xfb783e48, /// 0x668
			 0xf9fb61c3, /// 0x66c
			 0xa3ab7fb9, /// 0x670
			 0xf445374d, /// 0x674
			 0xe1a89a24, /// 0x678
			 0x42915f94, /// 0x67c
			 0x8a4b24d0, /// 0x680
			 0xa986370e, /// 0x684
			 0x36cb6d10, /// 0x688
			 0x166ba8db, /// 0x68c
			 0xe2e792f1, /// 0x690
			 0xa5efa15f, /// 0x694
			 0xe401fe7b, /// 0x698
			 0xec571b37, /// 0x69c
			 0x85521373, /// 0x6a0
			 0x6b5e18f6, /// 0x6a4
			 0xd62b8b57, /// 0x6a8
			 0x75e51b93, /// 0x6ac
			 0x37863179, /// 0x6b0
			 0x0d5cd175, /// 0x6b4
			 0x2e97e155, /// 0x6b8
			 0x6bc49642, /// 0x6bc
			 0xeedc312f, /// 0x6c0
			 0x75dbe78a, /// 0x6c4
			 0x92adbe2b, /// 0x6c8
			 0x09fc6655, /// 0x6cc
			 0x339bbbd0, /// 0x6d0
			 0x81d0d06b, /// 0x6d4
			 0x4fd450bf, /// 0x6d8
			 0x8fd59789, /// 0x6dc
			 0xbd331348, /// 0x6e0
			 0x7b4767af, /// 0x6e4
			 0x9a8d7d9c, /// 0x6e8
			 0xc9f0980a, /// 0x6ec
			 0x7f4e73ee, /// 0x6f0
			 0xcd0ea4ee, /// 0x6f4
			 0x5a17470d, /// 0x6f8
			 0x1ed5ff50, /// 0x6fc
			 0xfe688de6, /// 0x700
			 0x8623e2c1, /// 0x704
			 0x5ed7d7a2, /// 0x708
			 0x075cf9ec, /// 0x70c
			 0x43fbfd4f, /// 0x710
			 0x244ba2fe, /// 0x714
			 0xbb8685f1, /// 0x718
			 0xf53519ba, /// 0x71c
			 0x3b1a2a2d, /// 0x720
			 0xbb8892ff, /// 0x724
			 0x6ace8b83, /// 0x728
			 0x6e2ae536, /// 0x72c
			 0x7ccf64f8, /// 0x730
			 0x1441904d, /// 0x734
			 0x85a4bbda, /// 0x738
			 0x22afaffd, /// 0x73c
			 0xe9713f23, /// 0x740
			 0x02a120a6, /// 0x744
			 0xb6935b98, /// 0x748
			 0x82b0c505, /// 0x74c
			 0x28696c7b, /// 0x750
			 0xc1965452, /// 0x754
			 0x5f92ec16, /// 0x758
			 0xbd8ceb18, /// 0x75c
			 0x4b13e628, /// 0x760
			 0x1906af82, /// 0x764
			 0xca1ccbbb, /// 0x768
			 0x8db6f792, /// 0x76c
			 0x16d50e3c, /// 0x770
			 0xc19f7f37, /// 0x774
			 0xb5a980c6, /// 0x778
			 0x3702fbe7, /// 0x77c
			 0xf14b7240, /// 0x780
			 0xc983c2ff, /// 0x784
			 0xfac111bc, /// 0x788
			 0x4d01cbaa, /// 0x78c
			 0xc9f9bee7, /// 0x790
			 0x006dafd9, /// 0x794
			 0x9e41977c, /// 0x798
			 0x2ff253fe, /// 0x79c
			 0x43d68f39, /// 0x7a0
			 0xba3276de, /// 0x7a4
			 0x71baa7ff, /// 0x7a8
			 0x23a06be3, /// 0x7ac
			 0x06301d01, /// 0x7b0
			 0x9452ed26, /// 0x7b4
			 0x2090d0cf, /// 0x7b8
			 0xd8e171cb, /// 0x7bc
			 0xa8b39b9a, /// 0x7c0
			 0xb60139e6, /// 0x7c4
			 0x09815017, /// 0x7c8
			 0xa81aaf76, /// 0x7cc
			 0x978eade4, /// 0x7d0
			 0x31742c04, /// 0x7d4
			 0x28d1cf33, /// 0x7d8
			 0xb802a8fd, /// 0x7dc
			 0xdb4755bb, /// 0x7e0
			 0x0f6c559f, /// 0x7e4
			 0x343297fb, /// 0x7e8
			 0x76057067, /// 0x7ec
			 0xb5243a50, /// 0x7f0
			 0xe9dbdd16, /// 0x7f4
			 0xfd16a103, /// 0x7f8
			 0x4c1d88ea, /// 0x7fc
			 0x110132c8, /// 0x800
			 0xfb1f3d0f, /// 0x804
			 0x072ad27e, /// 0x808
			 0x57d348e4, /// 0x80c
			 0x6c051d34, /// 0x810
			 0x5b02df25, /// 0x814
			 0x904c0ba0, /// 0x818
			 0xf15e2d17, /// 0x81c
			 0x28717789, /// 0x820
			 0x863db136, /// 0x824
			 0x8a2b077c, /// 0x828
			 0x90dd404c, /// 0x82c
			 0x59de8c89, /// 0x830
			 0x2855997d, /// 0x834
			 0x1593c4dc, /// 0x838
			 0x5a8427c2, /// 0x83c
			 0x998d9491, /// 0x840
			 0xdc189f16, /// 0x844
			 0xd6450369, /// 0x848
			 0x1edd97f6, /// 0x84c
			 0x48cb4b5b, /// 0x850
			 0xab929335, /// 0x854
			 0x8067ffe9, /// 0x858
			 0xb561ebce, /// 0x85c
			 0x85cf517a, /// 0x860
			 0x91d209bc, /// 0x864
			 0x210221cc, /// 0x868
			 0x37e502a6, /// 0x86c
			 0xe56a2ffc, /// 0x870
			 0xa41da438, /// 0x874
			 0x1132e923, /// 0x878
			 0x6fce3ab2, /// 0x87c
			 0xc1a9a1fe, /// 0x880
			 0xced857f6, /// 0x884
			 0x15bc83dc, /// 0x888
			 0xf3e15e5d, /// 0x88c
			 0x55c057be, /// 0x890
			 0xa0508a76, /// 0x894
			 0x503084c7, /// 0x898
			 0x4ce0243c, /// 0x89c
			 0xb4b6bff7, /// 0x8a0
			 0xb6b5a81d, /// 0x8a4
			 0xbabea965, /// 0x8a8
			 0xe1f07304, /// 0x8ac
			 0x0da801f2, /// 0x8b0
			 0x9ad5bb02, /// 0x8b4
			 0xaa955e58, /// 0x8b8
			 0x635e41fc, /// 0x8bc
			 0x4b2e58b0, /// 0x8c0
			 0xa0312667, /// 0x8c4
			 0x78b09a0f, /// 0x8c8
			 0xce0eaec0, /// 0x8cc
			 0x950ab92f, /// 0x8d0
			 0x5b21b842, /// 0x8d4
			 0x32155c1e, /// 0x8d8
			 0x80bc01b3, /// 0x8dc
			 0x8c2c7892, /// 0x8e0
			 0x6d471150, /// 0x8e4
			 0x50b1d3da, /// 0x8e8
			 0xca07393a, /// 0x8ec
			 0x74105769, /// 0x8f0
			 0x4c5b2646, /// 0x8f4
			 0x6ba4f550, /// 0x8f8
			 0xce91ca27, /// 0x8fc
			 0x99a5d696, /// 0x900
			 0x6282a335, /// 0x904
			 0xbde70524, /// 0x908
			 0xc0c5a07a, /// 0x90c
			 0x3c3e1e73, /// 0x910
			 0xf7544bb4, /// 0x914
			 0xe8b1ed58, /// 0x918
			 0xa2ac2c2d, /// 0x91c
			 0x5b535c72, /// 0x920
			 0x3e63a5ec, /// 0x924
			 0x0bcc69aa, /// 0x928
			 0x2895874f, /// 0x92c
			 0xc535ad0f, /// 0x930
			 0xb4713363, /// 0x934
			 0x578b9099, /// 0x938
			 0xe454c1dd, /// 0x93c
			 0x4b86777e, /// 0x940
			 0x069dea64, /// 0x944
			 0x8b3d410b, /// 0x948
			 0x8bbb7c43, /// 0x94c
			 0xedf6b415, /// 0x950
			 0x5fd60ca9, /// 0x954
			 0xe2ed0526, /// 0x958
			 0x02a19bb7, /// 0x95c
			 0x2d9db469, /// 0x960
			 0xb5cb46f7, /// 0x964
			 0x58e42bd7, /// 0x968
			 0xf631eed0, /// 0x96c
			 0x5d07e78c, /// 0x970
			 0x58ab49b8, /// 0x974
			 0x5fb369ac, /// 0x978
			 0x3e32581c, /// 0x97c
			 0x2715bf36, /// 0x980
			 0x7cc8ed07, /// 0x984
			 0xcea6d525, /// 0x988
			 0x98976771, /// 0x98c
			 0xeea2d416, /// 0x990
			 0xd9592598, /// 0x994
			 0x28b91fc4, /// 0x998
			 0x413d5cf0, /// 0x99c
			 0xd82d9da2, /// 0x9a0
			 0xa13bd1a6, /// 0x9a4
			 0x716faacb, /// 0x9a8
			 0xb1281417, /// 0x9ac
			 0xa8b9d65f, /// 0x9b0
			 0xa53757cf, /// 0x9b4
			 0x97dccefa, /// 0x9b8
			 0x1d72e0ca, /// 0x9bc
			 0x4ff396ba, /// 0x9c0
			 0xc2f0d3ad, /// 0x9c4
			 0xcb58c09b, /// 0x9c8
			 0x6e7edb33, /// 0x9cc
			 0xebd2281a, /// 0x9d0
			 0xd4aeb8ff, /// 0x9d4
			 0xd66a6f4c, /// 0x9d8
			 0x9e83e2f3, /// 0x9dc
			 0xc54cbc2b, /// 0x9e0
			 0x6295b616, /// 0x9e4
			 0x201cf0f9, /// 0x9e8
			 0x889f9b64, /// 0x9ec
			 0xfd619ccf, /// 0x9f0
			 0x3fe4fc6b, /// 0x9f4
			 0x804befd1, /// 0x9f8
			 0x0cbb90d9, /// 0x9fc
			 0x6b66b4f5, /// 0xa00
			 0x4c983fba, /// 0xa04
			 0x0b63d86c, /// 0xa08
			 0x15b987e5, /// 0xa0c
			 0xd07d213b, /// 0xa10
			 0xa3de41b7, /// 0xa14
			 0x2efb3f14, /// 0xa18
			 0x26dad247, /// 0xa1c
			 0x21937592, /// 0xa20
			 0x263933c2, /// 0xa24
			 0xeafbc049, /// 0xa28
			 0x0c5d379a, /// 0xa2c
			 0x5df2ca11, /// 0xa30
			 0x442a0910, /// 0xa34
			 0x8eed400d, /// 0xa38
			 0x6736b9e8, /// 0xa3c
			 0x63ccaa6c, /// 0xa40
			 0x28cf25cd, /// 0xa44
			 0x5b866a03, /// 0xa48
			 0x67baf962, /// 0xa4c
			 0x4aa55e74, /// 0xa50
			 0x346080a9, /// 0xa54
			 0xe0753e24, /// 0xa58
			 0xfb3a1a72, /// 0xa5c
			 0xbf76e213, /// 0xa60
			 0xc7962543, /// 0xa64
			 0xb717ebea, /// 0xa68
			 0x14c6c4cc, /// 0xa6c
			 0xfc14afa4, /// 0xa70
			 0x7e7e5603, /// 0xa74
			 0x6e3ea738, /// 0xa78
			 0xfedb859e, /// 0xa7c
			 0xccc65378, /// 0xa80
			 0xfc1ac593, /// 0xa84
			 0xc58d3586, /// 0xa88
			 0x8ccce438, /// 0xa8c
			 0xd7fb9fab, /// 0xa90
			 0x6e6ff8e3, /// 0xa94
			 0x89bba498, /// 0xa98
			 0x0cf29caf, /// 0xa9c
			 0xfd590da9, /// 0xaa0
			 0x52d056a3, /// 0xaa4
			 0x3bfbfa56, /// 0xaa8
			 0xff4f858f, /// 0xaac
			 0xcee37243, /// 0xab0
			 0x48a4983a, /// 0xab4
			 0x1684ff4e, /// 0xab8
			 0x3a55dcc7, /// 0xabc
			 0xb9d3f172, /// 0xac0
			 0x15bd73d6, /// 0xac4
			 0x887b0a59, /// 0xac8
			 0x6ed7816a, /// 0xacc
			 0xa182fb06, /// 0xad0
			 0x7a08af8d, /// 0xad4
			 0xd31e6e8e, /// 0xad8
			 0x7c624d9f, /// 0xadc
			 0x01ad8433, /// 0xae0
			 0xa36c261e, /// 0xae4
			 0x411ad709, /// 0xae8
			 0x92eccf6e, /// 0xaec
			 0x552af248, /// 0xaf0
			 0x88a863a4, /// 0xaf4
			 0xfe76b188, /// 0xaf8
			 0x87fd796d, /// 0xafc
			 0xe0eaa668, /// 0xb00
			 0x377edafe, /// 0xb04
			 0xcc196cd3, /// 0xb08
			 0x1be67eb5, /// 0xb0c
			 0x1dfa40da, /// 0xb10
			 0x3b35fa79, /// 0xb14
			 0x2af806c3, /// 0xb18
			 0xb70bdc0a, /// 0xb1c
			 0x4a80749c, /// 0xb20
			 0xd0eb79c2, /// 0xb24
			 0x7323f3d4, /// 0xb28
			 0xfa20be23, /// 0xb2c
			 0xf0fc0761, /// 0xb30
			 0x7529725e, /// 0xb34
			 0x16af407b, /// 0xb38
			 0x1e136a1e, /// 0xb3c
			 0x180fcfe3, /// 0xb40
			 0x0b9f7e23, /// 0xb44
			 0xbe97dbb7, /// 0xb48
			 0x9578a16a, /// 0xb4c
			 0xc70e7bcb, /// 0xb50
			 0xb739db3e, /// 0xb54
			 0xd6bafa54, /// 0xb58
			 0x7c402810, /// 0xb5c
			 0x51f45a2a, /// 0xb60
			 0xc136d5c1, /// 0xb64
			 0x9d169ba2, /// 0xb68
			 0xfbe07516, /// 0xb6c
			 0x109d37c8, /// 0xb70
			 0x5e263db1, /// 0xb74
			 0x87a882e5, /// 0xb78
			 0x45b7bd0e, /// 0xb7c
			 0xcc47a0c3, /// 0xb80
			 0x1590d190, /// 0xb84
			 0xd4ec303e, /// 0xb88
			 0xc9770197, /// 0xb8c
			 0xcb052443, /// 0xb90
			 0x5ed08505, /// 0xb94
			 0xd1bf498a, /// 0xb98
			 0xdd4e036b, /// 0xb9c
			 0x72615929, /// 0xba0
			 0x7a7726c2, /// 0xba4
			 0x12c914c5, /// 0xba8
			 0x20a4d633, /// 0xbac
			 0x7bf0c0bf, /// 0xbb0
			 0x0e29d2d1, /// 0xbb4
			 0xa9e5f217, /// 0xbb8
			 0x252c6422, /// 0xbbc
			 0x3b259368, /// 0xbc0
			 0xf0192c4e, /// 0xbc4
			 0x09467ef6, /// 0xbc8
			 0xe723fb57, /// 0xbcc
			 0x40d090e7, /// 0xbd0
			 0x87e2ac15, /// 0xbd4
			 0xdf27bae7, /// 0xbd8
			 0xaacd4184, /// 0xbdc
			 0x6ab85a77, /// 0xbe0
			 0xb4f86051, /// 0xbe4
			 0x7cde88c5, /// 0xbe8
			 0xe660ab0e, /// 0xbec
			 0xc1afa2e4, /// 0xbf0
			 0xb4fa7302, /// 0xbf4
			 0xc33354a6, /// 0xbf8
			 0xe03d3622, /// 0xbfc
			 0x94f1e14c, /// 0xc00
			 0x6e60f69a, /// 0xc04
			 0xd348de1a, /// 0xc08
			 0x457d255e, /// 0xc0c
			 0x6e0bfe00, /// 0xc10
			 0xedeaf371, /// 0xc14
			 0x72b69a27, /// 0xc18
			 0x2608c416, /// 0xc1c
			 0xb4f3168e, /// 0xc20
			 0xc4476c1d, /// 0xc24
			 0xe08b98c8, /// 0xc28
			 0x941c9355, /// 0xc2c
			 0xef94317d, /// 0xc30
			 0x4d3b96ee, /// 0xc34
			 0x6aca44ae, /// 0xc38
			 0x2cf84be1, /// 0xc3c
			 0xd4040e14, /// 0xc40
			 0xabcb4bd0, /// 0xc44
			 0x0906b2c1, /// 0xc48
			 0xccda4210, /// 0xc4c
			 0xf10b9e0c, /// 0xc50
			 0x0393fae1, /// 0xc54
			 0x75a2443d, /// 0xc58
			 0x0330268a, /// 0xc5c
			 0xb95157a6, /// 0xc60
			 0xee0e9658, /// 0xc64
			 0x29b5a24b, /// 0xc68
			 0x45a18561, /// 0xc6c
			 0x3f7da680, /// 0xc70
			 0xfcdb12cb, /// 0xc74
			 0x15f0943c, /// 0xc78
			 0xc9f4e44d, /// 0xc7c
			 0x2aaeb89f, /// 0xc80
			 0xcde19b69, /// 0xc84
			 0xdc00e353, /// 0xc88
			 0x9236be70, /// 0xc8c
			 0x5815dcda, /// 0xc90
			 0x20a02e73, /// 0xc94
			 0x6aa114b1, /// 0xc98
			 0xb5681fcc, /// 0xc9c
			 0xde5dc294, /// 0xca0
			 0x82571f9d, /// 0xca4
			 0xf5a9794d, /// 0xca8
			 0x175458c1, /// 0xcac
			 0x64fe67d2, /// 0xcb0
			 0x7ae3ea3f, /// 0xcb4
			 0xfd70212b, /// 0xcb8
			 0x5378aad1, /// 0xcbc
			 0x019c4044, /// 0xcc0
			 0xe22796de, /// 0xcc4
			 0xb60480f9, /// 0xcc8
			 0xffc5a3c0, /// 0xccc
			 0x9cfd1589, /// 0xcd0
			 0x6ecbe983, /// 0xcd4
			 0x9fc3b369, /// 0xcd8
			 0x70e15a6d, /// 0xcdc
			 0x7a0287e5, /// 0xce0
			 0x49961004, /// 0xce4
			 0x956e16e7, /// 0xce8
			 0x4daf3767, /// 0xcec
			 0xd3c53560, /// 0xcf0
			 0xc28ea9e1, /// 0xcf4
			 0x9467c8b7, /// 0xcf8
			 0x96b12a08, /// 0xcfc
			 0x47fc48a2, /// 0xd00
			 0x0371dd17, /// 0xd04
			 0xd3e76517, /// 0xd08
			 0xfe0947ca, /// 0xd0c
			 0xdf98c7b6, /// 0xd10
			 0xec076496, /// 0xd14
			 0x960a6346, /// 0xd18
			 0x662baf33, /// 0xd1c
			 0x448cbd21, /// 0xd20
			 0x7bee3789, /// 0xd24
			 0xcb5d0991, /// 0xd28
			 0x1e6038b9, /// 0xd2c
			 0x99cee847, /// 0xd30
			 0x387b7c7b, /// 0xd34
			 0x78aab1fd, /// 0xd38
			 0x0dae82c4, /// 0xd3c
			 0x30f6f841, /// 0xd40
			 0x39551280, /// 0xd44
			 0x0a164f9c, /// 0xd48
			 0x9c12a861, /// 0xd4c
			 0x50839387, /// 0xd50
			 0x15b47090, /// 0xd54
			 0x45fbbd2a, /// 0xd58
			 0x50e876fa, /// 0xd5c
			 0x53ca91d7, /// 0xd60
			 0x072f1edd, /// 0xd64
			 0xd6817356, /// 0xd68
			 0x6cb6b454, /// 0xd6c
			 0xdd2f0ba5, /// 0xd70
			 0xa5fc8f8e, /// 0xd74
			 0xf7acd2cb, /// 0xd78
			 0x4036f15b, /// 0xd7c
			 0x12cfd1dc, /// 0xd80
			 0xf4817820, /// 0xd84
			 0x76651ffb, /// 0xd88
			 0xc53deb97, /// 0xd8c
			 0x3879f509, /// 0xd90
			 0xee54db9b, /// 0xd94
			 0x003bac87, /// 0xd98
			 0xd97afdc8, /// 0xd9c
			 0x7bf64fd5, /// 0xda0
			 0x1a8076ed, /// 0xda4
			 0x39c6ab41, /// 0xda8
			 0x63baa3de, /// 0xdac
			 0xd6d0d631, /// 0xdb0
			 0x345fa71f, /// 0xdb4
			 0x6ef8ad16, /// 0xdb8
			 0xcd15928c, /// 0xdbc
			 0x800093d1, /// 0xdc0
			 0x2d1febfa, /// 0xdc4
			 0x57f82e6d, /// 0xdc8
			 0xfaf2e3dc, /// 0xdcc
			 0xdf44ea2d, /// 0xdd0
			 0x9baadf13, /// 0xdd4
			 0x552296fd, /// 0xdd8
			 0xedf0484f, /// 0xddc
			 0x7576c3f6, /// 0xde0
			 0xc0d4188c, /// 0xde4
			 0x553d23f5, /// 0xde8
			 0xc79d5d2e, /// 0xdec
			 0xde460b83, /// 0xdf0
			 0x9ac319c5, /// 0xdf4
			 0x5493a0ea, /// 0xdf8
			 0xdd1aaf7c, /// 0xdfc
			 0x425bcaec, /// 0xe00
			 0x98ea8d47, /// 0xe04
			 0x01568032, /// 0xe08
			 0x9d8a835c, /// 0xe0c
			 0xbbdc584e, /// 0xe10
			 0x0ae44433, /// 0xe14
			 0x8d807fec, /// 0xe18
			 0x069558ff, /// 0xe1c
			 0xc1e9d665, /// 0xe20
			 0xb0c4027d, /// 0xe24
			 0xf16e2b88, /// 0xe28
			 0x7b0d8be4, /// 0xe2c
			 0x86c00ae0, /// 0xe30
			 0x58db2bd4, /// 0xe34
			 0x32ff812d, /// 0xe38
			 0x7ad6b30f, /// 0xe3c
			 0x0f57a87b, /// 0xe40
			 0x781fa4b1, /// 0xe44
			 0xd0795b03, /// 0xe48
			 0xd8aeab68, /// 0xe4c
			 0x3d337edd, /// 0xe50
			 0xfa10eab4, /// 0xe54
			 0xe8d93f6c, /// 0xe58
			 0x67d9e86b, /// 0xe5c
			 0x083ddc8f, /// 0xe60
			 0xc93d8a68, /// 0xe64
			 0xe9b5b47c, /// 0xe68
			 0xd7bbe3a1, /// 0xe6c
			 0xaaf450d2, /// 0xe70
			 0xc90eda6f, /// 0xe74
			 0x95cb849d, /// 0xe78
			 0x3505a683, /// 0xe7c
			 0xc090e22f, /// 0xe80
			 0x4e384072, /// 0xe84
			 0x05aaac27, /// 0xe88
			 0x4f8e6b28, /// 0xe8c
			 0x9f57d0b6, /// 0xe90
			 0xec27edca, /// 0xe94
			 0x4fb14528, /// 0xe98
			 0x7a6267fb, /// 0xe9c
			 0x15e3ac24, /// 0xea0
			 0x8c7e6a3c, /// 0xea4
			 0x31f825ed, /// 0xea8
			 0xdcfb6fa5, /// 0xeac
			 0xba59dc86, /// 0xeb0
			 0xa62f7bcd, /// 0xeb4
			 0x775490f0, /// 0xeb8
			 0xe32b183f, /// 0xebc
			 0x621e3b4e, /// 0xec0
			 0x7670d3fa, /// 0xec4
			 0x77d6e674, /// 0xec8
			 0xc356ff50, /// 0xecc
			 0xd85a5723, /// 0xed0
			 0x1175017d, /// 0xed4
			 0xdb9908d7, /// 0xed8
			 0x65675f7d, /// 0xedc
			 0xde7e5fae, /// 0xee0
			 0x2fe97336, /// 0xee4
			 0x465b888b, /// 0xee8
			 0x0ebd6c7b, /// 0xeec
			 0x81d08997, /// 0xef0
			 0xd3623d61, /// 0xef4
			 0xc25dc958, /// 0xef8
			 0x5a47d8e9, /// 0xefc
			 0x6260fc13, /// 0xf00
			 0x121bbaf9, /// 0xf04
			 0xdaa3aad4, /// 0xf08
			 0xc8533667, /// 0xf0c
			 0x55301185, /// 0xf10
			 0x3b241f67, /// 0xf14
			 0x35e88067, /// 0xf18
			 0x985fc6fb, /// 0xf1c
			 0xedfdd19a, /// 0xf20
			 0x032e59c0, /// 0xf24
			 0x73b67610, /// 0xf28
			 0x92eb9a0a, /// 0xf2c
			 0x55e49b1f, /// 0xf30
			 0x7e4b9537, /// 0xf34
			 0x18d7524f, /// 0xf38
			 0xa860adfe, /// 0xf3c
			 0x475fc386, /// 0xf40
			 0xdb8aac6a, /// 0xf44
			 0x922c36fd, /// 0xf48
			 0xe0d04513, /// 0xf4c
			 0x0338e0f3, /// 0xf50
			 0x83213fe5, /// 0xf54
			 0x4df40d8d, /// 0xf58
			 0x10935c5c, /// 0xf5c
			 0x18195165, /// 0xf60
			 0x592db66f, /// 0xf64
			 0x885ba328, /// 0xf68
			 0x0937b89b, /// 0xf6c
			 0xf45cb07e, /// 0xf70
			 0x3b4dbc33, /// 0xf74
			 0x41fafca9, /// 0xf78
			 0x2293a826, /// 0xf7c
			 0x0274781d, /// 0xf80
			 0x3aae6171, /// 0xf84
			 0x54d28f2c, /// 0xf88
			 0x66905913, /// 0xf8c
			 0xe6801e40, /// 0xf90
			 0x69be9a7f, /// 0xf94
			 0x79ed362a, /// 0xf98
			 0x817ac455, /// 0xf9c
			 0x55af1ab3, /// 0xfa0
			 0x1957b9ea, /// 0xfa4
			 0xebe14cfa, /// 0xfa8
			 0x02ee5cef, /// 0xfac
			 0x00b134e3, /// 0xfb0
			 0xfbbc1662, /// 0xfb4
			 0x2e8aa06a, /// 0xfb8
			 0x4ffd0f34, /// 0xfbc
			 0x87364654, /// 0xfc0
			 0xd7e0dfac, /// 0xfc4
			 0x2c25dcc1, /// 0xfc8
			 0x077fe7f4, /// 0xfcc
			 0x5064b8f1, /// 0xfd0
			 0x76a0c6c2, /// 0xfd4
			 0xfcca744f, /// 0xfd8
			 0x4582b0a0, /// 0xfdc
			 0x73f9f706, /// 0xfe0
			 0x2a1f9ba7, /// 0xfe4
			 0xb5b9764f, /// 0xfe8
			 0xf99d989b, /// 0xfec
			 0x78db0eb3, /// 0xff0
			 0xc089dd22, /// 0xff4
			 0xbbac2f7c, /// 0xff8
			 0x718520a5 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x6640fb05, /// 0x0
			 0xf05bcf98, /// 0x4
			 0xa367f4a4, /// 0x8
			 0x0bdee03f, /// 0xc
			 0x89da666f, /// 0x10
			 0x6ee21ccf, /// 0x14
			 0xa80f8fab, /// 0x18
			 0x18404f3f, /// 0x1c
			 0x19506407, /// 0x20
			 0x04cf9d12, /// 0x24
			 0x3e91511e, /// 0x28
			 0x27d4da10, /// 0x2c
			 0x9dcf20d6, /// 0x30
			 0x8d74c4d3, /// 0x34
			 0x411d8610, /// 0x38
			 0xd2aed714, /// 0x3c
			 0x4fe0d959, /// 0x40
			 0xef2b4fd3, /// 0x44
			 0x83806c5c, /// 0x48
			 0x6b089ac2, /// 0x4c
			 0x29d32b20, /// 0x50
			 0x78e8294a, /// 0x54
			 0xcbb94197, /// 0x58
			 0x4ba41bfb, /// 0x5c
			 0x3f19332c, /// 0x60
			 0xf47e9be2, /// 0x64
			 0xdb3b4477, /// 0x68
			 0xfd35a9c0, /// 0x6c
			 0x865221b5, /// 0x70
			 0x2bf075bb, /// 0x74
			 0x736d6cbe, /// 0x78
			 0xdb74c017, /// 0x7c
			 0xe0df2bbf, /// 0x80
			 0x3591e0d7, /// 0x84
			 0x6baa7c62, /// 0x88
			 0xb5370962, /// 0x8c
			 0xb60dbc52, /// 0x90
			 0x827ba74d, /// 0x94
			 0x09e65ec7, /// 0x98
			 0x2c52caf6, /// 0x9c
			 0xf26146f8, /// 0xa0
			 0x9634ebd1, /// 0xa4
			 0x2912e9d0, /// 0xa8
			 0xdabe1b78, /// 0xac
			 0xebd1d9b4, /// 0xb0
			 0x9f07f440, /// 0xb4
			 0x955c5813, /// 0xb8
			 0x1c4784ee, /// 0xbc
			 0x4e43fbbe, /// 0xc0
			 0xf2ec6190, /// 0xc4
			 0x025f89e9, /// 0xc8
			 0xc862c6f8, /// 0xcc
			 0xc4322c9c, /// 0xd0
			 0x9835bb93, /// 0xd4
			 0x985fa016, /// 0xd8
			 0x305d485b, /// 0xdc
			 0xac7ea83b, /// 0xe0
			 0x756b3bd6, /// 0xe4
			 0xd4d2c447, /// 0xe8
			 0x237ea0e2, /// 0xec
			 0xc77a5823, /// 0xf0
			 0x71e1df1c, /// 0xf4
			 0x80eccf3c, /// 0xf8
			 0xe50225cc, /// 0xfc
			 0xd2d2d601, /// 0x100
			 0x281cee8c, /// 0x104
			 0xb83b07b7, /// 0x108
			 0xe0c89876, /// 0x10c
			 0x2cc4d8fa, /// 0x110
			 0xe512497a, /// 0x114
			 0x3d798fa7, /// 0x118
			 0x3b47ddbd, /// 0x11c
			 0x0532a583, /// 0x120
			 0x04c6e843, /// 0x124
			 0x63b8c719, /// 0x128
			 0xaebc24bf, /// 0x12c
			 0xd44b4d23, /// 0x130
			 0x99dabd4f, /// 0x134
			 0x776b8668, /// 0x138
			 0x1dfee060, /// 0x13c
			 0x4aae32fe, /// 0x140
			 0xe51d70ec, /// 0x144
			 0x42aaf718, /// 0x148
			 0x1059d852, /// 0x14c
			 0xea40b6b0, /// 0x150
			 0x2b4857c6, /// 0x154
			 0x017d6480, /// 0x158
			 0xaadf9bae, /// 0x15c
			 0x79395e22, /// 0x160
			 0x5ecca78d, /// 0x164
			 0x56b5dcbf, /// 0x168
			 0x52eb133e, /// 0x16c
			 0x594e1d89, /// 0x170
			 0x2134f3e9, /// 0x174
			 0xb608492c, /// 0x178
			 0x92b367ab, /// 0x17c
			 0x11b36874, /// 0x180
			 0xabf5324d, /// 0x184
			 0xa095447a, /// 0x188
			 0x0813d85d, /// 0x18c
			 0xaa4f1f8d, /// 0x190
			 0xba058ccd, /// 0x194
			 0x2db12e92, /// 0x198
			 0x0c2d0ff4, /// 0x19c
			 0xfaabb190, /// 0x1a0
			 0x9529cff0, /// 0x1a4
			 0x1ac09ce0, /// 0x1a8
			 0x6121a97d, /// 0x1ac
			 0x4da268c6, /// 0x1b0
			 0x43c68fbe, /// 0x1b4
			 0x6a6a9c36, /// 0x1b8
			 0x361f82d6, /// 0x1bc
			 0x87393f5f, /// 0x1c0
			 0x66e219d1, /// 0x1c4
			 0x1ace15c4, /// 0x1c8
			 0xf0d2d400, /// 0x1cc
			 0x567e70bf, /// 0x1d0
			 0x345b4237, /// 0x1d4
			 0x42651d1c, /// 0x1d8
			 0xe6e75e05, /// 0x1dc
			 0x6bed17dc, /// 0x1e0
			 0x5e527131, /// 0x1e4
			 0xf5318d1f, /// 0x1e8
			 0xc482fc99, /// 0x1ec
			 0x238cc764, /// 0x1f0
			 0xcf254b67, /// 0x1f4
			 0x07e76b0c, /// 0x1f8
			 0x5cc7bc4e, /// 0x1fc
			 0xb9105bdc, /// 0x200
			 0x56dc15c7, /// 0x204
			 0x5ac8a43e, /// 0x208
			 0xe8026f78, /// 0x20c
			 0x989eb478, /// 0x210
			 0x0235459f, /// 0x214
			 0xb313dcb1, /// 0x218
			 0x9193b9d4, /// 0x21c
			 0x5217a6b7, /// 0x220
			 0x176e578d, /// 0x224
			 0xd67d6eb2, /// 0x228
			 0x5937db4f, /// 0x22c
			 0x7875567d, /// 0x230
			 0xa17cb0e2, /// 0x234
			 0x4eb0a1dd, /// 0x238
			 0x0e7ca376, /// 0x23c
			 0x0cfa5c62, /// 0x240
			 0xd48141ff, /// 0x244
			 0xdd54c9a7, /// 0x248
			 0xb2fcb43d, /// 0x24c
			 0xdb6b0497, /// 0x250
			 0x320670fa, /// 0x254
			 0x83662d59, /// 0x258
			 0x616f624d, /// 0x25c
			 0x08f47de9, /// 0x260
			 0x845b3bf2, /// 0x264
			 0xcbbb9c12, /// 0x268
			 0x7b0eb95d, /// 0x26c
			 0x5be2020d, /// 0x270
			 0x6f7c1917, /// 0x274
			 0xb6739fbb, /// 0x278
			 0x0495da1f, /// 0x27c
			 0xbb86c425, /// 0x280
			 0x9625ce64, /// 0x284
			 0xbe7213c7, /// 0x288
			 0xafc5b144, /// 0x28c
			 0xa647006f, /// 0x290
			 0x105d31ae, /// 0x294
			 0x82ce8bd4, /// 0x298
			 0x1fa4f61f, /// 0x29c
			 0x7e403431, /// 0x2a0
			 0x266dc69e, /// 0x2a4
			 0x2fdece40, /// 0x2a8
			 0xbbd39a5f, /// 0x2ac
			 0x54ba39fe, /// 0x2b0
			 0x6a159cdb, /// 0x2b4
			 0x89a0cb40, /// 0x2b8
			 0xb63c5c69, /// 0x2bc
			 0x7e70d204, /// 0x2c0
			 0x7416a64e, /// 0x2c4
			 0xd4f4b60b, /// 0x2c8
			 0xf321bb6a, /// 0x2cc
			 0xe382a0d2, /// 0x2d0
			 0x716891cb, /// 0x2d4
			 0x40218600, /// 0x2d8
			 0x739f9567, /// 0x2dc
			 0x63ad4e9b, /// 0x2e0
			 0x0ae5a45e, /// 0x2e4
			 0xedee1c2a, /// 0x2e8
			 0xb101f006, /// 0x2ec
			 0xae929226, /// 0x2f0
			 0x503ed175, /// 0x2f4
			 0x543fbdfe, /// 0x2f8
			 0x0e1fea8f, /// 0x2fc
			 0x09e0a127, /// 0x300
			 0xed306a2e, /// 0x304
			 0xcef0c4e7, /// 0x308
			 0x113311ed, /// 0x30c
			 0x0d9f64ad, /// 0x310
			 0xa15ab3a5, /// 0x314
			 0xe6e27382, /// 0x318
			 0xc773bc7b, /// 0x31c
			 0x8d56f808, /// 0x320
			 0x050f16cd, /// 0x324
			 0x04711ba0, /// 0x328
			 0x77f87879, /// 0x32c
			 0x7d38b374, /// 0x330
			 0xcab51694, /// 0x334
			 0xb65ea811, /// 0x338
			 0x1787f393, /// 0x33c
			 0x15d077e7, /// 0x340
			 0xff5672ec, /// 0x344
			 0x8ff6bc35, /// 0x348
			 0xe513a433, /// 0x34c
			 0x1f6b2566, /// 0x350
			 0x2eaa8754, /// 0x354
			 0xb87c712b, /// 0x358
			 0x68d5efa2, /// 0x35c
			 0x740ab99c, /// 0x360
			 0x645a4c87, /// 0x364
			 0x84818edc, /// 0x368
			 0x5121f41f, /// 0x36c
			 0x3ed55edf, /// 0x370
			 0xa0b52ac3, /// 0x374
			 0xc73ff28e, /// 0x378
			 0x48f17309, /// 0x37c
			 0xf8bb1cfc, /// 0x380
			 0x8e8a68a7, /// 0x384
			 0xa46ed1fb, /// 0x388
			 0xa761c03d, /// 0x38c
			 0xfc055c14, /// 0x390
			 0xb646cd31, /// 0x394
			 0x1be4fa04, /// 0x398
			 0x17195611, /// 0x39c
			 0x6531bcf8, /// 0x3a0
			 0xc73fad51, /// 0x3a4
			 0xc94bd341, /// 0x3a8
			 0x1eed68e2, /// 0x3ac
			 0x5887cad2, /// 0x3b0
			 0x886df48c, /// 0x3b4
			 0xeb1d0fc3, /// 0x3b8
			 0x881c7ba3, /// 0x3bc
			 0x0df347cc, /// 0x3c0
			 0x6ba5918a, /// 0x3c4
			 0x7f39a1f8, /// 0x3c8
			 0x91fb91a5, /// 0x3cc
			 0x73544dc4, /// 0x3d0
			 0xcde18d33, /// 0x3d4
			 0x3464795f, /// 0x3d8
			 0xb4ccee18, /// 0x3dc
			 0xf8f49f56, /// 0x3e0
			 0xb17ffcb0, /// 0x3e4
			 0xac86178d, /// 0x3e8
			 0xbea8e31c, /// 0x3ec
			 0x9183e6f6, /// 0x3f0
			 0xa2687b4e, /// 0x3f4
			 0x951df731, /// 0x3f8
			 0x66ab002d, /// 0x3fc
			 0xa9e04786, /// 0x400
			 0x46183dce, /// 0x404
			 0x91504551, /// 0x408
			 0x1e58c557, /// 0x40c
			 0x7a09e1fb, /// 0x410
			 0x1271dae5, /// 0x414
			 0x956b5069, /// 0x418
			 0xc4705c4a, /// 0x41c
			 0xbd1a9eba, /// 0x420
			 0x91b329c9, /// 0x424
			 0xe47a8bfc, /// 0x428
			 0xac9e492e, /// 0x42c
			 0x6f9e4528, /// 0x430
			 0xb75d23fc, /// 0x434
			 0x667026df, /// 0x438
			 0x40b7b669, /// 0x43c
			 0x999226c3, /// 0x440
			 0x20073580, /// 0x444
			 0xea3e7968, /// 0x448
			 0x08d59dda, /// 0x44c
			 0x0af08d9f, /// 0x450
			 0x906e0f2b, /// 0x454
			 0xe05d0e5a, /// 0x458
			 0xc9a55e67, /// 0x45c
			 0xfcf0882c, /// 0x460
			 0xccb0a070, /// 0x464
			 0x547a9ed9, /// 0x468
			 0x0a7c1361, /// 0x46c
			 0xf067b71a, /// 0x470
			 0x9124cbc3, /// 0x474
			 0x73824ccb, /// 0x478
			 0xfd43bb52, /// 0x47c
			 0x7ba27b83, /// 0x480
			 0x940f00b9, /// 0x484
			 0xb7fd2008, /// 0x488
			 0x37ef1e53, /// 0x48c
			 0x08dabb8a, /// 0x490
			 0x2709bf31, /// 0x494
			 0x8f93151a, /// 0x498
			 0xc58d6304, /// 0x49c
			 0xa0ea8cbf, /// 0x4a0
			 0xc6ed8a95, /// 0x4a4
			 0xfa769870, /// 0x4a8
			 0x508f0262, /// 0x4ac
			 0xb712b7ac, /// 0x4b0
			 0xe7c4532f, /// 0x4b4
			 0xa8bd95b1, /// 0x4b8
			 0x5b966f0e, /// 0x4bc
			 0xdcb7bbac, /// 0x4c0
			 0x6d2a2017, /// 0x4c4
			 0x1fa00c41, /// 0x4c8
			 0xb4d39f56, /// 0x4cc
			 0xe096c53d, /// 0x4d0
			 0x508af265, /// 0x4d4
			 0x39b61843, /// 0x4d8
			 0x3c3a1efa, /// 0x4dc
			 0x3d0e1959, /// 0x4e0
			 0xf59be518, /// 0x4e4
			 0x8d4930a7, /// 0x4e8
			 0xce48de0b, /// 0x4ec
			 0x631d358c, /// 0x4f0
			 0x090b61ab, /// 0x4f4
			 0xf3e07ce4, /// 0x4f8
			 0xe48c1fb2, /// 0x4fc
			 0xf8ba338c, /// 0x500
			 0x0fec7351, /// 0x504
			 0x56243b82, /// 0x508
			 0x41d92c72, /// 0x50c
			 0xbaee2830, /// 0x510
			 0xe1d65c40, /// 0x514
			 0x99db2ab1, /// 0x518
			 0x1ef6e15d, /// 0x51c
			 0x238f047d, /// 0x520
			 0xd239673c, /// 0x524
			 0xe62fbd91, /// 0x528
			 0x9284df88, /// 0x52c
			 0x216dd9f6, /// 0x530
			 0x8b9db018, /// 0x534
			 0x10cf7019, /// 0x538
			 0x7d9717a0, /// 0x53c
			 0xa96bf68e, /// 0x540
			 0x8af762f8, /// 0x544
			 0xaaac6335, /// 0x548
			 0xbc74b030, /// 0x54c
			 0x38567d17, /// 0x550
			 0x58ee612b, /// 0x554
			 0xb041fd02, /// 0x558
			 0x98d130d0, /// 0x55c
			 0x1ee43b31, /// 0x560
			 0x9affa94e, /// 0x564
			 0x711fdc97, /// 0x568
			 0xae9d344b, /// 0x56c
			 0xe27a648d, /// 0x570
			 0x5d805f6c, /// 0x574
			 0x8cc29ea2, /// 0x578
			 0x388f20d8, /// 0x57c
			 0x6039b250, /// 0x580
			 0x42c78fa0, /// 0x584
			 0x28b13bbc, /// 0x588
			 0x7ff4dd14, /// 0x58c
			 0x42174249, /// 0x590
			 0x950a18f5, /// 0x594
			 0x233f64d8, /// 0x598
			 0x25bc6274, /// 0x59c
			 0x92083f1a, /// 0x5a0
			 0xb3002e9f, /// 0x5a4
			 0x13ecd57a, /// 0x5a8
			 0x5ec8251a, /// 0x5ac
			 0xc69c980d, /// 0x5b0
			 0xadfd9bb5, /// 0x5b4
			 0xb843a9d7, /// 0x5b8
			 0x6235ce27, /// 0x5bc
			 0x14af049a, /// 0x5c0
			 0xfe42400c, /// 0x5c4
			 0xed661ad8, /// 0x5c8
			 0x0a578e45, /// 0x5cc
			 0x988d1ca8, /// 0x5d0
			 0x8ec8be9a, /// 0x5d4
			 0xc6055ff5, /// 0x5d8
			 0x61b6359f, /// 0x5dc
			 0x7718cf2c, /// 0x5e0
			 0x696b27f1, /// 0x5e4
			 0xa9162804, /// 0x5e8
			 0x3eac5f9d, /// 0x5ec
			 0xca4fa506, /// 0x5f0
			 0xf8f0f6e5, /// 0x5f4
			 0x869dcefa, /// 0x5f8
			 0x3434df1c, /// 0x5fc
			 0x7f52acec, /// 0x600
			 0x835121a1, /// 0x604
			 0xf3e9cc9d, /// 0x608
			 0x067047f4, /// 0x60c
			 0xce22ccbb, /// 0x610
			 0x11c22c2f, /// 0x614
			 0x33f00ccf, /// 0x618
			 0xe073d559, /// 0x61c
			 0xc1944d39, /// 0x620
			 0xa1b5188c, /// 0x624
			 0x448dc36a, /// 0x628
			 0xed474854, /// 0x62c
			 0x163d8971, /// 0x630
			 0x19d8b5ba, /// 0x634
			 0x930d6dea, /// 0x638
			 0x08b85816, /// 0x63c
			 0x01b70ad5, /// 0x640
			 0xda2504f1, /// 0x644
			 0x23da4e68, /// 0x648
			 0x9ae4e005, /// 0x64c
			 0xe26a67a5, /// 0x650
			 0x70b59a67, /// 0x654
			 0x76eb4902, /// 0x658
			 0xb26faef6, /// 0x65c
			 0x29681a2e, /// 0x660
			 0xb3926b25, /// 0x664
			 0x804a3264, /// 0x668
			 0x75885025, /// 0x66c
			 0x5761d0ed, /// 0x670
			 0x7b1d8e19, /// 0x674
			 0x4da6fe22, /// 0x678
			 0x543a5b23, /// 0x67c
			 0x76ef48d4, /// 0x680
			 0x0f544693, /// 0x684
			 0xb81eeee3, /// 0x688
			 0x545f3950, /// 0x68c
			 0x8b370da9, /// 0x690
			 0xe7ccd9f3, /// 0x694
			 0xbf8c6a5e, /// 0x698
			 0x22dfc0e6, /// 0x69c
			 0x4949f666, /// 0x6a0
			 0xb147e869, /// 0x6a4
			 0xf8797f84, /// 0x6a8
			 0x201f446e, /// 0x6ac
			 0x7636193a, /// 0x6b0
			 0xbf647439, /// 0x6b4
			 0xca0a74a4, /// 0x6b8
			 0x18f6300d, /// 0x6bc
			 0xb3f44e25, /// 0x6c0
			 0xa5812eaf, /// 0x6c4
			 0x0087ccb4, /// 0x6c8
			 0xa03b264e, /// 0x6cc
			 0x6de235a7, /// 0x6d0
			 0x295eb46d, /// 0x6d4
			 0x7500ddc5, /// 0x6d8
			 0x91309500, /// 0x6dc
			 0x95311b1b, /// 0x6e0
			 0xcd7cafca, /// 0x6e4
			 0xa1dbc251, /// 0x6e8
			 0x76afdc5c, /// 0x6ec
			 0xb544214b, /// 0x6f0
			 0x6e8d18e8, /// 0x6f4
			 0x034a1505, /// 0x6f8
			 0x303779c2, /// 0x6fc
			 0xa28c3426, /// 0x700
			 0x80c86951, /// 0x704
			 0x478aa673, /// 0x708
			 0xb2eb4f42, /// 0x70c
			 0xf940410d, /// 0x710
			 0x6430f6bb, /// 0x714
			 0x1ddcee63, /// 0x718
			 0x46ff598a, /// 0x71c
			 0xb48d618a, /// 0x720
			 0xd91e42f2, /// 0x724
			 0x642f72b4, /// 0x728
			 0x608b56be, /// 0x72c
			 0xaf55565a, /// 0x730
			 0xddea64d9, /// 0x734
			 0x8fc3ec3f, /// 0x738
			 0x67276af3, /// 0x73c
			 0x477197c5, /// 0x740
			 0x0c4b8b25, /// 0x744
			 0x6c89c334, /// 0x748
			 0xf3aad440, /// 0x74c
			 0xad7b2ff5, /// 0x750
			 0xaf4bc0fb, /// 0x754
			 0x99e876d2, /// 0x758
			 0x91c0aec1, /// 0x75c
			 0x96679508, /// 0x760
			 0xbc691a6a, /// 0x764
			 0x3e31dde5, /// 0x768
			 0x777ee0ad, /// 0x76c
			 0x74b0cd44, /// 0x770
			 0xe84b0abd, /// 0x774
			 0x6d7d79b6, /// 0x778
			 0xcae2e54f, /// 0x77c
			 0x9f5337b8, /// 0x780
			 0x0ecbe781, /// 0x784
			 0x87a7103a, /// 0x788
			 0x29009b1f, /// 0x78c
			 0x852aa2de, /// 0x790
			 0x3f48128b, /// 0x794
			 0xdfe3afbf, /// 0x798
			 0xf3a5fc65, /// 0x79c
			 0xb159096d, /// 0x7a0
			 0x164d98f7, /// 0x7a4
			 0xdf8f2d08, /// 0x7a8
			 0x4804e9f7, /// 0x7ac
			 0xfd3cdede, /// 0x7b0
			 0x75cd0bcb, /// 0x7b4
			 0x565a9653, /// 0x7b8
			 0xba9c7096, /// 0x7bc
			 0x519aa792, /// 0x7c0
			 0x1c149bdb, /// 0x7c4
			 0xb45518e9, /// 0x7c8
			 0x9fa2a991, /// 0x7cc
			 0x9003cdb5, /// 0x7d0
			 0x91226ad1, /// 0x7d4
			 0x4687d744, /// 0x7d8
			 0xfa777982, /// 0x7dc
			 0xb11ee6ee, /// 0x7e0
			 0x39ab5619, /// 0x7e4
			 0x5bbc826a, /// 0x7e8
			 0xb4fea215, /// 0x7ec
			 0x52a6c59e, /// 0x7f0
			 0xf79505b6, /// 0x7f4
			 0x6959ebba, /// 0x7f8
			 0x0f7e101d, /// 0x7fc
			 0xdc3cf1b4, /// 0x800
			 0x4a08d614, /// 0x804
			 0x9e42910f, /// 0x808
			 0xba66fa62, /// 0x80c
			 0xf032d17e, /// 0x810
			 0x38962059, /// 0x814
			 0x898a2ce4, /// 0x818
			 0x54b58511, /// 0x81c
			 0x566b951c, /// 0x820
			 0x383bc22b, /// 0x824
			 0xbd6e7db9, /// 0x828
			 0x828d2987, /// 0x82c
			 0x7f8135f1, /// 0x830
			 0xa5ea48bc, /// 0x834
			 0x64a048bd, /// 0x838
			 0xa9a4d2ee, /// 0x83c
			 0x5d003818, /// 0x840
			 0x9e3cedef, /// 0x844
			 0x5f566e83, /// 0x848
			 0x2807b56c, /// 0x84c
			 0xb8d2be7e, /// 0x850
			 0xfddf0845, /// 0x854
			 0x7a8e037e, /// 0x858
			 0x3fe2706d, /// 0x85c
			 0x69ab285f, /// 0x860
			 0x69fd0881, /// 0x864
			 0x94d94562, /// 0x868
			 0xd6afcd25, /// 0x86c
			 0xeafe8f25, /// 0x870
			 0x14b77305, /// 0x874
			 0x257641ca, /// 0x878
			 0x73f2501e, /// 0x87c
			 0xcc2085fb, /// 0x880
			 0x714c1ad0, /// 0x884
			 0x82c4c737, /// 0x888
			 0xef03a136, /// 0x88c
			 0x6b550a88, /// 0x890
			 0xee5df3e2, /// 0x894
			 0x67626667, /// 0x898
			 0xcfb8dc7e, /// 0x89c
			 0x12011778, /// 0x8a0
			 0xf3d3ff27, /// 0x8a4
			 0xefb3368f, /// 0x8a8
			 0x1023aff0, /// 0x8ac
			 0x9c735b48, /// 0x8b0
			 0x6fd1637f, /// 0x8b4
			 0xc6abfb90, /// 0x8b8
			 0xafdeb29b, /// 0x8bc
			 0x03adc91f, /// 0x8c0
			 0xd4cc4920, /// 0x8c4
			 0x5e8a3058, /// 0x8c8
			 0x4b5128f7, /// 0x8cc
			 0xad437884, /// 0x8d0
			 0xd07ae4e5, /// 0x8d4
			 0x25a6250d, /// 0x8d8
			 0xccadd7a6, /// 0x8dc
			 0xaa7aa9a1, /// 0x8e0
			 0x2a5229d2, /// 0x8e4
			 0x1f9c39b1, /// 0x8e8
			 0xc29d36a7, /// 0x8ec
			 0xe36f4dc0, /// 0x8f0
			 0x137a6d6f, /// 0x8f4
			 0x078d7ea1, /// 0x8f8
			 0x1d277dc0, /// 0x8fc
			 0xaebc60ae, /// 0x900
			 0xcd506d7a, /// 0x904
			 0x6a267e22, /// 0x908
			 0x9805bdd6, /// 0x90c
			 0xe82bd5cb, /// 0x910
			 0x777bd95f, /// 0x914
			 0x2ff61e36, /// 0x918
			 0x89bdb543, /// 0x91c
			 0x7b639081, /// 0x920
			 0xcc2ed3aa, /// 0x924
			 0xc008e173, /// 0x928
			 0xd856a1fe, /// 0x92c
			 0xbc0f291a, /// 0x930
			 0x6e01fd34, /// 0x934
			 0xbe02fe6f, /// 0x938
			 0xc355fe15, /// 0x93c
			 0xcb83a53f, /// 0x940
			 0x7101b972, /// 0x944
			 0x1ad32d7b, /// 0x948
			 0xb8f15b72, /// 0x94c
			 0xb4089d1f, /// 0x950
			 0x0a5b1a9a, /// 0x954
			 0xd2429efc, /// 0x958
			 0x1b5b9fa6, /// 0x95c
			 0x754dfdd8, /// 0x960
			 0x891c0f71, /// 0x964
			 0x7992368c, /// 0x968
			 0x6149d2b6, /// 0x96c
			 0x8fa6c177, /// 0x970
			 0x167c3626, /// 0x974
			 0xe1edf476, /// 0x978
			 0x670f238c, /// 0x97c
			 0x26a359a7, /// 0x980
			 0x50d72f39, /// 0x984
			 0x873ad43d, /// 0x988
			 0xb99ab39e, /// 0x98c
			 0x3c41924e, /// 0x990
			 0x9224fd6b, /// 0x994
			 0x730d682f, /// 0x998
			 0x654bbdb4, /// 0x99c
			 0x65b1c902, /// 0x9a0
			 0xe88b483c, /// 0x9a4
			 0xfed60f72, /// 0x9a8
			 0x630a519c, /// 0x9ac
			 0x209764c3, /// 0x9b0
			 0x44124bd0, /// 0x9b4
			 0x522b5521, /// 0x9b8
			 0x0bf82da3, /// 0x9bc
			 0x0933c0ef, /// 0x9c0
			 0xf7c02e58, /// 0x9c4
			 0x0808c1d3, /// 0x9c8
			 0x82e0a0fb, /// 0x9cc
			 0x9f929896, /// 0x9d0
			 0x2844ab30, /// 0x9d4
			 0x2eb76362, /// 0x9d8
			 0x44227ffe, /// 0x9dc
			 0xa8c97b30, /// 0x9e0
			 0x0f8d9dd5, /// 0x9e4
			 0x600018f3, /// 0x9e8
			 0x361417e0, /// 0x9ec
			 0x91645126, /// 0x9f0
			 0x8c63b17c, /// 0x9f4
			 0x1629a5a2, /// 0x9f8
			 0x6a00886f, /// 0x9fc
			 0xa1466255, /// 0xa00
			 0x35b18e32, /// 0xa04
			 0xc7a41bea, /// 0xa08
			 0x66bcacbd, /// 0xa0c
			 0x39a0c6bb, /// 0xa10
			 0x1864235b, /// 0xa14
			 0x4c4e3743, /// 0xa18
			 0x3cfaf09b, /// 0xa1c
			 0x6883c420, /// 0xa20
			 0xae07826d, /// 0xa24
			 0x63a52efe, /// 0xa28
			 0x74eb7467, /// 0xa2c
			 0xca1c2088, /// 0xa30
			 0x17376b39, /// 0xa34
			 0x7f18c8e4, /// 0xa38
			 0x214a778d, /// 0xa3c
			 0xd24178d3, /// 0xa40
			 0x0ff03c0e, /// 0xa44
			 0x954134ec, /// 0xa48
			 0xb706e722, /// 0xa4c
			 0xd1869acc, /// 0xa50
			 0x5decc66f, /// 0xa54
			 0x09c18c58, /// 0xa58
			 0x070e4799, /// 0xa5c
			 0xbffcfbd4, /// 0xa60
			 0xbc2ee32e, /// 0xa64
			 0x8e4b6f25, /// 0xa68
			 0xf644cdfd, /// 0xa6c
			 0xdd610bcf, /// 0xa70
			 0xe02ced12, /// 0xa74
			 0x8c48133e, /// 0xa78
			 0xbc65bafb, /// 0xa7c
			 0x79b14f71, /// 0xa80
			 0x1690520b, /// 0xa84
			 0xfba16b91, /// 0xa88
			 0x1b757fdf, /// 0xa8c
			 0x1709b5b9, /// 0xa90
			 0x4dcd0934, /// 0xa94
			 0xf3f26dda, /// 0xa98
			 0xfe1c1712, /// 0xa9c
			 0xd7d121f7, /// 0xaa0
			 0x40e9929e, /// 0xaa4
			 0xe0c4b1ae, /// 0xaa8
			 0x716f2a87, /// 0xaac
			 0xda0f7cab, /// 0xab0
			 0x81637032, /// 0xab4
			 0xbe2e11be, /// 0xab8
			 0x2be3dee1, /// 0xabc
			 0x3c76a39f, /// 0xac0
			 0x3e0955f6, /// 0xac4
			 0xffc737d0, /// 0xac8
			 0x62755f66, /// 0xacc
			 0xb43d2801, /// 0xad0
			 0x5c5acf2d, /// 0xad4
			 0x341c951b, /// 0xad8
			 0x491fc191, /// 0xadc
			 0xb650f2ae, /// 0xae0
			 0xcba96408, /// 0xae4
			 0x13a0981c, /// 0xae8
			 0x20da8b4c, /// 0xaec
			 0xf3645dfa, /// 0xaf0
			 0x43ce50c2, /// 0xaf4
			 0x8bab550f, /// 0xaf8
			 0x0a72d65f, /// 0xafc
			 0xe27a31a0, /// 0xb00
			 0x31ef7c5d, /// 0xb04
			 0x78fc1e5c, /// 0xb08
			 0x37665ac7, /// 0xb0c
			 0xa24df2c4, /// 0xb10
			 0xe0f74c57, /// 0xb14
			 0x1c92fcc7, /// 0xb18
			 0x81b3cbae, /// 0xb1c
			 0x9270f038, /// 0xb20
			 0x403edc43, /// 0xb24
			 0xf5f46ddf, /// 0xb28
			 0x70649e0b, /// 0xb2c
			 0x06d98fbc, /// 0xb30
			 0xfccd37c5, /// 0xb34
			 0xa64243ae, /// 0xb38
			 0xdfa12dcb, /// 0xb3c
			 0xf24b76e9, /// 0xb40
			 0x18d26e98, /// 0xb44
			 0x65aa673c, /// 0xb48
			 0xcdc801d5, /// 0xb4c
			 0x43734d5c, /// 0xb50
			 0x941d164a, /// 0xb54
			 0x5498b79d, /// 0xb58
			 0xb8d0d066, /// 0xb5c
			 0xa1adf927, /// 0xb60
			 0xfa6ed5aa, /// 0xb64
			 0x54d892d8, /// 0xb68
			 0x9b4aae27, /// 0xb6c
			 0x2ada8554, /// 0xb70
			 0xd579bded, /// 0xb74
			 0xc72e9766, /// 0xb78
			 0x424b66f9, /// 0xb7c
			 0x61e4558a, /// 0xb80
			 0xbbaa6c93, /// 0xb84
			 0x886b77ef, /// 0xb88
			 0x2c2aacf4, /// 0xb8c
			 0x7673cb55, /// 0xb90
			 0xdd7e3373, /// 0xb94
			 0x8f3535d0, /// 0xb98
			 0xe557cbf1, /// 0xb9c
			 0x0f2f8626, /// 0xba0
			 0x41cca3c9, /// 0xba4
			 0xabbc2a5f, /// 0xba8
			 0x2e9f1caa, /// 0xbac
			 0xcd4213d8, /// 0xbb0
			 0xd4b352b7, /// 0xbb4
			 0xce45f01a, /// 0xbb8
			 0xfed11ca8, /// 0xbbc
			 0x386c1c13, /// 0xbc0
			 0xe767929b, /// 0xbc4
			 0xd460e2bc, /// 0xbc8
			 0xcd24075b, /// 0xbcc
			 0xf5cbc2d6, /// 0xbd0
			 0x13de8004, /// 0xbd4
			 0x52e14173, /// 0xbd8
			 0xdc3fc4dc, /// 0xbdc
			 0xfd6d8da6, /// 0xbe0
			 0x8aa828b8, /// 0xbe4
			 0x62e28684, /// 0xbe8
			 0xcd3373c1, /// 0xbec
			 0x62f84726, /// 0xbf0
			 0x82a51ee9, /// 0xbf4
			 0xcd390c45, /// 0xbf8
			 0xa43f64e8, /// 0xbfc
			 0x58e13f1a, /// 0xc00
			 0xdcd26570, /// 0xc04
			 0xf6b809b0, /// 0xc08
			 0x099ca229, /// 0xc0c
			 0xfb067641, /// 0xc10
			 0xeb49ae5c, /// 0xc14
			 0x54b01535, /// 0xc18
			 0x937ecf33, /// 0xc1c
			 0x1a25fc21, /// 0xc20
			 0x310de6ca, /// 0xc24
			 0xdac0c716, /// 0xc28
			 0x3f9dba70, /// 0xc2c
			 0x712ff45b, /// 0xc30
			 0x73bbaeea, /// 0xc34
			 0x22e1b6b0, /// 0xc38
			 0x436b4dc0, /// 0xc3c
			 0x14c61f2c, /// 0xc40
			 0x22896b0a, /// 0xc44
			 0x41d30020, /// 0xc48
			 0x1a8b67b5, /// 0xc4c
			 0x41e2f2f1, /// 0xc50
			 0x28dae606, /// 0xc54
			 0x4bf0b915, /// 0xc58
			 0x69d15b56, /// 0xc5c
			 0xbdcd3b08, /// 0xc60
			 0x9a455b5d, /// 0xc64
			 0x919d241e, /// 0xc68
			 0x0c822fee, /// 0xc6c
			 0x60c65066, /// 0xc70
			 0x4794ed5e, /// 0xc74
			 0xe378bc46, /// 0xc78
			 0xea027b94, /// 0xc7c
			 0x3dc9c4bf, /// 0xc80
			 0x48ba5dae, /// 0xc84
			 0x05353a80, /// 0xc88
			 0xccf6cd25, /// 0xc8c
			 0xfa7e2aa1, /// 0xc90
			 0x1211325e, /// 0xc94
			 0x6320a40c, /// 0xc98
			 0xec3dd741, /// 0xc9c
			 0xf7f08d05, /// 0xca0
			 0xe146e1ac, /// 0xca4
			 0x808fd5ab, /// 0xca8
			 0x279475e0, /// 0xcac
			 0xf230da92, /// 0xcb0
			 0x85af37b6, /// 0xcb4
			 0x53d04963, /// 0xcb8
			 0x2f3f55ce, /// 0xcbc
			 0x669f5dda, /// 0xcc0
			 0x3df67af4, /// 0xcc4
			 0xb667c6b2, /// 0xcc8
			 0x577f25ba, /// 0xccc
			 0xcb924f2f, /// 0xcd0
			 0xe10d25fe, /// 0xcd4
			 0x5c112da4, /// 0xcd8
			 0xedf88380, /// 0xcdc
			 0x0e4626ff, /// 0xce0
			 0x17d44fb3, /// 0xce4
			 0x08bb439a, /// 0xce8
			 0xa68656fd, /// 0xcec
			 0xd23ada7c, /// 0xcf0
			 0xb6cd405a, /// 0xcf4
			 0xb9dee7fd, /// 0xcf8
			 0xc58a140d, /// 0xcfc
			 0x8b79b4d8, /// 0xd00
			 0x3152b87a, /// 0xd04
			 0xbd9f86cb, /// 0xd08
			 0xd4e06bc7, /// 0xd0c
			 0xdf458aa3, /// 0xd10
			 0x12d145f0, /// 0xd14
			 0x477f43fa, /// 0xd18
			 0x41d1b638, /// 0xd1c
			 0x2b753c51, /// 0xd20
			 0xab427667, /// 0xd24
			 0x7e129c92, /// 0xd28
			 0x16ab1250, /// 0xd2c
			 0x87c0c1db, /// 0xd30
			 0x8ae975d0, /// 0xd34
			 0xa333c148, /// 0xd38
			 0xcde30634, /// 0xd3c
			 0x6058c93c, /// 0xd40
			 0x23747145, /// 0xd44
			 0x6793bbb3, /// 0xd48
			 0x97293307, /// 0xd4c
			 0x5dec1edf, /// 0xd50
			 0x5d2aa49d, /// 0xd54
			 0xe29d02ff, /// 0xd58
			 0x5eb71c6c, /// 0xd5c
			 0x4d48af55, /// 0xd60
			 0x86f14d9e, /// 0xd64
			 0x885b3759, /// 0xd68
			 0xb20d9097, /// 0xd6c
			 0x1bb0ffb7, /// 0xd70
			 0x273fefcc, /// 0xd74
			 0x94ab1d18, /// 0xd78
			 0x28b92dbb, /// 0xd7c
			 0x0bda00a0, /// 0xd80
			 0x96205574, /// 0xd84
			 0x2b7abfc0, /// 0xd88
			 0x5f7821c3, /// 0xd8c
			 0xf9c3a805, /// 0xd90
			 0xa9bf95fe, /// 0xd94
			 0xc4b3434f, /// 0xd98
			 0x40f9cbab, /// 0xd9c
			 0x502fb061, /// 0xda0
			 0xfe0213b3, /// 0xda4
			 0xfff8d3e2, /// 0xda8
			 0x23c25d7b, /// 0xdac
			 0x7eb8d0e3, /// 0xdb0
			 0xe0218bc4, /// 0xdb4
			 0x368ab9fb, /// 0xdb8
			 0x277ddaa8, /// 0xdbc
			 0x41015183, /// 0xdc0
			 0xd200d76d, /// 0xdc4
			 0xd6ead9da, /// 0xdc8
			 0x848715e9, /// 0xdcc
			 0x85922d97, /// 0xdd0
			 0x29f852f7, /// 0xdd4
			 0x61932386, /// 0xdd8
			 0xc2514e9b, /// 0xddc
			 0xbce395a1, /// 0xde0
			 0xb9d1c214, /// 0xde4
			 0x6d4535d1, /// 0xde8
			 0x63cd947e, /// 0xdec
			 0x74bb1631, /// 0xdf0
			 0x85198bf3, /// 0xdf4
			 0x842aaa3f, /// 0xdf8
			 0x7d6f76fc, /// 0xdfc
			 0x6ce70a63, /// 0xe00
			 0x4d1f83b8, /// 0xe04
			 0xb93110a7, /// 0xe08
			 0x211cac6a, /// 0xe0c
			 0xc65c0f02, /// 0xe10
			 0xed877bf6, /// 0xe14
			 0x822249b1, /// 0xe18
			 0xf64def11, /// 0xe1c
			 0xab4554fa, /// 0xe20
			 0x1b6ee2f7, /// 0xe24
			 0x305ea1ee, /// 0xe28
			 0xb7f0b89c, /// 0xe2c
			 0xb7fe929d, /// 0xe30
			 0x9e65d412, /// 0xe34
			 0xed373e75, /// 0xe38
			 0xecfe39d2, /// 0xe3c
			 0x11b2a449, /// 0xe40
			 0xef9675f2, /// 0xe44
			 0xbacfefb2, /// 0xe48
			 0xe85d5de3, /// 0xe4c
			 0xcca50bf2, /// 0xe50
			 0x55d7f2d2, /// 0xe54
			 0xb5175099, /// 0xe58
			 0x101f0fea, /// 0xe5c
			 0x44666e35, /// 0xe60
			 0xe753de3a, /// 0xe64
			 0xa4d4cfd0, /// 0xe68
			 0x5c8af09e, /// 0xe6c
			 0x2cb54dfe, /// 0xe70
			 0x08b42e77, /// 0xe74
			 0x7858c314, /// 0xe78
			 0x7b870990, /// 0xe7c
			 0xfa98a3eb, /// 0xe80
			 0x8a242ad0, /// 0xe84
			 0xd9be76bf, /// 0xe88
			 0x490e4c7f, /// 0xe8c
			 0xe6e5ca0e, /// 0xe90
			 0x977a87a9, /// 0xe94
			 0xf6d59eef, /// 0xe98
			 0x75d0c93f, /// 0xe9c
			 0xab7a4b0a, /// 0xea0
			 0xf4b7ad49, /// 0xea4
			 0x1a98a0a3, /// 0xea8
			 0xb9478a65, /// 0xeac
			 0xe0b501d3, /// 0xeb0
			 0x8350e2c9, /// 0xeb4
			 0xedfe196b, /// 0xeb8
			 0x233d8017, /// 0xebc
			 0x55b69a9b, /// 0xec0
			 0x04809fb2, /// 0xec4
			 0xfd17684c, /// 0xec8
			 0x89d45b3a, /// 0xecc
			 0x0ae9097a, /// 0xed0
			 0x3ed5f9f9, /// 0xed4
			 0x34e19616, /// 0xed8
			 0xbd9c81b1, /// 0xedc
			 0x8ca1a92c, /// 0xee0
			 0x40c7b809, /// 0xee4
			 0xbee6a70c, /// 0xee8
			 0x3714c825, /// 0xeec
			 0xa6cb66cd, /// 0xef0
			 0xc9b5eade, /// 0xef4
			 0x06b9779c, /// 0xef8
			 0x89b3a199, /// 0xefc
			 0x1fe143b3, /// 0xf00
			 0x0731c935, /// 0xf04
			 0x3f9a1f9f, /// 0xf08
			 0xf0b0a2e4, /// 0xf0c
			 0x29c881ab, /// 0xf10
			 0xd0ea12e4, /// 0xf14
			 0x2385f57f, /// 0xf18
			 0x9cd29e49, /// 0xf1c
			 0xdd364c54, /// 0xf20
			 0xee6a4ea6, /// 0xf24
			 0x429e45de, /// 0xf28
			 0x1044d2cf, /// 0xf2c
			 0x65e76552, /// 0xf30
			 0x542e1bf8, /// 0xf34
			 0xad48c1a1, /// 0xf38
			 0xb8e372cd, /// 0xf3c
			 0xda111f90, /// 0xf40
			 0x0196525a, /// 0xf44
			 0x1c934cd4, /// 0xf48
			 0x35bf7918, /// 0xf4c
			 0xa78cf312, /// 0xf50
			 0xe16d7f81, /// 0xf54
			 0x5d979dad, /// 0xf58
			 0x83f0d347, /// 0xf5c
			 0x5924612b, /// 0xf60
			 0xf3fdc91f, /// 0xf64
			 0x8e87c890, /// 0xf68
			 0x89e2d7e6, /// 0xf6c
			 0x8b8c4e29, /// 0xf70
			 0x413a2245, /// 0xf74
			 0x59617d12, /// 0xf78
			 0x5dc5afc9, /// 0xf7c
			 0xedabf29a, /// 0xf80
			 0xde6cbf2f, /// 0xf84
			 0x383a0b8f, /// 0xf88
			 0xcedc7e3d, /// 0xf8c
			 0xefcad2ff, /// 0xf90
			 0x126b93f7, /// 0xf94
			 0x8711cefc, /// 0xf98
			 0x1ee4358c, /// 0xf9c
			 0xcc87c617, /// 0xfa0
			 0xd2730743, /// 0xfa4
			 0xf0a24622, /// 0xfa8
			 0x418d6147, /// 0xfac
			 0xb1c47ef9, /// 0xfb0
			 0xb2611488, /// 0xfb4
			 0x90a424b4, /// 0xfb8
			 0xba05ca54, /// 0xfbc
			 0xad374d52, /// 0xfc0
			 0x1188d584, /// 0xfc4
			 0x76538376, /// 0xfc8
			 0xa571f8eb, /// 0xfcc
			 0x73b4b771, /// 0xfd0
			 0x86a7d73e, /// 0xfd4
			 0xa17117a0, /// 0xfd8
			 0x416294c8, /// 0xfdc
			 0x9c938018, /// 0xfe0
			 0xe20e3b9f, /// 0xfe4
			 0x58bc21de, /// 0xfe8
			 0xa44ae7f9, /// 0xfec
			 0x391cfde5, /// 0xff0
			 0x4bcb191d, /// 0xff4
			 0x6adb6e1d, /// 0xff8
			 0xf04003ae /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00004
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00010
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00018
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0001c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00024
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00028
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00040
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00044
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00054
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00058
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0005c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00064
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00068
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0006c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00070
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00078
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0007c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00084
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00000000,                                                  // zero                                        /// 0008c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0c600000,                                                  // Leading 1s:                                 /// 00094
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00098
			 0x00000000,                                                  // zero                                        /// 0009c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e8
			 0xff800000,                                                  // -inf                                        /// 000ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00100
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00104
			 0x7f800000,                                                  // inf                                         /// 00108
			 0x0e000001,                                                  // Trailing 1s:                                /// 0010c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00114
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00118
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00120
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00128
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0012c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00134
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00138
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00140
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00144
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00148
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00150
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00158
			 0x80000000,                                                  // -zero                                       /// 0015c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00160
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00164
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00168
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0016c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00170
			 0xffc00001,                                                  // -signaling NaN                              /// 00174
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00178
			 0x0e000001,                                                  // Trailing 1s:                                /// 0017c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00180
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00190
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00194
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00198
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x7f800000,                                                  // inf                                         /// 001ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e4
			 0xffc00001,                                                  // -signaling NaN                              /// 001e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00200
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00204
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0020c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00218
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00220
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00228
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00230
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0023c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00244
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00250
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00258
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0025c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00260
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00264
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0026c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00278
			 0xff800000,                                                  // -inf                                        /// 0027c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00280
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00284
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00294
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00298
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 002b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c8
			 0x4b000000,                                                  // 8388608.0                                   /// 002cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00300
			 0x33333333,                                                  // 4 random values                             /// 00304
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00308
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00314
			 0xbf028f5c,                                                  // -0.51                                       /// 00318
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0031c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0032c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00344
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00358
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0035c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00364
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00368
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0036c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00374
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00378
			 0x0e000003,                                                  // Trailing 1s:                                /// 0037c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00384
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00388
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0038c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00394
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00398
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80000000,                                                  // -zero                                       /// 003fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00400
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00404
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00408
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0040c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00414
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00418
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0041c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x0c400000,                                                  // Leading 1s:                                 /// 00424
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00428
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0042c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00430
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00434
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00438
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0043c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00444
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00448
			 0x0c400000,                                                  // Leading 1s:                                 /// 0044c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00460
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00468
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00478
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0047c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00484
			 0x0e000003,                                                  // Trailing 1s:                                /// 00488
			 0x0e000001,                                                  // Trailing 1s:                                /// 0048c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00498
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0049c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00508
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00510
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00518
			 0x7f800000,                                                  // inf                                         /// 0051c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00520
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00524
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00528
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00530
			 0x80000000,                                                  // -zero                                       /// 00534
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0053c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00540
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00548
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0054c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00550
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00554
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0055c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00560
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00570
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00574
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00578
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0057c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00584
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00590
			 0x00011111,                                                  // 9.7958E-41                                  /// 00594
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00598
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0059c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x33333333,                                                  // 4 random values                             /// 005ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00600
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00604
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80011111,                                                  // -9.7958E-41                                 /// 0060c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00628
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00630
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00638
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00640
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x80000000,                                                  // -zero                                       /// 00648
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0064c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00650
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00654
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00658
			 0x0c700000,                                                  // Leading 1s:                                 /// 0065c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x3f028f5c,                                                  // 0.51                                        /// 00670
			 0x00000000,                                                  // zero                                        /// 00674
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00678
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0067c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00680
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00688
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00690
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00694
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00698
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006ec
			 0xffc00001,                                                  // -signaling NaN                              /// 006f0
			 0x7fc00001,                                                  // signaling NaN                               /// 006f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0070c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00714
			 0xcb000000,                                                  // -8388608.0                                  /// 00718
			 0xffc00001,                                                  // -signaling NaN                              /// 0071c
			 0x4b000000,                                                  // 8388608.0                                   /// 00720
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00724
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00740
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00744
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00748
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0074c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00758
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0075c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00764
			 0x33333333,                                                  // 4 random values                             /// 00768
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00774
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x4b000000,                                                  // 8388608.0                                   /// 0077c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00784
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00788
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007ac
			 0xffc00001,                                                  // -signaling NaN                              /// 007b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e4
			 0xcb000000,                                                  // -8388608.0                                  /// 007e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x55555555,                                                  // 4 random values                             /// 007f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00800
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00808
			 0x80000000,                                                  // -zero                                       /// 0080c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00810
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00814
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00818
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0xffc00001,                                                  // -signaling NaN                              /// 00820
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0082c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00838
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0083c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00840
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00848
			 0x33333333,                                                  // 4 random values                             /// 0084c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00850
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00854
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00860
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00864
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00874
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0087c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00884
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0088c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00890
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00894
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0089c
			 0x3f028f5c,                                                  // 0.51                                        /// 008a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00908
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00910
			 0xbf028f5c,                                                  // -0.51                                       /// 00914
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00918
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00920
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00924
			 0x0c700000,                                                  // Leading 1s:                                 /// 00928
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0092c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00930
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0093c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00940
			 0xffc00001,                                                  // -signaling NaN                              /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x80011111,                                                  // -9.7958E-41                                 /// 0094c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00950
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00958
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0095c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00960
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e000001,                                                  // Trailing 1s:                                /// 00968
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0096c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00978
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00980
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00984
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00988
			 0x0c700000,                                                  // Leading 1s:                                 /// 0098c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00990
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00994
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00998
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0099c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e4
			 0xff800000,                                                  // -inf                                        /// 009e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a88
			 0xbf028f5c,                                                  // -0.51                                       /// 00a8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00abc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00acc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad8
			 0x55555555,                                                  // 4 random values                             /// 00adc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ae4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00af0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00af8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00afc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b58
			 0x80000000,                                                  // -zero                                       /// 00b5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x3f028f5c,                                                  // 0.51                                        /// 00b68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b74
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ba0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bdc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00be0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be4
			 0xbf028f5c,                                                  // -0.51                                       /// 00be8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bf0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c2c
			 0x7f800000,                                                  // inf                                         /// 00c30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x33333333,                                                  // 4 random values                             /// 00c68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c7c
			 0x7f800000,                                                  // inf                                         /// 00c80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cb4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ccc
			 0x33333333,                                                  // 4 random values                             /// 00cd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x7f800000,                                                  // inf                                         /// 00d20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d24
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dcc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00df8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dfc
			 0x4b000000,                                                  // 8388608.0                                   /// 00e00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e4c
			 0xff800000,                                                  // -inf                                        /// 00e50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e54
			 0xff800000,                                                  // -inf                                        /// 00e58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ebc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ec4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ecc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ed0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ee4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ef4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00efc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xbf028f5c,                                                  // -0.51                                       /// 00f78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f98
			 0x4b000000,                                                  // 8388608.0                                   /// 00f9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fcc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fe4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ff8
			 0xff000000 // norm with max exp min mant                   // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00004
			 0x3f028f5c,                                                  // 0.51                                        /// 00008
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00014
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0001c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00020
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00024
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0002c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00034
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0003c
			 0x4b000000,                                                  // 8388608.0                                   /// 00040
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00044
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00048
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00050
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00058
			 0x0e000003,                                                  // Trailing 1s:                                /// 0005c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00060
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00064
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0006c
			 0xcb000000,                                                  // -8388608.0                                  /// 00070
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00074
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00078
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0007c
			 0xff800000,                                                  // -inf                                        /// 00080
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00084
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00090
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00094
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00098
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0009c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000ac
			 0x7fc00001,                                                  // signaling NaN                               /// 000b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000cc
			 0x33333333,                                                  // 4 random values                             /// 000d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00100
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00108
			 0x7f800000,                                                  // inf                                         /// 0010c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00110
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00114
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00120
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00130
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00134
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00138
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0013c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00140
			 0xff800000,                                                  // -inf                                        /// 00144
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00148
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0014c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00154
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00158
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0015c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00160
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00164
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00174
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00178
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0017c
			 0x3f028f5c,                                                  // 0.51                                        /// 00180
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00184
			 0x80000000,                                                  // -zero                                       /// 00188
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00194
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00198
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0019c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0xcb000000,                                                  // -8388608.0                                  /// 001a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001dc
			 0x55555555,                                                  // 4 random values                             /// 001e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00200
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00204
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00208
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00214
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00218
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00220
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00224
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00228
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00230
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00234
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00238
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0023c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00240
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00244
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00248
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000000,                                                  // -zero                                       /// 00254
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00258
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0xcb000000,                                                  // -8388608.0                                  /// 00264
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00268
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0026c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00270
			 0x55555555,                                                  // 4 random values                             /// 00274
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00278
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0027c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00280
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00284
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00294
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00298
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002c0
			 0x3f028f5c,                                                  // 0.51                                        /// 002c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00300
			 0xff800000,                                                  // -inf                                        /// 00304
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00308
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xbf028f5c,                                                  // -0.51                                       /// 00310
			 0x00011111,                                                  // 9.7958E-41                                  /// 00314
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00318
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0031c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80011111,                                                  // -9.7958E-41                                 /// 0032c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e000001,                                                  // Trailing 1s:                                /// 00334
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00338
			 0xcb000000,                                                  // -8388608.0                                  /// 0033c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0c780000,                                                  // Leading 1s:                                 /// 00344
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00348
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00350
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00354
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00358
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0035c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00364
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00368
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00370
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00374
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00378
			 0x3f028f5c,                                                  // 0.51                                        /// 0037c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00380
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0038c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00390
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00398
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0039c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003e0
			 0xcb000000,                                                  // -8388608.0                                  /// 003e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f0
			 0xbf028f5c,                                                  // -0.51                                       /// 003f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00410
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00414
			 0x3f028f5c,                                                  // 0.51                                        /// 00418
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00420
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00424
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00428
			 0x0c400000,                                                  // Leading 1s:                                 /// 0042c
			 0x80000000,                                                  // -zero                                       /// 00430
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00434
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00438
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0043c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00440
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0044c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00450
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00454
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00458
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0045c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00000000,                                                  // zero                                        /// 00464
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0046c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00470
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00474
			 0x7f800000,                                                  // inf                                         /// 00478
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0047c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00480
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00488
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0048c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00490
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00498
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0049c
			 0x0c400000,                                                  // Leading 1s:                                 /// 004a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004b8
			 0xcb000000,                                                  // -8388608.0                                  /// 004bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00504
			 0x0c780000,                                                  // Leading 1s:                                 /// 00508
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00510
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00514
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00518
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00520
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00524
			 0x4b000000,                                                  // 8388608.0                                   /// 00528
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00540
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00544
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00548
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00550
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0055c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00560
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00564
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00568
			 0x33333333,                                                  // 4 random values                             /// 0056c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00570
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0057c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00580
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0c600000,                                                  // Leading 1s:                                 /// 00588
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00594
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0059c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005dc
			 0x33333333,                                                  // 4 random values                             /// 005e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0xbf028f5c,                                                  // -0.51                                       /// 005fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00604
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00608
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00610
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00614
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0061c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00620
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0062c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00630
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00634
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00638
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0063c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00648
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0064c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00650
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00654
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00660
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00664
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00668
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0066c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00674
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00678
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0067c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00684
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00688
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0068c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000000,                                                  // zero                                        /// 00694
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00698
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a4
			 0x7f800000,                                                  // inf                                         /// 006a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006bc
			 0x7f800000,                                                  // inf                                         /// 006c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f8
			 0xffc00001,                                                  // -signaling NaN                              /// 006fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00700
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00704
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00710
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00718
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0071c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00720
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00724
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00728
			 0x0c400000,                                                  // Leading 1s:                                 /// 0072c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00730
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00738
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0073c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00744
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0074c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00754
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00758
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0075c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00764
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00768
			 0x00000000,                                                  // zero                                        /// 0076c
			 0x55555555,                                                  // 4 random values                             /// 00770
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x0e000003,                                                  // Trailing 1s:                                /// 0077c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00784
			 0x7fc00001,                                                  // signaling NaN                               /// 00788
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0078c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00790
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00794
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00798
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0079c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x3f028f5c,                                                  // 0.51                                        /// 007b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b4
			 0xbf028f5c,                                                  // -0.51                                       /// 007b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0xcb000000,                                                  // -8388608.0                                  /// 007e4
			 0x7fc00001,                                                  // signaling NaN                               /// 007e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00800
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00810
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00814
			 0x00011111,                                                  // 9.7958E-41                                  /// 00818
			 0x00000000,                                                  // zero                                        /// 0081c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00820
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00828
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0082c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00830
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00834
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00838
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0083c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00840
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00844
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00848
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00850
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00854
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00864
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00868
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0086c
			 0x00000000,                                                  // zero                                        /// 00870
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00874
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00878
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0xcb000000,                                                  // -8388608.0                                  /// 00880
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00884
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0088c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00890
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00894
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00898
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0089c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xcb000000,                                                  // -8388608.0                                  /// 008b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c8
			 0x7fc00001,                                                  // signaling NaN                               /// 008cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00900
			 0x7f800000,                                                  // inf                                         /// 00904
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0090c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00910
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x7fc00001,                                                  // signaling NaN                               /// 00918
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0091c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00924
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00928
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00934
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00938
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0093c
			 0x4b000000,                                                  // 8388608.0                                   /// 00940
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00944
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00948
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0094c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00950
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00954
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00958
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00960
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00964
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00970
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x80000000,                                                  // -zero                                       /// 00978
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00980
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00984
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00990
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x33333333,                                                  // 4 random values                             /// 0099c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xff800000,                                                  // -inf                                        /// 00a00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a70
			 0xcb000000,                                                  // -8388608.0                                  /// 00a74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ab8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00acc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00adc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ae8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aec
			 0x7f800000,                                                  // inf                                         /// 00af0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00af8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b1c
			 0xff800000,                                                  // -inf                                        /// 00b20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x55555555,                                                  // 4 random values                             /// 00d1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d20
			 0x00000000,                                                  // zero                                        /// 00d24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x7fc00001,                                                  // signaling NaN                               /// 00d44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00da8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00db0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dc0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0x33333333,                                                  // 4 random values                             /// 00ddc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x4b000000,                                                  // 8388608.0                                   /// 00de8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00df0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00df4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e14
			 0x33333333,                                                  // 4 random values                             /// 00e18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e60
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ec0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80000000,                                                  // -zero                                       /// 00ed0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ed4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ee8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ef8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00efc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f30
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00000000,                                                  // zero                                        /// 00f8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fa0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc4
			 0x55555555,                                                  // 4 random values                             /// 00fc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ff8
			 0x00008000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00011111,                                                  // 9.7958E-41                                  /// 00000
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00000000,                                                  // zero                                        /// 00008
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00010
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00014
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00024
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00030
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x55555555,                                                  // 4 random values                             /// 00038
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0003c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00048
			 0x0c600000,                                                  // Leading 1s:                                 /// 0004c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00054
			 0xcb000000,                                                  // -8388608.0                                  /// 00058
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0005c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00060
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00068
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0006c
			 0x4b000000,                                                  // 8388608.0                                   /// 00070
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00078
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00084
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00090
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00094
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00098
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x7f800000,                                                  // inf                                         /// 000ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000b0
			 0xbf028f5c,                                                  // -0.51                                       /// 000b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00100
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00104
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00108
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0010c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00114
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00118
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0011c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00120
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0012c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00130
			 0x0e000003,                                                  // Trailing 1s:                                /// 00134
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00138
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00140
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00144
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0014c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00158
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00164
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0016c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00174
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00178
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00184
			 0x55555555,                                                  // 4 random values                             /// 00188
			 0x4b000000,                                                  // 8388608.0                                   /// 0018c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00190
			 0x00000000,                                                  // zero                                        /// 00194
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00198
			 0x00011111,                                                  // 9.7958E-41                                  /// 0019c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001dc
			 0xff800000,                                                  // -inf                                        /// 001e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e4
			 0xffc00001,                                                  // -signaling NaN                              /// 001e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00200
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00204
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0020c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00210
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00214
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00218
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0021c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00220
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0e000003,                                                  // Trailing 1s:                                /// 00228
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0022c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00230
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00234
			 0x4b000000,                                                  // 8388608.0                                   /// 00238
			 0x7f800000,                                                  // inf                                         /// 0023c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00244
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0024c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00250
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0025c
			 0x33333333,                                                  // 4 random values                             /// 00260
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0c600000,                                                  // Leading 1s:                                 /// 00268
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0026c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00274
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00284
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00288
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0028c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00290
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xbf028f5c,                                                  // -0.51                                       /// 00298
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0029c
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7fc00001,                                                  // signaling NaN                               /// 002a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002c8
			 0x00000000,                                                  // zero                                        /// 002cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002dc
			 0xcb000000,                                                  // -8388608.0                                  /// 002e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00300
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00308
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0030c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00310
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0031c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00328
			 0x0c700000,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00330
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00334
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00338
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xcb000000,                                                  // -8388608.0                                  /// 00344
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00350
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00354
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00358
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0035c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00360
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00364
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00368
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00370
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00374
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00384
			 0x3f028f5c,                                                  // 0.51                                        /// 00388
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c4
			 0x7f800000,                                                  // inf                                         /// 003c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x3f028f5c,                                                  // 0.51                                        /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00410
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00414
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00418
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0041c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0042c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00430
			 0x4b000000,                                                  // 8388608.0                                   /// 00434
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00438
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00440
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00444
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00448
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00454
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00458
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00464
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00468
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0046c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0047c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00480
			 0x80011111,                                                  // -9.7958E-41                                 /// 00484
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0048c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00494
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00498
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0049c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x3f028f5c,                                                  // 0.51                                        /// 004e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00500
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00504
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0xcb000000,                                                  // -8388608.0                                  /// 00514
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00518
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00520
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00528
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00530
			 0x33333333,                                                  // 4 random values                             /// 00534
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00538
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0053c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00540
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00544
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00548
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00550
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00554
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00558
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00560
			 0x80011111,                                                  // -9.7958E-41                                 /// 00564
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00568
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00570
			 0x7fc00001,                                                  // signaling NaN                               /// 00574
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00580
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00584
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00588
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0058c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e4
			 0x7fc00001,                                                  // signaling NaN                               /// 005e8
			 0x7fc00001,                                                  // signaling NaN                               /// 005ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00600
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00608
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00610
			 0xff800000,                                                  // -inf                                        /// 00614
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0061c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00620
			 0x3f028f5c,                                                  // 0.51                                        /// 00624
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0062c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00630
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00634
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00638
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0063c
			 0x33333333,                                                  // 4 random values                             /// 00640
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00648
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00650
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00658
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0065c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00660
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00664
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00668
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00674
			 0x7f800000,                                                  // inf                                         /// 00678
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0067c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00680
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00684
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00688
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0068c
			 0xffc00001,                                                  // -signaling NaN                              /// 00690
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00698
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00700
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00704
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0070c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00710
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00714
			 0x00000000,                                                  // zero                                        /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00720
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00724
			 0xcb000000,                                                  // -8388608.0                                  /// 00728
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0072c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0e000003,                                                  // Trailing 1s:                                /// 00738
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00744
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00748
			 0x7f800000,                                                  // inf                                         /// 0074c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00750
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00754
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00758
			 0x0c780000,                                                  // Leading 1s:                                 /// 0075c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00764
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00768
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x3f028f5c,                                                  // 0.51                                        /// 00770
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00774
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00778
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00780
			 0xcb000000,                                                  // -8388608.0                                  /// 00784
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0078c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00790
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00794
			 0x80000000,                                                  // -zero                                       /// 00798
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0079c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x80000000,                                                  // -zero                                       /// 007ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00804
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00810
			 0x7fc00001,                                                  // signaling NaN                               /// 00814
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00818
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0081c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00824
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00828
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0082c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00830
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00838
			 0x80011111,                                                  // -9.7958E-41                                 /// 0083c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00840
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00844
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0084c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00854
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0085c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00860
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00868
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0086c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00870
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00874
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00878
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00888
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00894
			 0x0c400000,                                                  // Leading 1s:                                 /// 00898
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7fc00001,                                                  // signaling NaN                               /// 008a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00908
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00910
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0091c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00920
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00924
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00928
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0092c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00930
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00934
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00938
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0093c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00940
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0e000003,                                                  // Trailing 1s:                                /// 00948
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00954
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00958
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x55555555,                                                  // 4 random values                             /// 00968
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0096c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00970
			 0x80011111,                                                  // -9.7958E-41                                 /// 00974
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00978
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0097c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00980
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00984
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00988
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0098c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00990
			 0xbf028f5c,                                                  // -0.51                                       /// 00994
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0099c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xcb000000,                                                  // -8388608.0                                  /// 009e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a00
			 0x00000000,                                                  // zero                                        /// 00a04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a08
			 0xbf028f5c,                                                  // -0.51                                       /// 00a0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x55555555,                                                  // 4 random values                             /// 00a30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aac
			 0xff800000,                                                  // -inf                                        /// 00ab0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab4
			 0x55555555,                                                  // 4 random values                             /// 00ab8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ac8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00acc
			 0x00000000,                                                  // zero                                        /// 00ad0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0xff800000,                                                  // -inf                                        /// 00ae4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00aec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00af0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0xbf028f5c,                                                  // -0.51                                       /// 00b08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x55555555,                                                  // 4 random values                             /// 00b74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ba8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000000,                                                  // zero                                        /// 00bc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xbf028f5c,                                                  // -0.51                                       /// 00be4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00be8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bec
			 0x55555555,                                                  // 4 random values                             /// 00bf0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c8c
			 0xff800000,                                                  // -inf                                        /// 00c90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x7fc00001,                                                  // signaling NaN                               /// 00c98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ca4
			 0x55555555,                                                  // 4 random values                             /// 00ca8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cb0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ccc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cec
			 0x7fc00001,                                                  // signaling NaN                               /// 00cf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x4b000000,                                                  // 8388608.0                                   /// 00d0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d1c
			 0xff800000,                                                  // -inf                                        /// 00d20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d34
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00db8
			 0xff800000,                                                  // -inf                                        /// 00dbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ddc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dfc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e08
			 0x7fc00001,                                                  // signaling NaN                               /// 00e0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e14
			 0x80000000,                                                  // -zero                                       /// 00e18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e44
			 0x80000000,                                                  // -zero                                       /// 00e48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e60
			 0x80000000,                                                  // -zero                                       /// 00e64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e70
			 0x55555555,                                                  // 4 random values                             /// 00e74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x33333333,                                                  // 4 random values                             /// 00eb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ebc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ec4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ef8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0xffc00001,                                                  // -signaling NaN                              /// 00f34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f64
			 0x00000000,                                                  // zero                                        /// 00f68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f88
			 0x00000000,                                                  // zero                                        /// 00f8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fc0
			 0x00000000,                                                  // zero                                        /// 00fc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ff8
			 0x00020000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000068,
			 0x0000038c,
			 0x000001ac,
			 0x00000324,
			 0x000007a4,
			 0x000001c8,
			 0x0000073c,
			 0x000005bc,

			 /// vpu register f2
			 0x41e00000,
			 0x41800000,
			 0x41e80000,
			 0x40000000,
			 0x41c00000,
			 0x41600000,
			 0x41300000,
			 0x41b00000,

			 /// vpu register f3
			 0x40800000,
			 0x41f00000,
			 0x41b00000,
			 0x41000000,
			 0x41e00000,
			 0x41d00000,
			 0x3f800000,
			 0x41d00000,

			 /// vpu register f4
			 0x41c00000,
			 0x42000000,
			 0x41880000,
			 0x41a00000,
			 0x41980000,
			 0x41f00000,
			 0x41880000,
			 0x41a80000,

			 /// vpu register f5
			 0x41980000,
			 0x40800000,
			 0x40e00000,
			 0x40800000,
			 0x41a80000,
			 0x41500000,
			 0x41b80000,
			 0x41c80000,

			 /// vpu register f6
			 0x41e00000,
			 0x41c00000,
			 0x41200000,
			 0x41600000,
			 0x41a00000,
			 0x41400000,
			 0x41100000,
			 0x41880000,

			 /// vpu register f7
			 0x41b80000,
			 0x41600000,
			 0x42000000,
			 0x41200000,
			 0x41e80000,
			 0x41e80000,
			 0x40e00000,
			 0x41e00000,

			 /// vpu register f8
			 0x3f800000,
			 0x41100000,
			 0x41a00000,
			 0x40a00000,
			 0x40c00000,
			 0x41700000,
			 0x41d00000,
			 0x41e80000,

			 /// vpu register f9
			 0x40800000,
			 0x40800000,
			 0x41980000,
			 0x41300000,
			 0x41300000,
			 0x41100000,
			 0x41100000,
			 0x41600000,

			 /// vpu register f10
			 0x41e00000,
			 0x41f80000,
			 0x41100000,
			 0x41f80000,
			 0x41b80000,
			 0x41b00000,
			 0x41900000,
			 0x41f00000,

			 /// vpu register f11
			 0x41c00000,
			 0x41700000,
			 0x41900000,
			 0x41f80000,
			 0x41d80000,
			 0x41f00000,
			 0x41f80000,
			 0x41600000,

			 /// vpu register f12
			 0x41500000,
			 0x41200000,
			 0x41000000,
			 0x41a00000,
			 0x41300000,
			 0x41c80000,
			 0x41800000,
			 0x40000000,

			 /// vpu register f13
			 0x41000000,
			 0x41200000,
			 0x40e00000,
			 0x41500000,
			 0x41400000,
			 0x41700000,
			 0x41e00000,
			 0x41600000,

			 /// vpu register f14
			 0x41a80000,
			 0x42000000,
			 0x41c00000,
			 0x41d00000,
			 0x41e80000,
			 0x40a00000,
			 0x41d00000,
			 0x40800000,

			 /// vpu register f15
			 0x40800000,
			 0x41900000,
			 0x42000000,
			 0x41b00000,
			 0x40e00000,
			 0x40e00000,
			 0x3f800000,
			 0x41980000,

			 /// vpu register f16
			 0x41c80000,
			 0x41100000,
			 0x40000000,
			 0x41300000,
			 0x3f800000,
			 0x40400000,
			 0x41b80000,
			 0x40000000,

			 /// vpu register f17
			 0x41c00000,
			 0x41b80000,
			 0x41000000,
			 0x41f00000,
			 0x41980000,
			 0x41f80000,
			 0x41880000,
			 0x41c80000,

			 /// vpu register f18
			 0x41c80000,
			 0x40800000,
			 0x41f80000,
			 0x41800000,
			 0x40800000,
			 0x40e00000,
			 0x41b00000,
			 0x41a80000,

			 /// vpu register f19
			 0x40400000,
			 0x41f80000,
			 0x3f800000,
			 0x41a00000,
			 0x41a80000,
			 0x41e80000,
			 0x41200000,
			 0x41f00000,

			 /// vpu register f20
			 0x41200000,
			 0x41880000,
			 0x40e00000,
			 0x41700000,
			 0x41400000,
			 0x41900000,
			 0x41c80000,
			 0x40400000,

			 /// vpu register f21
			 0x41600000,
			 0x40c00000,
			 0x41100000,
			 0x41200000,
			 0x41600000,
			 0x41100000,
			 0x41880000,
			 0x41600000,

			 /// vpu register f22
			 0x40000000,
			 0x41d80000,
			 0x41200000,
			 0x41d80000,
			 0x41500000,
			 0x41a00000,
			 0x40800000,
			 0x41880000,

			 /// vpu register f23
			 0x41d80000,
			 0x41300000,
			 0x41a00000,
			 0x3f800000,
			 0x40c00000,
			 0x40400000,
			 0x41a80000,
			 0x40800000,

			 /// vpu register f24
			 0x41f80000,
			 0x41c00000,
			 0x41300000,
			 0x41900000,
			 0x41e00000,
			 0x41e00000,
			 0x41b80000,
			 0x40a00000,

			 /// vpu register f25
			 0x41f80000,
			 0x41500000,
			 0x40a00000,
			 0x42000000,
			 0x41c00000,
			 0x41900000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f26
			 0x40400000,
			 0x40e00000,
			 0x41600000,
			 0x42000000,
			 0x40a00000,
			 0x41a00000,
			 0x40c00000,
			 0x40800000,

			 /// vpu register f27
			 0x41880000,
			 0x41900000,
			 0x3f800000,
			 0x41880000,
			 0x41980000,
			 0x41600000,
			 0x41700000,
			 0x40a00000,

			 /// vpu register f28
			 0x42000000,
			 0x41e80000,
			 0x41f00000,
			 0x3f800000,
			 0x41980000,
			 0x41000000,
			 0x41a00000,
			 0x41000000,

			 /// vpu register f29
			 0x41500000,
			 0x41a80000,
			 0x40400000,
			 0x41a00000,
			 0x41000000,
			 0x41300000,
			 0x41e80000,
			 0x41f80000,

			 /// vpu register f30
			 0x41800000,
			 0x41f80000,
			 0x41700000,
			 0x41700000,
			 0x41100000,
			 0x41900000,
			 0x41800000,
			 0x41400000,

			 /// vpu register f31
			 0x41200000,
			 0x41500000,
			 0x41880000,
			 0x41a00000,
			 0x41a00000,
			 0x41600000,
			 0x41d00000,
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
		"maskxor m6, m3, m7\n"                                ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m1, m4\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m0, m6\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m5, m1\n"                                ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m0, m3\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m1, m7\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m1, m4\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m4, m7\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m4, m1\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m5, m6\n"                                ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m4, m7\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m4, m1\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m4, m7\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m2, m5\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m4, m5\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m5, m2\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m0, m5\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m4, m5\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m6, m5\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m6, m0\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m3, m2\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m5, m7\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m4, m0\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m2, m1\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m0, m2\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m5, m4\n"                                ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m4, m1\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m2, m3, m2\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m2, m7\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m0, m3\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m2, m7\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m1, m5\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m0, m0\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m7, m6\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m3, m2\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m1, m6\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m0, m1\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m6, m3\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m2, m6, m1\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m2, m1\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m6, m5\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m0, m5\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m7, m1\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m5, m6\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m3, m0\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m3, m3\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m2, m3, m5\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m0, m6\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m2, m6\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m3, m1\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m5, m1\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m7, m2\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m2, m4\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m7, m3\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m2, m0\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m2, m1\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m7, m3\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m4, m7\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m5, m0\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m1, m1\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m1, m3\n"                                ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m1, m2\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m1, m2\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m5, m5\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m7, m4\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m0, m7\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m7, m2\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m3, m3, m4\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m3, m5\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m4, m7\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m6, m1\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m7, m4\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m4, m3\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m4, m5\n"                                ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m5, m5\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m1, m1\n"                                ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m6, m5\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m5, m5\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m6, m5\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m4, m7\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m3, m7\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m1, m7\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m4, m3\n"                                ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m1, m5\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m1, m1\n"                                ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m0, m3, m6\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m3, m2\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m6, m2\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m7, m4, m7\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m7, m6\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m7, m4\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m2, m6\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m6, m4, m5\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m4, m7\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m0, m0\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m4, m0, m4\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m2, m1, m1\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m5, m6, m5\n"                                ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskxor m1, m4, m6\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
