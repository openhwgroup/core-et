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
			 0x84d0736d, /// 0x0
			 0x59247ce3, /// 0x4
			 0xf37155d5, /// 0x8
			 0xcccc4d0e, /// 0xc
			 0x011f48ff, /// 0x10
			 0x2a46d6f3, /// 0x14
			 0xbf344dd0, /// 0x18
			 0x61022759, /// 0x1c
			 0x9ad28ec5, /// 0x20
			 0x99d87beb, /// 0x24
			 0x717e0dff, /// 0x28
			 0x687e7591, /// 0x2c
			 0x8d8d986e, /// 0x30
			 0x41f2396d, /// 0x34
			 0xd03a6b36, /// 0x38
			 0xf9168278, /// 0x3c
			 0x75d47e51, /// 0x40
			 0x16861b06, /// 0x44
			 0xc5d8c07b, /// 0x48
			 0x71904e55, /// 0x4c
			 0x0c296cf3, /// 0x50
			 0x17259263, /// 0x54
			 0x9ae5c58e, /// 0x58
			 0x3d3502da, /// 0x5c
			 0x124582d5, /// 0x60
			 0x1bb93b34, /// 0x64
			 0x570a2eb6, /// 0x68
			 0xd1a33ab6, /// 0x6c
			 0xac40eeb0, /// 0x70
			 0x743021ba, /// 0x74
			 0xc07bbd00, /// 0x78
			 0x5a8deaa9, /// 0x7c
			 0x9cc4ce8a, /// 0x80
			 0x977c6c3e, /// 0x84
			 0xe1c41f62, /// 0x88
			 0x292a7e03, /// 0x8c
			 0x40def4d3, /// 0x90
			 0x3d1dfaf7, /// 0x94
			 0x8952c76d, /// 0x98
			 0x62a60502, /// 0x9c
			 0xfa22ccd2, /// 0xa0
			 0x00d1b057, /// 0xa4
			 0xfed9def9, /// 0xa8
			 0x65bf416d, /// 0xac
			 0x341c397a, /// 0xb0
			 0x0ab0935b, /// 0xb4
			 0x5784c04e, /// 0xb8
			 0x139bc2ab, /// 0xbc
			 0x8a1a25b8, /// 0xc0
			 0xe9152f47, /// 0xc4
			 0xb9f0b4d4, /// 0xc8
			 0x37885c6c, /// 0xcc
			 0x08a06fb7, /// 0xd0
			 0x1062a315, /// 0xd4
			 0xa27d3cb3, /// 0xd8
			 0x24fff322, /// 0xdc
			 0xf5a8d9dd, /// 0xe0
			 0x59dcca6e, /// 0xe4
			 0x22c97bd8, /// 0xe8
			 0xedde10da, /// 0xec
			 0x4c34164c, /// 0xf0
			 0x3e47c478, /// 0xf4
			 0xd99fd08c, /// 0xf8
			 0xf5f91e07, /// 0xfc
			 0xe60f9cd2, /// 0x100
			 0xe119d8ef, /// 0x104
			 0x5c335948, /// 0x108
			 0xe1da531b, /// 0x10c
			 0xdc772a11, /// 0x110
			 0x64c40fc0, /// 0x114
			 0xe560084f, /// 0x118
			 0x0db1f057, /// 0x11c
			 0x4e76db27, /// 0x120
			 0x6f5b82a8, /// 0x124
			 0x7364ba93, /// 0x128
			 0x011ce760, /// 0x12c
			 0x6b218e83, /// 0x130
			 0x8ab2ff34, /// 0x134
			 0xe6f59568, /// 0x138
			 0xe4d3a9d6, /// 0x13c
			 0x0b40a961, /// 0x140
			 0x3a418e42, /// 0x144
			 0x3a7a3992, /// 0x148
			 0xca0e5328, /// 0x14c
			 0xa436b622, /// 0x150
			 0x76dc381e, /// 0x154
			 0xed43a1bb, /// 0x158
			 0x29397b84, /// 0x15c
			 0x0dab23ea, /// 0x160
			 0xb28ba90f, /// 0x164
			 0xc18432bd, /// 0x168
			 0x40f5c0ae, /// 0x16c
			 0x900673d6, /// 0x170
			 0x083aebf3, /// 0x174
			 0x19c0b18e, /// 0x178
			 0x97fb4e40, /// 0x17c
			 0xa1a53cab, /// 0x180
			 0xabd6debf, /// 0x184
			 0x1d54ac6c, /// 0x188
			 0x7fb83647, /// 0x18c
			 0x343cd356, /// 0x190
			 0x9f471e5f, /// 0x194
			 0x5741e7a5, /// 0x198
			 0x5e2827fe, /// 0x19c
			 0xcdb1e541, /// 0x1a0
			 0x1132596e, /// 0x1a4
			 0x83f848f8, /// 0x1a8
			 0xdb468cfd, /// 0x1ac
			 0xe56d8a6d, /// 0x1b0
			 0xafceb4a0, /// 0x1b4
			 0x887e4039, /// 0x1b8
			 0x9db3fcbc, /// 0x1bc
			 0xfff9ab0f, /// 0x1c0
			 0xdb414144, /// 0x1c4
			 0x89a49a8e, /// 0x1c8
			 0xbefab275, /// 0x1cc
			 0xed43b28d, /// 0x1d0
			 0xd2d415b7, /// 0x1d4
			 0x5f8d8edd, /// 0x1d8
			 0xd4d35e15, /// 0x1dc
			 0xcc3278cc, /// 0x1e0
			 0x64efa67b, /// 0x1e4
			 0xdc56176c, /// 0x1e8
			 0x9eacdf81, /// 0x1ec
			 0x969084a1, /// 0x1f0
			 0x3659455b, /// 0x1f4
			 0xc3280bd9, /// 0x1f8
			 0xf3e2eddd, /// 0x1fc
			 0x755f2390, /// 0x200
			 0xa549fe1e, /// 0x204
			 0x76c4b9ae, /// 0x208
			 0xd1bd11e6, /// 0x20c
			 0x3742426b, /// 0x210
			 0xec3c4904, /// 0x214
			 0x10b3f017, /// 0x218
			 0xac183521, /// 0x21c
			 0xf89039a5, /// 0x220
			 0x8f426deb, /// 0x224
			 0x88cc09b2, /// 0x228
			 0xec8c4d15, /// 0x22c
			 0xfa14ca70, /// 0x230
			 0x4d2b17e9, /// 0x234
			 0x210e0cb5, /// 0x238
			 0xf6da1974, /// 0x23c
			 0x4f25db8d, /// 0x240
			 0x8ceda754, /// 0x244
			 0xf2e07510, /// 0x248
			 0xd7dbb927, /// 0x24c
			 0xf902e11c, /// 0x250
			 0x2d562135, /// 0x254
			 0xc67a9afe, /// 0x258
			 0x4c4307fe, /// 0x25c
			 0x78c415ce, /// 0x260
			 0x963e6a2e, /// 0x264
			 0xeb532b42, /// 0x268
			 0x045f9fb3, /// 0x26c
			 0xcbc3e3c9, /// 0x270
			 0x66689370, /// 0x274
			 0xc4e735e8, /// 0x278
			 0x93d82257, /// 0x27c
			 0xdcfd06b1, /// 0x280
			 0xd84b4970, /// 0x284
			 0x6b94b020, /// 0x288
			 0x97de9b19, /// 0x28c
			 0x892392ca, /// 0x290
			 0x00a894d1, /// 0x294
			 0x2620570f, /// 0x298
			 0x8096ab03, /// 0x29c
			 0xe2844c17, /// 0x2a0
			 0x610c6d5e, /// 0x2a4
			 0x83283760, /// 0x2a8
			 0x49dce43a, /// 0x2ac
			 0x3150cfd8, /// 0x2b0
			 0x8a1d7d63, /// 0x2b4
			 0xc3aeb2a3, /// 0x2b8
			 0x647610b5, /// 0x2bc
			 0xa116b051, /// 0x2c0
			 0xacbd5f14, /// 0x2c4
			 0x527f0f64, /// 0x2c8
			 0xced90557, /// 0x2cc
			 0x9723fa74, /// 0x2d0
			 0xaf712090, /// 0x2d4
			 0x70aa289b, /// 0x2d8
			 0x2f698f96, /// 0x2dc
			 0xfd049d89, /// 0x2e0
			 0x0ea03d3d, /// 0x2e4
			 0xbe426323, /// 0x2e8
			 0xc32e1d15, /// 0x2ec
			 0x4ea90558, /// 0x2f0
			 0x3aa0fb05, /// 0x2f4
			 0x47c96d1f, /// 0x2f8
			 0x70faee4d, /// 0x2fc
			 0x4c077006, /// 0x300
			 0x65860a64, /// 0x304
			 0x0ce217ab, /// 0x308
			 0x99ad0a47, /// 0x30c
			 0x93d541e6, /// 0x310
			 0xe899d3b0, /// 0x314
			 0x82f5e53f, /// 0x318
			 0x483b0afe, /// 0x31c
			 0x20b312e5, /// 0x320
			 0xb1a4d74d, /// 0x324
			 0x98620b65, /// 0x328
			 0xfc88559a, /// 0x32c
			 0xa71c177a, /// 0x330
			 0x2ab267d8, /// 0x334
			 0xa6cf4ca5, /// 0x338
			 0x27f7cba5, /// 0x33c
			 0xbba1b569, /// 0x340
			 0xef639d48, /// 0x344
			 0x7cb8ac90, /// 0x348
			 0xe35770f0, /// 0x34c
			 0xc1413f04, /// 0x350
			 0x4ec78dae, /// 0x354
			 0xece24c9c, /// 0x358
			 0x7e6f7ebe, /// 0x35c
			 0xc1ed83e9, /// 0x360
			 0xd431f0e2, /// 0x364
			 0x7c9b0b33, /// 0x368
			 0x4e603eca, /// 0x36c
			 0xeb1791ed, /// 0x370
			 0x3e2d8ee7, /// 0x374
			 0x21342d64, /// 0x378
			 0xaf8d8292, /// 0x37c
			 0x00d7c257, /// 0x380
			 0xe060bed9, /// 0x384
			 0x2c49dcc2, /// 0x388
			 0x97714b27, /// 0x38c
			 0x8dc419b0, /// 0x390
			 0x49d40078, /// 0x394
			 0x57d527a6, /// 0x398
			 0x70f4a4a1, /// 0x39c
			 0x80bdd646, /// 0x3a0
			 0x4c0a3237, /// 0x3a4
			 0xc9ffe228, /// 0x3a8
			 0xaf6518a3, /// 0x3ac
			 0x9ba82895, /// 0x3b0
			 0x7ddfb337, /// 0x3b4
			 0xe7c05faf, /// 0x3b8
			 0x6e778fdd, /// 0x3bc
			 0x972b33a4, /// 0x3c0
			 0x4539365f, /// 0x3c4
			 0xf8100c03, /// 0x3c8
			 0x6a9741fc, /// 0x3cc
			 0x34a86a1c, /// 0x3d0
			 0x786e671e, /// 0x3d4
			 0x0a9d8d2d, /// 0x3d8
			 0xc8f39e28, /// 0x3dc
			 0xfc51c32d, /// 0x3e0
			 0xd5d85451, /// 0x3e4
			 0x3f8a9ee7, /// 0x3e8
			 0xdbabe903, /// 0x3ec
			 0xf7b20802, /// 0x3f0
			 0x06227795, /// 0x3f4
			 0x806bd135, /// 0x3f8
			 0x3d2cbe72, /// 0x3fc
			 0xa8d8faaf, /// 0x400
			 0x73d67637, /// 0x404
			 0x3718a89e, /// 0x408
			 0xc1cb10ee, /// 0x40c
			 0x1c46abc0, /// 0x410
			 0x72a22112, /// 0x414
			 0xe96d547d, /// 0x418
			 0xe476bcbe, /// 0x41c
			 0x91c88a04, /// 0x420
			 0x69dee035, /// 0x424
			 0x94db428f, /// 0x428
			 0xf9494fb0, /// 0x42c
			 0x6210fa6c, /// 0x430
			 0xb2c29328, /// 0x434
			 0x6101b0e9, /// 0x438
			 0xcf8e9846, /// 0x43c
			 0xd3706536, /// 0x440
			 0x2c34b964, /// 0x444
			 0x030c1b6e, /// 0x448
			 0x42eb4c08, /// 0x44c
			 0x48996d90, /// 0x450
			 0x75aae556, /// 0x454
			 0x412e4745, /// 0x458
			 0x9e655405, /// 0x45c
			 0xbfc308b7, /// 0x460
			 0xf1c69cb3, /// 0x464
			 0x1aacc332, /// 0x468
			 0xe63bcdf2, /// 0x46c
			 0xd8786fa6, /// 0x470
			 0xaac1499b, /// 0x474
			 0x845a5620, /// 0x478
			 0x8872cad4, /// 0x47c
			 0xff2a4d66, /// 0x480
			 0x3b51435a, /// 0x484
			 0x9255be27, /// 0x488
			 0xbd107b22, /// 0x48c
			 0xefbef39d, /// 0x490
			 0x8d8ab1f2, /// 0x494
			 0x274fca01, /// 0x498
			 0xca1bab5d, /// 0x49c
			 0x914949fc, /// 0x4a0
			 0xb0e6bea7, /// 0x4a4
			 0x9e0f5d88, /// 0x4a8
			 0x53c008d5, /// 0x4ac
			 0x77c08e05, /// 0x4b0
			 0x834be01f, /// 0x4b4
			 0x6804ad17, /// 0x4b8
			 0xa7364649, /// 0x4bc
			 0xbeb63780, /// 0x4c0
			 0xb99fda76, /// 0x4c4
			 0xfe86363e, /// 0x4c8
			 0x4ffec2cf, /// 0x4cc
			 0xf1bda787, /// 0x4d0
			 0xb81adeeb, /// 0x4d4
			 0xc3bdad7b, /// 0x4d8
			 0xdd15ead1, /// 0x4dc
			 0xb22bce56, /// 0x4e0
			 0xe30ed29b, /// 0x4e4
			 0x73840ec4, /// 0x4e8
			 0x3f408902, /// 0x4ec
			 0xd0158b26, /// 0x4f0
			 0x2dbf2a48, /// 0x4f4
			 0x669b87a7, /// 0x4f8
			 0xf2eeaf06, /// 0x4fc
			 0x7122aae7, /// 0x500
			 0x86e6bf89, /// 0x504
			 0x7bdeb48d, /// 0x508
			 0x5951e141, /// 0x50c
			 0xbac54b42, /// 0x510
			 0xf1433be0, /// 0x514
			 0x7bd9e450, /// 0x518
			 0xcdd06892, /// 0x51c
			 0xa33a67ec, /// 0x520
			 0x18234a30, /// 0x524
			 0x9eae7cea, /// 0x528
			 0x3b192509, /// 0x52c
			 0x1a88d360, /// 0x530
			 0x2e3b21c4, /// 0x534
			 0x5696c2a7, /// 0x538
			 0xb1b7c840, /// 0x53c
			 0x61ec8782, /// 0x540
			 0xf69e58be, /// 0x544
			 0x66043416, /// 0x548
			 0x517a8dca, /// 0x54c
			 0x8df5b12c, /// 0x550
			 0x4ace3ca3, /// 0x554
			 0x23f812fb, /// 0x558
			 0x1ed51740, /// 0x55c
			 0x64dd6b4a, /// 0x560
			 0x12db7217, /// 0x564
			 0x979eeed0, /// 0x568
			 0xb230de5c, /// 0x56c
			 0x133c03ad, /// 0x570
			 0x73f8bc19, /// 0x574
			 0x59691417, /// 0x578
			 0x64dc29d0, /// 0x57c
			 0x1d191719, /// 0x580
			 0x2c9af915, /// 0x584
			 0x2ba9584c, /// 0x588
			 0x283c2fcb, /// 0x58c
			 0xc32cfeda, /// 0x590
			 0x3bd3a591, /// 0x594
			 0x1d4cdd5c, /// 0x598
			 0x7ed7d897, /// 0x59c
			 0x61a7f61b, /// 0x5a0
			 0x1e9b377f, /// 0x5a4
			 0x36e347af, /// 0x5a8
			 0x0556bdfd, /// 0x5ac
			 0x6e63b324, /// 0x5b0
			 0x31c49f70, /// 0x5b4
			 0x81219fa9, /// 0x5b8
			 0x2fa61cf4, /// 0x5bc
			 0x53e5ecaf, /// 0x5c0
			 0xaaf9960c, /// 0x5c4
			 0x5a07d47b, /// 0x5c8
			 0x3547bcfd, /// 0x5cc
			 0x102b25f5, /// 0x5d0
			 0xa143bb9a, /// 0x5d4
			 0xb4e90a0b, /// 0x5d8
			 0x96dc6dec, /// 0x5dc
			 0x70e69bf1, /// 0x5e0
			 0xed217e50, /// 0x5e4
			 0x07331a6f, /// 0x5e8
			 0x01eeaba5, /// 0x5ec
			 0xa898ba5d, /// 0x5f0
			 0x36263e82, /// 0x5f4
			 0x32da1074, /// 0x5f8
			 0x59cb4959, /// 0x5fc
			 0x234b5d28, /// 0x600
			 0x2509be22, /// 0x604
			 0x526c75ef, /// 0x608
			 0xc8dc249e, /// 0x60c
			 0x8cc44902, /// 0x610
			 0xcc2f280d, /// 0x614
			 0x272885a8, /// 0x618
			 0x73fdac77, /// 0x61c
			 0xc2be14b3, /// 0x620
			 0x8bdfbb06, /// 0x624
			 0xcf52fa05, /// 0x628
			 0xba740ba8, /// 0x62c
			 0x6283d61f, /// 0x630
			 0xc26d74fa, /// 0x634
			 0x5304116e, /// 0x638
			 0x1f6c1bc1, /// 0x63c
			 0xc9c8e68a, /// 0x640
			 0xef1cf456, /// 0x644
			 0xa82c67a2, /// 0x648
			 0xa629dae6, /// 0x64c
			 0x3500a1e2, /// 0x650
			 0xa2dab3de, /// 0x654
			 0xba593faf, /// 0x658
			 0x69a88b32, /// 0x65c
			 0x136a201f, /// 0x660
			 0xc562ee36, /// 0x664
			 0xdf0b3a99, /// 0x668
			 0x1ddcf133, /// 0x66c
			 0x47f5c3c4, /// 0x670
			 0x59cb3a2a, /// 0x674
			 0x1337cc29, /// 0x678
			 0x30efe94c, /// 0x67c
			 0xfcfa24eb, /// 0x680
			 0x5a5c35e3, /// 0x684
			 0xc38a06a0, /// 0x688
			 0xe8a9c2d5, /// 0x68c
			 0x0faffb06, /// 0x690
			 0x94196a24, /// 0x694
			 0x6f35de62, /// 0x698
			 0x26f154aa, /// 0x69c
			 0x6fd937da, /// 0x6a0
			 0x814aef2b, /// 0x6a4
			 0xd381557e, /// 0x6a8
			 0xcec870dd, /// 0x6ac
			 0xa9f844b6, /// 0x6b0
			 0x7e52f964, /// 0x6b4
			 0xf8d73859, /// 0x6b8
			 0x8626bc34, /// 0x6bc
			 0xcd1c83dd, /// 0x6c0
			 0x29be9903, /// 0x6c4
			 0xf9ba5ed9, /// 0x6c8
			 0x5103f7fc, /// 0x6cc
			 0xbb364a27, /// 0x6d0
			 0xb2288243, /// 0x6d4
			 0x76cd87d0, /// 0x6d8
			 0x0194b68c, /// 0x6dc
			 0xb29983b2, /// 0x6e0
			 0xc6bcdea4, /// 0x6e4
			 0x8c76cd11, /// 0x6e8
			 0x51fb36cc, /// 0x6ec
			 0xed8579d5, /// 0x6f0
			 0xd2e72000, /// 0x6f4
			 0xfb28a526, /// 0x6f8
			 0x2503d0e8, /// 0x6fc
			 0x5e551577, /// 0x700
			 0x9c1a97e8, /// 0x704
			 0xb842bd75, /// 0x708
			 0x6a190a2d, /// 0x70c
			 0x5fd07b2d, /// 0x710
			 0x83baea98, /// 0x714
			 0x5ec9f0d3, /// 0x718
			 0x1cbbd1fa, /// 0x71c
			 0x13ab18c7, /// 0x720
			 0x4a95695e, /// 0x724
			 0xaa057eb2, /// 0x728
			 0x22814591, /// 0x72c
			 0x8fdadc98, /// 0x730
			 0xf9101f30, /// 0x734
			 0x20922890, /// 0x738
			 0x2d857f7e, /// 0x73c
			 0x28b067e9, /// 0x740
			 0xfcf87108, /// 0x744
			 0x3a4ac7cf, /// 0x748
			 0x49947661, /// 0x74c
			 0xbeacf27d, /// 0x750
			 0x6c5be5e1, /// 0x754
			 0xa659389b, /// 0x758
			 0x9d76a679, /// 0x75c
			 0xc653de31, /// 0x760
			 0xdf8be2b6, /// 0x764
			 0x93cb2b45, /// 0x768
			 0x4390d5e6, /// 0x76c
			 0xae6b6944, /// 0x770
			 0x89656e54, /// 0x774
			 0x759b92ad, /// 0x778
			 0xdcbb52b5, /// 0x77c
			 0xa0b558df, /// 0x780
			 0x81effabf, /// 0x784
			 0xeffd73dd, /// 0x788
			 0xfa0aca8c, /// 0x78c
			 0x557fbff0, /// 0x790
			 0xc9730939, /// 0x794
			 0x638c6266, /// 0x798
			 0x5da9d139, /// 0x79c
			 0x093aa5f6, /// 0x7a0
			 0xec85aa47, /// 0x7a4
			 0xb74d824e, /// 0x7a8
			 0x16d9cdeb, /// 0x7ac
			 0x26a4afd1, /// 0x7b0
			 0xdd259bdb, /// 0x7b4
			 0x5b9f1803, /// 0x7b8
			 0xd45feefa, /// 0x7bc
			 0x4564ebff, /// 0x7c0
			 0xe9619356, /// 0x7c4
			 0x12a9738f, /// 0x7c8
			 0x51ab900a, /// 0x7cc
			 0x02362c92, /// 0x7d0
			 0x03dda97b, /// 0x7d4
			 0x4edf2c3c, /// 0x7d8
			 0x563cf2d3, /// 0x7dc
			 0x2e16769b, /// 0x7e0
			 0x917c3911, /// 0x7e4
			 0x2c8d4b74, /// 0x7e8
			 0x8fb1cd0c, /// 0x7ec
			 0x34d209fd, /// 0x7f0
			 0xf810927d, /// 0x7f4
			 0x5a2f11e5, /// 0x7f8
			 0xc0ccbf7e, /// 0x7fc
			 0xd63f7e94, /// 0x800
			 0xc5f95526, /// 0x804
			 0x4d569ce5, /// 0x808
			 0x747bec1d, /// 0x80c
			 0xab630daf, /// 0x810
			 0xaf664027, /// 0x814
			 0x9ebf4f7d, /// 0x818
			 0x590813c5, /// 0x81c
			 0x9e324537, /// 0x820
			 0x3b360966, /// 0x824
			 0xea1bcbdb, /// 0x828
			 0x097dd423, /// 0x82c
			 0x3ed62fc9, /// 0x830
			 0x6a2a52dd, /// 0x834
			 0xb3f0dccf, /// 0x838
			 0xb04003e2, /// 0x83c
			 0x5072b63a, /// 0x840
			 0x4b1985d8, /// 0x844
			 0x9c9f00f4, /// 0x848
			 0x6b2bda62, /// 0x84c
			 0xd5214590, /// 0x850
			 0x742569ad, /// 0x854
			 0x55cc261d, /// 0x858
			 0x9103fb5f, /// 0x85c
			 0x165822bd, /// 0x860
			 0xe187dda3, /// 0x864
			 0x5672f1ab, /// 0x868
			 0x2a961b08, /// 0x86c
			 0x1c295ff8, /// 0x870
			 0x0b332002, /// 0x874
			 0x18e3b18d, /// 0x878
			 0x2a4c4b43, /// 0x87c
			 0x5c4c4067, /// 0x880
			 0xf52e517d, /// 0x884
			 0xebcc00ad, /// 0x888
			 0x3f493e2e, /// 0x88c
			 0x40424c71, /// 0x890
			 0x7dff5f5b, /// 0x894
			 0xff883262, /// 0x898
			 0x702c7422, /// 0x89c
			 0xb587c615, /// 0x8a0
			 0x39c2bc25, /// 0x8a4
			 0x77253088, /// 0x8a8
			 0x3d309068, /// 0x8ac
			 0x64e104dc, /// 0x8b0
			 0x372ff23b, /// 0x8b4
			 0xa18761a2, /// 0x8b8
			 0x91fa8051, /// 0x8bc
			 0xb5fa846f, /// 0x8c0
			 0x84b75658, /// 0x8c4
			 0x289ad037, /// 0x8c8
			 0x988972d6, /// 0x8cc
			 0x6a522768, /// 0x8d0
			 0x4c6e78d0, /// 0x8d4
			 0x1b885eb3, /// 0x8d8
			 0xdd0f2e8d, /// 0x8dc
			 0x322e8700, /// 0x8e0
			 0x565763fa, /// 0x8e4
			 0x39ffee5f, /// 0x8e8
			 0x00b08b0c, /// 0x8ec
			 0xc6dcc972, /// 0x8f0
			 0xb171ed9f, /// 0x8f4
			 0x00687ecd, /// 0x8f8
			 0xd94ed397, /// 0x8fc
			 0x40217151, /// 0x900
			 0x57fdd403, /// 0x904
			 0xdc542bb1, /// 0x908
			 0x38efc889, /// 0x90c
			 0x175a7f3a, /// 0x910
			 0xd2ae0331, /// 0x914
			 0xc35f7b54, /// 0x918
			 0x4c450309, /// 0x91c
			 0xb495f289, /// 0x920
			 0xa49fed32, /// 0x924
			 0x1843ffd0, /// 0x928
			 0xa3c44b66, /// 0x92c
			 0x7fadbc4f, /// 0x930
			 0x83a64290, /// 0x934
			 0x0b8edb4e, /// 0x938
			 0x560f54eb, /// 0x93c
			 0x9fd0afd7, /// 0x940
			 0x4a4cc000, /// 0x944
			 0x1f794b61, /// 0x948
			 0x8598abaa, /// 0x94c
			 0x6b89a6c3, /// 0x950
			 0x21806632, /// 0x954
			 0x94346c64, /// 0x958
			 0x81f9e8c2, /// 0x95c
			 0x594b7d97, /// 0x960
			 0xbf9997c5, /// 0x964
			 0x5491e2ed, /// 0x968
			 0x075624fc, /// 0x96c
			 0xd43f8e40, /// 0x970
			 0x61c13108, /// 0x974
			 0xe45d0ce1, /// 0x978
			 0x990eb961, /// 0x97c
			 0x76acbbe8, /// 0x980
			 0xf7e7c316, /// 0x984
			 0x790dad97, /// 0x988
			 0x97a7ad27, /// 0x98c
			 0xb9d77934, /// 0x990
			 0x6ab3ae7e, /// 0x994
			 0xc6b61893, /// 0x998
			 0x994ddcdb, /// 0x99c
			 0xd474115b, /// 0x9a0
			 0xbf2ef2c4, /// 0x9a4
			 0x3c8ec964, /// 0x9a8
			 0xa08c714a, /// 0x9ac
			 0x591e1a40, /// 0x9b0
			 0xb1b47a90, /// 0x9b4
			 0x65e18aa1, /// 0x9b8
			 0xad43cc9d, /// 0x9bc
			 0x9ccf46ab, /// 0x9c0
			 0x5c02f07d, /// 0x9c4
			 0x184df81f, /// 0x9c8
			 0x741b63c9, /// 0x9cc
			 0x67c0b133, /// 0x9d0
			 0x852ee482, /// 0x9d4
			 0x91880c40, /// 0x9d8
			 0xf771b602, /// 0x9dc
			 0x78910871, /// 0x9e0
			 0x80a417b2, /// 0x9e4
			 0xf0a11cd0, /// 0x9e8
			 0xb87b30cb, /// 0x9ec
			 0x12420d25, /// 0x9f0
			 0x990ee84a, /// 0x9f4
			 0xf173749a, /// 0x9f8
			 0xcdbc031d, /// 0x9fc
			 0x656d8038, /// 0xa00
			 0xc0dd9795, /// 0xa04
			 0x133cbc31, /// 0xa08
			 0x1cda91e0, /// 0xa0c
			 0x24d16ad0, /// 0xa10
			 0x84a504f9, /// 0xa14
			 0x5c61ca0e, /// 0xa18
			 0xc71d3338, /// 0xa1c
			 0x6412fe02, /// 0xa20
			 0x37debfcb, /// 0xa24
			 0xd4478ee1, /// 0xa28
			 0x52389a58, /// 0xa2c
			 0xa58d3bd4, /// 0xa30
			 0xeaeaeeee, /// 0xa34
			 0xbbe3a8dc, /// 0xa38
			 0x01edf856, /// 0xa3c
			 0x95efa7c5, /// 0xa40
			 0x3cd7ab96, /// 0xa44
			 0x90addb5d, /// 0xa48
			 0x15f119f0, /// 0xa4c
			 0xef60c830, /// 0xa50
			 0x837cbd9b, /// 0xa54
			 0x2d6b3fd8, /// 0xa58
			 0x6d4ce27c, /// 0xa5c
			 0x98c086ec, /// 0xa60
			 0xee7ba44c, /// 0xa64
			 0xfbc63532, /// 0xa68
			 0xddb5a98d, /// 0xa6c
			 0xd025dc61, /// 0xa70
			 0xaf8f0c23, /// 0xa74
			 0x5a8a386f, /// 0xa78
			 0x7b5f63d8, /// 0xa7c
			 0x0958e1fa, /// 0xa80
			 0x7bb63e7a, /// 0xa84
			 0xd2e93987, /// 0xa88
			 0xdb5201e6, /// 0xa8c
			 0xc4822af6, /// 0xa90
			 0xd1ad3b10, /// 0xa94
			 0x3256f325, /// 0xa98
			 0x4ba5fab5, /// 0xa9c
			 0xae594b05, /// 0xaa0
			 0x4a825e04, /// 0xaa4
			 0xd4106bf7, /// 0xaa8
			 0x2735faa8, /// 0xaac
			 0x1fee8b64, /// 0xab0
			 0x72d8d0b3, /// 0xab4
			 0x00d81a9f, /// 0xab8
			 0xdb9fc0fe, /// 0xabc
			 0x63c7284c, /// 0xac0
			 0xb7cc6cc1, /// 0xac4
			 0x0e9f9e21, /// 0xac8
			 0xa93bc024, /// 0xacc
			 0x9cafbbe2, /// 0xad0
			 0x08b55548, /// 0xad4
			 0xc405547b, /// 0xad8
			 0x5ca9accb, /// 0xadc
			 0x535c1e7f, /// 0xae0
			 0x51436b63, /// 0xae4
			 0xc46973c5, /// 0xae8
			 0xf9acfb6a, /// 0xaec
			 0x5c1ccfab, /// 0xaf0
			 0x44c7fa64, /// 0xaf4
			 0xab042a64, /// 0xaf8
			 0xcba6089a, /// 0xafc
			 0xf37f20ce, /// 0xb00
			 0xb3f82867, /// 0xb04
			 0x280684ca, /// 0xb08
			 0xf89a1b9a, /// 0xb0c
			 0xd07fbda8, /// 0xb10
			 0xb98cff35, /// 0xb14
			 0x6d0ed073, /// 0xb18
			 0x8799cd4f, /// 0xb1c
			 0xadabce66, /// 0xb20
			 0xb6cc5e1b, /// 0xb24
			 0xcc423d47, /// 0xb28
			 0x894121f0, /// 0xb2c
			 0xa5187b0f, /// 0xb30
			 0x4044e9a4, /// 0xb34
			 0xe98abb86, /// 0xb38
			 0x9b0e0a72, /// 0xb3c
			 0xd6ba63db, /// 0xb40
			 0xc490dfd3, /// 0xb44
			 0x59353157, /// 0xb48
			 0x35b22bbd, /// 0xb4c
			 0x9a501a86, /// 0xb50
			 0x72ee03a0, /// 0xb54
			 0x43f36933, /// 0xb58
			 0x6d77b868, /// 0xb5c
			 0x936528d7, /// 0xb60
			 0x4f0bc331, /// 0xb64
			 0x494cbf42, /// 0xb68
			 0x555995be, /// 0xb6c
			 0xb437c699, /// 0xb70
			 0xe71122ad, /// 0xb74
			 0xb1f7c354, /// 0xb78
			 0xecb03161, /// 0xb7c
			 0xf9f4e595, /// 0xb80
			 0xcf72c956, /// 0xb84
			 0x0dd783e2, /// 0xb88
			 0x625dca67, /// 0xb8c
			 0x4302e317, /// 0xb90
			 0xf5ee7b58, /// 0xb94
			 0x5f31a79f, /// 0xb98
			 0xa44954a6, /// 0xb9c
			 0xa34a9fb7, /// 0xba0
			 0x1d588ad0, /// 0xba4
			 0xbfd69502, /// 0xba8
			 0x98083be2, /// 0xbac
			 0x555e2faa, /// 0xbb0
			 0x07d169bb, /// 0xbb4
			 0x0dcc1a53, /// 0xbb8
			 0x44f14b46, /// 0xbbc
			 0xf8bfde9d, /// 0xbc0
			 0x683993aa, /// 0xbc4
			 0x85930e41, /// 0xbc8
			 0xd0b32362, /// 0xbcc
			 0x9a6bad32, /// 0xbd0
			 0xf039461f, /// 0xbd4
			 0x39720e4e, /// 0xbd8
			 0x838613ae, /// 0xbdc
			 0x0e77ec95, /// 0xbe0
			 0x79a8f279, /// 0xbe4
			 0x9f86b916, /// 0xbe8
			 0xab0b08cb, /// 0xbec
			 0x479c444f, /// 0xbf0
			 0xfe6a0097, /// 0xbf4
			 0x6211d9cd, /// 0xbf8
			 0xd7ac74aa, /// 0xbfc
			 0x33e5e386, /// 0xc00
			 0xd0a02fc4, /// 0xc04
			 0x0b378e53, /// 0xc08
			 0x06680038, /// 0xc0c
			 0x72f86462, /// 0xc10
			 0x7aceb031, /// 0xc14
			 0x3690027b, /// 0xc18
			 0x1867c173, /// 0xc1c
			 0x110cec3f, /// 0xc20
			 0xd4b6c47f, /// 0xc24
			 0x31f21a93, /// 0xc28
			 0x6deb587a, /// 0xc2c
			 0x1e2e3cfc, /// 0xc30
			 0xb9bb5789, /// 0xc34
			 0x466b3e72, /// 0xc38
			 0xf4a66bd1, /// 0xc3c
			 0xcbafea8b, /// 0xc40
			 0x0e419b0f, /// 0xc44
			 0xecf5966d, /// 0xc48
			 0xc71a0ab7, /// 0xc4c
			 0xb2f954fc, /// 0xc50
			 0x59f8d7ba, /// 0xc54
			 0x2ba0cf73, /// 0xc58
			 0x6d6c6458, /// 0xc5c
			 0xd311a206, /// 0xc60
			 0xbc177a54, /// 0xc64
			 0x52730821, /// 0xc68
			 0x9fd18e07, /// 0xc6c
			 0x7aa1c580, /// 0xc70
			 0x7d161017, /// 0xc74
			 0x1cc6eda7, /// 0xc78
			 0x4278a35f, /// 0xc7c
			 0x761586d4, /// 0xc80
			 0x1dabc5ea, /// 0xc84
			 0x463b1dc5, /// 0xc88
			 0x303f14cb, /// 0xc8c
			 0x8c906ae9, /// 0xc90
			 0xefe8ea24, /// 0xc94
			 0xb3620355, /// 0xc98
			 0x9657f1f8, /// 0xc9c
			 0xfdb98820, /// 0xca0
			 0x3759511a, /// 0xca4
			 0xade3c952, /// 0xca8
			 0xed99b5e5, /// 0xcac
			 0x4c2a1e09, /// 0xcb0
			 0x91d9e3d2, /// 0xcb4
			 0xc3b27e8b, /// 0xcb8
			 0xc141f6ad, /// 0xcbc
			 0xc4f67af7, /// 0xcc0
			 0xf2bb5b5d, /// 0xcc4
			 0xb25df5cd, /// 0xcc8
			 0x1f339d06, /// 0xccc
			 0x58e59d42, /// 0xcd0
			 0x1e4d54e8, /// 0xcd4
			 0x8e09846f, /// 0xcd8
			 0xaa158645, /// 0xcdc
			 0xd44cd9e8, /// 0xce0
			 0x84b68799, /// 0xce4
			 0xf89a425d, /// 0xce8
			 0x1d34decd, /// 0xcec
			 0x6f049743, /// 0xcf0
			 0xbb59cc7a, /// 0xcf4
			 0x93a9f13e, /// 0xcf8
			 0x1a0e965a, /// 0xcfc
			 0x21148e83, /// 0xd00
			 0xd3b45dca, /// 0xd04
			 0x8cac136a, /// 0xd08
			 0xa1abe666, /// 0xd0c
			 0x0e9b63ba, /// 0xd10
			 0x2e60801c, /// 0xd14
			 0x9a49d50f, /// 0xd18
			 0xaf66ddf2, /// 0xd1c
			 0x98859355, /// 0xd20
			 0x9cec63b7, /// 0xd24
			 0x06c3a223, /// 0xd28
			 0xc84717bd, /// 0xd2c
			 0x5223fcb1, /// 0xd30
			 0x8786b78e, /// 0xd34
			 0x3d591eaf, /// 0xd38
			 0x05f4efff, /// 0xd3c
			 0x59ccf180, /// 0xd40
			 0xf724482b, /// 0xd44
			 0x8f11f7e5, /// 0xd48
			 0x127da9c0, /// 0xd4c
			 0xeec402c4, /// 0xd50
			 0x8b98819d, /// 0xd54
			 0x83ad02f8, /// 0xd58
			 0x0198c4c2, /// 0xd5c
			 0x12bb265a, /// 0xd60
			 0x4fdd1294, /// 0xd64
			 0x4f1e6488, /// 0xd68
			 0x9d15bbe4, /// 0xd6c
			 0xaca4d067, /// 0xd70
			 0xd86f51ef, /// 0xd74
			 0x4c4e9721, /// 0xd78
			 0xdc264803, /// 0xd7c
			 0xc34087b1, /// 0xd80
			 0xeeb394ff, /// 0xd84
			 0x98db1ed5, /// 0xd88
			 0xd0fc1e2b, /// 0xd8c
			 0x680293f1, /// 0xd90
			 0xcb9b6a45, /// 0xd94
			 0x071433ed, /// 0xd98
			 0x13ec1908, /// 0xd9c
			 0x7a1bf65e, /// 0xda0
			 0xf6e76b34, /// 0xda4
			 0xd533d0ea, /// 0xda8
			 0x155a6352, /// 0xdac
			 0x493c6ce5, /// 0xdb0
			 0x448100f2, /// 0xdb4
			 0x9c81bdf7, /// 0xdb8
			 0x8e452f0c, /// 0xdbc
			 0x53d9b6cd, /// 0xdc0
			 0xdc36f2ec, /// 0xdc4
			 0xf6777a27, /// 0xdc8
			 0xc653e82a, /// 0xdcc
			 0xde91ae04, /// 0xdd0
			 0xfc326f57, /// 0xdd4
			 0xdc2d2b2c, /// 0xdd8
			 0xc9e73525, /// 0xddc
			 0x98d2897b, /// 0xde0
			 0x764701be, /// 0xde4
			 0xfd4742fd, /// 0xde8
			 0x394caaf4, /// 0xdec
			 0x005172f0, /// 0xdf0
			 0x4c02d410, /// 0xdf4
			 0x5b5ac1af, /// 0xdf8
			 0xe60fbec6, /// 0xdfc
			 0x8630aa8f, /// 0xe00
			 0xe94323db, /// 0xe04
			 0x65c11bc1, /// 0xe08
			 0x4339a33a, /// 0xe0c
			 0x3c18c566, /// 0xe10
			 0xb2341372, /// 0xe14
			 0xdbfd3d13, /// 0xe18
			 0xfb18cc7d, /// 0xe1c
			 0xf7372f87, /// 0xe20
			 0xa661922f, /// 0xe24
			 0xd9a0404c, /// 0xe28
			 0xe259a61e, /// 0xe2c
			 0x5ee7b3fe, /// 0xe30
			 0x4dce4f1f, /// 0xe34
			 0xe9cadbda, /// 0xe38
			 0x64d755a9, /// 0xe3c
			 0x7c7ef654, /// 0xe40
			 0x20616cd3, /// 0xe44
			 0x623e863a, /// 0xe48
			 0x0904b329, /// 0xe4c
			 0xeb594a58, /// 0xe50
			 0x238806c6, /// 0xe54
			 0x2a7f3012, /// 0xe58
			 0x19c7a9e8, /// 0xe5c
			 0xe846adf9, /// 0xe60
			 0x3eaea77d, /// 0xe64
			 0xaefc75f8, /// 0xe68
			 0x73864152, /// 0xe6c
			 0x2386e91f, /// 0xe70
			 0x0dde2b4f, /// 0xe74
			 0xc5a02f2b, /// 0xe78
			 0x5a448386, /// 0xe7c
			 0xf4d2651a, /// 0xe80
			 0x10fadc57, /// 0xe84
			 0x530d93f9, /// 0xe88
			 0x07c7d763, /// 0xe8c
			 0xe83d4a38, /// 0xe90
			 0xc3bf1024, /// 0xe94
			 0xfcbbf90b, /// 0xe98
			 0xf1072af5, /// 0xe9c
			 0x9342013a, /// 0xea0
			 0x19e18ff5, /// 0xea4
			 0x7cc03f6a, /// 0xea8
			 0x9f02e946, /// 0xeac
			 0xb3089e1f, /// 0xeb0
			 0x897b1a53, /// 0xeb4
			 0xa5c8a1f8, /// 0xeb8
			 0xe1392125, /// 0xebc
			 0x1407cd49, /// 0xec0
			 0x164ce31d, /// 0xec4
			 0xec1838cb, /// 0xec8
			 0x37d39a70, /// 0xecc
			 0x8e113124, /// 0xed0
			 0xc21b9ee5, /// 0xed4
			 0xaee300e8, /// 0xed8
			 0xa27b78eb, /// 0xedc
			 0x78a4d412, /// 0xee0
			 0x8be56ecb, /// 0xee4
			 0xdc92e15a, /// 0xee8
			 0xc2565eab, /// 0xeec
			 0x0b05643b, /// 0xef0
			 0xe2d9959f, /// 0xef4
			 0xbeb5d1d5, /// 0xef8
			 0x3ddb89d6, /// 0xefc
			 0xc3679874, /// 0xf00
			 0x3f38ed64, /// 0xf04
			 0xfebf0121, /// 0xf08
			 0x9a093bef, /// 0xf0c
			 0x50c91440, /// 0xf10
			 0x2f22e31c, /// 0xf14
			 0x68d1b93b, /// 0xf18
			 0x61277607, /// 0xf1c
			 0x5cac761d, /// 0xf20
			 0x12d4e73a, /// 0xf24
			 0x912c022b, /// 0xf28
			 0xaf9016a5, /// 0xf2c
			 0xfe766b72, /// 0xf30
			 0xcb68e3cd, /// 0xf34
			 0x3d93cc07, /// 0xf38
			 0x486b2c63, /// 0xf3c
			 0xce36b2b5, /// 0xf40
			 0x62ade46e, /// 0xf44
			 0x3e7afd09, /// 0xf48
			 0x9abb6028, /// 0xf4c
			 0x6b506e0f, /// 0xf50
			 0x1530cf6c, /// 0xf54
			 0xa72c0a68, /// 0xf58
			 0x2477ca30, /// 0xf5c
			 0x24e31c7c, /// 0xf60
			 0x45b223dd, /// 0xf64
			 0xf36524d0, /// 0xf68
			 0xca47a8a1, /// 0xf6c
			 0x036bb510, /// 0xf70
			 0x2968b73f, /// 0xf74
			 0x60bcc44a, /// 0xf78
			 0xe061f2bd, /// 0xf7c
			 0x547e5b01, /// 0xf80
			 0x4444fb87, /// 0xf84
			 0x6c6e7b2f, /// 0xf88
			 0x58e60395, /// 0xf8c
			 0x524edb14, /// 0xf90
			 0x68d92b9f, /// 0xf94
			 0x5064201e, /// 0xf98
			 0xd777ac43, /// 0xf9c
			 0x4be4aa6f, /// 0xfa0
			 0x4a5c28ef, /// 0xfa4
			 0xeecf90f0, /// 0xfa8
			 0xf649a4ce, /// 0xfac
			 0xa1fe52d0, /// 0xfb0
			 0x40cc8781, /// 0xfb4
			 0xe4d6161f, /// 0xfb8
			 0x0da33588, /// 0xfbc
			 0x63d90375, /// 0xfc0
			 0xea6547e1, /// 0xfc4
			 0x313ac7ea, /// 0xfc8
			 0xde09dfd5, /// 0xfcc
			 0x8de48f03, /// 0xfd0
			 0x1f5f2357, /// 0xfd4
			 0xbb99cbaf, /// 0xfd8
			 0x089e6705, /// 0xfdc
			 0x0da685fd, /// 0xfe0
			 0x19eb9619, /// 0xfe4
			 0x48cc367a, /// 0xfe8
			 0x9c9d1750, /// 0xfec
			 0x4a89aa88, /// 0xff0
			 0xa94e6803, /// 0xff4
			 0x9fcc2d71, /// 0xff8
			 0x8aad9b9f /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00000
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x7fc00001,                                                  // signaling NaN                               /// 00010
			 0xcb000000,                                                  // -8388608.0                                  /// 00014
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00018
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0001c
			 0x7fc00001,                                                  // signaling NaN                               /// 00020
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00024
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00028
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0002c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00034
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00044
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00048
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00050
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00054
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00058
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0005c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00060
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00064
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00068
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0006c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00074
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0007c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00084
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00088
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000d0
			 0x4b000000,                                                  // 8388608.0                                   /// 000d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00100
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00104
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00110
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00114
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00118
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00120
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00124
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00128
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0012c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00134
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00140
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00144
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0014c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00158
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00160
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x0c600000,                                                  // Leading 1s:                                 /// 0016c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00178
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x4b000000,                                                  // 8388608.0                                   /// 00180
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00184
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00188
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0018c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00190
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00194
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00198
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0019c
			 0x33333333,                                                  // 4 random values                             /// 001a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0xcb000000,                                                  // -8388608.0                                  /// 001c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00200
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00204
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0020c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00210
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00214
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00218
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0021c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00220
			 0xcb000000,                                                  // -8388608.0                                  /// 00224
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0022c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00234
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00238
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00244
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00248
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0024c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00250
			 0x7f800000,                                                  // inf                                         /// 00254
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0025c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00260
			 0xff800000,                                                  // -inf                                        /// 00264
			 0xff800000,                                                  // -inf                                        /// 00268
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0026c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00270
			 0x7fc00001,                                                  // signaling NaN                               /// 00274
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0c400000,                                                  // Leading 1s:                                 /// 0027c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00284
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00288
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0028c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00290
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00294
			 0xcb000000,                                                  // -8388608.0                                  /// 00298
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0029c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002ec
			 0xcb000000,                                                  // -8388608.0                                  /// 002f0
			 0x7f800000,                                                  // inf                                         /// 002f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00300
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00308
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0030c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00310
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00320
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00324
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00328
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0032c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0c780000,                                                  // Leading 1s:                                 /// 00334
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0033c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00340
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00344
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00348
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0034c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0035c
			 0xff800000,                                                  // -inf                                        /// 00360
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00364
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0036c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00370
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00374
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00378
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00380
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00384
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0038c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00394
			 0x55555555,                                                  // 4 random values                             /// 00398
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0039c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a0
			 0x7fc00001,                                                  // signaling NaN                               /// 003a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x55555555,                                                  // 4 random values                             /// 003d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xff800000,                                                  // -inf                                        /// 003f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00408
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00410
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00418
			 0x0c600000,                                                  // Leading 1s:                                 /// 0041c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00420
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00424
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00428
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0043c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00444
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0044c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00454
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0045c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00460
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x7f800000,                                                  // inf                                         /// 00468
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00470
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00474
			 0xffc00001,                                                  // -signaling NaN                              /// 00478
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00484
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00488
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00490
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00494
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00504
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0050c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00510
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0051c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00528
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x00011111,                                                  // 9.7958E-41                                  /// 00534
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00538
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00544
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0055c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00564
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0056c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00570
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00578
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0057c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00580
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x7fc00001,                                                  // signaling NaN                               /// 00588
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0058c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00590
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00594
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00598
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0059c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b8
			 0xbf028f5c,                                                  // -0.51                                       /// 005bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005fc
			 0x33333333,                                                  // 4 random values                             /// 00600
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00604
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00610
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0061c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x33333333,                                                  // 4 random values                             /// 00624
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00628
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0062c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00630
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0063c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00640
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00644
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00650
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00660
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0066c
			 0x33333333,                                                  // 4 random values                             /// 00670
			 0x7f800000,                                                  // inf                                         /// 00674
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00678
			 0x0e000003,                                                  // Trailing 1s:                                /// 0067c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00684
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00688
			 0x7fc00001,                                                  // signaling NaN                               /// 0068c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00694
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0069c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x55555555,                                                  // 4 random values                             /// 006bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006c4
			 0x3f028f5c,                                                  // 0.51                                        /// 006c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00700
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00704
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0070c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00720
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00724
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00728
			 0x0e000007,                                                  // Trailing 1s:                                /// 0072c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00730
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00738
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x4b000000,                                                  // 8388608.0                                   /// 00744
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0074c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x0e000003,                                                  // Trailing 1s:                                /// 0075c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00760
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00764
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00770
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00774
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00778
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0077c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00780
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00794
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00798
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0079c
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a4
			 0x7f800000,                                                  // inf                                         /// 007a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00800
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00804
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00808
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00810
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x3f028f5c,                                                  // 0.51                                        /// 00818
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00820
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00828
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0082c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00830
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00838
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00840
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0xff800000,                                                  // -inf                                        /// 00848
			 0x7f800000,                                                  // inf                                         /// 0084c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00850
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00858
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0085c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00864
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00878
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0087c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00880
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00888
			 0x7fc00001,                                                  // signaling NaN                               /// 0088c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00894
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0089c
			 0x0e000003,                                                  // Trailing 1s:                                /// 008a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00900
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00904
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00908
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0091c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00920
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0092c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00930
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00938
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0093c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00940
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00950
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00954
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0095c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00960
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00964
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00968
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00970
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0097c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00980
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00984
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00988
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0098c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00994
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b4
			 0xbf028f5c,                                                  // -0.51                                       /// 009b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a74
			 0x7f800000,                                                  // inf                                         /// 00a78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x55555555,                                                  // 4 random values                             /// 00a80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aa0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ac4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ac8
			 0x00000000,                                                  // zero                                        /// 00acc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ad0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00adc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ae4
			 0x7f800000,                                                  // inf                                         /// 00ae8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00af8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b08
			 0xff800000,                                                  // -inf                                        /// 00b0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xbf028f5c,                                                  // -0.51                                       /// 00b6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ba0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ba8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x33333333,                                                  // 4 random values                             /// 00c90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c9c
			 0x33333333,                                                  // 4 random values                             /// 00ca0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cb4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ccc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00000000,                                                  // zero                                        /// 00ce4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ce8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d18
			 0x7f800000,                                                  // inf                                         /// 00d1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d40
			 0x4b000000,                                                  // 8388608.0                                   /// 00d44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00db4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00db8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x7f800000,                                                  // inf                                         /// 00dc0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x7f800000,                                                  // inf                                         /// 00dd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00df0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ebc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ecc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ed0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00edc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ef0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00efc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f34
			 0xff800000,                                                  // -inf                                        /// 00f38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00000000,                                                  // zero                                        /// 00f8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fa8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fe4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ff4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00000002 // min subnorm + 1 ulp                           // SP +ve numbers                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00004
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0000c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xff800000,                                                  // -inf                                        /// 00014
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00020
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00028
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00030
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00034
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00038
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x80011111,                                                  // -9.7958E-41                                 /// 00044
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0004c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x33333333,                                                  // 4 random values                             /// 0005c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00060
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00064
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0006c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00070
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00074
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00078
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00088
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0008c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00094
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00100
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00108
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00114
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00118
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0011c
			 0x55555555,                                                  // 4 random values                             /// 00120
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00124
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00134
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xff800000,                                                  // -inf                                        /// 0013c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00140
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00144
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00148
			 0xcb000000,                                                  // -8388608.0                                  /// 0014c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00154
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00158
			 0xffc00001,                                                  // -signaling NaN                              /// 0015c
			 0x3f028f5c,                                                  // 0.51                                        /// 00160
			 0x4b000000,                                                  // 8388608.0                                   /// 00164
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00168
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0016c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00180
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00184
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00190
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xffc00001,                                                  // -signaling NaN                              /// 00198
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0019c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00204
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00208
			 0x0c700000,                                                  // Leading 1s:                                 /// 0020c
			 0x00000000,                                                  // zero                                        /// 00210
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00214
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00218
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00220
			 0x80000000,                                                  // -zero                                       /// 00224
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00228
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0022c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0e000007,                                                  // Trailing 1s:                                /// 0023c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00240
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00244
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00250
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00254
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00258
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0xcb000000,                                                  // -8388608.0                                  /// 00264
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0026c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00274
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00278
			 0x3f028f5c,                                                  // 0.51                                        /// 0027c
			 0x33333333,                                                  // 4 random values                             /// 00280
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0028c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00290
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00298
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0029c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x80000000,                                                  // -zero                                       /// 002a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00300
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00304
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00308
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00310
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00314
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0031c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00328
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0032c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00334
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00338
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0033c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00340
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00348
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00350
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00354
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00358
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0035c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00360
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00374
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00378
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0038c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00394
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0039c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000000,                                                  // zero                                        /// 003a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00400
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00404
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0040c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00410
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00418
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0041c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00424
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0042c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00430
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00434
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00438
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0043c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00444
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00448
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0044c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00450
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00454
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00458
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0045c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00460
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00468
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0046c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00474
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00478
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00480
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0048c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00490
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00494
			 0x55555555,                                                  // 4 random values                             /// 00498
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0049c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f4
			 0xffc00001,                                                  // -signaling NaN                              /// 004f8
			 0x55555555,                                                  // 4 random values                             /// 004fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00500
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00510
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00518
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00524
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00528
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0052c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00538
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0053c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00544
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00550
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00554
			 0xff800000,                                                  // -inf                                        /// 00558
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0055c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00560
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00564
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00568
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0056c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00574
			 0x0c400000,                                                  // Leading 1s:                                 /// 00578
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00580
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00588
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0058c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00594
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00000000,                                                  // zero                                        /// 005a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00600
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00604
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x7fc00001,                                                  // signaling NaN                               /// 00610
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00614
			 0x80000000,                                                  // -zero                                       /// 00618
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00628
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00630
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00634
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00638
			 0x00000000,                                                  // zero                                        /// 0063c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00640
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00654
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00658
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0065c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00660
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00678
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00680
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00684
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00688
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00694
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0069c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00704
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e000003,                                                  // Trailing 1s:                                /// 0070c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00714
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0071c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00720
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00724
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0072c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00730
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00748
			 0x0e000007,                                                  // Trailing 1s:                                /// 0074c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00754
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0075c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00760
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x33333333,                                                  // 4 random values                             /// 00768
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0077c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x00011111,                                                  // 9.7958E-41                                  /// 00784
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00788
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0078c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00794
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00798
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0079c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00800
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00804
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00808
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00810
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00814
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0081c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00820
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00828
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00838
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0083c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00840
			 0x0c780000,                                                  // Leading 1s:                                 /// 00844
			 0x0e000001,                                                  // Trailing 1s:                                /// 00848
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00854
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00860
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00864
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0086c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e000003,                                                  // Trailing 1s:                                /// 00874
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0087c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00884
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00888
			 0x0e000007,                                                  // Trailing 1s:                                /// 0088c
			 0xcb000000,                                                  // -8388608.0                                  /// 00890
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00894
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00898
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0089c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x4b000000,                                                  // 8388608.0                                   /// 008b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d0
			 0x00000000,                                                  // zero                                        /// 008d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d8
			 0x4b000000,                                                  // 8388608.0                                   /// 008dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00904
			 0x00011111,                                                  // 9.7958E-41                                  /// 00908
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00011111,                                                  // 9.7958E-41                                  /// 00914
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00918
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00920
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00924
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0092c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00934
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00938
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00940
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00944
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00948
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00950
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00958
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0095c
			 0xbf028f5c,                                                  // -0.51                                       /// 00960
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00964
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0096c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00970
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00974
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00978
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0xcb000000,                                                  // -8388608.0                                  /// 00980
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00984
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00988
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0098c
			 0x3f028f5c,                                                  // 0.51                                        /// 00990
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009a4
			 0x80000000,                                                  // -zero                                       /// 009a8
			 0x3f028f5c,                                                  // 0.51                                        /// 009ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c4
			 0x00000000,                                                  // zero                                        /// 009c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d4
			 0xbf028f5c,                                                  // -0.51                                       /// 009d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0xbf028f5c,                                                  // -0.51                                       /// 009e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x4b000000,                                                  // 8388608.0                                   /// 00a04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x55555555,                                                  // 4 random values                             /// 00a78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00abc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00acc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ad8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00af8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00afc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x4b000000,                                                  // 8388608.0                                   /// 00b24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x55555555,                                                  // 4 random values                             /// 00ba0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0xff800000,                                                  // -inf                                        /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bd0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x7f800000,                                                  // inf                                         /// 00be8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bec
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ce4
			 0x55555555,                                                  // 4 random values                             /// 00ce8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xcb000000,                                                  // -8388608.0                                  /// 00d1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00da8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00db0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00db8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x55555555,                                                  // 4 random values                             /// 00dc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ddc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00de0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00de4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00df4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dfc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e04
			 0x7f800000,                                                  // inf                                         /// 00e08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0xcb000000,                                                  // -8388608.0                                  /// 00e14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ea8
			 0x7fc00001,                                                  // signaling NaN                               /// 00eac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00eb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ecc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ed0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00edc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00efc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0xff800000,                                                  // -inf                                        /// 00f3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f54
			 0x33333333,                                                  // 4 random values                             /// 00f58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f6c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f74
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x33333333,                                                  // 4 random values                             /// 00f7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00fb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fdc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fec
			 0x33333333,                                                  // 4 random values                             /// 00ff0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ff8
			 0xff7ffffe // max norm - 1ulp                               // max norm -ve                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xc4d9179c, /// 0x0
			 0x82db1555, /// 0x4
			 0xdfd9598f, /// 0x8
			 0x6bef5252, /// 0xc
			 0x76288684, /// 0x10
			 0x16555f13, /// 0x14
			 0x955024c7, /// 0x18
			 0x371e813a, /// 0x1c
			 0x5c3dfdab, /// 0x20
			 0x2dad1597, /// 0x24
			 0x2eb4d3a0, /// 0x28
			 0x594c708f, /// 0x2c
			 0x34dfe993, /// 0x30
			 0x08c59b1f, /// 0x34
			 0x5b44c223, /// 0x38
			 0x4921805c, /// 0x3c
			 0x485c5ebd, /// 0x40
			 0x72d844bc, /// 0x44
			 0xd5100733, /// 0x48
			 0x8abb34ea, /// 0x4c
			 0x99ff398b, /// 0x50
			 0x4a3253fe, /// 0x54
			 0x169273c4, /// 0x58
			 0x5883fe58, /// 0x5c
			 0x262fca67, /// 0x60
			 0x53605ed9, /// 0x64
			 0x0ab290cb, /// 0x68
			 0xda803ca6, /// 0x6c
			 0x216c3a59, /// 0x70
			 0xe87d3473, /// 0x74
			 0xf950e2cd, /// 0x78
			 0x8f41142a, /// 0x7c
			 0x317cfbcf, /// 0x80
			 0x806e584e, /// 0x84
			 0xe3659e63, /// 0x88
			 0xbdac5469, /// 0x8c
			 0x6d7ac7b6, /// 0x90
			 0xe8a9fee7, /// 0x94
			 0xdd92be9b, /// 0x98
			 0xe468947c, /// 0x9c
			 0x3134555c, /// 0xa0
			 0xefd6da95, /// 0xa4
			 0x92c37487, /// 0xa8
			 0xe2d1dccb, /// 0xac
			 0x399628cd, /// 0xb0
			 0x86e8d8db, /// 0xb4
			 0xe7bfa638, /// 0xb8
			 0xd576fb76, /// 0xbc
			 0x6ab4b62e, /// 0xc0
			 0x43a27c6a, /// 0xc4
			 0xa241633b, /// 0xc8
			 0xb8ecd35e, /// 0xcc
			 0xc7868740, /// 0xd0
			 0x5a84d3bb, /// 0xd4
			 0x90eb5b28, /// 0xd8
			 0x1d2bf6c3, /// 0xdc
			 0x76050f47, /// 0xe0
			 0x2b1ba78d, /// 0xe4
			 0xac1720e9, /// 0xe8
			 0xfe467d5d, /// 0xec
			 0x24efbc6c, /// 0xf0
			 0x600e3a9c, /// 0xf4
			 0xd99b2ffa, /// 0xf8
			 0x9decd709, /// 0xfc
			 0xfa486806, /// 0x100
			 0xa1bbff57, /// 0x104
			 0xece3f03e, /// 0x108
			 0x36d4b70d, /// 0x10c
			 0x8f939f41, /// 0x110
			 0x509a7c30, /// 0x114
			 0x28251e25, /// 0x118
			 0xa2759cda, /// 0x11c
			 0xf50cd291, /// 0x120
			 0x69015449, /// 0x124
			 0x3c43c152, /// 0x128
			 0x04bd4119, /// 0x12c
			 0x39e2f2ac, /// 0x130
			 0xb2159383, /// 0x134
			 0x7d978a48, /// 0x138
			 0xf42a31da, /// 0x13c
			 0x74106fa6, /// 0x140
			 0x5a2edf07, /// 0x144
			 0x18abbe81, /// 0x148
			 0xd657c675, /// 0x14c
			 0xdeb589c6, /// 0x150
			 0x5c8c783b, /// 0x154
			 0x038e32e3, /// 0x158
			 0xbc422658, /// 0x15c
			 0x641c17f3, /// 0x160
			 0xa4db8ede, /// 0x164
			 0xa1eb3cfe, /// 0x168
			 0x318bf5a6, /// 0x16c
			 0x9a92c68a, /// 0x170
			 0x2e8dbcf9, /// 0x174
			 0xcf556410, /// 0x178
			 0x22dbf6b2, /// 0x17c
			 0x66e4fc71, /// 0x180
			 0x723f2452, /// 0x184
			 0x885b8882, /// 0x188
			 0xf7e22327, /// 0x18c
			 0x51deab66, /// 0x190
			 0x25ee17c8, /// 0x194
			 0x395f50a6, /// 0x198
			 0x318db43a, /// 0x19c
			 0x474d998b, /// 0x1a0
			 0xbcd214f9, /// 0x1a4
			 0xcfa82abb, /// 0x1a8
			 0x104b8b58, /// 0x1ac
			 0x1c99b151, /// 0x1b0
			 0x44c6aa3e, /// 0x1b4
			 0x847e0021, /// 0x1b8
			 0x9ab6ba05, /// 0x1bc
			 0xea1038b6, /// 0x1c0
			 0x35cc7a83, /// 0x1c4
			 0x95fac0e5, /// 0x1c8
			 0xd1259a57, /// 0x1cc
			 0xdc74b9bf, /// 0x1d0
			 0xb3d6eaf6, /// 0x1d4
			 0xe189bade, /// 0x1d8
			 0xca594037, /// 0x1dc
			 0x5bf4ad42, /// 0x1e0
			 0x3810a83e, /// 0x1e4
			 0x7b27e3e1, /// 0x1e8
			 0x620fee2d, /// 0x1ec
			 0x6bbd8e3c, /// 0x1f0
			 0x7dd92b5a, /// 0x1f4
			 0x3d046f8f, /// 0x1f8
			 0xcccb9d0c, /// 0x1fc
			 0x350acbc3, /// 0x200
			 0x296d0f68, /// 0x204
			 0x155a228e, /// 0x208
			 0x629567fd, /// 0x20c
			 0x556778e4, /// 0x210
			 0x8f43cf3f, /// 0x214
			 0x6f3961a0, /// 0x218
			 0xaf3b52c4, /// 0x21c
			 0x063969a5, /// 0x220
			 0x7ef3df19, /// 0x224
			 0xc751ae48, /// 0x228
			 0xca26ec3b, /// 0x22c
			 0xf13a56bf, /// 0x230
			 0x8027411c, /// 0x234
			 0xc7659181, /// 0x238
			 0x6869fff8, /// 0x23c
			 0x93edaea9, /// 0x240
			 0x41bb43c5, /// 0x244
			 0xd64c35e9, /// 0x248
			 0x79506692, /// 0x24c
			 0x75f8e2d9, /// 0x250
			 0x503e429d, /// 0x254
			 0xbe97ce1d, /// 0x258
			 0x36dd9c3e, /// 0x25c
			 0x4bac4384, /// 0x260
			 0xf46f9103, /// 0x264
			 0xbdaa1f61, /// 0x268
			 0x8386b164, /// 0x26c
			 0x94137d96, /// 0x270
			 0x9e620460, /// 0x274
			 0x58762568, /// 0x278
			 0xd9d31069, /// 0x27c
			 0x0826b26f, /// 0x280
			 0x249bf9e2, /// 0x284
			 0xdeb2484c, /// 0x288
			 0x6693c8cb, /// 0x28c
			 0x7edb35d8, /// 0x290
			 0xe7d41c94, /// 0x294
			 0x6354ac1a, /// 0x298
			 0x54970d8a, /// 0x29c
			 0xca89a018, /// 0x2a0
			 0xf01057bc, /// 0x2a4
			 0x174d2256, /// 0x2a8
			 0x0fe2b524, /// 0x2ac
			 0xc91cacee, /// 0x2b0
			 0x3c251bab, /// 0x2b4
			 0xd28e6749, /// 0x2b8
			 0x433d25af, /// 0x2bc
			 0x37e69868, /// 0x2c0
			 0x76bc0e40, /// 0x2c4
			 0x5302ef87, /// 0x2c8
			 0x206e24f0, /// 0x2cc
			 0x3e1a6d9a, /// 0x2d0
			 0xe19929d2, /// 0x2d4
			 0xc9529583, /// 0x2d8
			 0x5d5c2f1d, /// 0x2dc
			 0xc0a46e88, /// 0x2e0
			 0x3694c85c, /// 0x2e4
			 0xcf4b876a, /// 0x2e8
			 0xc2384a64, /// 0x2ec
			 0x0e0a6946, /// 0x2f0
			 0xe478491c, /// 0x2f4
			 0x1e08494a, /// 0x2f8
			 0xec9ec19d, /// 0x2fc
			 0x0b8b4063, /// 0x300
			 0x1d5af5b8, /// 0x304
			 0x3939a17a, /// 0x308
			 0x3784ff08, /// 0x30c
			 0x6a5e3508, /// 0x310
			 0x9dcf199c, /// 0x314
			 0x696a80b2, /// 0x318
			 0xc3c04bf8, /// 0x31c
			 0xe447f1eb, /// 0x320
			 0x4090d0db, /// 0x324
			 0x08a62112, /// 0x328
			 0x5e308635, /// 0x32c
			 0xe79253df, /// 0x330
			 0xb47c07bc, /// 0x334
			 0x4237e8f9, /// 0x338
			 0x8ee58284, /// 0x33c
			 0xd1627102, /// 0x340
			 0x2e250e8e, /// 0x344
			 0x190cd9c1, /// 0x348
			 0xd9246f37, /// 0x34c
			 0x2923b5c7, /// 0x350
			 0x03a3c693, /// 0x354
			 0x4b2ab9cb, /// 0x358
			 0xf68806a8, /// 0x35c
			 0xc256881f, /// 0x360
			 0x2cada75e, /// 0x364
			 0x9b2416c1, /// 0x368
			 0xc94c7d8c, /// 0x36c
			 0x9ab650fd, /// 0x370
			 0xe5025f10, /// 0x374
			 0x135b1fe3, /// 0x378
			 0x513e00f4, /// 0x37c
			 0x9d035908, /// 0x380
			 0xbb4a9601, /// 0x384
			 0x1dbae5b1, /// 0x388
			 0x9cf3e521, /// 0x38c
			 0x23906e17, /// 0x390
			 0xc1ebe9ea, /// 0x394
			 0x07f7fa35, /// 0x398
			 0x0333ac04, /// 0x39c
			 0x1f8a44a9, /// 0x3a0
			 0xf4359b00, /// 0x3a4
			 0x26ff2a8c, /// 0x3a8
			 0xd6e2999f, /// 0x3ac
			 0x9933588b, /// 0x3b0
			 0x30103192, /// 0x3b4
			 0x2bea2fdf, /// 0x3b8
			 0x28dcc63e, /// 0x3bc
			 0x0377f40c, /// 0x3c0
			 0x9a8c5bbf, /// 0x3c4
			 0xb9453e6d, /// 0x3c8
			 0x231c4a9a, /// 0x3cc
			 0x323a09b1, /// 0x3d0
			 0x46b102b1, /// 0x3d4
			 0xfbd78945, /// 0x3d8
			 0x51847f3e, /// 0x3dc
			 0x4e135ace, /// 0x3e0
			 0xc7c2d33c, /// 0x3e4
			 0xc58fcadb, /// 0x3e8
			 0xdf7a6cd6, /// 0x3ec
			 0x9dbce41d, /// 0x3f0
			 0xd7903979, /// 0x3f4
			 0x8b1587b8, /// 0x3f8
			 0x9f177f04, /// 0x3fc
			 0xe24529d2, /// 0x400
			 0x8b8871c2, /// 0x404
			 0x26dbca0b, /// 0x408
			 0xb34129c7, /// 0x40c
			 0xe530beb5, /// 0x410
			 0x900eab7d, /// 0x414
			 0x2ef93a1a, /// 0x418
			 0x547871ce, /// 0x41c
			 0x3a328ab6, /// 0x420
			 0x01fd804a, /// 0x424
			 0x6001fc88, /// 0x428
			 0x9be5eee8, /// 0x42c
			 0x0efa08cb, /// 0x430
			 0xc2e62ce6, /// 0x434
			 0xb80a5b96, /// 0x438
			 0x1e0243ce, /// 0x43c
			 0xa0f81702, /// 0x440
			 0xaa18f3b2, /// 0x444
			 0xb850e519, /// 0x448
			 0x7a0456b2, /// 0x44c
			 0x4a74dfac, /// 0x450
			 0xfab7c2f1, /// 0x454
			 0xd7dfb6a4, /// 0x458
			 0xc80d62da, /// 0x45c
			 0x8c0603b9, /// 0x460
			 0x80a4b68e, /// 0x464
			 0xf34692a6, /// 0x468
			 0x388623f6, /// 0x46c
			 0x8baa8716, /// 0x470
			 0x1006bafa, /// 0x474
			 0x5b1e6551, /// 0x478
			 0xf4220511, /// 0x47c
			 0xb0cf0485, /// 0x480
			 0xf21cd599, /// 0x484
			 0x8cf02459, /// 0x488
			 0x18b6c524, /// 0x48c
			 0xd2f9723f, /// 0x490
			 0x414e1fbd, /// 0x494
			 0x61c8ae63, /// 0x498
			 0xc28f436f, /// 0x49c
			 0xe80d0673, /// 0x4a0
			 0x2b74fc25, /// 0x4a4
			 0x860579b8, /// 0x4a8
			 0x86bfef20, /// 0x4ac
			 0xe916f359, /// 0x4b0
			 0x95b6ecd8, /// 0x4b4
			 0xeb14dc8b, /// 0x4b8
			 0x4598cbe6, /// 0x4bc
			 0x65814fb6, /// 0x4c0
			 0xa74a06f4, /// 0x4c4
			 0x42be7aca, /// 0x4c8
			 0xe6c6b382, /// 0x4cc
			 0x2b3d28e7, /// 0x4d0
			 0xdadb6f35, /// 0x4d4
			 0xf3d46665, /// 0x4d8
			 0x9044359d, /// 0x4dc
			 0x067803f4, /// 0x4e0
			 0x260b1ef4, /// 0x4e4
			 0x59265604, /// 0x4e8
			 0x6715c427, /// 0x4ec
			 0x52827f83, /// 0x4f0
			 0x6936737e, /// 0x4f4
			 0x5e22aee6, /// 0x4f8
			 0xbbd79984, /// 0x4fc
			 0x483a81d4, /// 0x500
			 0xf3bcb0d3, /// 0x504
			 0xd7041785, /// 0x508
			 0xff6e8ddd, /// 0x50c
			 0xc4ff6eab, /// 0x510
			 0xbdde3a1d, /// 0x514
			 0x938c4ea4, /// 0x518
			 0x43a4f2da, /// 0x51c
			 0x96e4fec9, /// 0x520
			 0x2aba3b0c, /// 0x524
			 0x8401457a, /// 0x528
			 0xd21c9b83, /// 0x52c
			 0xa4b9ff26, /// 0x530
			 0xda5f69c8, /// 0x534
			 0x311ba852, /// 0x538
			 0x4f318b23, /// 0x53c
			 0x402ac705, /// 0x540
			 0x81f2afa9, /// 0x544
			 0x2f7efdff, /// 0x548
			 0x87ff1bfb, /// 0x54c
			 0x5cc64dce, /// 0x550
			 0xbbe86ee7, /// 0x554
			 0xb1bfde29, /// 0x558
			 0x8894ee9d, /// 0x55c
			 0xb6048b1c, /// 0x560
			 0x547cd61b, /// 0x564
			 0x8d570dfe, /// 0x568
			 0xde5ba752, /// 0x56c
			 0x7a2cc66c, /// 0x570
			 0xf3f41402, /// 0x574
			 0x6c483e2e, /// 0x578
			 0x4693a031, /// 0x57c
			 0x83161d8c, /// 0x580
			 0x040da32b, /// 0x584
			 0x174de320, /// 0x588
			 0xfc6f0989, /// 0x58c
			 0xa0d055de, /// 0x590
			 0x3d5e9897, /// 0x594
			 0xd74ff2af, /// 0x598
			 0x347b3a7a, /// 0x59c
			 0xba4db8ab, /// 0x5a0
			 0xb3b18259, /// 0x5a4
			 0x0710f7db, /// 0x5a8
			 0x7c67cc4c, /// 0x5ac
			 0x2d015c95, /// 0x5b0
			 0xa25e0e62, /// 0x5b4
			 0x1e354162, /// 0x5b8
			 0x4717844a, /// 0x5bc
			 0x3b2f6e0d, /// 0x5c0
			 0x5849721d, /// 0x5c4
			 0x89d15e58, /// 0x5c8
			 0x6b695229, /// 0x5cc
			 0x20453a81, /// 0x5d0
			 0x32e44c46, /// 0x5d4
			 0xbd2b54a9, /// 0x5d8
			 0x5dd514d0, /// 0x5dc
			 0x2f3dcbe2, /// 0x5e0
			 0xb43ad608, /// 0x5e4
			 0xb32c9b11, /// 0x5e8
			 0xf67efd90, /// 0x5ec
			 0x21e3c3b0, /// 0x5f0
			 0x410d0d4e, /// 0x5f4
			 0xc961bf2b, /// 0x5f8
			 0x07c89516, /// 0x5fc
			 0x671017b7, /// 0x600
			 0x011ca857, /// 0x604
			 0x6cb8de28, /// 0x608
			 0x08b045ce, /// 0x60c
			 0xcd41b7c5, /// 0x610
			 0x5fa88d0c, /// 0x614
			 0x460ded98, /// 0x618
			 0xb4284e85, /// 0x61c
			 0xc11e8f5b, /// 0x620
			 0x2897aa97, /// 0x624
			 0xcac9aff8, /// 0x628
			 0x6d7a1a33, /// 0x62c
			 0x63e512b8, /// 0x630
			 0x1b076aed, /// 0x634
			 0x53216640, /// 0x638
			 0x64c574fd, /// 0x63c
			 0xacf997d8, /// 0x640
			 0x7b5a219e, /// 0x644
			 0xc23ad16c, /// 0x648
			 0xe1a9528d, /// 0x64c
			 0x6b54e02e, /// 0x650
			 0xce2f6cf3, /// 0x654
			 0xee713c81, /// 0x658
			 0x19319c89, /// 0x65c
			 0x62afa795, /// 0x660
			 0x65d98523, /// 0x664
			 0xc097e73b, /// 0x668
			 0xdd607d54, /// 0x66c
			 0x6214249d, /// 0x670
			 0xaae6fda8, /// 0x674
			 0x5171399c, /// 0x678
			 0xd2e27148, /// 0x67c
			 0x5b859e7b, /// 0x680
			 0x68ff496b, /// 0x684
			 0x314d84fd, /// 0x688
			 0x257c17b2, /// 0x68c
			 0x2e88c148, /// 0x690
			 0x280ebf9f, /// 0x694
			 0xb0c84925, /// 0x698
			 0x4154af2a, /// 0x69c
			 0xe6f112b4, /// 0x6a0
			 0xcaa10cf3, /// 0x6a4
			 0x6270a88a, /// 0x6a8
			 0xfc1d5700, /// 0x6ac
			 0x1bd88b04, /// 0x6b0
			 0xcee035a7, /// 0x6b4
			 0x9b4455fb, /// 0x6b8
			 0x7f06a53c, /// 0x6bc
			 0x46270180, /// 0x6c0
			 0xcf03f2aa, /// 0x6c4
			 0x6ecaef8f, /// 0x6c8
			 0x64ae68c7, /// 0x6cc
			 0xac1d7a5d, /// 0x6d0
			 0x05eb8046, /// 0x6d4
			 0x725d4f2c, /// 0x6d8
			 0x971a56ef, /// 0x6dc
			 0x99ed20da, /// 0x6e0
			 0xbd296833, /// 0x6e4
			 0x029cd21f, /// 0x6e8
			 0xdaab3bed, /// 0x6ec
			 0xcbcf74d1, /// 0x6f0
			 0x075070c6, /// 0x6f4
			 0x8ed2aece, /// 0x6f8
			 0xb0e0e178, /// 0x6fc
			 0x671b8c16, /// 0x700
			 0x31fb1877, /// 0x704
			 0x22ea7e3b, /// 0x708
			 0x6d5ef007, /// 0x70c
			 0x1500c93d, /// 0x710
			 0x75ed86ea, /// 0x714
			 0x1c4e7152, /// 0x718
			 0x03590d49, /// 0x71c
			 0x08536605, /// 0x720
			 0x10dade10, /// 0x724
			 0xebcfd673, /// 0x728
			 0xad00b162, /// 0x72c
			 0x2d6cc83f, /// 0x730
			 0x8023897c, /// 0x734
			 0x508f9328, /// 0x738
			 0x273ed31d, /// 0x73c
			 0x727d8b14, /// 0x740
			 0x84ac2fe8, /// 0x744
			 0xaf658693, /// 0x748
			 0x368460a8, /// 0x74c
			 0xb46c65ca, /// 0x750
			 0x7497b66f, /// 0x754
			 0x5573018e, /// 0x758
			 0x05df2c95, /// 0x75c
			 0x1ed06edd, /// 0x760
			 0x88c9f075, /// 0x764
			 0xe07e74ec, /// 0x768
			 0x82ec74e5, /// 0x76c
			 0xa1e03a0d, /// 0x770
			 0x94b91fb5, /// 0x774
			 0x8200f917, /// 0x778
			 0xc6496167, /// 0x77c
			 0xf1396d8c, /// 0x780
			 0x6f7b9b62, /// 0x784
			 0xd22068a4, /// 0x788
			 0x9e404d44, /// 0x78c
			 0xfb7480bf, /// 0x790
			 0xcc479876, /// 0x794
			 0x9e069c7e, /// 0x798
			 0x63571ab0, /// 0x79c
			 0x75f35057, /// 0x7a0
			 0x75d51bd3, /// 0x7a4
			 0x5ac103cf, /// 0x7a8
			 0xa7a9f1a2, /// 0x7ac
			 0x536f2969, /// 0x7b0
			 0x3ef51749, /// 0x7b4
			 0x6043d9e2, /// 0x7b8
			 0x2797b7aa, /// 0x7bc
			 0x3d7668ea, /// 0x7c0
			 0x0fbcf129, /// 0x7c4
			 0xcf3afe02, /// 0x7c8
			 0xe2cbc8e8, /// 0x7cc
			 0x9ffe64e6, /// 0x7d0
			 0xd95ef3c3, /// 0x7d4
			 0x625131e5, /// 0x7d8
			 0x515e92f2, /// 0x7dc
			 0x32e28eb0, /// 0x7e0
			 0x5d1b0b69, /// 0x7e4
			 0x2e0813ae, /// 0x7e8
			 0xc5593524, /// 0x7ec
			 0x75145d7d, /// 0x7f0
			 0x3a432655, /// 0x7f4
			 0xaf26d832, /// 0x7f8
			 0x07845b29, /// 0x7fc
			 0xeb87627d, /// 0x800
			 0xc953c435, /// 0x804
			 0xf2cdc816, /// 0x808
			 0x3ba72fd7, /// 0x80c
			 0x497bda2f, /// 0x810
			 0xccf5fe5e, /// 0x814
			 0x21718788, /// 0x818
			 0x28afd0e0, /// 0x81c
			 0x9d5e453c, /// 0x820
			 0xcc7cebc8, /// 0x824
			 0x9eab1493, /// 0x828
			 0x807f97ec, /// 0x82c
			 0xb1121d63, /// 0x830
			 0x7a7ae6da, /// 0x834
			 0x2024adaa, /// 0x838
			 0xc43ca229, /// 0x83c
			 0x8e2c468f, /// 0x840
			 0x7ecfa2aa, /// 0x844
			 0x187da6a0, /// 0x848
			 0x10514901, /// 0x84c
			 0x65578207, /// 0x850
			 0xce5760ff, /// 0x854
			 0x10663330, /// 0x858
			 0x7d7ed4b2, /// 0x85c
			 0x96380413, /// 0x860
			 0x68fca2ab, /// 0x864
			 0x979b31e5, /// 0x868
			 0xdef11bd6, /// 0x86c
			 0x650a48d9, /// 0x870
			 0x9a1b8c40, /// 0x874
			 0x2a2084ec, /// 0x878
			 0xd4e4e6e7, /// 0x87c
			 0x76c41c87, /// 0x880
			 0xddcadf59, /// 0x884
			 0xeb2f5489, /// 0x888
			 0xc2fdb175, /// 0x88c
			 0xa497b81b, /// 0x890
			 0xd7e842de, /// 0x894
			 0xdd80473c, /// 0x898
			 0xb06f92d1, /// 0x89c
			 0x2e2471a2, /// 0x8a0
			 0xd192711b, /// 0x8a4
			 0x9b005e50, /// 0x8a8
			 0x26afae58, /// 0x8ac
			 0xc46b334f, /// 0x8b0
			 0x9ee5f9d8, /// 0x8b4
			 0x266443fe, /// 0x8b8
			 0xec1649a1, /// 0x8bc
			 0x291c24cd, /// 0x8c0
			 0x12013df9, /// 0x8c4
			 0xd72daf7d, /// 0x8c8
			 0x08331c84, /// 0x8cc
			 0xcfe066f5, /// 0x8d0
			 0xc628dc22, /// 0x8d4
			 0x83f8a5b4, /// 0x8d8
			 0xfa69bc8a, /// 0x8dc
			 0x8bbf9c7b, /// 0x8e0
			 0xbd11f868, /// 0x8e4
			 0x20b052c0, /// 0x8e8
			 0x7986486f, /// 0x8ec
			 0x0af9c09c, /// 0x8f0
			 0xbb8d7d19, /// 0x8f4
			 0xea1eca7d, /// 0x8f8
			 0x9631c8c7, /// 0x8fc
			 0x59e6078b, /// 0x900
			 0xa64f1750, /// 0x904
			 0xfe6542bb, /// 0x908
			 0xdd425112, /// 0x90c
			 0x2683fadf, /// 0x910
			 0x02c783a6, /// 0x914
			 0x3bf4f9b4, /// 0x918
			 0x6592434c, /// 0x91c
			 0x7333421f, /// 0x920
			 0xfeaaaf37, /// 0x924
			 0xb700b25c, /// 0x928
			 0x2008a803, /// 0x92c
			 0x0df43e52, /// 0x930
			 0x37daca5f, /// 0x934
			 0xfd9013fd, /// 0x938
			 0x5d9779b9, /// 0x93c
			 0xf0a745ff, /// 0x940
			 0x70338a48, /// 0x944
			 0xbbdc28e7, /// 0x948
			 0x6037fcdf, /// 0x94c
			 0xc4e7bd65, /// 0x950
			 0x7c4cb325, /// 0x954
			 0x42ab6d13, /// 0x958
			 0x87b1aa74, /// 0x95c
			 0x2b1e38a4, /// 0x960
			 0x92b34569, /// 0x964
			 0x0020da01, /// 0x968
			 0xd7b306b4, /// 0x96c
			 0x1d9fa633, /// 0x970
			 0x39d1d224, /// 0x974
			 0xc8304d08, /// 0x978
			 0xcda287eb, /// 0x97c
			 0x3b974965, /// 0x980
			 0x32ca5372, /// 0x984
			 0xa04c9572, /// 0x988
			 0x0e6f1cf6, /// 0x98c
			 0xa602e4ef, /// 0x990
			 0xc0bba523, /// 0x994
			 0x34d3b4d6, /// 0x998
			 0x7c0d80f9, /// 0x99c
			 0xca7c1bbf, /// 0x9a0
			 0x328ec994, /// 0x9a4
			 0x492f3fb0, /// 0x9a8
			 0xfb82796a, /// 0x9ac
			 0x976d7473, /// 0x9b0
			 0xe11da4eb, /// 0x9b4
			 0xaf9d3585, /// 0x9b8
			 0x3a71ce72, /// 0x9bc
			 0xe05f1465, /// 0x9c0
			 0xff0e278d, /// 0x9c4
			 0x0c45b335, /// 0x9c8
			 0xf94a0991, /// 0x9cc
			 0x3d52b61f, /// 0x9d0
			 0x8e311a54, /// 0x9d4
			 0x805de94d, /// 0x9d8
			 0xfbaf57c7, /// 0x9dc
			 0x5b208879, /// 0x9e0
			 0xdce5f144, /// 0x9e4
			 0x16da01b0, /// 0x9e8
			 0xf8a5e471, /// 0x9ec
			 0xa36c89a4, /// 0x9f0
			 0x9822359c, /// 0x9f4
			 0x1bf8aa7a, /// 0x9f8
			 0xd568d04c, /// 0x9fc
			 0x30a732a0, /// 0xa00
			 0x42afa7b0, /// 0xa04
			 0x4500a830, /// 0xa08
			 0x3200b35d, /// 0xa0c
			 0x5c9657c2, /// 0xa10
			 0x257c9d49, /// 0xa14
			 0x84c756ba, /// 0xa18
			 0xacde0d5e, /// 0xa1c
			 0xec810964, /// 0xa20
			 0xfbcfeb93, /// 0xa24
			 0x7ae1a2b7, /// 0xa28
			 0xbc0a423e, /// 0xa2c
			 0x4dab9d9a, /// 0xa30
			 0xf4cae554, /// 0xa34
			 0x0db6ed26, /// 0xa38
			 0xb1007a73, /// 0xa3c
			 0x5b29053f, /// 0xa40
			 0xb1cc5224, /// 0xa44
			 0xf6114b8b, /// 0xa48
			 0x18908c72, /// 0xa4c
			 0x4637bd18, /// 0xa50
			 0xa1523f3a, /// 0xa54
			 0xd0360e55, /// 0xa58
			 0x555bbf4c, /// 0xa5c
			 0xa22f44cb, /// 0xa60
			 0xf6b727f0, /// 0xa64
			 0xfd049e7b, /// 0xa68
			 0x26aaea87, /// 0xa6c
			 0x72064746, /// 0xa70
			 0xd91c5ffb, /// 0xa74
			 0x52378ae1, /// 0xa78
			 0xc5a78051, /// 0xa7c
			 0xcd591518, /// 0xa80
			 0x82cd3103, /// 0xa84
			 0x27147326, /// 0xa88
			 0xacfd35fd, /// 0xa8c
			 0x232b495e, /// 0xa90
			 0xf3f4892f, /// 0xa94
			 0x3fb790d5, /// 0xa98
			 0xd5ad8303, /// 0xa9c
			 0x46567bda, /// 0xaa0
			 0x7fa61930, /// 0xaa4
			 0xccadafac, /// 0xaa8
			 0x4319784f, /// 0xaac
			 0x4cbbf028, /// 0xab0
			 0x387aa9c8, /// 0xab4
			 0x94f4915b, /// 0xab8
			 0x277e3520, /// 0xabc
			 0x5d471caa, /// 0xac0
			 0x4f9e2b43, /// 0xac4
			 0x55d6a4c6, /// 0xac8
			 0xbce7ecdf, /// 0xacc
			 0x76844945, /// 0xad0
			 0xb5f4a7dd, /// 0xad4
			 0x0cc04e7a, /// 0xad8
			 0xd954a756, /// 0xadc
			 0xee678870, /// 0xae0
			 0xea3011b1, /// 0xae4
			 0x8bdac92a, /// 0xae8
			 0xec82ecc9, /// 0xaec
			 0x35dddb04, /// 0xaf0
			 0x8c5d9e96, /// 0xaf4
			 0x6963388e, /// 0xaf8
			 0x1c922870, /// 0xafc
			 0x47bf1f99, /// 0xb00
			 0x41e18900, /// 0xb04
			 0x4a599023, /// 0xb08
			 0xa6902a61, /// 0xb0c
			 0xaac0478a, /// 0xb10
			 0xe60629db, /// 0xb14
			 0x3e043dd5, /// 0xb18
			 0x1211232a, /// 0xb1c
			 0xa5771bb4, /// 0xb20
			 0xdb50b52c, /// 0xb24
			 0x7c4fad0b, /// 0xb28
			 0x4f2da102, /// 0xb2c
			 0x5fe5d65d, /// 0xb30
			 0x739a7f3e, /// 0xb34
			 0x811b4982, /// 0xb38
			 0x160b2698, /// 0xb3c
			 0x7d8d3243, /// 0xb40
			 0xa61ceaf6, /// 0xb44
			 0xb0f25ed6, /// 0xb48
			 0xfc7330ca, /// 0xb4c
			 0xf6bc3d92, /// 0xb50
			 0x1309dc87, /// 0xb54
			 0xaa161380, /// 0xb58
			 0x6861bf68, /// 0xb5c
			 0x7188f95c, /// 0xb60
			 0x2970111e, /// 0xb64
			 0x414da210, /// 0xb68
			 0x4d086255, /// 0xb6c
			 0x625a49ce, /// 0xb70
			 0xf3ece38c, /// 0xb74
			 0x57ea8742, /// 0xb78
			 0x81503eab, /// 0xb7c
			 0xe23e112e, /// 0xb80
			 0x654aafb3, /// 0xb84
			 0x1394daf4, /// 0xb88
			 0x990993aa, /// 0xb8c
			 0xdc79771e, /// 0xb90
			 0x0ef0e23c, /// 0xb94
			 0x2ceec19b, /// 0xb98
			 0xe4c0831f, /// 0xb9c
			 0x56fd6386, /// 0xba0
			 0xf9648d2b, /// 0xba4
			 0x8299c5d9, /// 0xba8
			 0x200b473e, /// 0xbac
			 0x90e5a098, /// 0xbb0
			 0xfcaed812, /// 0xbb4
			 0x5f4c9f3d, /// 0xbb8
			 0x8cbd794b, /// 0xbbc
			 0x16e6c482, /// 0xbc0
			 0x6eebbb68, /// 0xbc4
			 0x4defe47a, /// 0xbc8
			 0xb622bef9, /// 0xbcc
			 0x8c7f619e, /// 0xbd0
			 0xc2993a10, /// 0xbd4
			 0x2e9e70fd, /// 0xbd8
			 0xc74a8c5e, /// 0xbdc
			 0x5b9f4284, /// 0xbe0
			 0xe7aaf65b, /// 0xbe4
			 0xdfeede85, /// 0xbe8
			 0x83f31f66, /// 0xbec
			 0x549a3272, /// 0xbf0
			 0x36114bb9, /// 0xbf4
			 0xd27e9df2, /// 0xbf8
			 0x6b38da0d, /// 0xbfc
			 0x25f64a7e, /// 0xc00
			 0x31b432c1, /// 0xc04
			 0xd985c853, /// 0xc08
			 0x07d83aef, /// 0xc0c
			 0x51f4d964, /// 0xc10
			 0x24af81d1, /// 0xc14
			 0xdfedf4e0, /// 0xc18
			 0x1d3025b8, /// 0xc1c
			 0xfcd20dd3, /// 0xc20
			 0x043dd01f, /// 0xc24
			 0x6740af72, /// 0xc28
			 0x46bc7332, /// 0xc2c
			 0x4e41dc1c, /// 0xc30
			 0x15c684d9, /// 0xc34
			 0x71b562c6, /// 0xc38
			 0x88747579, /// 0xc3c
			 0xb8190371, /// 0xc40
			 0x21ecf29d, /// 0xc44
			 0xa2428635, /// 0xc48
			 0xb22c6d29, /// 0xc4c
			 0x4802a76b, /// 0xc50
			 0x2524bdc8, /// 0xc54
			 0xb7cc3b24, /// 0xc58
			 0xb7f9065a, /// 0xc5c
			 0x6fa44640, /// 0xc60
			 0xd0c44a1d, /// 0xc64
			 0xcd0d774e, /// 0xc68
			 0x80830e7f, /// 0xc6c
			 0x3e97dfb5, /// 0xc70
			 0xac21c0f7, /// 0xc74
			 0x902b2927, /// 0xc78
			 0x7a3f048b, /// 0xc7c
			 0x91d4e6d1, /// 0xc80
			 0xa6dd3fe5, /// 0xc84
			 0x482a43ff, /// 0xc88
			 0xa4f02f37, /// 0xc8c
			 0x906aba3f, /// 0xc90
			 0x8f26a58b, /// 0xc94
			 0x9403f217, /// 0xc98
			 0x14193131, /// 0xc9c
			 0xd5dff3bf, /// 0xca0
			 0x9f0b82ab, /// 0xca4
			 0x107e99af, /// 0xca8
			 0x95455664, /// 0xcac
			 0x40e33c70, /// 0xcb0
			 0xd377993c, /// 0xcb4
			 0x89fa4fac, /// 0xcb8
			 0xdf989848, /// 0xcbc
			 0xd3bef605, /// 0xcc0
			 0xdb68450c, /// 0xcc4
			 0x2e763f3c, /// 0xcc8
			 0xb9a408d3, /// 0xccc
			 0x38539453, /// 0xcd0
			 0xec9bc800, /// 0xcd4
			 0xc921dfe0, /// 0xcd8
			 0x5471cc67, /// 0xcdc
			 0xe81a76c3, /// 0xce0
			 0x39126993, /// 0xce4
			 0x1ffdf9d5, /// 0xce8
			 0x1d81d83f, /// 0xcec
			 0x67d87ded, /// 0xcf0
			 0xf1bd7a2c, /// 0xcf4
			 0x94f33140, /// 0xcf8
			 0x3fb0f490, /// 0xcfc
			 0xefda6d5f, /// 0xd00
			 0x44786b51, /// 0xd04
			 0x288b2db2, /// 0xd08
			 0x2d54a1a7, /// 0xd0c
			 0x71188806, /// 0xd10
			 0x7825fcf5, /// 0xd14
			 0x22d4f19d, /// 0xd18
			 0xb846f41b, /// 0xd1c
			 0x44c3b7d7, /// 0xd20
			 0x3c63d649, /// 0xd24
			 0x2e67df05, /// 0xd28
			 0x06af0700, /// 0xd2c
			 0x4a3d8d4a, /// 0xd30
			 0xf8f856c2, /// 0xd34
			 0xba79f822, /// 0xd38
			 0x10f019f7, /// 0xd3c
			 0x59c1ffbe, /// 0xd40
			 0xc78c067b, /// 0xd44
			 0x0e524875, /// 0xd48
			 0x1b847daf, /// 0xd4c
			 0x96142430, /// 0xd50
			 0x025d7eb2, /// 0xd54
			 0x64736ee9, /// 0xd58
			 0x6e1fc988, /// 0xd5c
			 0xb64fe695, /// 0xd60
			 0xecfe09aa, /// 0xd64
			 0x6f270d06, /// 0xd68
			 0x4937a4cb, /// 0xd6c
			 0xbe6c67b0, /// 0xd70
			 0x7d357bfc, /// 0xd74
			 0x91103c85, /// 0xd78
			 0xe8c5e29c, /// 0xd7c
			 0x6b820179, /// 0xd80
			 0xe31c1975, /// 0xd84
			 0x82977aa1, /// 0xd88
			 0x00761d7c, /// 0xd8c
			 0x73b812ae, /// 0xd90
			 0xd7d7a401, /// 0xd94
			 0x437974e3, /// 0xd98
			 0xed87172c, /// 0xd9c
			 0x571b41a9, /// 0xda0
			 0x71b110e1, /// 0xda4
			 0x06c0a037, /// 0xda8
			 0x4f60d96e, /// 0xdac
			 0x6cad8c66, /// 0xdb0
			 0x54bfb0a5, /// 0xdb4
			 0x05bc41c6, /// 0xdb8
			 0x2a2207f7, /// 0xdbc
			 0x76eae422, /// 0xdc0
			 0xcf94d113, /// 0xdc4
			 0x4e5992fc, /// 0xdc8
			 0xdf72d23d, /// 0xdcc
			 0xff8965f1, /// 0xdd0
			 0x42a2c66b, /// 0xdd4
			 0x777d10b7, /// 0xdd8
			 0xcd4556cf, /// 0xddc
			 0x5d7cf683, /// 0xde0
			 0x5d69c7f2, /// 0xde4
			 0x2a797ce3, /// 0xde8
			 0x2468fbce, /// 0xdec
			 0x2f27faf3, /// 0xdf0
			 0x0a84f5e9, /// 0xdf4
			 0x2798aa5c, /// 0xdf8
			 0x0c7b74e5, /// 0xdfc
			 0xb1588831, /// 0xe00
			 0x8ce0e4ca, /// 0xe04
			 0x0179f438, /// 0xe08
			 0xc660b62c, /// 0xe0c
			 0xd058efb2, /// 0xe10
			 0x231f8dbc, /// 0xe14
			 0xe0276a47, /// 0xe18
			 0x4a63563e, /// 0xe1c
			 0x5dc78a17, /// 0xe20
			 0xf4d8724b, /// 0xe24
			 0x7f320c75, /// 0xe28
			 0x1d3f1c2f, /// 0xe2c
			 0x1021dafc, /// 0xe30
			 0x122b860e, /// 0xe34
			 0xa50fd047, /// 0xe38
			 0x5330c170, /// 0xe3c
			 0x797d9ea2, /// 0xe40
			 0x2044bc6d, /// 0xe44
			 0x603a969c, /// 0xe48
			 0x2b83ad69, /// 0xe4c
			 0x298949a4, /// 0xe50
			 0x9ad72420, /// 0xe54
			 0xfb30dd6e, /// 0xe58
			 0xad7cebd3, /// 0xe5c
			 0x267db74c, /// 0xe60
			 0x71b6eab5, /// 0xe64
			 0x7ad8cbbc, /// 0xe68
			 0x2f899d44, /// 0xe6c
			 0x989acc9b, /// 0xe70
			 0x383e8b97, /// 0xe74
			 0x85aa4fce, /// 0xe78
			 0x16efceea, /// 0xe7c
			 0x4c70df2f, /// 0xe80
			 0x250e7912, /// 0xe84
			 0xfb0bdb79, /// 0xe88
			 0x074b6cb5, /// 0xe8c
			 0xf6631299, /// 0xe90
			 0x14f535a5, /// 0xe94
			 0xf316bfd7, /// 0xe98
			 0x40794d93, /// 0xe9c
			 0x7e02cbd5, /// 0xea0
			 0x0f4fa5c7, /// 0xea4
			 0xee056007, /// 0xea8
			 0x7c004a77, /// 0xeac
			 0xf100b22a, /// 0xeb0
			 0x738b9ff0, /// 0xeb4
			 0x7806bf1b, /// 0xeb8
			 0x8f3c56f2, /// 0xebc
			 0x263e1fec, /// 0xec0
			 0x2ffb7e42, /// 0xec4
			 0xd127722a, /// 0xec8
			 0x2827e159, /// 0xecc
			 0xc38197d9, /// 0xed0
			 0xa9674e47, /// 0xed4
			 0xcd54a9fa, /// 0xed8
			 0x5649c61f, /// 0xedc
			 0x558fe871, /// 0xee0
			 0xd992bac2, /// 0xee4
			 0xabe37a2c, /// 0xee8
			 0xd398cade, /// 0xeec
			 0x655146f9, /// 0xef0
			 0x36bee645, /// 0xef4
			 0xbfbac490, /// 0xef8
			 0x311077ab, /// 0xefc
			 0xf5602086, /// 0xf00
			 0x2a075f64, /// 0xf04
			 0xc51999f9, /// 0xf08
			 0x5a905c64, /// 0xf0c
			 0xa79a3e8c, /// 0xf10
			 0xf7b32596, /// 0xf14
			 0x50584a6b, /// 0xf18
			 0xee3fd773, /// 0xf1c
			 0xfa8ec1c0, /// 0xf20
			 0xb0d2eee8, /// 0xf24
			 0x05fedd00, /// 0xf28
			 0x00fe6227, /// 0xf2c
			 0xd5d4a96b, /// 0xf30
			 0x97f66ce9, /// 0xf34
			 0x2a8599e2, /// 0xf38
			 0xaf64cce7, /// 0xf3c
			 0x7664e02e, /// 0xf40
			 0x4463da06, /// 0xf44
			 0xf4c9437b, /// 0xf48
			 0x437f1595, /// 0xf4c
			 0x6534282a, /// 0xf50
			 0x35f60894, /// 0xf54
			 0x1700aa62, /// 0xf58
			 0x90d1971a, /// 0xf5c
			 0xd46c1fd0, /// 0xf60
			 0xa452af5e, /// 0xf64
			 0x284f4b62, /// 0xf68
			 0x6712d744, /// 0xf6c
			 0x7c4a23ac, /// 0xf70
			 0xecba8b84, /// 0xf74
			 0x4730148c, /// 0xf78
			 0x2b5d0d41, /// 0xf7c
			 0xa0cf1ed2, /// 0xf80
			 0xca69c9d0, /// 0xf84
			 0xfa1105cf, /// 0xf88
			 0xf8523492, /// 0xf8c
			 0x2845647e, /// 0xf90
			 0xfe4bd8a6, /// 0xf94
			 0x28cd0b06, /// 0xf98
			 0xafcfaeb3, /// 0xf9c
			 0x577f4fab, /// 0xfa0
			 0xece35ce6, /// 0xfa4
			 0x5ac90b97, /// 0xfa8
			 0x23fa89ff, /// 0xfac
			 0x64e84d96, /// 0xfb0
			 0x308e2f11, /// 0xfb4
			 0x2d6c9920, /// 0xfb8
			 0x0747bd04, /// 0xfbc
			 0xf7d4e4ab, /// 0xfc0
			 0x2d7dbfa0, /// 0xfc4
			 0x46300b1a, /// 0xfc8
			 0x1bd8530b, /// 0xfcc
			 0xa8a15487, /// 0xfd0
			 0xc125920b, /// 0xfd4
			 0xa2b14d99, /// 0xfd8
			 0x25419ceb, /// 0xfdc
			 0x085f951b, /// 0xfe0
			 0x4f437551, /// 0xfe4
			 0x1c609655, /// 0xfe8
			 0xdda721dc, /// 0xfec
			 0x38454737, /// 0xff0
			 0x2c7240da, /// 0xff4
			 0x76c749b9, /// 0xff8
			 0x6868597c /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00000
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00004
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0000c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00014
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0001c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00020
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00024
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00028
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0002c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x00011111,                                                  // 9.7958E-41                                  /// 00034
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00048
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00050
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00054
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0005c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00064
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00068
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00070
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00074
			 0x80011111,                                                  // -9.7958E-41                                 /// 00078
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00080
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00088
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0008c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00090
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x4b000000,                                                  // 8388608.0                                   /// 00098
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0009c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a4
			 0x00000000,                                                  // zero                                        /// 000a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b0
			 0xffc00001,                                                  // -signaling NaN                              /// 000b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000dc
			 0xffc00001,                                                  // -signaling NaN                              /// 000e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f0
			 0x80000000,                                                  // -zero                                       /// 000f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00100
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00108
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0010c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00110
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00114
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00118
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00120
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00124
			 0x00000000,                                                  // zero                                        /// 00128
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00134
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00138
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0013c
			 0x00000000,                                                  // zero                                        /// 00140
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00144
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x3f028f5c,                                                  // 0.51                                        /// 0014c
			 0xbf028f5c,                                                  // -0.51                                       /// 00150
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00154
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0015c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00160
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00164
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00174
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00178
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0017c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00184
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00188
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0018c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00190
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x33333333,                                                  // 4 random values                             /// 00198
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0019c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0xffc00001,                                                  // -signaling NaN                              /// 001bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0xffc00001,                                                  // -signaling NaN                              /// 001d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x80000000,                                                  // -zero                                       /// 001e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00200
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00210
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00218
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00220
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00228
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0022c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00230
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00238
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0023c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0024c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00254
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x7f800000,                                                  // inf                                         /// 0025c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00264
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00268
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00274
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00284
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00288
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0028c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00294
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00298
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00300
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00304
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x0c700000,                                                  // Leading 1s:                                 /// 0030c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00310
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00318
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0031c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00320
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00324
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00328
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00330
			 0x7fc00001,                                                  // signaling NaN                               /// 00334
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00340
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0034c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00350
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00354
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00358
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e000001,                                                  // Trailing 1s:                                /// 0036c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00370
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00378
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0037c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00390
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00394
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00398
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0039c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 003e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00400
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00404
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00408
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0040c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00418
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0041c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00420
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x3f028f5c,                                                  // 0.51                                        /// 0042c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x4b000000,                                                  // 8388608.0                                   /// 00434
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00438
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0043c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00444
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0044c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00450
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0045c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00464
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00468
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0046c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80011111,                                                  // -9.7958E-41                                 /// 0047c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00488
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0048c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00490
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00494
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00498
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00500
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00508
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0050c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00514
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00520
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00524
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00528
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00530
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x0c600000,                                                  // Leading 1s:                                 /// 00538
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00540
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80000000,                                                  // -zero                                       /// 00548
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0054c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0xff800000,                                                  // -inf                                        /// 00558
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0xcb000000,                                                  // -8388608.0                                  /// 00560
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00564
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00568
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00570
			 0x55555555,                                                  // 4 random values                             /// 00574
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00584
			 0x80011111,                                                  // -9.7958E-41                                 /// 00588
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0058c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00590
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00598
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0xbf028f5c,                                                  // -0.51                                       /// 005b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0xffc00001,                                                  // -signaling NaN                              /// 005c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x55555555,                                                  // 4 random values                             /// 00600
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00604
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00608
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00614
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00618
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0061c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00620
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00630
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00634
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0063c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00640
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00644
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00648
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00650
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00654
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00658
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0065c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00660
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00664
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0066c
			 0xffc00001,                                                  // -signaling NaN                              /// 00670
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00674
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00678
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0067c
			 0x80000000,                                                  // -zero                                       /// 00680
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00688
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x4b000000,                                                  // 8388608.0                                   /// 00690
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x0c600000,                                                  // Leading 1s:                                 /// 00698
			 0x0c600000,                                                  // Leading 1s:                                 /// 0069c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006e4
			 0x00000000,                                                  // zero                                        /// 006e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 006f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00700
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00710
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00718
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00720
			 0x55555555,                                                  // 4 random values                             /// 00724
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00728
			 0xffc00001,                                                  // -signaling NaN                              /// 0072c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00730
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00738
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00740
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00744
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00748
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00750
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00754
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0075c
			 0x55555555,                                                  // 4 random values                             /// 00760
			 0x0e000007,                                                  // Trailing 1s:                                /// 00764
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0076c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00770
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00774
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0077c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00784
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00788
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0078c
			 0x7f800000,                                                  // inf                                         /// 00790
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0079c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00800
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00804
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00808
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00818
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00820
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00824
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00828
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0082c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c700000,                                                  // Leading 1s:                                 /// 00834
			 0x7f800000,                                                  // inf                                         /// 00838
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0083c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00848
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0084c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00850
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00860
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00864
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00868
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00874
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00878
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00880
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00888
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x3f028f5c,                                                  // 0.51                                        /// 00890
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00894
			 0xcb000000,                                                  // -8388608.0                                  /// 00898
			 0x00011111,                                                  // 9.7958E-41                                  /// 0089c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008a4
			 0x80000000,                                                  // -zero                                       /// 008a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x4b000000,                                                  // 8388608.0                                   /// 008d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00900
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0090c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00910
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00914
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00920
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00928
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00930
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00934
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00940
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0094c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00970
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00974
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0097c
			 0x7fc00001,                                                  // signaling NaN                               /// 00980
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00984
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0098c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0c400000,                                                  // Leading 1s:                                 /// 00994
			 0x0e000007,                                                  // Trailing 1s:                                /// 00998
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0099c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a5c
			 0xff800000,                                                  // -inf                                        /// 00a60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a84
			 0x33333333,                                                  // 4 random values                             /// 00a88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0xffc00001,                                                  // -signaling NaN                              /// 00a9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab4
			 0x33333333,                                                  // 4 random values                             /// 00ab8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00abc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ae4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00afc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b3c
			 0x80000000,                                                  // -zero                                       /// 00b40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x33333333,                                                  // 4 random values                             /// 00b7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bbc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bc0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00be4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bf4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x55555555,                                                  // 4 random values                             /// 00c18
			 0x55555555,                                                  // 4 random values                             /// 00c1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c20
			 0x80000000,                                                  // -zero                                       /// 00c24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x4b000000,                                                  // 8388608.0                                   /// 00c5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ca4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca8
			 0x55555555,                                                  // 4 random values                             /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cbc
			 0x55555555,                                                  // 4 random values                             /// 00cc0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ce0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ce4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cfc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d80
			 0xffc00001,                                                  // -signaling NaN                              /// 00d84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00000000,                                                  // zero                                        /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dac
			 0x55555555,                                                  // 4 random values                             /// 00db0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00db4
			 0x33333333,                                                  // 4 random values                             /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ddc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00de0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e64
			 0x80000000,                                                  // -zero                                       /// 00e68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e84
			 0x80000000,                                                  // -zero                                       /// 00e88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000000,                                                  // zero                                        /// 00ea4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00eb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ec4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ecc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ed0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ed4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ed8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00edc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ef8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f14
			 0xff800000,                                                  // -inf                                        /// 00f18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f58
			 0x4b000000,                                                  // 8388608.0                                   /// 00f5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fa4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0xffc00001,                                                  // -signaling NaN                              /// 00fe0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fe8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0e003fff                                                  // Trailing 1s:                                /// last
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
			 0x00000070,
			 0x00000018,
			 0x0000048c,
			 0x00000664,
			 0x00000488,
			 0x0000064c,
			 0x0000075c,
			 0x000006f8,

			 /// vpu register f2
			 0x41900000,
			 0x41300000,
			 0x41980000,
			 0x41100000,
			 0x40800000,
			 0x40000000,
			 0x41e80000,
			 0x40c00000,

			 /// vpu register f3
			 0x41e80000,
			 0x41700000,
			 0x3f800000,
			 0x41000000,
			 0x40400000,
			 0x41e80000,
			 0x41900000,
			 0x41600000,

			 /// vpu register f4
			 0x40000000,
			 0x3f800000,
			 0x41e00000,
			 0x40400000,
			 0x40c00000,
			 0x41880000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f5
			 0x40400000,
			 0x41100000,
			 0x41c00000,
			 0x41d80000,
			 0x41d00000,
			 0x41f00000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f6
			 0x41a80000,
			 0x41700000,
			 0x40a00000,
			 0x41c80000,
			 0x41e00000,
			 0x41400000,
			 0x41880000,
			 0x41b80000,

			 /// vpu register f7
			 0x41400000,
			 0x41c00000,
			 0x41300000,
			 0x41200000,
			 0x41700000,
			 0x41e80000,
			 0x3f800000,
			 0x41900000,

			 /// vpu register f8
			 0x40c00000,
			 0x41d00000,
			 0x41000000,
			 0x41f80000,
			 0x3f800000,
			 0x41f00000,
			 0x41980000,
			 0x40e00000,

			 /// vpu register f9
			 0x41600000,
			 0x41a00000,
			 0x41c00000,
			 0x40c00000,
			 0x40000000,
			 0x40c00000,
			 0x41900000,
			 0x41900000,

			 /// vpu register f10
			 0x40a00000,
			 0x41000000,
			 0x41800000,
			 0x41d00000,
			 0x42000000,
			 0x41900000,
			 0x41a80000,
			 0x40400000,

			 /// vpu register f11
			 0x41100000,
			 0x41a80000,
			 0x41700000,
			 0x41600000,
			 0x41a80000,
			 0x41e00000,
			 0x41f00000,
			 0x41100000,

			 /// vpu register f12
			 0x41e00000,
			 0x41d00000,
			 0x41d00000,
			 0x40c00000,
			 0x41300000,
			 0x41100000,
			 0x41400000,
			 0x41e80000,

			 /// vpu register f13
			 0x41300000,
			 0x40400000,
			 0x41b80000,
			 0x41e80000,
			 0x41c80000,
			 0x41c80000,
			 0x41b00000,
			 0x41f00000,

			 /// vpu register f14
			 0x41400000,
			 0x41800000,
			 0x42000000,
			 0x41e80000,
			 0x40c00000,
			 0x41e00000,
			 0x41000000,
			 0x41d00000,

			 /// vpu register f15
			 0x40a00000,
			 0x41c00000,
			 0x41100000,
			 0x41100000,
			 0x41400000,
			 0x41b00000,
			 0x41d80000,
			 0x41b80000,

			 /// vpu register f16
			 0x40000000,
			 0x41980000,
			 0x41500000,
			 0x41d00000,
			 0x40800000,
			 0x40400000,
			 0x41700000,
			 0x42000000,

			 /// vpu register f17
			 0x40c00000,
			 0x41800000,
			 0x41c80000,
			 0x41000000,
			 0x41a80000,
			 0x41b00000,
			 0x41d00000,
			 0x41900000,

			 /// vpu register f18
			 0x3f800000,
			 0x40c00000,
			 0x40c00000,
			 0x41700000,
			 0x41b00000,
			 0x41d80000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f19
			 0x41b80000,
			 0x41b00000,
			 0x40a00000,
			 0x41980000,
			 0x41100000,
			 0x41a00000,
			 0x41c80000,
			 0x41200000,

			 /// vpu register f20
			 0x41800000,
			 0x40400000,
			 0x41600000,
			 0x40400000,
			 0x41900000,
			 0x40a00000,
			 0x41d00000,
			 0x41900000,

			 /// vpu register f21
			 0x40e00000,
			 0x41a80000,
			 0x40000000,
			 0x40400000,
			 0x41500000,
			 0x41900000,
			 0x41700000,
			 0x41d80000,

			 /// vpu register f22
			 0x42000000,
			 0x41600000,
			 0x41d80000,
			 0x41c00000,
			 0x40400000,
			 0x41000000,
			 0x41000000,
			 0x41f00000,

			 /// vpu register f23
			 0x41900000,
			 0x3f800000,
			 0x40000000,
			 0x41900000,
			 0x41c00000,
			 0x41000000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f24
			 0x41980000,
			 0x41f80000,
			 0x41a80000,
			 0x41a80000,
			 0x41200000,
			 0x41200000,
			 0x41a80000,
			 0x41200000,

			 /// vpu register f25
			 0x41980000,
			 0x41300000,
			 0x41980000,
			 0x40a00000,
			 0x41e00000,
			 0x41100000,
			 0x41700000,
			 0x41700000,

			 /// vpu register f26
			 0x41100000,
			 0x41980000,
			 0x41f00000,
			 0x41f00000,
			 0x41a80000,
			 0x41900000,
			 0x41880000,
			 0x41880000,

			 /// vpu register f27
			 0x41e00000,
			 0x41200000,
			 0x41c80000,
			 0x41400000,
			 0x42000000,
			 0x42000000,
			 0x40a00000,
			 0x40000000,

			 /// vpu register f28
			 0x41f00000,
			 0x41b00000,
			 0x41e80000,
			 0x41c00000,
			 0x41a80000,
			 0x40800000,
			 0x41d80000,
			 0x41f80000,

			 /// vpu register f29
			 0x41500000,
			 0x40400000,
			 0x41100000,
			 0x41b80000,
			 0x41900000,
			 0x41000000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f30
			 0x41100000,
			 0x41b80000,
			 0x41400000,
			 0x41b80000,
			 0x41980000,
			 0x41b00000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f31
			 0x40400000,
			 0x41d80000,
			 0x41e00000,
			 0x41600000,
			 0x3f800000,
			 0x41f80000,
			 0x41500000,
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
		"fbci.pi f8, 351816\n"                                ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f24, 504226\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f6, 479463\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f11, 176646\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f25, 189916\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 470490\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 425297\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f12, 508859\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f9, 82613\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f26, 280975\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f23, 250405\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f29, 448096\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f27, 148106\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f9, 76202\n"                                 ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f18, 390754\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f2, 263554\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f21, 428807\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f23, 261691\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f4, 412474\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f6, 285776\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f26, 47397\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f21, 154403\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f28, 480330\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 330579\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f4, 221463\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f14, 240959\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f8, 249545\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f27, 443016\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 479202\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f18, 418745\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f6, 411677\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f29, 464867\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f4, 349365\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f29, 501313\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f2, 240472\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f18, 524012\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 240470\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f17, 208621\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f25, 185394\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f22, 484238\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f18, 67494\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f19, 77243\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f4, 195591\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f14, 396458\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f18, 406103\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f12, 135684\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f20, 510398\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f9, 440298\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f28, 239143\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f7, 59376\n"                                 ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f8, 311116\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f30, 128856\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f4, 383254\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f10, 349705\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f7, 416830\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f17, 412776\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f27, 337421\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f5, 493567\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f11, 306538\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f11, 278838\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f25, 183998\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f14, 331489\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f6, 101096\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f28, 209072\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f8, 210773\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f29, 42068\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f26, 54969\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f29, 82032\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f7, 121130\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f11, 14691\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f6, 473841\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f22, 347090\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f20, 72108\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 436511\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f27, 490851\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f29, 46281\n"                                ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f2, 287027\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f30, 398118\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f11, 119990\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f26, 181692\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f12, 497822\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f17, 408647\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f12, 274272\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f25, 89778\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f2, 25989\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f5, 177824\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f17, 486098\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f19, 30883\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f15, 137943\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f13, 240962\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f16, 133102\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f26, 169568\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f13, 355389\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f28, 279695\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f22, 243343\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f11, 354370\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f21, 64355\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f28, 441114\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fbci.pi f5, 98730\n"                                 ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
