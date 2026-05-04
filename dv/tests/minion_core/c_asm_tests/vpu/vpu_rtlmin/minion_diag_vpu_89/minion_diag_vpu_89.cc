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
			 0x91426feb, /// 0x0
			 0x5bf87174, /// 0x4
			 0x3ab08f6b, /// 0x8
			 0x07fd8fe1, /// 0xc
			 0xe0bc3851, /// 0x10
			 0x8cc21586, /// 0x14
			 0x97adccac, /// 0x18
			 0xf7c498d1, /// 0x1c
			 0xe74dbfe5, /// 0x20
			 0x33762932, /// 0x24
			 0x5b0238a3, /// 0x28
			 0x8bd2711c, /// 0x2c
			 0x76d37df5, /// 0x30
			 0xcf9a3e61, /// 0x34
			 0x03726788, /// 0x38
			 0x49bb707f, /// 0x3c
			 0x32ea800d, /// 0x40
			 0xa13a1609, /// 0x44
			 0x02fbcd6c, /// 0x48
			 0x0c578631, /// 0x4c
			 0x5c69e5f7, /// 0x50
			 0xe30653cc, /// 0x54
			 0x78293ce2, /// 0x58
			 0x74eb1ddb, /// 0x5c
			 0x09410b0f, /// 0x60
			 0xbc8759c7, /// 0x64
			 0xacabca69, /// 0x68
			 0x76ca9bed, /// 0x6c
			 0x8ceea439, /// 0x70
			 0xe7152ee8, /// 0x74
			 0xe563d92f, /// 0x78
			 0x5a49f789, /// 0x7c
			 0xe9ed5de9, /// 0x80
			 0x42148da3, /// 0x84
			 0x880963a1, /// 0x88
			 0x094b9e09, /// 0x8c
			 0x2dcd85e0, /// 0x90
			 0x858a7dcc, /// 0x94
			 0x1fa0bda2, /// 0x98
			 0xda45e7e8, /// 0x9c
			 0x9c906e24, /// 0xa0
			 0x73a32c08, /// 0xa4
			 0xabde896d, /// 0xa8
			 0x1316a4a2, /// 0xac
			 0xc3240477, /// 0xb0
			 0x0dd4e9eb, /// 0xb4
			 0x9ed48a48, /// 0xb8
			 0x22be875c, /// 0xbc
			 0x9458273e, /// 0xc0
			 0xe9e91dd1, /// 0xc4
			 0xcf153b31, /// 0xc8
			 0x2ca16564, /// 0xcc
			 0x4c8e051d, /// 0xd0
			 0xac56d05b, /// 0xd4
			 0xec2baba1, /// 0xd8
			 0xc3f893de, /// 0xdc
			 0x4ea939f3, /// 0xe0
			 0x476ed41f, /// 0xe4
			 0x897276ea, /// 0xe8
			 0x93c25a5b, /// 0xec
			 0xff84ab24, /// 0xf0
			 0xb76c5372, /// 0xf4
			 0xbea3aa84, /// 0xf8
			 0x67327011, /// 0xfc
			 0xd0a3b45a, /// 0x100
			 0x8772e651, /// 0x104
			 0x29c23111, /// 0x108
			 0x08a8e654, /// 0x10c
			 0xfd53af45, /// 0x110
			 0x8da5bcc5, /// 0x114
			 0x09910de4, /// 0x118
			 0x2874c8b1, /// 0x11c
			 0x6abf6cf0, /// 0x120
			 0x47fee14f, /// 0x124
			 0xf1b3e303, /// 0x128
			 0x00d22c42, /// 0x12c
			 0x1806b115, /// 0x130
			 0x6ce65357, /// 0x134
			 0x463e2051, /// 0x138
			 0xc68df43f, /// 0x13c
			 0x5ec3972f, /// 0x140
			 0x0c921817, /// 0x144
			 0xcf5187c9, /// 0x148
			 0xd800bd02, /// 0x14c
			 0x64915edd, /// 0x150
			 0x6f9dbc94, /// 0x154
			 0xa640b1f6, /// 0x158
			 0xd780fb5e, /// 0x15c
			 0x7a7a0140, /// 0x160
			 0xc6620672, /// 0x164
			 0xaf3df0de, /// 0x168
			 0xa43e0dee, /// 0x16c
			 0xdbfa1503, /// 0x170
			 0x9526e307, /// 0x174
			 0x5a9eda5f, /// 0x178
			 0x0d0a7d22, /// 0x17c
			 0xc9db66de, /// 0x180
			 0xca493eb8, /// 0x184
			 0x77aa0dba, /// 0x188
			 0x17abfc9d, /// 0x18c
			 0xf7d07d91, /// 0x190
			 0x0417cc42, /// 0x194
			 0xc2c8ee28, /// 0x198
			 0xb3dc3394, /// 0x19c
			 0x19645383, /// 0x1a0
			 0x6fea1970, /// 0x1a4
			 0x73ee2efc, /// 0x1a8
			 0x1fcaea51, /// 0x1ac
			 0x5a9ea7ee, /// 0x1b0
			 0x9d7266b6, /// 0x1b4
			 0x2b6949bd, /// 0x1b8
			 0xf23aa75d, /// 0x1bc
			 0x1544b14c, /// 0x1c0
			 0xfd3b4518, /// 0x1c4
			 0x303647a2, /// 0x1c8
			 0x42758cae, /// 0x1cc
			 0xf16c621f, /// 0x1d0
			 0x1bc83829, /// 0x1d4
			 0xbbe07d1f, /// 0x1d8
			 0x128ced70, /// 0x1dc
			 0xf5ec7a38, /// 0x1e0
			 0x3ed70c9f, /// 0x1e4
			 0x3d34f443, /// 0x1e8
			 0xff8286c2, /// 0x1ec
			 0x7e6bfd3f, /// 0x1f0
			 0xc7b0f032, /// 0x1f4
			 0x702a6979, /// 0x1f8
			 0xda3d4023, /// 0x1fc
			 0xd9c3af2d, /// 0x200
			 0x0b015ba9, /// 0x204
			 0x8eefea65, /// 0x208
			 0xcb107716, /// 0x20c
			 0x745da790, /// 0x210
			 0xf04aa35d, /// 0x214
			 0x04f8e5f4, /// 0x218
			 0x9a45c1b0, /// 0x21c
			 0xdb91d8aa, /// 0x220
			 0xd0bed23c, /// 0x224
			 0x2363d7b3, /// 0x228
			 0x8de07d04, /// 0x22c
			 0xd834d56e, /// 0x230
			 0x5cc4e413, /// 0x234
			 0x3aa330b6, /// 0x238
			 0xb61ff098, /// 0x23c
			 0xfa572c03, /// 0x240
			 0xbef10f91, /// 0x244
			 0x53b1badc, /// 0x248
			 0x377e9365, /// 0x24c
			 0x88dccd38, /// 0x250
			 0xc3e7ce09, /// 0x254
			 0x6318e2aa, /// 0x258
			 0x8c31ccd7, /// 0x25c
			 0xe438a7f1, /// 0x260
			 0xf47bf01b, /// 0x264
			 0xa2439302, /// 0x268
			 0xb36bbecd, /// 0x26c
			 0x480fb25f, /// 0x270
			 0xeddf5273, /// 0x274
			 0x9e9421df, /// 0x278
			 0x2bd7205c, /// 0x27c
			 0x69a03862, /// 0x280
			 0x4a63749f, /// 0x284
			 0x12cb3efc, /// 0x288
			 0x602f2c2b, /// 0x28c
			 0x488d310c, /// 0x290
			 0x63d4c19f, /// 0x294
			 0x42554c6f, /// 0x298
			 0x689f2eb1, /// 0x29c
			 0x6756b5e7, /// 0x2a0
			 0x3c33a690, /// 0x2a4
			 0xd8ab6022, /// 0x2a8
			 0xf80c9929, /// 0x2ac
			 0xd8b9a8df, /// 0x2b0
			 0xd7da46f4, /// 0x2b4
			 0x89d77649, /// 0x2b8
			 0x1064a103, /// 0x2bc
			 0x91002d4c, /// 0x2c0
			 0xd0dffd0b, /// 0x2c4
			 0x961d4e46, /// 0x2c8
			 0x1fc73d8e, /// 0x2cc
			 0x4be45077, /// 0x2d0
			 0x685f42a8, /// 0x2d4
			 0x5da357b0, /// 0x2d8
			 0x9a1a6890, /// 0x2dc
			 0x40feb290, /// 0x2e0
			 0x277c152d, /// 0x2e4
			 0xef137306, /// 0x2e8
			 0x661299ec, /// 0x2ec
			 0x8ccba300, /// 0x2f0
			 0xce17ca7b, /// 0x2f4
			 0xd53ecbf6, /// 0x2f8
			 0x5305ae42, /// 0x2fc
			 0x7f6826df, /// 0x300
			 0xe9adc6c7, /// 0x304
			 0x3a00cd0e, /// 0x308
			 0x09e09665, /// 0x30c
			 0x33459a36, /// 0x310
			 0x73bfe449, /// 0x314
			 0x9bb36250, /// 0x318
			 0x2958274e, /// 0x31c
			 0x00844089, /// 0x320
			 0x16610933, /// 0x324
			 0x2cb6b68b, /// 0x328
			 0x473af3e1, /// 0x32c
			 0x5ce261cc, /// 0x330
			 0xb3026c58, /// 0x334
			 0x8cd2c8ef, /// 0x338
			 0x2e71477d, /// 0x33c
			 0xe6c748bb, /// 0x340
			 0x9b58d32d, /// 0x344
			 0x63495b82, /// 0x348
			 0x11040618, /// 0x34c
			 0x1c0b80cf, /// 0x350
			 0xc93fbe02, /// 0x354
			 0x83375d59, /// 0x358
			 0x58294681, /// 0x35c
			 0xdc0a3862, /// 0x360
			 0x02d57c7f, /// 0x364
			 0x606289b5, /// 0x368
			 0x5162ab32, /// 0x36c
			 0xa4a3bc91, /// 0x370
			 0xcf04d421, /// 0x374
			 0xa7381d49, /// 0x378
			 0x1d20bd44, /// 0x37c
			 0x3e1cf613, /// 0x380
			 0x0c023d7c, /// 0x384
			 0x5f111976, /// 0x388
			 0xf8164621, /// 0x38c
			 0xdf79c8d9, /// 0x390
			 0xda788444, /// 0x394
			 0x96ffc615, /// 0x398
			 0xf651d9ef, /// 0x39c
			 0xfb10cbe0, /// 0x3a0
			 0x126a4b2a, /// 0x3a4
			 0xb392420d, /// 0x3a8
			 0x8a4cfb53, /// 0x3ac
			 0x3372991c, /// 0x3b0
			 0x68c37c4b, /// 0x3b4
			 0x57abedee, /// 0x3b8
			 0x37c13e7d, /// 0x3bc
			 0xec7c9271, /// 0x3c0
			 0x217f77c5, /// 0x3c4
			 0x8afbfa07, /// 0x3c8
			 0xbf2025c9, /// 0x3cc
			 0x505dadb4, /// 0x3d0
			 0xb3bad2b1, /// 0x3d4
			 0x51faa7bc, /// 0x3d8
			 0x2dce4b7f, /// 0x3dc
			 0x9c803b1e, /// 0x3e0
			 0x67e74485, /// 0x3e4
			 0x1b64562c, /// 0x3e8
			 0x32b0d8bb, /// 0x3ec
			 0xbca18d09, /// 0x3f0
			 0x57300388, /// 0x3f4
			 0x5d9031f0, /// 0x3f8
			 0x39aa8731, /// 0x3fc
			 0xfc22de47, /// 0x400
			 0x05ffa6c4, /// 0x404
			 0xa481dd91, /// 0x408
			 0x89da69c2, /// 0x40c
			 0xf93402b1, /// 0x410
			 0xeb5a4756, /// 0x414
			 0x0ec62328, /// 0x418
			 0xde48d656, /// 0x41c
			 0x21429a99, /// 0x420
			 0x59172c44, /// 0x424
			 0x7e49619a, /// 0x428
			 0xef2c52af, /// 0x42c
			 0x420ae822, /// 0x430
			 0x59b29be8, /// 0x434
			 0xf72c7904, /// 0x438
			 0xa7c3f4b7, /// 0x43c
			 0x049ec837, /// 0x440
			 0x97c0562f, /// 0x444
			 0x4ce1d8ab, /// 0x448
			 0x77a00650, /// 0x44c
			 0x05431d62, /// 0x450
			 0x4595f5f8, /// 0x454
			 0x4bdd3786, /// 0x458
			 0x7a64bc5c, /// 0x45c
			 0xf60de313, /// 0x460
			 0x7233503c, /// 0x464
			 0xc0759e78, /// 0x468
			 0x40a721b4, /// 0x46c
			 0x34008171, /// 0x470
			 0x49bf4352, /// 0x474
			 0xb8a11d75, /// 0x478
			 0x4aac346a, /// 0x47c
			 0x7e76cf4e, /// 0x480
			 0x91af850b, /// 0x484
			 0x67f268f8, /// 0x488
			 0x67b2e963, /// 0x48c
			 0xc6616ff3, /// 0x490
			 0x56d09d09, /// 0x494
			 0x8a060f55, /// 0x498
			 0x567b88c1, /// 0x49c
			 0x51e539c8, /// 0x4a0
			 0xff44e334, /// 0x4a4
			 0x845336ac, /// 0x4a8
			 0xb75bb286, /// 0x4ac
			 0xccd43ad6, /// 0x4b0
			 0x6e985c3b, /// 0x4b4
			 0xd5e08595, /// 0x4b8
			 0x9a0aa815, /// 0x4bc
			 0x128d1ff6, /// 0x4c0
			 0xec1998cb, /// 0x4c4
			 0xf8e0bd3b, /// 0x4c8
			 0xab74e31d, /// 0x4cc
			 0xae0e892c, /// 0x4d0
			 0x35177762, /// 0x4d4
			 0xd48ec1ed, /// 0x4d8
			 0xa2491dde, /// 0x4dc
			 0x6038a905, /// 0x4e0
			 0x6f6b2e6d, /// 0x4e4
			 0x980c90fa, /// 0x4e8
			 0x66374fcd, /// 0x4ec
			 0x1b2f688c, /// 0x4f0
			 0x88f3d4c3, /// 0x4f4
			 0x7bf3dda1, /// 0x4f8
			 0xe16c807e, /// 0x4fc
			 0x3ff29504, /// 0x500
			 0x3532febd, /// 0x504
			 0x56638730, /// 0x508
			 0x8fa00726, /// 0x50c
			 0x412ff9ce, /// 0x510
			 0x3008620a, /// 0x514
			 0x582aafd9, /// 0x518
			 0x3b9e8c7d, /// 0x51c
			 0xdf4c09bc, /// 0x520
			 0xdd2f4fb6, /// 0x524
			 0x39844e82, /// 0x528
			 0xef44085a, /// 0x52c
			 0x2e0bf68e, /// 0x530
			 0xc2a9ac60, /// 0x534
			 0x9822dc41, /// 0x538
			 0x97ad78e1, /// 0x53c
			 0x1b348495, /// 0x540
			 0x8793d4f4, /// 0x544
			 0xb01f72db, /// 0x548
			 0xa5097a4d, /// 0x54c
			 0xb34241dc, /// 0x550
			 0xbed7e2c3, /// 0x554
			 0xe519b86d, /// 0x558
			 0x3b710e54, /// 0x55c
			 0x17f68206, /// 0x560
			 0x2c43b801, /// 0x564
			 0xf4c4980c, /// 0x568
			 0xa8035dbb, /// 0x56c
			 0x4c3748c4, /// 0x570
			 0x649a3b9f, /// 0x574
			 0x0454c830, /// 0x578
			 0x45321358, /// 0x57c
			 0xc4fd46dd, /// 0x580
			 0x03aeee4a, /// 0x584
			 0x41ccfcf8, /// 0x588
			 0x678460d4, /// 0x58c
			 0x62c4ab1e, /// 0x590
			 0xdca8ad5b, /// 0x594
			 0xcdff8e28, /// 0x598
			 0x868f4cc7, /// 0x59c
			 0xd3823b98, /// 0x5a0
			 0xf1ad8c3a, /// 0x5a4
			 0xbc860d57, /// 0x5a8
			 0xf50512b4, /// 0x5ac
			 0x3ad37f24, /// 0x5b0
			 0xf5894910, /// 0x5b4
			 0xd183df48, /// 0x5b8
			 0x3398f515, /// 0x5bc
			 0x59c3bf6d, /// 0x5c0
			 0xb8f253d5, /// 0x5c4
			 0x53326c68, /// 0x5c8
			 0x6ebad0ae, /// 0x5cc
			 0x99058f1a, /// 0x5d0
			 0x0aa7aaa3, /// 0x5d4
			 0x867a10be, /// 0x5d8
			 0x1684f4b7, /// 0x5dc
			 0x791db642, /// 0x5e0
			 0x840862d7, /// 0x5e4
			 0xc4cf1efb, /// 0x5e8
			 0x814fecb6, /// 0x5ec
			 0x302dfec1, /// 0x5f0
			 0xfb7718ce, /// 0x5f4
			 0xf0213713, /// 0x5f8
			 0x4bcd651e, /// 0x5fc
			 0xf4effc17, /// 0x600
			 0xe72215cf, /// 0x604
			 0x4a42b099, /// 0x608
			 0x4c32c6a6, /// 0x60c
			 0x941c3f66, /// 0x610
			 0x24fba041, /// 0x614
			 0x76700934, /// 0x618
			 0x2b983897, /// 0x61c
			 0x8bc62f19, /// 0x620
			 0xa7910f37, /// 0x624
			 0x7723d88e, /// 0x628
			 0x76865b84, /// 0x62c
			 0x31181cc2, /// 0x630
			 0x2976ddd0, /// 0x634
			 0xd192899f, /// 0x638
			 0xfc13eacf, /// 0x63c
			 0xbe6e4bd0, /// 0x640
			 0x7cbb7a37, /// 0x644
			 0xc62f5e16, /// 0x648
			 0xd0750c52, /// 0x64c
			 0xc1bb1716, /// 0x650
			 0xaf4d3eea, /// 0x654
			 0x0bede8e4, /// 0x658
			 0x2aef41f7, /// 0x65c
			 0xc2ee8161, /// 0x660
			 0x92f263c0, /// 0x664
			 0x2f3ddd0e, /// 0x668
			 0xf24c46c4, /// 0x66c
			 0xdd9847d9, /// 0x670
			 0x10a9acd3, /// 0x674
			 0xdffa5aee, /// 0x678
			 0xe6865e30, /// 0x67c
			 0xd2e03e5e, /// 0x680
			 0xa8961301, /// 0x684
			 0x1283bebd, /// 0x688
			 0x2bde0a5f, /// 0x68c
			 0x025473b0, /// 0x690
			 0x550931e0, /// 0x694
			 0xa2ed7196, /// 0x698
			 0xba75d439, /// 0x69c
			 0x528b9a31, /// 0x6a0
			 0x7e6587fe, /// 0x6a4
			 0x17199907, /// 0x6a8
			 0x49c34003, /// 0x6ac
			 0xc3aebccf, /// 0x6b0
			 0x6fa6c37b, /// 0x6b4
			 0x25e5ef55, /// 0x6b8
			 0x012650ec, /// 0x6bc
			 0x585741bc, /// 0x6c0
			 0x29b3e909, /// 0x6c4
			 0x3039b916, /// 0x6c8
			 0x99d872f7, /// 0x6cc
			 0xcccf2321, /// 0x6d0
			 0xd74e9d57, /// 0x6d4
			 0xdee74058, /// 0x6d8
			 0x6b100e8f, /// 0x6dc
			 0xfea8bd2c, /// 0x6e0
			 0x1104419f, /// 0x6e4
			 0x12afa2d7, /// 0x6e8
			 0x1cde0436, /// 0x6ec
			 0x0a27d80f, /// 0x6f0
			 0x1e80f7db, /// 0x6f4
			 0x25e715c1, /// 0x6f8
			 0xf0677ea8, /// 0x6fc
			 0x7c3ed000, /// 0x700
			 0x1455e859, /// 0x704
			 0x92555b63, /// 0x708
			 0x47fe1e10, /// 0x70c
			 0x03815031, /// 0x710
			 0x5d4487ba, /// 0x714
			 0x3070f4ab, /// 0x718
			 0x5d5a81f8, /// 0x71c
			 0x43b434cf, /// 0x720
			 0x8000d588, /// 0x724
			 0xb6dce76f, /// 0x728
			 0xb95ad0df, /// 0x72c
			 0xeb7431fc, /// 0x730
			 0x19749d6b, /// 0x734
			 0xaadfe802, /// 0x738
			 0x35297ccb, /// 0x73c
			 0x20a9bb42, /// 0x740
			 0x51deba76, /// 0x744
			 0xa9ac8fe8, /// 0x748
			 0xd84d633a, /// 0x74c
			 0x1f3fe0b1, /// 0x750
			 0xa22e32eb, /// 0x754
			 0xbd05efe1, /// 0x758
			 0x1ffc891d, /// 0x75c
			 0x87b4e0c2, /// 0x760
			 0x254b3a8b, /// 0x764
			 0x52f18f56, /// 0x768
			 0x44726c0c, /// 0x76c
			 0xcff33930, /// 0x770
			 0x971221a6, /// 0x774
			 0x10b707d4, /// 0x778
			 0x7fa40868, /// 0x77c
			 0x1d84daea, /// 0x780
			 0x7fae05bf, /// 0x784
			 0xd96759fe, /// 0x788
			 0x51f1a3c5, /// 0x78c
			 0x819579f9, /// 0x790
			 0x182d4761, /// 0x794
			 0x81e5ec0b, /// 0x798
			 0xff990917, /// 0x79c
			 0x3d4175f8, /// 0x7a0
			 0xb4a40cd0, /// 0x7a4
			 0x15218393, /// 0x7a8
			 0xbcae8d49, /// 0x7ac
			 0x9295be20, /// 0x7b0
			 0xaf354c7e, /// 0x7b4
			 0x5b9b0d4c, /// 0x7b8
			 0xabac9e7d, /// 0x7bc
			 0x2b6c5cfc, /// 0x7c0
			 0xfd7b997e, /// 0x7c4
			 0xe2c4ce08, /// 0x7c8
			 0xdaea57e4, /// 0x7cc
			 0xe7612d62, /// 0x7d0
			 0x51ee8c04, /// 0x7d4
			 0xbc07cd9c, /// 0x7d8
			 0x05a1c1ec, /// 0x7dc
			 0xc5e336ec, /// 0x7e0
			 0xe775edb7, /// 0x7e4
			 0xb36a8c40, /// 0x7e8
			 0xc4e64b62, /// 0x7ec
			 0xd5536d55, /// 0x7f0
			 0x0c8b7c0c, /// 0x7f4
			 0xd48f2522, /// 0x7f8
			 0xd6f4b0a4, /// 0x7fc
			 0x8affeb50, /// 0x800
			 0x02acb3a4, /// 0x804
			 0x89ef8a07, /// 0x808
			 0x88332c4d, /// 0x80c
			 0x267e3696, /// 0x810
			 0x33fe918a, /// 0x814
			 0x8a49b32b, /// 0x818
			 0x326b1225, /// 0x81c
			 0x84ba9860, /// 0x820
			 0xf3ed6b19, /// 0x824
			 0xb358beff, /// 0x828
			 0x2b32dbb6, /// 0x82c
			 0x6ecb250a, /// 0x830
			 0xc3913048, /// 0x834
			 0x615391f0, /// 0x838
			 0x361cd376, /// 0x83c
			 0xe69f4b16, /// 0x840
			 0x3bb2d237, /// 0x844
			 0xc20df56b, /// 0x848
			 0xf77d5d9b, /// 0x84c
			 0x2cf824eb, /// 0x850
			 0x250c60f7, /// 0x854
			 0x02141642, /// 0x858
			 0xe26b922a, /// 0x85c
			 0x8330037c, /// 0x860
			 0xc5ef9993, /// 0x864
			 0x2adcc689, /// 0x868
			 0x10e253dd, /// 0x86c
			 0x59529447, /// 0x870
			 0x76f7c9e9, /// 0x874
			 0x9f4315df, /// 0x878
			 0x1dd82e06, /// 0x87c
			 0x1262bd94, /// 0x880
			 0x1c3374ee, /// 0x884
			 0xbf89d6d3, /// 0x888
			 0x4d0beade, /// 0x88c
			 0x7721a000, /// 0x890
			 0xca44edf6, /// 0x894
			 0xdba0e889, /// 0x898
			 0x3ea965b8, /// 0x89c
			 0xa2dc8e7d, /// 0x8a0
			 0x64b74c55, /// 0x8a4
			 0x4fadc95a, /// 0x8a8
			 0x8ce56b9e, /// 0x8ac
			 0xe6f42ef6, /// 0x8b0
			 0x6a198eb7, /// 0x8b4
			 0x45f26fa6, /// 0x8b8
			 0x51412cfe, /// 0x8bc
			 0xca3172b7, /// 0x8c0
			 0x95256c42, /// 0x8c4
			 0x04d7e233, /// 0x8c8
			 0x66ff9ab1, /// 0x8cc
			 0x477b83b7, /// 0x8d0
			 0x3a232c4f, /// 0x8d4
			 0x54b97611, /// 0x8d8
			 0x356351d8, /// 0x8dc
			 0x249efd84, /// 0x8e0
			 0xd185fc1f, /// 0x8e4
			 0x203e72c6, /// 0x8e8
			 0x6a54dd3d, /// 0x8ec
			 0xe504abab, /// 0x8f0
			 0x9cd39a1c, /// 0x8f4
			 0x000fcc73, /// 0x8f8
			 0x54a4bd9a, /// 0x8fc
			 0xd7ccdd70, /// 0x900
			 0xdfc51677, /// 0x904
			 0xc3e3c074, /// 0x908
			 0x2dab4e87, /// 0x90c
			 0x2390c656, /// 0x910
			 0xdaf8aeeb, /// 0x914
			 0xbb072807, /// 0x918
			 0x4da81206, /// 0x91c
			 0x1de50fbd, /// 0x920
			 0xa22948c0, /// 0x924
			 0xc5788b8a, /// 0x928
			 0x5aebe701, /// 0x92c
			 0x21476190, /// 0x930
			 0xe2dc0de6, /// 0x934
			 0xbdc08fac, /// 0x938
			 0xd4970904, /// 0x93c
			 0xd0da05fe, /// 0x940
			 0xb6958915, /// 0x944
			 0x78b7ac84, /// 0x948
			 0x9643bc9c, /// 0x94c
			 0x6a177fcf, /// 0x950
			 0x9f44d32e, /// 0x954
			 0x7255dfde, /// 0x958
			 0x5738c753, /// 0x95c
			 0xfc2b1d7e, /// 0x960
			 0xcc151d9f, /// 0x964
			 0xbe1cd784, /// 0x968
			 0x2d947a47, /// 0x96c
			 0x9c344097, /// 0x970
			 0x7902ee44, /// 0x974
			 0x31c9d07a, /// 0x978
			 0xa23b790c, /// 0x97c
			 0x4af96633, /// 0x980
			 0x24d95d1b, /// 0x984
			 0xc31deee9, /// 0x988
			 0x35dd9963, /// 0x98c
			 0xaa2eb3b9, /// 0x990
			 0xf7d46f8c, /// 0x994
			 0x8059ee76, /// 0x998
			 0xc656da59, /// 0x99c
			 0x6bed34e9, /// 0x9a0
			 0x48af19a7, /// 0x9a4
			 0x0810082f, /// 0x9a8
			 0xfd9b1af1, /// 0x9ac
			 0xfca6dab1, /// 0x9b0
			 0x192bcb24, /// 0x9b4
			 0x0335aef1, /// 0x9b8
			 0xc42605d2, /// 0x9bc
			 0xa594882a, /// 0x9c0
			 0x0559b480, /// 0x9c4
			 0x334e7127, /// 0x9c8
			 0x7b1c9fd2, /// 0x9cc
			 0x42e4d911, /// 0x9d0
			 0x0a52dbea, /// 0x9d4
			 0x1081dd96, /// 0x9d8
			 0x9d2b2766, /// 0x9dc
			 0x9c37d5f7, /// 0x9e0
			 0xd759640e, /// 0x9e4
			 0xb6942dd9, /// 0x9e8
			 0x1569ac29, /// 0x9ec
			 0x45b90aeb, /// 0x9f0
			 0xfc6b7af6, /// 0x9f4
			 0x6e0ecd2f, /// 0x9f8
			 0xe1a7b2e8, /// 0x9fc
			 0x3d954aac, /// 0xa00
			 0x61b8045a, /// 0xa04
			 0x43af3424, /// 0xa08
			 0x09dbd465, /// 0xa0c
			 0x0de4105a, /// 0xa10
			 0x2a6bde95, /// 0xa14
			 0xebef1632, /// 0xa18
			 0x5e2032ca, /// 0xa1c
			 0xf1702329, /// 0xa20
			 0x5b71e52e, /// 0xa24
			 0x59eed30c, /// 0xa28
			 0x48ede0f1, /// 0xa2c
			 0xcd62b258, /// 0xa30
			 0x9017153c, /// 0xa34
			 0x7a537d1b, /// 0xa38
			 0xa4afff6b, /// 0xa3c
			 0xf9920f24, /// 0xa40
			 0x270f39bf, /// 0xa44
			 0x410e66c5, /// 0xa48
			 0xa380e7f8, /// 0xa4c
			 0xf47da5c1, /// 0xa50
			 0x9b6f28a9, /// 0xa54
			 0x9694d509, /// 0xa58
			 0xc89513e5, /// 0xa5c
			 0xfffdc405, /// 0xa60
			 0xb9a58fdd, /// 0xa64
			 0x94e03e8f, /// 0xa68
			 0x51434b1a, /// 0xa6c
			 0xc650b6fb, /// 0xa70
			 0xd038f819, /// 0xa74
			 0xf9f00b88, /// 0xa78
			 0xc5a5f5e5, /// 0xa7c
			 0x05d3b8d2, /// 0xa80
			 0x44c3e479, /// 0xa84
			 0x59fe9838, /// 0xa88
			 0x602e1a9f, /// 0xa8c
			 0xdb6d6790, /// 0xa90
			 0xdc3e77b0, /// 0xa94
			 0xce417020, /// 0xa98
			 0xbca5e689, /// 0xa9c
			 0xbbbd85cf, /// 0xaa0
			 0xcfa88f14, /// 0xaa4
			 0x3385259c, /// 0xaa8
			 0xd30ea609, /// 0xaac
			 0x470549be, /// 0xab0
			 0xa3b6bb97, /// 0xab4
			 0x8281360f, /// 0xab8
			 0x54a8ca90, /// 0xabc
			 0x720bedf4, /// 0xac0
			 0x214c9215, /// 0xac4
			 0x34327395, /// 0xac8
			 0xae9cf8d5, /// 0xacc
			 0x57bc7050, /// 0xad0
			 0xba61172c, /// 0xad4
			 0xbc8bdb25, /// 0xad8
			 0x71e056a2, /// 0xadc
			 0x199440cc, /// 0xae0
			 0x17a6c58c, /// 0xae4
			 0xa03a268a, /// 0xae8
			 0x628a5c72, /// 0xaec
			 0xd0ed8c98, /// 0xaf0
			 0x7ed6c036, /// 0xaf4
			 0x00c97e67, /// 0xaf8
			 0x10a96c40, /// 0xafc
			 0x286a5e19, /// 0xb00
			 0x2a12f973, /// 0xb04
			 0x5d4f9cb9, /// 0xb08
			 0xf7523297, /// 0xb0c
			 0xa7145d78, /// 0xb10
			 0xaf915e95, /// 0xb14
			 0x55981617, /// 0xb18
			 0x5d016cba, /// 0xb1c
			 0xa9f04729, /// 0xb20
			 0xb66a9966, /// 0xb24
			 0xb10985bb, /// 0xb28
			 0x2ccc2aba, /// 0xb2c
			 0xedf3a9ff, /// 0xb30
			 0xd6019334, /// 0xb34
			 0x0702537e, /// 0xb38
			 0x24a217ef, /// 0xb3c
			 0xd987264d, /// 0xb40
			 0x2a5e4a03, /// 0xb44
			 0x65122f4d, /// 0xb48
			 0xbb2b4082, /// 0xb4c
			 0xad7b184c, /// 0xb50
			 0xb12f6a1d, /// 0xb54
			 0xb7fbdeca, /// 0xb58
			 0xcb91c51e, /// 0xb5c
			 0x4f8d077f, /// 0xb60
			 0x2c4b855f, /// 0xb64
			 0xdb5c8afb, /// 0xb68
			 0x1835032a, /// 0xb6c
			 0xca8e9af1, /// 0xb70
			 0x4ee2b58e, /// 0xb74
			 0x463c59a6, /// 0xb78
			 0x2a264cd4, /// 0xb7c
			 0xdf7c8845, /// 0xb80
			 0xb337f9a5, /// 0xb84
			 0x97b58950, /// 0xb88
			 0xb2b881ad, /// 0xb8c
			 0xfdc0bc55, /// 0xb90
			 0xd36cd2eb, /// 0xb94
			 0xbc29377f, /// 0xb98
			 0x3f7f48ad, /// 0xb9c
			 0x08c844ff, /// 0xba0
			 0x4e0a43ae, /// 0xba4
			 0x77323b0d, /// 0xba8
			 0x71578e4e, /// 0xbac
			 0xc96565b6, /// 0xbb0
			 0xfe2f0314, /// 0xbb4
			 0xc0c59ec8, /// 0xbb8
			 0x75b310a8, /// 0xbbc
			 0x5275dfa8, /// 0xbc0
			 0x664082db, /// 0xbc4
			 0xba374807, /// 0xbc8
			 0xbc6bf0c1, /// 0xbcc
			 0xe70ce289, /// 0xbd0
			 0xefd83b3f, /// 0xbd4
			 0x8faaf429, /// 0xbd8
			 0xa6774a6c, /// 0xbdc
			 0x1a35c15c, /// 0xbe0
			 0x6c613402, /// 0xbe4
			 0xc511823a, /// 0xbe8
			 0xecbf570b, /// 0xbec
			 0xf5e8fa87, /// 0xbf0
			 0x48744157, /// 0xbf4
			 0xadf07dd8, /// 0xbf8
			 0x2630bb24, /// 0xbfc
			 0x24e2a1af, /// 0xc00
			 0x2953ccc2, /// 0xc04
			 0xb483330d, /// 0xc08
			 0xd98f7883, /// 0xc0c
			 0x7fd24b4f, /// 0xc10
			 0xa79c7427, /// 0xc14
			 0x61857752, /// 0xc18
			 0xa91a834a, /// 0xc1c
			 0x4986eb6a, /// 0xc20
			 0x9514f2da, /// 0xc24
			 0x8c1e39b2, /// 0xc28
			 0xa6b4303a, /// 0xc2c
			 0xdf251506, /// 0xc30
			 0x941a41f8, /// 0xc34
			 0xeb6fbd6c, /// 0xc38
			 0xd40f7c9d, /// 0xc3c
			 0xa8723c18, /// 0xc40
			 0xb5ac8978, /// 0xc44
			 0xbfafa7de, /// 0xc48
			 0xa2f3824c, /// 0xc4c
			 0xa8288c3f, /// 0xc50
			 0xfd0e4420, /// 0xc54
			 0x0e461407, /// 0xc58
			 0x427c5c2b, /// 0xc5c
			 0x87133fce, /// 0xc60
			 0xdceb4dfb, /// 0xc64
			 0xbb00d0e8, /// 0xc68
			 0x4ee26e99, /// 0xc6c
			 0x0ea0cb2a, /// 0xc70
			 0xc3fd4594, /// 0xc74
			 0xdecaeb86, /// 0xc78
			 0xad13f7e1, /// 0xc7c
			 0x5e297e5e, /// 0xc80
			 0x35154d65, /// 0xc84
			 0xe599d48f, /// 0xc88
			 0xfdcf7164, /// 0xc8c
			 0xbc26c8ba, /// 0xc90
			 0xaacb4339, /// 0xc94
			 0x8e1922d5, /// 0xc98
			 0xc3a2d699, /// 0xc9c
			 0x9e6b0f77, /// 0xca0
			 0xe14bf404, /// 0xca4
			 0x744c018c, /// 0xca8
			 0xf9cda457, /// 0xcac
			 0xcaa73133, /// 0xcb0
			 0x86d3a875, /// 0xcb4
			 0xd7ec5c71, /// 0xcb8
			 0x5e0526ff, /// 0xcbc
			 0x356cdec0, /// 0xcc0
			 0x062d8801, /// 0xcc4
			 0x1a1b659c, /// 0xcc8
			 0x1a1803d3, /// 0xccc
			 0xf5defcb2, /// 0xcd0
			 0x0440a81f, /// 0xcd4
			 0x053ff153, /// 0xcd8
			 0x3a469933, /// 0xcdc
			 0x1c7f9c8d, /// 0xce0
			 0xcd3cde9b, /// 0xce4
			 0x5948aba7, /// 0xce8
			 0xb29af2d3, /// 0xcec
			 0x364067ab, /// 0xcf0
			 0x78c9f519, /// 0xcf4
			 0xfdd98ab0, /// 0xcf8
			 0xc2822222, /// 0xcfc
			 0x72127451, /// 0xd00
			 0xcb27916a, /// 0xd04
			 0x3172e43c, /// 0xd08
			 0xe9424381, /// 0xd0c
			 0x8563df1b, /// 0xd10
			 0xec167336, /// 0xd14
			 0xe39739f4, /// 0xd18
			 0xf2359379, /// 0xd1c
			 0x1d296854, /// 0xd20
			 0xb835eb2c, /// 0xd24
			 0x6ba251f0, /// 0xd28
			 0x7d631c57, /// 0xd2c
			 0x90f7ae4d, /// 0xd30
			 0x7a02dc56, /// 0xd34
			 0x9232ce55, /// 0xd38
			 0xe24d834b, /// 0xd3c
			 0x7ed2b352, /// 0xd40
			 0xa8d65723, /// 0xd44
			 0x11b797e4, /// 0xd48
			 0x136bdbc7, /// 0xd4c
			 0x8c2164af, /// 0xd50
			 0xdfdb8809, /// 0xd54
			 0x7035cb9f, /// 0xd58
			 0x973f782f, /// 0xd5c
			 0x4c55c061, /// 0xd60
			 0x955406c0, /// 0xd64
			 0xd2f2b4ac, /// 0xd68
			 0x3ebac134, /// 0xd6c
			 0xd6833c8f, /// 0xd70
			 0x4a933ac4, /// 0xd74
			 0x0a9a11de, /// 0xd78
			 0x60d42dc0, /// 0xd7c
			 0x656b2a87, /// 0xd80
			 0xd7f753a1, /// 0xd84
			 0x1361e854, /// 0xd88
			 0x7fc16a77, /// 0xd8c
			 0x5d213e65, /// 0xd90
			 0xfefb8fb4, /// 0xd94
			 0x3a36e5fc, /// 0xd98
			 0xce5c060c, /// 0xd9c
			 0x3d39739f, /// 0xda0
			 0x8fbc73a7, /// 0xda4
			 0x1337ed26, /// 0xda8
			 0xbfd01e9e, /// 0xdac
			 0x8ef7e1ea, /// 0xdb0
			 0x2a8d85cc, /// 0xdb4
			 0x56903feb, /// 0xdb8
			 0xf8e6f953, /// 0xdbc
			 0x760f2f9d, /// 0xdc0
			 0x1bbdd4c1, /// 0xdc4
			 0xc9336fde, /// 0xdc8
			 0x3cb2e832, /// 0xdcc
			 0xbb725950, /// 0xdd0
			 0xc5c8744f, /// 0xdd4
			 0x6a93df64, /// 0xdd8
			 0x2ee5fca4, /// 0xddc
			 0x13538f44, /// 0xde0
			 0xff490ae9, /// 0xde4
			 0xdb37328b, /// 0xde8
			 0x68881c27, /// 0xdec
			 0x49645f09, /// 0xdf0
			 0x686a67c6, /// 0xdf4
			 0xe12a561a, /// 0xdf8
			 0xb9a0f5e1, /// 0xdfc
			 0xbf199789, /// 0xe00
			 0xe28850db, /// 0xe04
			 0xee031587, /// 0xe08
			 0x28f98320, /// 0xe0c
			 0xcb4b13cc, /// 0xe10
			 0x4ca6efa9, /// 0xe14
			 0xdbd0c658, /// 0xe18
			 0x01096aca, /// 0xe1c
			 0xd6a0f627, /// 0xe20
			 0x11077d02, /// 0xe24
			 0x332f276c, /// 0xe28
			 0x774af3b8, /// 0xe2c
			 0xeac35872, /// 0xe30
			 0xc5b39fa5, /// 0xe34
			 0x8d513979, /// 0xe38
			 0x093abc75, /// 0xe3c
			 0xf5490fb3, /// 0xe40
			 0x3baa2425, /// 0xe44
			 0x0ca554cf, /// 0xe48
			 0x5aac5096, /// 0xe4c
			 0x06504a35, /// 0xe50
			 0x51ec3c3e, /// 0xe54
			 0xca932f0e, /// 0xe58
			 0x0e8b76ed, /// 0xe5c
			 0xd3eb689f, /// 0xe60
			 0xfbf2a38a, /// 0xe64
			 0x2dbdc01d, /// 0xe68
			 0x9a0d61cc, /// 0xe6c
			 0x931e425f, /// 0xe70
			 0x45a30992, /// 0xe74
			 0x8b7852a5, /// 0xe78
			 0x0a75ebbb, /// 0xe7c
			 0xee98b3c3, /// 0xe80
			 0xd43b6c19, /// 0xe84
			 0xd2116c65, /// 0xe88
			 0x7b85f44a, /// 0xe8c
			 0x4cc8c522, /// 0xe90
			 0xddeda013, /// 0xe94
			 0xd827b4c1, /// 0xe98
			 0x2690346f, /// 0xe9c
			 0x44b576eb, /// 0xea0
			 0xa23b4ba6, /// 0xea4
			 0xa6e81f38, /// 0xea8
			 0x0c1cbdd6, /// 0xeac
			 0x02535974, /// 0xeb0
			 0x2d46eb1e, /// 0xeb4
			 0xcadf1565, /// 0xeb8
			 0x5484a097, /// 0xebc
			 0x28f25007, /// 0xec0
			 0x95c421bb, /// 0xec4
			 0x1c344fdf, /// 0xec8
			 0x9f5df148, /// 0xecc
			 0xa48b6177, /// 0xed0
			 0xf1c8d1a4, /// 0xed4
			 0x9aab6ed0, /// 0xed8
			 0x0428de14, /// 0xedc
			 0x9c9ce8a4, /// 0xee0
			 0xd309165a, /// 0xee4
			 0xb08d1f7b, /// 0xee8
			 0x1ff69b3a, /// 0xeec
			 0x5dbc9290, /// 0xef0
			 0x92eb1892, /// 0xef4
			 0x983341e9, /// 0xef8
			 0x2066bb30, /// 0xefc
			 0x7dc12e2e, /// 0xf00
			 0x8f6b6168, /// 0xf04
			 0xf666edc9, /// 0xf08
			 0x08be0538, /// 0xf0c
			 0x25764661, /// 0xf10
			 0x8a6ffccd, /// 0xf14
			 0x51aba4e6, /// 0xf18
			 0xb5885875, /// 0xf1c
			 0xce643589, /// 0xf20
			 0x5739a371, /// 0xf24
			 0xfc690dc3, /// 0xf28
			 0xae0835bf, /// 0xf2c
			 0x68a8f54e, /// 0xf30
			 0xda58c958, /// 0xf34
			 0xe0822249, /// 0xf38
			 0xa8502efa, /// 0xf3c
			 0x279a4ec0, /// 0xf40
			 0xf92676c3, /// 0xf44
			 0x4f274a3d, /// 0xf48
			 0x0a33af5d, /// 0xf4c
			 0x46b9914e, /// 0xf50
			 0x8509ccc1, /// 0xf54
			 0xba5ee210, /// 0xf58
			 0xf5a0aee8, /// 0xf5c
			 0x42a76c17, /// 0xf60
			 0xde5bfae8, /// 0xf64
			 0x3633bd20, /// 0xf68
			 0x6e010734, /// 0xf6c
			 0x8529e581, /// 0xf70
			 0x797a4bfd, /// 0xf74
			 0x039fd102, /// 0xf78
			 0xafdca526, /// 0xf7c
			 0xcd65239e, /// 0xf80
			 0xa9836ad9, /// 0xf84
			 0x40ca7efd, /// 0xf88
			 0x37368e69, /// 0xf8c
			 0x29d5706f, /// 0xf90
			 0x27e101af, /// 0xf94
			 0x9f8f6099, /// 0xf98
			 0x79700096, /// 0xf9c
			 0x1aa5fee2, /// 0xfa0
			 0x32cc7e76, /// 0xfa4
			 0xa75aa687, /// 0xfa8
			 0xe1557709, /// 0xfac
			 0xbadc86c1, /// 0xfb0
			 0x468419b2, /// 0xfb4
			 0xb94b8b90, /// 0xfb8
			 0xc1ab0b4e, /// 0xfbc
			 0xf9ad8887, /// 0xfc0
			 0xb25cf518, /// 0xfc4
			 0x5cc52d76, /// 0xfc8
			 0x9118158d, /// 0xfcc
			 0x5195a71d, /// 0xfd0
			 0x3863405c, /// 0xfd4
			 0x6571cdc8, /// 0xfd8
			 0x21d35ac1, /// 0xfdc
			 0x7e892769, /// 0xfe0
			 0xbabe2f53, /// 0xfe4
			 0x4128f851, /// 0xfe8
			 0x5e58a7e0, /// 0xfec
			 0x31872aac, /// 0xff0
			 0xd6637ce6, /// 0xff4
			 0x38345285, /// 0xff8
			 0xd0990da0 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00000
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00004
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00008
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00010
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00014
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0001c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x7f800000,                                                  // inf                                         /// 0002c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00038
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0003c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00040
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0004c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00054
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00058
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0005c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00060
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00068
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0006c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00070
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00074
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00078
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0007c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00080
			 0x0e000001,                                                  // Trailing 1s:                                /// 00084
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00088
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00090
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00094
			 0x0c780000,                                                  // Leading 1s:                                 /// 00098
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c4
			 0x00000000,                                                  // zero                                        /// 000c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f0
			 0xcb000000,                                                  // -8388608.0                                  /// 000f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00100
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00104
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00110
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00118
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00124
			 0x0e000007,                                                  // Trailing 1s:                                /// 00128
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0012c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00134
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00138
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x33333333,                                                  // 4 random values                             /// 00148
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00150
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xcb000000,                                                  // -8388608.0                                  /// 00160
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00164
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00168
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0016c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00178
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00180
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00188
			 0xbf028f5c,                                                  // -0.51                                       /// 0018c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00198
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0019c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001a4
			 0xffc00001,                                                  // -signaling NaN                              /// 001a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x4b000000,                                                  // 8388608.0                                   /// 001c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00200
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00204
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00208
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0020c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00210
			 0x0c700000,                                                  // Leading 1s:                                 /// 00214
			 0x0c780000,                                                  // Leading 1s:                                 /// 00218
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0021c
			 0x4b000000,                                                  // 8388608.0                                   /// 00220
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00224
			 0xff800000,                                                  // -inf                                        /// 00228
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0022c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00230
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00234
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00238
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0xbf028f5c,                                                  // -0.51                                       /// 00244
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00248
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0024c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00264
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00268
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0026c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00270
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00274
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0027c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00284
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x7f800000,                                                  // inf                                         /// 00298
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0029c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e0
			 0xcb000000,                                                  // -8388608.0                                  /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00304
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00308
			 0x80011111,                                                  // -9.7958E-41                                 /// 0030c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00310
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00318
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00011111,                                                  // 9.7958E-41                                  /// 00324
			 0xff800000,                                                  // -inf                                        /// 00328
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00330
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00334
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00338
			 0x7f800000,                                                  // inf                                         /// 0033c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00340
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00348
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00350
			 0x00000000,                                                  // zero                                        /// 00354
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00358
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00364
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00368
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00370
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00374
			 0x80000000,                                                  // -zero                                       /// 00378
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00380
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00384
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00388
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0038c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00390
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00394
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00398
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0039c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00400
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00404
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00408
			 0x7fc00001,                                                  // signaling NaN                               /// 0040c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00410
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00414
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00424
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00428
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0042c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00430
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00438
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0043c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00444
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00448
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00450
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00454
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00468
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00478
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0047c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00480
			 0x4b000000,                                                  // 8388608.0                                   /// 00484
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00488
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00490
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e000003,                                                  // Trailing 1s:                                /// 0049c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004dc
			 0x00000000,                                                  // zero                                        /// 004e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00500
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00504
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00508
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00510
			 0x0c400000,                                                  // Leading 1s:                                 /// 00514
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00520
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00530
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00538
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0053c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00540
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00544
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00548
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0054c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00550
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00554
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00558
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0055c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00560
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00568
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00574
			 0x0c700000,                                                  // Leading 1s:                                 /// 00578
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0057c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00588
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00590
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x33333333,                                                  // 4 random values                             /// 00600
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00608
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00610
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00614
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00618
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00620
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0xcb000000,                                                  // -8388608.0                                  /// 00628
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00634
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00638
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00640
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00648
			 0x80000000,                                                  // -zero                                       /// 0064c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00650
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00654
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0065c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00660
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80011111,                                                  // -9.7958E-41                                 /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00674
			 0x0c400000,                                                  // Leading 1s:                                 /// 00678
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c780000,                                                  // Leading 1s:                                 /// 00688
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00690
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00694
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0069c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xcb000000,                                                  // -8388608.0                                  /// 006b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006cc
			 0x55555555,                                                  // 4 random values                             /// 006d0
			 0x80000000,                                                  // -zero                                       /// 006d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006d8
			 0xbf028f5c,                                                  // -0.51                                       /// 006dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 006e0
			 0x80000000,                                                  // -zero                                       /// 006e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00700
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00704
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0070c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00710
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00718
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0e000003,                                                  // Trailing 1s:                                /// 00728
			 0xbf028f5c,                                                  // -0.51                                       /// 0072c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00738
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x7f800000,                                                  // inf                                         /// 00740
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00744
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00748
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0074c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00750
			 0xff800000,                                                  // -inf                                        /// 00754
			 0x7f800000,                                                  // inf                                         /// 00758
			 0x0c780000,                                                  // Leading 1s:                                 /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00768
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0076c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00770
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0077c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00780
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0078c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00798
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0079c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007a4
			 0xbf028f5c,                                                  // -0.51                                       /// 007a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b0
			 0x33333333,                                                  // 4 random values                             /// 007b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c8
			 0xffc00001,                                                  // -signaling NaN                              /// 007cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xff800000,                                                  // -inf                                        /// 007ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00804
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00808
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00814
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00818
			 0x0e000001,                                                  // Trailing 1s:                                /// 0081c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00820
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00824
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00828
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0082c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00830
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00834
			 0x80011111,                                                  // -9.7958E-41                                 /// 00838
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0083c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00840
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00848
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00850
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00858
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00870
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00874
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0087c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0088c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00890
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0089c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00900
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00904
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00908
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0090c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00914
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0091c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00920
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00930
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00934
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00938
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0093c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00944
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00948
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0094c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00950
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0095c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00960
			 0x3f028f5c,                                                  // 0.51                                        /// 00964
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00968
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0096c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00970
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xcb000000,                                                  // -8388608.0                                  /// 00978
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00988
			 0x33333333,                                                  // 4 random values                             /// 0098c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00990
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00998
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0099c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00a00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a18
			 0x33333333,                                                  // 4 random values                             /// 00a1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aa4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00abc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00acc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ad0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ad8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00adc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00afc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b30
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b58
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b88
			 0x3f028f5c,                                                  // 0.51                                        /// 00b8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ba4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be0
			 0xbf028f5c,                                                  // -0.51                                       /// 00be4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bf8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x7f800000,                                                  // inf                                         /// 00ca4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cc0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ccc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ce0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cf4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x33333333,                                                  // 4 random values                             /// 00d64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00db0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ddc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00de0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00de4
			 0x80000000,                                                  // -zero                                       /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0xffc00001,                                                  // -signaling NaN                              /// 00df8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ebc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ec0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00edc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ef4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x4b000000,                                                  // 8388608.0                                   /// 00f18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f58
			 0xbf028f5c,                                                  // -0.51                                       /// 00f5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x7f800000,                                                  // inf                                         /// 00fb4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fbc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd0
			 0x33333333,                                                  // 4 random values                             /// 00fd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ff0
			 0x55555555,                                                  // 4 random values                             /// 00ff4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x0e001fff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7589a8e2, /// 0x0
			 0x5d444986, /// 0x4
			 0x9324ade9, /// 0x8
			 0xc3f6f464, /// 0xc
			 0xaf943049, /// 0x10
			 0x2426d994, /// 0x14
			 0x8453ab63, /// 0x18
			 0x88cd31fc, /// 0x1c
			 0x58b1e120, /// 0x20
			 0x02d0d580, /// 0x24
			 0x2f53a241, /// 0x28
			 0x8c4d46ad, /// 0x2c
			 0xa18af303, /// 0x30
			 0xafc08ce0, /// 0x34
			 0x2494133c, /// 0x38
			 0x73fa00c9, /// 0x3c
			 0x46599532, /// 0x40
			 0xff88dbaf, /// 0x44
			 0xcbe42854, /// 0x48
			 0x4a78eea3, /// 0x4c
			 0xc31d27dd, /// 0x50
			 0x39d86a8d, /// 0x54
			 0x8d745961, /// 0x58
			 0x40c04855, /// 0x5c
			 0xd8d04b2d, /// 0x60
			 0xa81f1bda, /// 0x64
			 0x409ce680, /// 0x68
			 0xbf0ca5e9, /// 0x6c
			 0xafe9bba6, /// 0x70
			 0x9a00e744, /// 0x74
			 0x7277d730, /// 0x78
			 0x449dd2f4, /// 0x7c
			 0xbd673b9a, /// 0x80
			 0x77116427, /// 0x84
			 0x60c2de5b, /// 0x88
			 0xaa79d25d, /// 0x8c
			 0x7fdde2b4, /// 0x90
			 0x3172b430, /// 0x94
			 0x1025f03f, /// 0x98
			 0x5550dee8, /// 0x9c
			 0xd1aecb67, /// 0xa0
			 0xb6ffff6f, /// 0xa4
			 0x72215f5f, /// 0xa8
			 0x34013a7b, /// 0xac
			 0x7e1fd6bf, /// 0xb0
			 0x55493ce4, /// 0xb4
			 0x9ee4bc85, /// 0xb8
			 0x37cb359f, /// 0xbc
			 0x8bad9c42, /// 0xc0
			 0x2a2b0cc0, /// 0xc4
			 0x499bfde7, /// 0xc8
			 0x5795a169, /// 0xcc
			 0xc465b0f6, /// 0xd0
			 0xb32ba692, /// 0xd4
			 0xa758a78d, /// 0xd8
			 0x3b26ea47, /// 0xdc
			 0x0e2caee4, /// 0xe0
			 0x984ceef8, /// 0xe4
			 0x8b385964, /// 0xe8
			 0xb80b5e6c, /// 0xec
			 0x118e1655, /// 0xf0
			 0x4a2f5175, /// 0xf4
			 0x2a09aa96, /// 0xf8
			 0xd21fb589, /// 0xfc
			 0xb3745d68, /// 0x100
			 0xd33a137f, /// 0x104
			 0x2f38cb61, /// 0x108
			 0xb711c1d5, /// 0x10c
			 0xf08fbc32, /// 0x110
			 0x68d4164d, /// 0x114
			 0x24951b57, /// 0x118
			 0x5e1c36b8, /// 0x11c
			 0x852a77bc, /// 0x120
			 0xb3b3a517, /// 0x124
			 0x839078d7, /// 0x128
			 0x5e437f47, /// 0x12c
			 0xa8527bc7, /// 0x130
			 0x49bffb3e, /// 0x134
			 0xfb14ce05, /// 0x138
			 0x872ccd52, /// 0x13c
			 0x4f2e8669, /// 0x140
			 0x8636430c, /// 0x144
			 0xff0c421c, /// 0x148
			 0xd37409dc, /// 0x14c
			 0x4702769e, /// 0x150
			 0x4c2c115b, /// 0x154
			 0xdde24e5f, /// 0x158
			 0xe0aba686, /// 0x15c
			 0x5e1e8dc8, /// 0x160
			 0x3cd0de20, /// 0x164
			 0x684adfcb, /// 0x168
			 0x2fab6c05, /// 0x16c
			 0xe284ed3a, /// 0x170
			 0x11f3fa89, /// 0x174
			 0xdec26c68, /// 0x178
			 0x08143720, /// 0x17c
			 0xbb253aa4, /// 0x180
			 0xc7f8fb47, /// 0x184
			 0xb655ee6e, /// 0x188
			 0x04194928, /// 0x18c
			 0xd199ddde, /// 0x190
			 0xf3031ff1, /// 0x194
			 0x25c0eb2e, /// 0x198
			 0xb11d904e, /// 0x19c
			 0xbbae41a8, /// 0x1a0
			 0x53e74272, /// 0x1a4
			 0xe9da728c, /// 0x1a8
			 0xcf1922cf, /// 0x1ac
			 0xbee79427, /// 0x1b0
			 0x227083bd, /// 0x1b4
			 0x4dbaae54, /// 0x1b8
			 0xb47f20db, /// 0x1bc
			 0x616d2588, /// 0x1c0
			 0xc009d206, /// 0x1c4
			 0x10f95734, /// 0x1c8
			 0x54dc022d, /// 0x1cc
			 0xc5f43713, /// 0x1d0
			 0x16ca2cbc, /// 0x1d4
			 0x94212e9a, /// 0x1d8
			 0x9b68174b, /// 0x1dc
			 0x56704db9, /// 0x1e0
			 0x37cefca0, /// 0x1e4
			 0x6975bbae, /// 0x1e8
			 0xcfb226fa, /// 0x1ec
			 0x0b52695d, /// 0x1f0
			 0x25633b1d, /// 0x1f4
			 0x9cc54894, /// 0x1f8
			 0x600502bc, /// 0x1fc
			 0xee32f221, /// 0x200
			 0xf518c216, /// 0x204
			 0xabae1dce, /// 0x208
			 0x6d38aba0, /// 0x20c
			 0x495c67fb, /// 0x210
			 0xb4bc0548, /// 0x214
			 0x6b2b9410, /// 0x218
			 0x3479a661, /// 0x21c
			 0x531b403a, /// 0x220
			 0x963843db, /// 0x224
			 0x016d39a2, /// 0x228
			 0x1ab8d954, /// 0x22c
			 0x6f8b57ea, /// 0x230
			 0x6d7f1484, /// 0x234
			 0x07d22ddc, /// 0x238
			 0x152fd60d, /// 0x23c
			 0x8885b3e2, /// 0x240
			 0xce3fde4a, /// 0x244
			 0x35488513, /// 0x248
			 0x3721a497, /// 0x24c
			 0x590761c3, /// 0x250
			 0xd5fedd79, /// 0x254
			 0x4127fda7, /// 0x258
			 0xcf8f71d4, /// 0x25c
			 0xef94b8ae, /// 0x260
			 0x7837a3b6, /// 0x264
			 0xaa2658a0, /// 0x268
			 0xb755be83, /// 0x26c
			 0xaa16b514, /// 0x270
			 0xa3c68fe0, /// 0x274
			 0x3eed1e18, /// 0x278
			 0x0b6c4e6c, /// 0x27c
			 0x65151a53, /// 0x280
			 0xd8e0796c, /// 0x284
			 0x5cacafd7, /// 0x288
			 0xa71a4ffa, /// 0x28c
			 0xd7f8e93b, /// 0x290
			 0xd068e633, /// 0x294
			 0x25974635, /// 0x298
			 0xe74f325c, /// 0x29c
			 0x9800cb19, /// 0x2a0
			 0x340b85b8, /// 0x2a4
			 0x49a616b9, /// 0x2a8
			 0x86dd97a8, /// 0x2ac
			 0xdbca8f24, /// 0x2b0
			 0x23b44805, /// 0x2b4
			 0x8336cd24, /// 0x2b8
			 0x61bdaedf, /// 0x2bc
			 0xd5641dce, /// 0x2c0
			 0x89b70239, /// 0x2c4
			 0x76692b33, /// 0x2c8
			 0xc9e45739, /// 0x2cc
			 0x7199f2da, /// 0x2d0
			 0x6337fb80, /// 0x2d4
			 0x8e00fc98, /// 0x2d8
			 0x8af06fc4, /// 0x2dc
			 0xc11be5f7, /// 0x2e0
			 0x0fce4a85, /// 0x2e4
			 0xa820e8cd, /// 0x2e8
			 0xc39c5a5d, /// 0x2ec
			 0xf2beed8a, /// 0x2f0
			 0xa9f1fab3, /// 0x2f4
			 0x21e3f77e, /// 0x2f8
			 0x872f30b1, /// 0x2fc
			 0xfb45cb70, /// 0x300
			 0x775d751b, /// 0x304
			 0xba6cb00e, /// 0x308
			 0x68bf5ca7, /// 0x30c
			 0xea7c036b, /// 0x310
			 0xb87c2be8, /// 0x314
			 0xfa40fb08, /// 0x318
			 0x27923ed3, /// 0x31c
			 0x76e9d85e, /// 0x320
			 0xdfa67bda, /// 0x324
			 0x60c80b73, /// 0x328
			 0x8535ae81, /// 0x32c
			 0x7e60e1c7, /// 0x330
			 0x6be6e7aa, /// 0x334
			 0x6192f92b, /// 0x338
			 0xfb253109, /// 0x33c
			 0x80423e2e, /// 0x340
			 0xcd28fe5e, /// 0x344
			 0x26b08c3e, /// 0x348
			 0x64af4abc, /// 0x34c
			 0x32d44810, /// 0x350
			 0x699ddc02, /// 0x354
			 0x2d563a00, /// 0x358
			 0x71a344ab, /// 0x35c
			 0xd018f070, /// 0x360
			 0xde44cd28, /// 0x364
			 0x06ca47fd, /// 0x368
			 0xd3b4fda5, /// 0x36c
			 0x5aa087b7, /// 0x370
			 0x52941cb1, /// 0x374
			 0xbe517379, /// 0x378
			 0xa99149cc, /// 0x37c
			 0x0228e930, /// 0x380
			 0x04b96aa0, /// 0x384
			 0x970a7171, /// 0x388
			 0x16059ef8, /// 0x38c
			 0x5a2b3d17, /// 0x390
			 0xf24bb3da, /// 0x394
			 0x702b1b05, /// 0x398
			 0xc0389967, /// 0x39c
			 0x6fcb8da7, /// 0x3a0
			 0x3c62d52d, /// 0x3a4
			 0x9e1e90e6, /// 0x3a8
			 0x5552f63d, /// 0x3ac
			 0x3bb6499d, /// 0x3b0
			 0x3e617084, /// 0x3b4
			 0xb41c4632, /// 0x3b8
			 0x9311724c, /// 0x3bc
			 0x8a7f4524, /// 0x3c0
			 0x82b8309e, /// 0x3c4
			 0x5ab8cb53, /// 0x3c8
			 0x02f456c8, /// 0x3cc
			 0xb746aa11, /// 0x3d0
			 0x4cd9334c, /// 0x3d4
			 0x8d8fbb29, /// 0x3d8
			 0xf226d61b, /// 0x3dc
			 0x13252999, /// 0x3e0
			 0x8069933c, /// 0x3e4
			 0x5982dd3b, /// 0x3e8
			 0xa32c17af, /// 0x3ec
			 0xd5d56e07, /// 0x3f0
			 0x0e34d2aa, /// 0x3f4
			 0x85596697, /// 0x3f8
			 0x5943d1fa, /// 0x3fc
			 0xb00d3fa4, /// 0x400
			 0x16055f84, /// 0x404
			 0x072518df, /// 0x408
			 0xb65cbfb4, /// 0x40c
			 0xc37c8bdf, /// 0x410
			 0x53120601, /// 0x414
			 0x9202d402, /// 0x418
			 0x1bc9115d, /// 0x41c
			 0xcdc173e0, /// 0x420
			 0x9d91d8c8, /// 0x424
			 0x58e493be, /// 0x428
			 0x205b7302, /// 0x42c
			 0xdf0bfc7c, /// 0x430
			 0xfcb6c02a, /// 0x434
			 0x7582d068, /// 0x438
			 0x8cac9329, /// 0x43c
			 0xc9fbaa63, /// 0x440
			 0x36f161ca, /// 0x444
			 0x44d65ddf, /// 0x448
			 0xafea9f6f, /// 0x44c
			 0x803e4f11, /// 0x450
			 0x0fd16f51, /// 0x454
			 0xcb1a90e1, /// 0x458
			 0xc5ac7b7a, /// 0x45c
			 0xb3b1c406, /// 0x460
			 0x861ce535, /// 0x464
			 0x8c004e5c, /// 0x468
			 0xdc609835, /// 0x46c
			 0x12bab4e9, /// 0x470
			 0x11a4597e, /// 0x474
			 0xb9ca91b8, /// 0x478
			 0xaab9e7e0, /// 0x47c
			 0xf4a1de3c, /// 0x480
			 0xf373b038, /// 0x484
			 0x963cf4ed, /// 0x488
			 0x4afbf57f, /// 0x48c
			 0x697179fd, /// 0x490
			 0xe1164113, /// 0x494
			 0x705ece92, /// 0x498
			 0x57eb08f8, /// 0x49c
			 0xe364b635, /// 0x4a0
			 0xc7dcec77, /// 0x4a4
			 0x6fa0caf9, /// 0x4a8
			 0x7e9ff109, /// 0x4ac
			 0x0ea50db7, /// 0x4b0
			 0x5965443f, /// 0x4b4
			 0x16cd2e3f, /// 0x4b8
			 0x071e91cb, /// 0x4bc
			 0xb5423f19, /// 0x4c0
			 0x331dedac, /// 0x4c4
			 0x572f87c1, /// 0x4c8
			 0x87d456f7, /// 0x4cc
			 0xb47c3965, /// 0x4d0
			 0x357441f4, /// 0x4d4
			 0x3ece9f80, /// 0x4d8
			 0x097c8b1d, /// 0x4dc
			 0xcb6cfa24, /// 0x4e0
			 0x39391ca4, /// 0x4e4
			 0x96eb560b, /// 0x4e8
			 0x684615c2, /// 0x4ec
			 0x0d1c35a5, /// 0x4f0
			 0xc7b27094, /// 0x4f4
			 0x6feb49d2, /// 0x4f8
			 0x83f58438, /// 0x4fc
			 0xc1a9eb43, /// 0x500
			 0x92b9aa11, /// 0x504
			 0xafa319ec, /// 0x508
			 0x66e3f7d4, /// 0x50c
			 0x9d98fa6d, /// 0x510
			 0x5c823fa4, /// 0x514
			 0x6a629298, /// 0x518
			 0xf6af984e, /// 0x51c
			 0x099be757, /// 0x520
			 0x712cc867, /// 0x524
			 0x144a1162, /// 0x528
			 0xa29630a6, /// 0x52c
			 0xcaf015a7, /// 0x530
			 0xee408dcf, /// 0x534
			 0xc91e673b, /// 0x538
			 0xb6d62fbc, /// 0x53c
			 0x5fb5339f, /// 0x540
			 0x3b8eecd0, /// 0x544
			 0x8fb7cb9e, /// 0x548
			 0x362ad8e8, /// 0x54c
			 0x5f3fde71, /// 0x550
			 0x00b9342a, /// 0x554
			 0xbd9bf8aa, /// 0x558
			 0x3d29f12c, /// 0x55c
			 0x7714250e, /// 0x560
			 0xd1fc0d12, /// 0x564
			 0xd4416e2d, /// 0x568
			 0xe749a8bd, /// 0x56c
			 0x9b4c36f9, /// 0x570
			 0x2dae076f, /// 0x574
			 0x3d104c78, /// 0x578
			 0x4d2395c4, /// 0x57c
			 0x6878165e, /// 0x580
			 0x6fcdc2f3, /// 0x584
			 0x760db439, /// 0x588
			 0x2e6e8159, /// 0x58c
			 0xafe087a8, /// 0x590
			 0x3a5da145, /// 0x594
			 0xbbd4075a, /// 0x598
			 0x4d1e2c8c, /// 0x59c
			 0x52d19704, /// 0x5a0
			 0xdacbb1be, /// 0x5a4
			 0xea049de3, /// 0x5a8
			 0x168d0184, /// 0x5ac
			 0x7c630d5d, /// 0x5b0
			 0x50c9c778, /// 0x5b4
			 0x430cdfc6, /// 0x5b8
			 0x692e65ba, /// 0x5bc
			 0xbe159477, /// 0x5c0
			 0x5c66886a, /// 0x5c4
			 0x2d7d8730, /// 0x5c8
			 0xadf6a822, /// 0x5cc
			 0x8461c2a4, /// 0x5d0
			 0x0166cd7f, /// 0x5d4
			 0xab51de1b, /// 0x5d8
			 0x5e95a375, /// 0x5dc
			 0x5f25c32f, /// 0x5e0
			 0x5b83a88b, /// 0x5e4
			 0xc3a4b271, /// 0x5e8
			 0x296b6976, /// 0x5ec
			 0x9c9991c8, /// 0x5f0
			 0xec171957, /// 0x5f4
			 0xba04f696, /// 0x5f8
			 0xdc3ac643, /// 0x5fc
			 0x8b96a735, /// 0x600
			 0xba9109c3, /// 0x604
			 0xb253287c, /// 0x608
			 0x1c3f5eaa, /// 0x60c
			 0xdac05900, /// 0x610
			 0x6c8c084f, /// 0x614
			 0x95b97446, /// 0x618
			 0x1242e461, /// 0x61c
			 0x3b902f2c, /// 0x620
			 0x5403ff72, /// 0x624
			 0x68e95d16, /// 0x628
			 0xc0e8beb8, /// 0x62c
			 0x5240518d, /// 0x630
			 0x6196a8dc, /// 0x634
			 0x8d6f9211, /// 0x638
			 0xb879b531, /// 0x63c
			 0xab3590aa, /// 0x640
			 0x564bd009, /// 0x644
			 0x2684a9f7, /// 0x648
			 0x55df51df, /// 0x64c
			 0x4b79a4ac, /// 0x650
			 0x3652d7c7, /// 0x654
			 0x48f5c597, /// 0x658
			 0xd85a6228, /// 0x65c
			 0xe44b9fef, /// 0x660
			 0x9d5c8241, /// 0x664
			 0xe37709c9, /// 0x668
			 0xaac1a729, /// 0x66c
			 0x3ff3d607, /// 0x670
			 0x0dc38475, /// 0x674
			 0x6debebff, /// 0x678
			 0xa28fa138, /// 0x67c
			 0xff1dfe8a, /// 0x680
			 0xcc14648a, /// 0x684
			 0x9263f521, /// 0x688
			 0x1d281c4a, /// 0x68c
			 0xdaa26c7a, /// 0x690
			 0x5a5eec15, /// 0x694
			 0x35a1ebd5, /// 0x698
			 0x0a0e3bce, /// 0x69c
			 0x2beccebb, /// 0x6a0
			 0xfc3b785c, /// 0x6a4
			 0xc937a80f, /// 0x6a8
			 0x6344d3cc, /// 0x6ac
			 0x26e4dab9, /// 0x6b0
			 0x9788c080, /// 0x6b4
			 0x655fb913, /// 0x6b8
			 0xfc98ff4d, /// 0x6bc
			 0x66bb8c06, /// 0x6c0
			 0x1efb5db9, /// 0x6c4
			 0xbf6de13b, /// 0x6c8
			 0x48addeda, /// 0x6cc
			 0x5b4270c6, /// 0x6d0
			 0x4883a7fb, /// 0x6d4
			 0xeca49194, /// 0x6d8
			 0xdb510609, /// 0x6dc
			 0x55b03bd0, /// 0x6e0
			 0x6f1b1c1c, /// 0x6e4
			 0xcade2899, /// 0x6e8
			 0xc5ee1242, /// 0x6ec
			 0x9ca74439, /// 0x6f0
			 0xbc7d6e3e, /// 0x6f4
			 0xbdfe8ea3, /// 0x6f8
			 0xd84c2751, /// 0x6fc
			 0xa5bc9b96, /// 0x700
			 0x3265dc20, /// 0x704
			 0x856285f2, /// 0x708
			 0x281df7c7, /// 0x70c
			 0xee8facb7, /// 0x710
			 0x2a0259a2, /// 0x714
			 0xcbc10c11, /// 0x718
			 0xf10231d8, /// 0x71c
			 0x4b4761c9, /// 0x720
			 0x04963417, /// 0x724
			 0xcdbe4cc9, /// 0x728
			 0xaa6336ca, /// 0x72c
			 0x2fa33b86, /// 0x730
			 0x84908465, /// 0x734
			 0x526f60e2, /// 0x738
			 0x2792edf6, /// 0x73c
			 0xb32f80d7, /// 0x740
			 0xfa446c49, /// 0x744
			 0xdeb2cb2e, /// 0x748
			 0xb9bd07ed, /// 0x74c
			 0xf452a6f9, /// 0x750
			 0x1b736a53, /// 0x754
			 0xfa778fd1, /// 0x758
			 0xa4ae2f9a, /// 0x75c
			 0x9ca16b0d, /// 0x760
			 0x794c2a23, /// 0x764
			 0x523e1032, /// 0x768
			 0xf446679c, /// 0x76c
			 0xfe3275fe, /// 0x770
			 0x6030b8ad, /// 0x774
			 0xfd01af36, /// 0x778
			 0xbe9e5a98, /// 0x77c
			 0xd4f3e3b0, /// 0x780
			 0x7113c3bb, /// 0x784
			 0x0ca7ad46, /// 0x788
			 0x49a50e19, /// 0x78c
			 0x7b513189, /// 0x790
			 0x35afd590, /// 0x794
			 0x033691dd, /// 0x798
			 0xb9bbea29, /// 0x79c
			 0xeac248d5, /// 0x7a0
			 0x47b97214, /// 0x7a4
			 0x5f42b07b, /// 0x7a8
			 0x58863d4f, /// 0x7ac
			 0x9d9f6370, /// 0x7b0
			 0x32744330, /// 0x7b4
			 0xcdca6814, /// 0x7b8
			 0x11a7f5af, /// 0x7bc
			 0x1fb9e0c5, /// 0x7c0
			 0xce29466d, /// 0x7c4
			 0xeb6abf23, /// 0x7c8
			 0x60d9a90b, /// 0x7cc
			 0x5e5208b3, /// 0x7d0
			 0x388dd25b, /// 0x7d4
			 0x161f0d72, /// 0x7d8
			 0x72ee4150, /// 0x7dc
			 0x9bdda75c, /// 0x7e0
			 0x0c0f420f, /// 0x7e4
			 0x507d7057, /// 0x7e8
			 0xccafbb7f, /// 0x7ec
			 0x45ec8efa, /// 0x7f0
			 0x432c2bdc, /// 0x7f4
			 0xf06baeef, /// 0x7f8
			 0x72d5b4ab, /// 0x7fc
			 0x965e1194, /// 0x800
			 0x2de1d532, /// 0x804
			 0xa98adaf3, /// 0x808
			 0xeabe1a35, /// 0x80c
			 0xf9c028b0, /// 0x810
			 0xd17084c3, /// 0x814
			 0x6e66258c, /// 0x818
			 0x0ff1cc7c, /// 0x81c
			 0x9cc3cbde, /// 0x820
			 0x8ac3b78f, /// 0x824
			 0x93003f4d, /// 0x828
			 0xbcf636ed, /// 0x82c
			 0x447d4255, /// 0x830
			 0x9ff0bc51, /// 0x834
			 0x9fcceba7, /// 0x838
			 0xe932d65a, /// 0x83c
			 0xa4d92fea, /// 0x840
			 0xb0c2534d, /// 0x844
			 0x6278588f, /// 0x848
			 0x9048c9f1, /// 0x84c
			 0xa2744f6f, /// 0x850
			 0x3bb65ad8, /// 0x854
			 0x2fbd3dae, /// 0x858
			 0xe598e546, /// 0x85c
			 0xa22dcba1, /// 0x860
			 0x057a3c04, /// 0x864
			 0xe554bde5, /// 0x868
			 0x75f74883, /// 0x86c
			 0x9b5efb13, /// 0x870
			 0xd0714b98, /// 0x874
			 0x655c86f1, /// 0x878
			 0x2b598307, /// 0x87c
			 0xfe34d8ae, /// 0x880
			 0x3cd78449, /// 0x884
			 0xa794cbfe, /// 0x888
			 0x47db0507, /// 0x88c
			 0x001e518d, /// 0x890
			 0x96ef21d8, /// 0x894
			 0x9188084b, /// 0x898
			 0xb6aea416, /// 0x89c
			 0x94478b14, /// 0x8a0
			 0xe73d1e29, /// 0x8a4
			 0x0e002f89, /// 0x8a8
			 0x3362f98d, /// 0x8ac
			 0xa6642054, /// 0x8b0
			 0xd6c70e48, /// 0x8b4
			 0x77dc0968, /// 0x8b8
			 0xc5c7eb7f, /// 0x8bc
			 0x2d7a2321, /// 0x8c0
			 0x0aab7852, /// 0x8c4
			 0x75938e9f, /// 0x8c8
			 0x87e064c5, /// 0x8cc
			 0xbdce1d80, /// 0x8d0
			 0x6967ec40, /// 0x8d4
			 0x4f0ba3de, /// 0x8d8
			 0xecdcfa41, /// 0x8dc
			 0x9322095f, /// 0x8e0
			 0xd6bdff1e, /// 0x8e4
			 0xfb0b0f08, /// 0x8e8
			 0xf7a325ac, /// 0x8ec
			 0x166f55b3, /// 0x8f0
			 0x352815ae, /// 0x8f4
			 0x6a0828e4, /// 0x8f8
			 0x1eb9f055, /// 0x8fc
			 0xe56c29f3, /// 0x900
			 0x0b1fcaef, /// 0x904
			 0x63f10a02, /// 0x908
			 0x47f5dc9d, /// 0x90c
			 0xe8e47b37, /// 0x910
			 0x0fa840bd, /// 0x914
			 0x24b80e3e, /// 0x918
			 0x81f5e0b7, /// 0x91c
			 0x787d0bcd, /// 0x920
			 0xd9320629, /// 0x924
			 0x8c75df63, /// 0x928
			 0x8fbabc0c, /// 0x92c
			 0x2ec91e23, /// 0x930
			 0xbdfb7fd0, /// 0x934
			 0x6e31092d, /// 0x938
			 0x1dbb16ef, /// 0x93c
			 0xd8bc7774, /// 0x940
			 0xfb33ac39, /// 0x944
			 0x6678e9f9, /// 0x948
			 0x6e96d0d7, /// 0x94c
			 0x4f303c94, /// 0x950
			 0xb00aaab1, /// 0x954
			 0x14bdc7fe, /// 0x958
			 0x8810f3f6, /// 0x95c
			 0x5b067c58, /// 0x960
			 0xdfb6589d, /// 0x964
			 0xb87bee4a, /// 0x968
			 0x928748c3, /// 0x96c
			 0x865f4f53, /// 0x970
			 0x08b78556, /// 0x974
			 0xae05e686, /// 0x978
			 0xb8e1aa33, /// 0x97c
			 0xa3843834, /// 0x980
			 0xb8a2baf8, /// 0x984
			 0x801da4e1, /// 0x988
			 0xf330209c, /// 0x98c
			 0x74019e4a, /// 0x990
			 0xa08ad03a, /// 0x994
			 0xdba6fe23, /// 0x998
			 0x9a09a2dd, /// 0x99c
			 0xaeaa4860, /// 0x9a0
			 0x10f3430e, /// 0x9a4
			 0xd0b728e6, /// 0x9a8
			 0xb0848990, /// 0x9ac
			 0x4ffd663a, /// 0x9b0
			 0x4cfa664c, /// 0x9b4
			 0x4685ab91, /// 0x9b8
			 0xc63349e6, /// 0x9bc
			 0x37b63b3e, /// 0x9c0
			 0x9cc74467, /// 0x9c4
			 0xfc581bab, /// 0x9c8
			 0xe27b5c81, /// 0x9cc
			 0x340dca61, /// 0x9d0
			 0x4252b0f8, /// 0x9d4
			 0xb25a8e54, /// 0x9d8
			 0xa08d1b81, /// 0x9dc
			 0x551e9157, /// 0x9e0
			 0xde7e0064, /// 0x9e4
			 0x4c2693fe, /// 0x9e8
			 0x5557e293, /// 0x9ec
			 0x0e4379f1, /// 0x9f0
			 0x83320573, /// 0x9f4
			 0x67423f70, /// 0x9f8
			 0xd789e2c6, /// 0x9fc
			 0x59d8ae0e, /// 0xa00
			 0xe99e8030, /// 0xa04
			 0x5b97a4fa, /// 0xa08
			 0x15287dc8, /// 0xa0c
			 0xf27a50bc, /// 0xa10
			 0xfc763015, /// 0xa14
			 0xdba3b8c9, /// 0xa18
			 0xf16ec00c, /// 0xa1c
			 0x72ec312f, /// 0xa20
			 0x5581f7db, /// 0xa24
			 0x5826ea5a, /// 0xa28
			 0xe6368ff6, /// 0xa2c
			 0xc8b75579, /// 0xa30
			 0x9ce6c1db, /// 0xa34
			 0xd03fe5e4, /// 0xa38
			 0x6c43d3df, /// 0xa3c
			 0x616c866c, /// 0xa40
			 0x67d4b642, /// 0xa44
			 0xbf44d9c0, /// 0xa48
			 0x1005f2d2, /// 0xa4c
			 0xc6b7866d, /// 0xa50
			 0x2b526f31, /// 0xa54
			 0x3a678293, /// 0xa58
			 0x6bdf2788, /// 0xa5c
			 0xc780e752, /// 0xa60
			 0x54995f40, /// 0xa64
			 0x0a4e2d54, /// 0xa68
			 0xce3ae7eb, /// 0xa6c
			 0x101ac271, /// 0xa70
			 0xced18b53, /// 0xa74
			 0x2baee834, /// 0xa78
			 0x06a22c65, /// 0xa7c
			 0x98020ae1, /// 0xa80
			 0x3de3faf3, /// 0xa84
			 0x6bb62378, /// 0xa88
			 0x9311f166, /// 0xa8c
			 0x9d3a5461, /// 0xa90
			 0xbdc24f78, /// 0xa94
			 0xdfa8eaf8, /// 0xa98
			 0xfed7c3af, /// 0xa9c
			 0xef5189ce, /// 0xaa0
			 0x4deac90f, /// 0xaa4
			 0x19e33411, /// 0xaa8
			 0x2200a151, /// 0xaac
			 0xdd40f1d6, /// 0xab0
			 0x0a66338b, /// 0xab4
			 0x0e57d9a1, /// 0xab8
			 0x3749b7b7, /// 0xabc
			 0x86586ce9, /// 0xac0
			 0xa23401d7, /// 0xac4
			 0x60c85d0d, /// 0xac8
			 0x9693c70f, /// 0xacc
			 0x93930169, /// 0xad0
			 0x683fe7cd, /// 0xad4
			 0x0755c889, /// 0xad8
			 0x3d142d2e, /// 0xadc
			 0xcf67ff36, /// 0xae0
			 0xc09aa41d, /// 0xae4
			 0x65800e65, /// 0xae8
			 0x17ae2ed9, /// 0xaec
			 0x8f4b6709, /// 0xaf0
			 0x6cbd05d9, /// 0xaf4
			 0x3f9ffaa2, /// 0xaf8
			 0xe961fe2c, /// 0xafc
			 0xfa190bae, /// 0xb00
			 0xe0bfe147, /// 0xb04
			 0x75803b34, /// 0xb08
			 0x1e79fb8d, /// 0xb0c
			 0x6c762c1b, /// 0xb10
			 0x9730991f, /// 0xb14
			 0x8d124e3c, /// 0xb18
			 0xb689fb58, /// 0xb1c
			 0x8effcada, /// 0xb20
			 0xa822f548, /// 0xb24
			 0xb689b22d, /// 0xb28
			 0x63073934, /// 0xb2c
			 0xab31e0f7, /// 0xb30
			 0xc99cd6c5, /// 0xb34
			 0xf37e2956, /// 0xb38
			 0x8d70a9e0, /// 0xb3c
			 0xb9fb00ff, /// 0xb40
			 0xd2875fd5, /// 0xb44
			 0x58c5c229, /// 0xb48
			 0x825a1b62, /// 0xb4c
			 0xdbdc04f1, /// 0xb50
			 0x67ab9f22, /// 0xb54
			 0xcdc9fb01, /// 0xb58
			 0x8db8a4b3, /// 0xb5c
			 0x6ca4d5b5, /// 0xb60
			 0xae58ab82, /// 0xb64
			 0x4e01865e, /// 0xb68
			 0xc058f00c, /// 0xb6c
			 0x820e63ca, /// 0xb70
			 0xe277521f, /// 0xb74
			 0x8251f99c, /// 0xb78
			 0x78c6cd1a, /// 0xb7c
			 0x699c61f0, /// 0xb80
			 0x27dc5c7c, /// 0xb84
			 0xd2a3851c, /// 0xb88
			 0x26057161, /// 0xb8c
			 0x5e2889bc, /// 0xb90
			 0x5d9a07ee, /// 0xb94
			 0x0603e10e, /// 0xb98
			 0x7c525c41, /// 0xb9c
			 0xd277c83d, /// 0xba0
			 0x3f97e732, /// 0xba4
			 0x198a49f2, /// 0xba8
			 0x54af40b8, /// 0xbac
			 0xd4a3b740, /// 0xbb0
			 0xe48325de, /// 0xbb4
			 0x50761053, /// 0xbb8
			 0xd3a71772, /// 0xbbc
			 0x6650e7a0, /// 0xbc0
			 0xe340b3fe, /// 0xbc4
			 0x589f868b, /// 0xbc8
			 0x3f5357f5, /// 0xbcc
			 0x35cebac4, /// 0xbd0
			 0x2e5f8b0a, /// 0xbd4
			 0xb1a6d18a, /// 0xbd8
			 0x4d588b9f, /// 0xbdc
			 0xa9fffa0f, /// 0xbe0
			 0xb1bd0761, /// 0xbe4
			 0xb0a30ade, /// 0xbe8
			 0x6686c2c3, /// 0xbec
			 0x6c3d5f9b, /// 0xbf0
			 0x3e06a542, /// 0xbf4
			 0x4dc8cd03, /// 0xbf8
			 0xaf58e375, /// 0xbfc
			 0xbf3ca7a2, /// 0xc00
			 0x28051958, /// 0xc04
			 0x9dc4ae95, /// 0xc08
			 0xf354443a, /// 0xc0c
			 0xd1575a6a, /// 0xc10
			 0xd00be923, /// 0xc14
			 0x39bc5d4e, /// 0xc18
			 0x9b000307, /// 0xc1c
			 0x7d58f1d8, /// 0xc20
			 0x351cfc99, /// 0xc24
			 0xbf6e7617, /// 0xc28
			 0xfad2dd21, /// 0xc2c
			 0x1b6b2164, /// 0xc30
			 0x390f758d, /// 0xc34
			 0x9c8d1b1f, /// 0xc38
			 0xf8dd4ea0, /// 0xc3c
			 0x393552cc, /// 0xc40
			 0x3292f608, /// 0xc44
			 0xeabb05ea, /// 0xc48
			 0x7fff8c2f, /// 0xc4c
			 0x46d4efb5, /// 0xc50
			 0x283a7491, /// 0xc54
			 0xb7d41e5a, /// 0xc58
			 0xad9a85c7, /// 0xc5c
			 0x9ca6624a, /// 0xc60
			 0xbd5bf26d, /// 0xc64
			 0x02310670, /// 0xc68
			 0xbb88de52, /// 0xc6c
			 0x10c99509, /// 0xc70
			 0xaee02550, /// 0xc74
			 0x0d5e0354, /// 0xc78
			 0x974408e5, /// 0xc7c
			 0xa43a4e62, /// 0xc80
			 0x35985ba2, /// 0xc84
			 0x20b35ae0, /// 0xc88
			 0xafc74616, /// 0xc8c
			 0xb83cc90c, /// 0xc90
			 0xd56e1b17, /// 0xc94
			 0x02be620f, /// 0xc98
			 0xe38740c7, /// 0xc9c
			 0x9e06d6ae, /// 0xca0
			 0xd36b443c, /// 0xca4
			 0xbb6314a5, /// 0xca8
			 0x2d91ddbf, /// 0xcac
			 0x0ccfddd0, /// 0xcb0
			 0x5a121bbb, /// 0xcb4
			 0xc5d72019, /// 0xcb8
			 0xf8dd59d3, /// 0xcbc
			 0x3f5396e5, /// 0xcc0
			 0x0d66dbe5, /// 0xcc4
			 0x65deed4e, /// 0xcc8
			 0xadf332dd, /// 0xccc
			 0x4cfbfb52, /// 0xcd0
			 0x6df2283e, /// 0xcd4
			 0x958a3d4c, /// 0xcd8
			 0x851348ac, /// 0xcdc
			 0x6af5fc93, /// 0xce0
			 0xf333a78c, /// 0xce4
			 0x7d3440a1, /// 0xce8
			 0x34b497fe, /// 0xcec
			 0x2f51dd2a, /// 0xcf0
			 0x13f60289, /// 0xcf4
			 0x5c659a3b, /// 0xcf8
			 0xabb19610, /// 0xcfc
			 0x7192f2e1, /// 0xd00
			 0x2a1af40f, /// 0xd04
			 0x2a4d02f2, /// 0xd08
			 0xeaf9f95d, /// 0xd0c
			 0xf6bf7988, /// 0xd10
			 0xbc3e6445, /// 0xd14
			 0x4f4e4231, /// 0xd18
			 0x48231737, /// 0xd1c
			 0x73f03847, /// 0xd20
			 0xcdc25afd, /// 0xd24
			 0xeb8814d2, /// 0xd28
			 0xfd48e9d1, /// 0xd2c
			 0xedf34a3c, /// 0xd30
			 0x4e14a9c7, /// 0xd34
			 0x214f511e, /// 0xd38
			 0xaf34099a, /// 0xd3c
			 0x5418932f, /// 0xd40
			 0xeaa350f4, /// 0xd44
			 0x06efaa64, /// 0xd48
			 0xcf6281e2, /// 0xd4c
			 0x3e5a4e90, /// 0xd50
			 0x62fdd942, /// 0xd54
			 0xd55e0c6a, /// 0xd58
			 0x43f5576a, /// 0xd5c
			 0x6e5d1913, /// 0xd60
			 0xb1998765, /// 0xd64
			 0x39238bf5, /// 0xd68
			 0x6fb8f69d, /// 0xd6c
			 0x0a5796bc, /// 0xd70
			 0x8d8c5ebb, /// 0xd74
			 0x5a962e66, /// 0xd78
			 0xeb42388d, /// 0xd7c
			 0x7895457c, /// 0xd80
			 0xc26ae674, /// 0xd84
			 0x9bd53d8c, /// 0xd88
			 0x8acc90ea, /// 0xd8c
			 0x4b71257f, /// 0xd90
			 0x320aa507, /// 0xd94
			 0x47503173, /// 0xd98
			 0xbc950946, /// 0xd9c
			 0x7871d2c4, /// 0xda0
			 0xc62fdfdb, /// 0xda4
			 0x6bc2ce8b, /// 0xda8
			 0x0c0781d2, /// 0xdac
			 0xd9986e56, /// 0xdb0
			 0x012d929f, /// 0xdb4
			 0xbbb04de8, /// 0xdb8
			 0x9f053c06, /// 0xdbc
			 0x92ceb6e9, /// 0xdc0
			 0x3637bd52, /// 0xdc4
			 0x3a38e6fd, /// 0xdc8
			 0x5130e70b, /// 0xdcc
			 0x539ba56a, /// 0xdd0
			 0x1f365f01, /// 0xdd4
			 0x6292ecc1, /// 0xdd8
			 0xd8c1b4fe, /// 0xddc
			 0x9889b69e, /// 0xde0
			 0x9ff1a85c, /// 0xde4
			 0x7035ecc6, /// 0xde8
			 0x5f85c036, /// 0xdec
			 0xbac14f54, /// 0xdf0
			 0xd1fd8e95, /// 0xdf4
			 0x9de13f8a, /// 0xdf8
			 0x5c65ae58, /// 0xdfc
			 0x7ca279a0, /// 0xe00
			 0x182a3119, /// 0xe04
			 0x212768eb, /// 0xe08
			 0xcb8bd6da, /// 0xe0c
			 0xdebb4609, /// 0xe10
			 0x0180d053, /// 0xe14
			 0x236c208e, /// 0xe18
			 0x0ef8d8cd, /// 0xe1c
			 0x86d4a4ff, /// 0xe20
			 0x5d3872ca, /// 0xe24
			 0xc946cca9, /// 0xe28
			 0x1a9e7a52, /// 0xe2c
			 0x3e1ab898, /// 0xe30
			 0xd6a2b89c, /// 0xe34
			 0x61d7c188, /// 0xe38
			 0xd09441fc, /// 0xe3c
			 0x0d572c47, /// 0xe40
			 0x4dbe3a09, /// 0xe44
			 0x71c74f32, /// 0xe48
			 0xbd7bc7e7, /// 0xe4c
			 0x666efe86, /// 0xe50
			 0xeaf64660, /// 0xe54
			 0xc5934529, /// 0xe58
			 0x507c7eba, /// 0xe5c
			 0x6ee555fb, /// 0xe60
			 0xa58ede81, /// 0xe64
			 0x61fc13ff, /// 0xe68
			 0xda1bd37c, /// 0xe6c
			 0x798f6293, /// 0xe70
			 0xfcc1eacd, /// 0xe74
			 0x8d1ac035, /// 0xe78
			 0x7745130b, /// 0xe7c
			 0x1ee29778, /// 0xe80
			 0xf2145e37, /// 0xe84
			 0x16524360, /// 0xe88
			 0x463875ba, /// 0xe8c
			 0xa8394bfc, /// 0xe90
			 0x1561e539, /// 0xe94
			 0x8f62db76, /// 0xe98
			 0xa1ff08a5, /// 0xe9c
			 0x5e4e3fb2, /// 0xea0
			 0x8d422209, /// 0xea4
			 0xfda51381, /// 0xea8
			 0xf96b49f5, /// 0xeac
			 0xd73ad31e, /// 0xeb0
			 0x607098dc, /// 0xeb4
			 0xd1d5102e, /// 0xeb8
			 0xb3789c0c, /// 0xebc
			 0x0c7bcd33, /// 0xec0
			 0xd2a8c613, /// 0xec4
			 0x12331540, /// 0xec8
			 0x3f250c9c, /// 0xecc
			 0x1d1d313e, /// 0xed0
			 0x1da0c6b7, /// 0xed4
			 0xe44a6ea0, /// 0xed8
			 0x5b5aea61, /// 0xedc
			 0x3d647f96, /// 0xee0
			 0x54865d3d, /// 0xee4
			 0x06e67a7f, /// 0xee8
			 0xdde21f4e, /// 0xeec
			 0x36c5e3de, /// 0xef0
			 0x32d12de7, /// 0xef4
			 0x90391007, /// 0xef8
			 0x562908b4, /// 0xefc
			 0x48e20af4, /// 0xf00
			 0x8cc6aa0e, /// 0xf04
			 0x402c08d2, /// 0xf08
			 0xa7f480bf, /// 0xf0c
			 0x2b8d4490, /// 0xf10
			 0xfe33ccb5, /// 0xf14
			 0x88df987a, /// 0xf18
			 0x6cba778b, /// 0xf1c
			 0xd290b5f0, /// 0xf20
			 0x87780945, /// 0xf24
			 0xbeb28415, /// 0xf28
			 0x31cb364e, /// 0xf2c
			 0xde940b21, /// 0xf30
			 0xf11e091e, /// 0xf34
			 0x21d6edce, /// 0xf38
			 0x897a9c67, /// 0xf3c
			 0x75b077de, /// 0xf40
			 0x884db35c, /// 0xf44
			 0x3efc9237, /// 0xf48
			 0x4ad9231c, /// 0xf4c
			 0x2f708674, /// 0xf50
			 0x50709ade, /// 0xf54
			 0x186db3ed, /// 0xf58
			 0x922d1c0f, /// 0xf5c
			 0x8d5b61c4, /// 0xf60
			 0x36f63de0, /// 0xf64
			 0x5c87ea24, /// 0xf68
			 0x5f1cefbf, /// 0xf6c
			 0x8ec12107, /// 0xf70
			 0x70e6300c, /// 0xf74
			 0xf5b47c2d, /// 0xf78
			 0x00317440, /// 0xf7c
			 0x095545bf, /// 0xf80
			 0x61938054, /// 0xf84
			 0x731467b5, /// 0xf88
			 0x4e180a93, /// 0xf8c
			 0x1d0654b5, /// 0xf90
			 0xd2bcc541, /// 0xf94
			 0x66a76dbf, /// 0xf98
			 0xffc1aec7, /// 0xf9c
			 0x4de911e8, /// 0xfa0
			 0x80717344, /// 0xfa4
			 0x9eb673d5, /// 0xfa8
			 0x757ab7e8, /// 0xfac
			 0xdcb9f856, /// 0xfb0
			 0xc9d857ab, /// 0xfb4
			 0x8451b956, /// 0xfb8
			 0xead5b14b, /// 0xfbc
			 0xe8a55c6c, /// 0xfc0
			 0x90bd0f17, /// 0xfc4
			 0x33ef7391, /// 0xfc8
			 0xeb849272, /// 0xfcc
			 0x94e78e0a, /// 0xfd0
			 0x9eb71d82, /// 0xfd4
			 0x8622d68a, /// 0xfd8
			 0x2868fde6, /// 0xfdc
			 0x492a0a60, /// 0xfe0
			 0x714eaca9, /// 0xfe4
			 0x2185d985, /// 0xfe8
			 0xe469c97b, /// 0xfec
			 0x0c84fcda, /// 0xff0
			 0x8f9aa933, /// 0xff4
			 0x72b74aeb, /// 0xff8
			 0x4913611f /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00000
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00004
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00008
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00010
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00014
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00018
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0001c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00000000,                                                  // zero                                        /// 00024
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00028
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00030
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00038
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0003c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00048
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x4b000000,                                                  // 8388608.0                                   /// 00050
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00054
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00058
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0005c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00068
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xbf028f5c,                                                  // -0.51                                       /// 00078
			 0x0e000001,                                                  // Trailing 1s:                                /// 0007c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00080
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00084
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00090
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00094
			 0x00000000,                                                  // zero                                        /// 00098
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x7f800000,                                                  // inf                                         /// 000a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000ac
			 0x7fc00001,                                                  // signaling NaN                               /// 000b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000c8
			 0x7f800000,                                                  // inf                                         /// 000cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0xbf028f5c,                                                  // -0.51                                       /// 000ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00104
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00108
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80000000,                                                  // -zero                                       /// 00110
			 0x4b000000,                                                  // 8388608.0                                   /// 00114
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0011c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00120
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00124
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0012c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x33333333,                                                  // 4 random values                             /// 00140
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00144
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xbf028f5c,                                                  // -0.51                                       /// 00154
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00158
			 0x3f028f5c,                                                  // 0.51                                        /// 0015c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00164
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00170
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00174
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0017c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00180
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00188
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0018c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00190
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00198
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0019c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00200
			 0x0c700000,                                                  // Leading 1s:                                 /// 00204
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0020c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00220
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00248
			 0x00000000,                                                  // zero                                        /// 0024c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00254
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00258
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0025c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00260
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00264
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x55555555,                                                  // 4 random values                             /// 0026c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00274
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00278
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0027c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00280
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00288
			 0x00011111,                                                  // 9.7958E-41                                  /// 0028c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00294
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0029c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00300
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00304
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00308
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0030c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0031c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00324
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0032c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00330
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00340
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000000,                                                  // -zero                                       /// 00348
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0034c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00358
			 0x0c400000,                                                  // Leading 1s:                                 /// 0035c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00360
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00364
			 0x0e000003,                                                  // Trailing 1s:                                /// 00368
			 0x4b000000,                                                  // 8388608.0                                   /// 0036c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00370
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00378
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00384
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00388
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00390
			 0x00000000,                                                  // zero                                        /// 00394
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00398
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00404
			 0x0c400000,                                                  // Leading 1s:                                 /// 00408
			 0x0e000003,                                                  // Trailing 1s:                                /// 0040c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00414
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x0c400000,                                                  // Leading 1s:                                 /// 0041c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00420
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00424
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00428
			 0x0c400000,                                                  // Leading 1s:                                 /// 0042c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00430
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00434
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00440
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0044c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00450
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00454
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00458
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0045c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00460
			 0x3f028f5c,                                                  // 0.51                                        /// 00464
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00468
			 0x0c400000,                                                  // Leading 1s:                                 /// 0046c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00470
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0047c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x4b000000,                                                  // 8388608.0                                   /// 00484
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00488
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0048c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00490
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00494
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x7f800000,                                                  // inf                                         /// 0049c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004d0
			 0x33333333,                                                  // 4 random values                             /// 004d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00500
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x3f028f5c,                                                  // 0.51                                        /// 00508
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00514
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00518
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00520
			 0x0c400000,                                                  // Leading 1s:                                 /// 00524
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00528
			 0x80000000,                                                  // -zero                                       /// 0052c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00540
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00544
			 0x0e000007,                                                  // Trailing 1s:                                /// 00548
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0054c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00550
			 0x7fc00001,                                                  // signaling NaN                               /// 00554
			 0x0e000007,                                                  // Trailing 1s:                                /// 00558
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00560
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00564
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00570
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00574
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00578
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0057c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00580
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00584
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0058c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00590
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00594
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a0
			 0xbf028f5c,                                                  // -0.51                                       /// 005a4
			 0x00000000,                                                  // zero                                        /// 005a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b0
			 0xff800000,                                                  // -inf                                        /// 005b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005cc
			 0xffc00001,                                                  // -signaling NaN                              /// 005d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0c700000,                                                  // Leading 1s:                                 /// 00604
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0060c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00614
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00620
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00624
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00628
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0062c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00630
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00634
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00640
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00644
			 0x55555555,                                                  // 4 random values                             /// 00648
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00650
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0c780000,                                                  // Leading 1s:                                 /// 00658
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0065c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00660
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00664
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00668
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00678
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00680
			 0x80000000,                                                  // -zero                                       /// 00684
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00694
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00698
			 0x7fc00001,                                                  // signaling NaN                               /// 0069c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0xcb000000,                                                  // -8388608.0                                  /// 006b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e8
			 0xff800000,                                                  // -inf                                        /// 006ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00704
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0070c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00710
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00714
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0071c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00720
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00724
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00728
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0072c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00730
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0073c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00740
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0074c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00754
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0075c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00760
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00764
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00774
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00778
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0077c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00784
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00788
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0078c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00794
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00798
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007b4
			 0x4b000000,                                                  // 8388608.0                                   /// 007b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00000000,                                                  // zero                                        /// 007c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00808
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0080c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00814
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00818
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0081c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0c400000,                                                  // Leading 1s:                                 /// 00824
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0082c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00834
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00838
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0083c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00840
			 0x80000000,                                                  // -zero                                       /// 00844
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00848
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0084c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00854
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00860
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00870
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c600000,                                                  // Leading 1s:                                 /// 0087c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00884
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00888
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x0c700000,                                                  // Leading 1s:                                 /// 00894
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00898
			 0xbf028f5c,                                                  // -0.51                                       /// 0089c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a4
			 0x33333333,                                                  // 4 random values                             /// 008a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0xff800000,                                                  // -inf                                        /// 008bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00900
			 0x3f028f5c,                                                  // 0.51                                        /// 00904
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00910
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00914
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00918
			 0x0c700000,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00920
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00924
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0092c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00930
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00934
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00938
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00940
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00948
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0094c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00954
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c780000,                                                  // Leading 1s:                                 /// 00964
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00968
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0096c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00970
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x0c700000,                                                  // Leading 1s:                                 /// 00978
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00988
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0099c
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 009b0
			 0x55555555,                                                  // 4 random values                             /// 009b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009c0
			 0x00000000,                                                  // zero                                        /// 009c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 009f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xcb000000,                                                  // -8388608.0                                  /// 00a28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a44
			 0xcb000000,                                                  // -8388608.0                                  /// 00a48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0xffc00001,                                                  // -signaling NaN                              /// 00a88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00abc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ac8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00acc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ad4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00adc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ae8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00af0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00af8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b34
			 0x4b000000,                                                  // 8388608.0                                   /// 00b38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b40
			 0x00000000,                                                  // zero                                        /// 00b44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00be4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bfc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c48
			 0xbf028f5c,                                                  // -0.51                                       /// 00c4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f800000,                                                  // inf                                         /// 00c60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x7f800000,                                                  // inf                                         /// 00c68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x55555555,                                                  // 4 random values                             /// 00c9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca4
			 0x55555555,                                                  // 4 random values                             /// 00ca8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ce8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d3c
			 0xff800000,                                                  // -inf                                        /// 00d40
			 0x55555555,                                                  // 4 random values                             /// 00d44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d54
			 0x33333333,                                                  // 4 random values                             /// 00d58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db8
			 0x55555555,                                                  // 4 random values                             /// 00dbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00000000,                                                  // zero                                        /// 00e4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e60
			 0x3f028f5c,                                                  // 0.51                                        /// 00e64
			 0x7fc00001,                                                  // signaling NaN                               /// 00e68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x3f028f5c,                                                  // 0.51                                        /// 00e78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e88
			 0x33333333,                                                  // 4 random values                             /// 00e8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eb8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ebc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ec0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ecc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ed0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ee0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ee8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ef0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0x80000000,                                                  // -zero                                       /// 00f8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00fa0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x55555555,                                                  // 4 random values                             /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fe0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ff4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80000040                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00000
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00010
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00014
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00018
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0001c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00020
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00024
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00030
			 0x0c600000,                                                  // Leading 1s:                                 /// 00034
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00038
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00040
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00048
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0004c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x33333333,                                                  // 4 random values                             /// 00054
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0005c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00060
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00064
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00068
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0006c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00070
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00078
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0007c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00080
			 0x00011111,                                                  // 9.7958E-41                                  /// 00084
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0008c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00090
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00094
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c600000,                                                  // Leading 1s:                                 /// 0009c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000a8
			 0x4b000000,                                                  // 8388608.0                                   /// 000ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00100
			 0xff800000,                                                  // -inf                                        /// 00104
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00108
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0010c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00110
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00114
			 0xffc00001,                                                  // -signaling NaN                              /// 00118
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0c600000,                                                  // Leading 1s:                                 /// 00124
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0012c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00130
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00134
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00138
			 0x80011111,                                                  // -9.7958E-41                                 /// 0013c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00140
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00144
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0014c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0xcb000000,                                                  // -8388608.0                                  /// 00154
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00158
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00164
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00168
			 0x00011111,                                                  // 9.7958E-41                                  /// 0016c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00170
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00174
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0017c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00180
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00184
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00188
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00198
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0019c
			 0x7f800000,                                                  // inf                                         /// 001a0
			 0x33333333,                                                  // 4 random values                             /// 001a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0xbf028f5c,                                                  // -0.51                                       /// 001b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c0
			 0xbf028f5c,                                                  // -0.51                                       /// 001c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x33333333,                                                  // 4 random values                             /// 001d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f8
			 0x33333333,                                                  // 4 random values                             /// 001fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00200
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00210
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00214
			 0xff800000,                                                  // -inf                                        /// 00218
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0021c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00220
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00224
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00228
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00230
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00238
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00240
			 0x0e000003,                                                  // Trailing 1s:                                /// 00244
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x55555555,                                                  // 4 random values                             /// 0024c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00254
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0025c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00260
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00264
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0xffc00001,                                                  // -signaling NaN                              /// 0026c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00278
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0027c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00284
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000000,                                                  // zero                                        /// 0028c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00290
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00294
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00298
			 0x0e000007,                                                  // Trailing 1s:                                /// 0029c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002a0
			 0x33333333,                                                  // 4 random values                             /// 002a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b0
			 0x80000000,                                                  // -zero                                       /// 002b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002cc
			 0xbf028f5c,                                                  // -0.51                                       /// 002d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0xcb000000,                                                  // -8388608.0                                  /// 002dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00300
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00304
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00308
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00310
			 0xff800000,                                                  // -inf                                        /// 00314
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00324
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00328
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0032c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80011111,                                                  // -9.7958E-41                                 /// 00338
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00340
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00348
			 0xcb000000,                                                  // -8388608.0                                  /// 0034c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00354
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0035c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00364
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0036c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00374
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00378
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00384
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00398
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x33333333,                                                  // 4 random values                             /// 003d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00410
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00414
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00418
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00420
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00428
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0042c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0043c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00444
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00448
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0044c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00450
			 0x00011111,                                                  // 9.7958E-41                                  /// 00454
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00458
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00470
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00474
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00478
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c780000,                                                  // Leading 1s:                                 /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00490
			 0xffc00001,                                                  // -signaling NaN                              /// 00494
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0xcb000000,                                                  // -8388608.0                                  /// 004a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x7fc00001,                                                  // signaling NaN                               /// 004f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00500
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e000007,                                                  // Trailing 1s:                                /// 00508
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0050c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00510
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00514
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00518
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0051c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00524
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00528
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00530
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00534
			 0x55555555,                                                  // 4 random values                             /// 00538
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0053c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00548
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00554
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0056c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00570
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00574
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00580
			 0x0c780000,                                                  // Leading 1s:                                 /// 00584
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00588
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0058c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00598
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0059c
			 0x7f800000,                                                  // inf                                         /// 005a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0xff800000,                                                  // -inf                                        /// 00604
			 0xffc00001,                                                  // -signaling NaN                              /// 00608
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0060c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00610
			 0x55555555,                                                  // 4 random values                             /// 00614
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00618
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0061c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00624
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00628
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00630
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00634
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00638
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0063c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x55555555,                                                  // 4 random values                             /// 00644
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0064c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00650
			 0x00000000,                                                  // zero                                        /// 00654
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00658
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0065c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x3f028f5c,                                                  // 0.51                                        /// 00664
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00668
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00670
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00674
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00678
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00684
			 0xffc00001,                                                  // -signaling NaN                              /// 00688
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00690
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00698
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006ac
			 0x3f028f5c,                                                  // 0.51                                        /// 006b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00704
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00708
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00714
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00718
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0071c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00720
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00728
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00730
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00734
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00740
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c600000,                                                  // Leading 1s:                                 /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00750
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00754
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0075c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00760
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00764
			 0x00011111,                                                  // 9.7958E-41                                  /// 00768
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0076c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00770
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00774
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00778
			 0x55555555,                                                  // 4 random values                             /// 0077c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00780
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00784
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00788
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0078c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00790
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00794
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00798
			 0xffc00001,                                                  // -signaling NaN                              /// 0079c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0xffc00001,                                                  // -signaling NaN                              /// 007bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007cc
			 0xcb000000,                                                  // -8388608.0                                  /// 007d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f8
			 0xcb000000,                                                  // -8388608.0                                  /// 007fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00800
			 0x0c700000,                                                  // Leading 1s:                                 /// 00804
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00808
			 0x0c400000,                                                  // Leading 1s:                                 /// 0080c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00810
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80000000,                                                  // -zero                                       /// 0081c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00820
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00828
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0082c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00834
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00838
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00840
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00844
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0084c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00850
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00854
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0085c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00860
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00864
			 0x80011111,                                                  // -9.7958E-41                                 /// 00868
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0086c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00870
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c400000,                                                  // Leading 1s:                                 /// 0087c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00880
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00884
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00894
			 0x4b000000,                                                  // 8388608.0                                   /// 00898
			 0xff800000,                                                  // -inf                                        /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f4
			 0x00000000,                                                  // zero                                        /// 008f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x0c780000,                                                  // Leading 1s:                                 /// 00908
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0090c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00910
			 0xcb000000,                                                  // -8388608.0                                  /// 00914
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00918
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0091c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00928
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0092c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00938
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0093c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00948
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00954
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00958
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00960
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x7fc00001,                                                  // signaling NaN                               /// 00968
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00970
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00978
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00980
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00984
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0098c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00990
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00994
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0099c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x7f800000,                                                  // inf                                         /// 00a10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7fc00001,                                                  // signaling NaN                               /// 00a48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa4
			 0xbf028f5c,                                                  // -0.51                                       /// 00aa8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ac4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00acc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00adc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ae8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00afc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ba8
			 0x55555555,                                                  // 4 random values                             /// 00bac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bcc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bdc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00be4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bf4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x3f028f5c,                                                  // 0.51                                        /// 00c3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ccc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cdc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ce8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80000000,                                                  // -zero                                       /// 00cf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d30
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d50
			 0x4b000000,                                                  // 8388608.0                                   /// 00d54
			 0x4b000000,                                                  // 8388608.0                                   /// 00d58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d80
			 0x4b000000,                                                  // 8388608.0                                   /// 00d84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f800000,                                                  // inf                                         /// 00d8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00da0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x7f800000,                                                  // inf                                         /// 00db0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xff800000,                                                  // -inf                                        /// 00db8
			 0x55555555,                                                  // 4 random values                             /// 00dbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ddc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00df0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0xff800000,                                                  // -inf                                        /// 00e20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xbf028f5c,                                                  // -0.51                                       /// 00e94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ea0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ecc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ed0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ee4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0xff800000,                                                  // -inf                                        /// 00f10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f20
			 0x4b000000,                                                  // 8388608.0                                   /// 00f24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f48
			 0xbf028f5c,                                                  // -0.51                                       /// 00f4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f7c
			 0x7f800000,                                                  // inf                                         /// 00f80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fe8
			 0x00000000,                                                  // zero                                        /// 00fec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
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
			 0x000000fc,
			 0x000000cc,
			 0x000002e8,
			 0x00000418,
			 0x00000214,
			 0x00000454,
			 0x0000017c,
			 0x0000052c,

			 /// vpu register f2
			 0x41200000,
			 0x40e00000,
			 0x40a00000,
			 0x41300000,
			 0x41c00000,
			 0x41500000,
			 0x40c00000,
			 0x41e80000,

			 /// vpu register f3
			 0x40e00000,
			 0x41880000,
			 0x41000000,
			 0x40000000,
			 0x40a00000,
			 0x41200000,
			 0x41d00000,
			 0x3f800000,

			 /// vpu register f4
			 0x41f00000,
			 0x40000000,
			 0x41700000,
			 0x41c00000,
			 0x41980000,
			 0x40800000,
			 0x41600000,
			 0x41c00000,

			 /// vpu register f5
			 0x40800000,
			 0x41b80000,
			 0x40c00000,
			 0x41300000,
			 0x41a80000,
			 0x41c00000,
			 0x41900000,
			 0x40000000,

			 /// vpu register f6
			 0x41800000,
			 0x41c80000,
			 0x42000000,
			 0x42000000,
			 0x41800000,
			 0x41100000,
			 0x41b00000,
			 0x41200000,

			 /// vpu register f7
			 0x40a00000,
			 0x41980000,
			 0x3f800000,
			 0x41900000,
			 0x42000000,
			 0x41600000,
			 0x41400000,
			 0x41e00000,

			 /// vpu register f8
			 0x41d00000,
			 0x41000000,
			 0x41b80000,
			 0x41100000,
			 0x41800000,
			 0x42000000,
			 0x41c80000,
			 0x41d00000,

			 /// vpu register f9
			 0x41c80000,
			 0x41400000,
			 0x41a80000,
			 0x41e80000,
			 0x42000000,
			 0x41100000,
			 0x41d00000,
			 0x40400000,

			 /// vpu register f10
			 0x41f80000,
			 0x41d00000,
			 0x40400000,
			 0x41900000,
			 0x41200000,
			 0x42000000,
			 0x41b80000,
			 0x41c80000,

			 /// vpu register f11
			 0x40c00000,
			 0x40c00000,
			 0x40a00000,
			 0x41500000,
			 0x41e00000,
			 0x41000000,
			 0x41600000,
			 0x40400000,

			 /// vpu register f12
			 0x40000000,
			 0x40a00000,
			 0x41e80000,
			 0x41f00000,
			 0x41e80000,
			 0x41200000,
			 0x41300000,
			 0x40800000,

			 /// vpu register f13
			 0x41880000,
			 0x41e80000,
			 0x41700000,
			 0x41100000,
			 0x41a80000,
			 0x41300000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f14
			 0x3f800000,
			 0x41900000,
			 0x41800000,
			 0x41700000,
			 0x41980000,
			 0x40000000,
			 0x41980000,
			 0x41000000,

			 /// vpu register f15
			 0x40400000,
			 0x40000000,
			 0x40400000,
			 0x40e00000,
			 0x41f00000,
			 0x41f80000,
			 0x41e00000,
			 0x41c80000,

			 /// vpu register f16
			 0x41c80000,
			 0x41100000,
			 0x41600000,
			 0x41a80000,
			 0x41100000,
			 0x41f00000,
			 0x41e00000,
			 0x41b80000,

			 /// vpu register f17
			 0x41d00000,
			 0x40800000,
			 0x41500000,
			 0x41b80000,
			 0x41f80000,
			 0x41b80000,
			 0x41a80000,
			 0x40e00000,

			 /// vpu register f18
			 0x41f00000,
			 0x41400000,
			 0x41a80000,
			 0x41e80000,
			 0x40c00000,
			 0x41a80000,
			 0x41700000,
			 0x41c80000,

			 /// vpu register f19
			 0x41e00000,
			 0x41f00000,
			 0x41800000,
			 0x41600000,
			 0x41e80000,
			 0x41e00000,
			 0x40e00000,
			 0x40800000,

			 /// vpu register f20
			 0x41b80000,
			 0x41d80000,
			 0x41200000,
			 0x41000000,
			 0x41200000,
			 0x41800000,
			 0x42000000,
			 0x41600000,

			 /// vpu register f21
			 0x41880000,
			 0x41f00000,
			 0x41f00000,
			 0x41b00000,
			 0x41100000,
			 0x41200000,
			 0x41300000,
			 0x41f80000,

			 /// vpu register f22
			 0x40000000,
			 0x42000000,
			 0x41900000,
			 0x41800000,
			 0x41980000,
			 0x41400000,
			 0x41c00000,
			 0x41100000,

			 /// vpu register f23
			 0x41700000,
			 0x41c80000,
			 0x40c00000,
			 0x41f80000,
			 0x41e80000,
			 0x40c00000,
			 0x41d00000,
			 0x40a00000,

			 /// vpu register f24
			 0x40e00000,
			 0x41a80000,
			 0x42000000,
			 0x41e80000,
			 0x41900000,
			 0x41300000,
			 0x40400000,
			 0x41700000,

			 /// vpu register f25
			 0x41900000,
			 0x41f80000,
			 0x41200000,
			 0x41980000,
			 0x41b80000,
			 0x41800000,
			 0x40a00000,
			 0x40c00000,

			 /// vpu register f26
			 0x41a80000,
			 0x41980000,
			 0x41600000,
			 0x41c80000,
			 0x41f80000,
			 0x41880000,
			 0x41900000,
			 0x41d00000,

			 /// vpu register f27
			 0x41f00000,
			 0x41a80000,
			 0x41c80000,
			 0x41b80000,
			 0x41a80000,
			 0x40000000,
			 0x41d80000,
			 0x41f80000,

			 /// vpu register f28
			 0x41d00000,
			 0x41600000,
			 0x41e80000,
			 0x41000000,
			 0x41b80000,
			 0x41400000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f29
			 0x41900000,
			 0x41e00000,
			 0x41300000,
			 0x40400000,
			 0x41e00000,
			 0x40e00000,
			 0x41c80000,
			 0x41200000,

			 /// vpu register f30
			 0x40000000,
			 0x41b80000,
			 0x41c00000,
			 0x41500000,
			 0x40400000,
			 0x41e80000,
			 0x41600000,
			 0x42000000,

			 /// vpu register f31
			 0x40a00000,
			 0x40800000,
			 0x41c00000,
			 0x40400000,
			 0x41c00000,
			 0x41c00000,
			 0x3f800000,
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
		"fcvt.un10.ps f9, f12\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f11, f16\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f17, f28\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f21\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f8, f27\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f8, f9\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f12, f17\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f18, f20\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f18, f10\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f29, f3\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f24, f14\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f20, f22\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f29, f4\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f28, f23\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f29, f10\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f12, f27\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f25, f8\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f4\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f10, f3\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f30, f19\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f9, f25\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f21, f21\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f25, f3\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f22, f23\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f10, f12\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f18, f4\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f11, f2\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f21\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f20, f18\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f27, f5\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f12, f16\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f27, f27\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f2, f19\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f25, f7\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f25, f20\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f8, f24\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f4\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f26\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f27, f11\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f5, f8\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f30, f14\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f10, f4\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f27, f3\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f14, f23\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f10, f27\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f7, f24\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f11, f9\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f14, f25\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f6, f30\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f29\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f14, f12\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f2, f19\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f17, f10\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f26, f6\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f14, f29\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f18, f28\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f23, f29\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f25, f8\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f20, f22\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f3, f26\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f12, f17\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f23, f1\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f7, f26\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f3\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f17, f11\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f23, f13\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f7, f23\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f2, f14\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f10, f13\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f13, f24\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f25, f7\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f28, f2\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f21, f29\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f9, f7\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f12, f13\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f14, f4\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f13, f2\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f4, f17\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f17, f17\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f8, f28\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f11, f17\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f21, f2\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f21, f0\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f18, f7\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f9, f26\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f15, f10\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f18, f14\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f5, f23\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f9, f1\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f16, f24\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f9, f22\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f13, f18\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f6, f27\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f17, f10\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f13, f6\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f14, f23\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f5, f16\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f3, f0\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un10.ps f20, f30\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
