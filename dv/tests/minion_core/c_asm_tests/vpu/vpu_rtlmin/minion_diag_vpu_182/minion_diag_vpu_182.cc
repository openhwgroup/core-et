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
			 0xf074bee0, /// 0x0
			 0x1bb1ba96, /// 0x4
			 0xd7a9ed00, /// 0x8
			 0x6c1a4511, /// 0xc
			 0x009cfb97, /// 0x10
			 0x06cc8346, /// 0x14
			 0x4fa4b148, /// 0x18
			 0xbbb306bb, /// 0x1c
			 0x1fc2207e, /// 0x20
			 0x05009c9c, /// 0x24
			 0x7094e18f, /// 0x28
			 0x98ebb8ee, /// 0x2c
			 0x781835fa, /// 0x30
			 0xd2c15d39, /// 0x34
			 0xb749aa0f, /// 0x38
			 0x1b85b0cb, /// 0x3c
			 0x6d2ded2a, /// 0x40
			 0x72914040, /// 0x44
			 0xff79edb2, /// 0x48
			 0xabb2701d, /// 0x4c
			 0x6b464b38, /// 0x50
			 0x669f73cf, /// 0x54
			 0x5aca7623, /// 0x58
			 0x20ab33a1, /// 0x5c
			 0x2ff6289c, /// 0x60
			 0xb4130493, /// 0x64
			 0xae4ad100, /// 0x68
			 0xc0b1acab, /// 0x6c
			 0xa617abc4, /// 0x70
			 0xed1ed4c3, /// 0x74
			 0x5cade6cf, /// 0x78
			 0xa040af8e, /// 0x7c
			 0xd198595c, /// 0x80
			 0xe385f3e5, /// 0x84
			 0x8b92a7e3, /// 0x88
			 0x6f1421c1, /// 0x8c
			 0x7611ac7b, /// 0x90
			 0x90ceff62, /// 0x94
			 0x2626c586, /// 0x98
			 0x75c7fa75, /// 0x9c
			 0xb48ecea8, /// 0xa0
			 0x925242bc, /// 0xa4
			 0x20eaa06b, /// 0xa8
			 0x511a72b0, /// 0xac
			 0xef4d2d41, /// 0xb0
			 0x96015b0e, /// 0xb4
			 0xa4d275ac, /// 0xb8
			 0x05ce4a59, /// 0xbc
			 0x61c199b3, /// 0xc0
			 0xf0eb655d, /// 0xc4
			 0xb772fea9, /// 0xc8
			 0xfa75d54e, /// 0xcc
			 0x17334ebb, /// 0xd0
			 0x4fea995d, /// 0xd4
			 0xc87d3d74, /// 0xd8
			 0x35c4a9a2, /// 0xdc
			 0x393793df, /// 0xe0
			 0x8918d36c, /// 0xe4
			 0x1f0d8591, /// 0xe8
			 0x2245c78e, /// 0xec
			 0x35894273, /// 0xf0
			 0xef0c9688, /// 0xf4
			 0xbc7df852, /// 0xf8
			 0x0bdc97fb, /// 0xfc
			 0x31119111, /// 0x100
			 0x73d7ed2d, /// 0x104
			 0xc982296e, /// 0x108
			 0x259abc02, /// 0x10c
			 0x54490df3, /// 0x110
			 0x7ad3277e, /// 0x114
			 0x4542f3d7, /// 0x118
			 0x756f862c, /// 0x11c
			 0xa69a5a98, /// 0x120
			 0x5f31913b, /// 0x124
			 0x8fef525b, /// 0x128
			 0x85f60f0e, /// 0x12c
			 0xd091e557, /// 0x130
			 0xe98018c4, /// 0x134
			 0x065bc9d9, /// 0x138
			 0x3a83d92d, /// 0x13c
			 0xbf54181a, /// 0x140
			 0x5eca5cb4, /// 0x144
			 0xd1f722b8, /// 0x148
			 0xede49f73, /// 0x14c
			 0x1217476b, /// 0x150
			 0x14b0d56f, /// 0x154
			 0xa51700d6, /// 0x158
			 0x7aa04f3e, /// 0x15c
			 0xce485710, /// 0x160
			 0x396d58b7, /// 0x164
			 0xb212899a, /// 0x168
			 0x5865cff8, /// 0x16c
			 0xc95f5696, /// 0x170
			 0xa7c9f6a8, /// 0x174
			 0xc2c52744, /// 0x178
			 0x85e33aa6, /// 0x17c
			 0xb4f4cda1, /// 0x180
			 0x7e92dae0, /// 0x184
			 0xb3e65f66, /// 0x188
			 0x4f8d4900, /// 0x18c
			 0xec764a0e, /// 0x190
			 0x960fb96c, /// 0x194
			 0xf683c950, /// 0x198
			 0x8094ef25, /// 0x19c
			 0x4f0e9b72, /// 0x1a0
			 0x2e3ccfeb, /// 0x1a4
			 0x8c1b47b5, /// 0x1a8
			 0xe97f5010, /// 0x1ac
			 0x304c933b, /// 0x1b0
			 0xa0b1dae2, /// 0x1b4
			 0x64c3da26, /// 0x1b8
			 0x1f645769, /// 0x1bc
			 0xef0f3f78, /// 0x1c0
			 0xaa71ee3b, /// 0x1c4
			 0xc248f533, /// 0x1c8
			 0xf4683454, /// 0x1cc
			 0x513d53bf, /// 0x1d0
			 0xf0e418c5, /// 0x1d4
			 0x23543c57, /// 0x1d8
			 0x91c86c2a, /// 0x1dc
			 0xaf5bea34, /// 0x1e0
			 0x1e46ede2, /// 0x1e4
			 0x43f31cff, /// 0x1e8
			 0xc77ed13b, /// 0x1ec
			 0x6ad3290d, /// 0x1f0
			 0x148eb812, /// 0x1f4
			 0xd2c50310, /// 0x1f8
			 0xcaff104f, /// 0x1fc
			 0x15d9b3d1, /// 0x200
			 0xcc81fc77, /// 0x204
			 0xb5a4c8ae, /// 0x208
			 0x4860b7be, /// 0x20c
			 0xe654f238, /// 0x210
			 0x88c816f8, /// 0x214
			 0x574859a6, /// 0x218
			 0x2725f9fb, /// 0x21c
			 0xac6e0158, /// 0x220
			 0x8f286adc, /// 0x224
			 0x0b89d973, /// 0x228
			 0x500c5aaa, /// 0x22c
			 0x908357c5, /// 0x230
			 0xe4b908c5, /// 0x234
			 0xebea6c23, /// 0x238
			 0xe4cce47f, /// 0x23c
			 0xface4fb5, /// 0x240
			 0x59d19414, /// 0x244
			 0x8835c2ed, /// 0x248
			 0x32722770, /// 0x24c
			 0x89c08e4d, /// 0x250
			 0x0a86a11e, /// 0x254
			 0x932f03d1, /// 0x258
			 0xd419b49c, /// 0x25c
			 0xa491ee4b, /// 0x260
			 0xdadcc7c2, /// 0x264
			 0xcbe5a939, /// 0x268
			 0xa7902ea6, /// 0x26c
			 0x5366084a, /// 0x270
			 0x55da2f54, /// 0x274
			 0x45c17f2a, /// 0x278
			 0x8d99f61f, /// 0x27c
			 0x2b135473, /// 0x280
			 0x92030099, /// 0x284
			 0x6f681dcc, /// 0x288
			 0x8525fb74, /// 0x28c
			 0x74d1a747, /// 0x290
			 0x2a1b3816, /// 0x294
			 0xcb79347b, /// 0x298
			 0xd92b0d85, /// 0x29c
			 0x5ec22b54, /// 0x2a0
			 0xd9cbbc30, /// 0x2a4
			 0x3672bb35, /// 0x2a8
			 0xfb3f4526, /// 0x2ac
			 0xf878cb4e, /// 0x2b0
			 0xbaa15a87, /// 0x2b4
			 0x3868f20e, /// 0x2b8
			 0x34be5a34, /// 0x2bc
			 0xf242f8d5, /// 0x2c0
			 0x87adec22, /// 0x2c4
			 0xd5b2e593, /// 0x2c8
			 0xfc6a6d69, /// 0x2cc
			 0x0666bba8, /// 0x2d0
			 0x0548bd1d, /// 0x2d4
			 0x9fc70cc3, /// 0x2d8
			 0xe792eaa9, /// 0x2dc
			 0x2dbb6b95, /// 0x2e0
			 0x839c6cb8, /// 0x2e4
			 0xda425973, /// 0x2e8
			 0x95e8fed7, /// 0x2ec
			 0xece57a17, /// 0x2f0
			 0x736243c0, /// 0x2f4
			 0x9267c678, /// 0x2f8
			 0x2cb94c15, /// 0x2fc
			 0x0031f8e9, /// 0x300
			 0xa7f409cf, /// 0x304
			 0x4f48fd52, /// 0x308
			 0x0a3a0cf5, /// 0x30c
			 0x47d6ebb1, /// 0x310
			 0x2d367da9, /// 0x314
			 0x73e9f5ef, /// 0x318
			 0xa9103464, /// 0x31c
			 0x7e7442ed, /// 0x320
			 0x6066af3f, /// 0x324
			 0x77ef5d44, /// 0x328
			 0xfd6b15b6, /// 0x32c
			 0xd9b1bc3e, /// 0x330
			 0x782f11f3, /// 0x334
			 0xcc2fc0ef, /// 0x338
			 0xa7627e93, /// 0x33c
			 0x75df7181, /// 0x340
			 0x394743bf, /// 0x344
			 0x64961f9c, /// 0x348
			 0x8e0f34f3, /// 0x34c
			 0x7c260a75, /// 0x350
			 0xe36a07fa, /// 0x354
			 0xc8f48084, /// 0x358
			 0x588c11fd, /// 0x35c
			 0x37c8395b, /// 0x360
			 0xcdf9f290, /// 0x364
			 0x28e43e91, /// 0x368
			 0x75a46718, /// 0x36c
			 0x80426541, /// 0x370
			 0x1d0cc552, /// 0x374
			 0x0f13820e, /// 0x378
			 0x7cea21a4, /// 0x37c
			 0xd60d8b95, /// 0x380
			 0x3e64e4e0, /// 0x384
			 0xdf4b963f, /// 0x388
			 0x1dacedb2, /// 0x38c
			 0x5b494690, /// 0x390
			 0x0ede1e74, /// 0x394
			 0x9d8fb7c7, /// 0x398
			 0xc6648e6b, /// 0x39c
			 0x5e07af74, /// 0x3a0
			 0x24d50cc3, /// 0x3a4
			 0x90e0b7fd, /// 0x3a8
			 0x144a198c, /// 0x3ac
			 0x2e2cdac7, /// 0x3b0
			 0xcbed5070, /// 0x3b4
			 0x69432678, /// 0x3b8
			 0x5d18d1a1, /// 0x3bc
			 0xc77075d1, /// 0x3c0
			 0x4fc3fc0d, /// 0x3c4
			 0xcb4215f4, /// 0x3c8
			 0xbc4e8657, /// 0x3cc
			 0xa5829f9c, /// 0x3d0
			 0xf61209d1, /// 0x3d4
			 0xf82a1afa, /// 0x3d8
			 0xfef16fce, /// 0x3dc
			 0xd7696945, /// 0x3e0
			 0x9e9b10df, /// 0x3e4
			 0xf63ceba0, /// 0x3e8
			 0x0a0ded02, /// 0x3ec
			 0x2d6fda17, /// 0x3f0
			 0xa372bc1c, /// 0x3f4
			 0xa2219175, /// 0x3f8
			 0x4e7056ba, /// 0x3fc
			 0x258a2409, /// 0x400
			 0xf1cbb92a, /// 0x404
			 0x850354c2, /// 0x408
			 0x5b10485a, /// 0x40c
			 0xa6197d60, /// 0x410
			 0x659e448d, /// 0x414
			 0x8cb2ccb5, /// 0x418
			 0xd136dbc5, /// 0x41c
			 0x0428deca, /// 0x420
			 0xc5e677df, /// 0x424
			 0x2b52fd0a, /// 0x428
			 0xb8f9be7a, /// 0x42c
			 0x2427a408, /// 0x430
			 0xd34e0227, /// 0x434
			 0x32a97db9, /// 0x438
			 0x7efa3853, /// 0x43c
			 0x826075d9, /// 0x440
			 0xb2e80e88, /// 0x444
			 0xdfcdee26, /// 0x448
			 0x4a7af465, /// 0x44c
			 0x8d964b51, /// 0x450
			 0x5d533e2b, /// 0x454
			 0xf2959707, /// 0x458
			 0xd2efbef4, /// 0x45c
			 0x5ad62882, /// 0x460
			 0x4d9bd16f, /// 0x464
			 0xada022bf, /// 0x468
			 0x6631fe9b, /// 0x46c
			 0x75430739, /// 0x470
			 0xcf7171e3, /// 0x474
			 0xd169d517, /// 0x478
			 0x74a0af3e, /// 0x47c
			 0x258f4413, /// 0x480
			 0xf5f6b622, /// 0x484
			 0x94f5091e, /// 0x488
			 0x8dcadb8d, /// 0x48c
			 0x76e7b9f7, /// 0x490
			 0xffbadd47, /// 0x494
			 0x38f9cffd, /// 0x498
			 0x989cb7bf, /// 0x49c
			 0x02fa58db, /// 0x4a0
			 0xf121cf6c, /// 0x4a4
			 0x4bf975b6, /// 0x4a8
			 0x81a69d1f, /// 0x4ac
			 0x8355b917, /// 0x4b0
			 0xf5d44547, /// 0x4b4
			 0x96d77ac8, /// 0x4b8
			 0x11f740e9, /// 0x4bc
			 0x0494aeeb, /// 0x4c0
			 0x526a155a, /// 0x4c4
			 0x7dafe9f7, /// 0x4c8
			 0xa14dff1e, /// 0x4cc
			 0xc31e3b47, /// 0x4d0
			 0xe2269e28, /// 0x4d4
			 0x14a640ef, /// 0x4d8
			 0x4815a416, /// 0x4dc
			 0xb3862ad1, /// 0x4e0
			 0x35770b9a, /// 0x4e4
			 0x54f5c7b0, /// 0x4e8
			 0x8b23ba59, /// 0x4ec
			 0xacc57536, /// 0x4f0
			 0x16ad2e21, /// 0x4f4
			 0xb2fdf65b, /// 0x4f8
			 0x42181593, /// 0x4fc
			 0x6974733d, /// 0x500
			 0x765a48ef, /// 0x504
			 0x11a885ce, /// 0x508
			 0x226e050e, /// 0x50c
			 0x22cedce9, /// 0x510
			 0xd3e6114b, /// 0x514
			 0x924652e7, /// 0x518
			 0x120e2574, /// 0x51c
			 0xc8d99256, /// 0x520
			 0xcab1c56f, /// 0x524
			 0xc71faf59, /// 0x528
			 0x0bb46d7d, /// 0x52c
			 0x2c12acf8, /// 0x530
			 0x4a2ec17d, /// 0x534
			 0xd207c162, /// 0x538
			 0x74c2b72e, /// 0x53c
			 0x382909dd, /// 0x540
			 0x84a5609b, /// 0x544
			 0xc8d00005, /// 0x548
			 0xf8c76a91, /// 0x54c
			 0xab74cb48, /// 0x550
			 0x41ecf6ef, /// 0x554
			 0x1f8c741d, /// 0x558
			 0x09b296e5, /// 0x55c
			 0x728f921c, /// 0x560
			 0xd34edaae, /// 0x564
			 0x8b01e5a2, /// 0x568
			 0x9f036cd9, /// 0x56c
			 0x8138e6ce, /// 0x570
			 0xfed7e671, /// 0x574
			 0x0d313fc1, /// 0x578
			 0xb1686195, /// 0x57c
			 0x4fb17c40, /// 0x580
			 0x648ab13f, /// 0x584
			 0x11371d2f, /// 0x588
			 0x2b7ec180, /// 0x58c
			 0xfddb66c9, /// 0x590
			 0x63a41f51, /// 0x594
			 0xd216d807, /// 0x598
			 0x6630ffbd, /// 0x59c
			 0xbdc8dc63, /// 0x5a0
			 0xdcc82938, /// 0x5a4
			 0xeb45240d, /// 0x5a8
			 0x133d1bcf, /// 0x5ac
			 0xd8674fa4, /// 0x5b0
			 0xab5516a7, /// 0x5b4
			 0xa4ad9006, /// 0x5b8
			 0x338a3318, /// 0x5bc
			 0x4ededa8c, /// 0x5c0
			 0xa5ae6ac8, /// 0x5c4
			 0x180c4acf, /// 0x5c8
			 0xfb19c249, /// 0x5cc
			 0x97256bf6, /// 0x5d0
			 0x6b6f513c, /// 0x5d4
			 0x565284fa, /// 0x5d8
			 0x24e4b77a, /// 0x5dc
			 0x7e67798f, /// 0x5e0
			 0xfd08d4a9, /// 0x5e4
			 0xb8714b99, /// 0x5e8
			 0xb313f080, /// 0x5ec
			 0x33f43eb4, /// 0x5f0
			 0xcfea8dd5, /// 0x5f4
			 0x86ee0ec4, /// 0x5f8
			 0x643e1fc0, /// 0x5fc
			 0x8d3e17cd, /// 0x600
			 0x8cdb4500, /// 0x604
			 0x1cc9a617, /// 0x608
			 0xd95a9ca9, /// 0x60c
			 0x2f2d5dbf, /// 0x610
			 0x92c6143d, /// 0x614
			 0x59f54aef, /// 0x618
			 0x072437fa, /// 0x61c
			 0xe01a808f, /// 0x620
			 0xffeb83d4, /// 0x624
			 0xd120f1d0, /// 0x628
			 0xb8860484, /// 0x62c
			 0xe8733266, /// 0x630
			 0xd46a80ad, /// 0x634
			 0xf817a6ed, /// 0x638
			 0x85315f7e, /// 0x63c
			 0x8c54c603, /// 0x640
			 0xc66ec352, /// 0x644
			 0x11572a97, /// 0x648
			 0x59d21ebe, /// 0x64c
			 0x04aed3e4, /// 0x650
			 0x448ab203, /// 0x654
			 0xbbc9932a, /// 0x658
			 0x24b4ff0f, /// 0x65c
			 0x37651c0d, /// 0x660
			 0xc2ca9d84, /// 0x664
			 0xe36c2ae9, /// 0x668
			 0xd9e2d5e9, /// 0x66c
			 0xb2fa3f57, /// 0x670
			 0x880476f9, /// 0x674
			 0x739f1fe3, /// 0x678
			 0xdc7c8c42, /// 0x67c
			 0xd22149bd, /// 0x680
			 0x87ef542a, /// 0x684
			 0x09ab3211, /// 0x688
			 0x11ac2792, /// 0x68c
			 0x8d40dc11, /// 0x690
			 0xd2cce94a, /// 0x694
			 0x2744ad40, /// 0x698
			 0x97b2e4a5, /// 0x69c
			 0x64e5c4c1, /// 0x6a0
			 0x4a3cb54b, /// 0x6a4
			 0xbcdfc383, /// 0x6a8
			 0x87b89854, /// 0x6ac
			 0x4afd36f3, /// 0x6b0
			 0x210e5ae5, /// 0x6b4
			 0x0b71bc06, /// 0x6b8
			 0x497dcb8b, /// 0x6bc
			 0xf973a411, /// 0x6c0
			 0xa9c3f079, /// 0x6c4
			 0x8f132ef0, /// 0x6c8
			 0x733376b9, /// 0x6cc
			 0x8dfffce5, /// 0x6d0
			 0x3712051f, /// 0x6d4
			 0xd945cf57, /// 0x6d8
			 0x71868f91, /// 0x6dc
			 0x81b3ac9b, /// 0x6e0
			 0xd9dd4da5, /// 0x6e4
			 0x59bef612, /// 0x6e8
			 0x7fd7997d, /// 0x6ec
			 0x4fd8f021, /// 0x6f0
			 0xa8c3a444, /// 0x6f4
			 0x788e9058, /// 0x6f8
			 0x855073d4, /// 0x6fc
			 0x1a415882, /// 0x700
			 0x2c4d26d1, /// 0x704
			 0xd74810ee, /// 0x708
			 0xabcc2f60, /// 0x70c
			 0xa0eaf9db, /// 0x710
			 0xf08bd05d, /// 0x714
			 0x21b39fb6, /// 0x718
			 0x80af247e, /// 0x71c
			 0xddce27fd, /// 0x720
			 0xce8bcd92, /// 0x724
			 0xec8d1ce2, /// 0x728
			 0x71879fe4, /// 0x72c
			 0x195227fc, /// 0x730
			 0x7b0c35e0, /// 0x734
			 0x440a1c8a, /// 0x738
			 0x176cd346, /// 0x73c
			 0x5fa53af0, /// 0x740
			 0x6764a61f, /// 0x744
			 0x1b391b69, /// 0x748
			 0xb857626e, /// 0x74c
			 0x436702c6, /// 0x750
			 0xd7f69d41, /// 0x754
			 0x10cb9534, /// 0x758
			 0x0d38bca7, /// 0x75c
			 0x9cfe3057, /// 0x760
			 0xd657afa7, /// 0x764
			 0x925a2be8, /// 0x768
			 0xb4e29468, /// 0x76c
			 0x6a8e14d7, /// 0x770
			 0x6d2e5429, /// 0x774
			 0x168cfba4, /// 0x778
			 0x5d8ee9a3, /// 0x77c
			 0xfb08ac3c, /// 0x780
			 0xe945c6ae, /// 0x784
			 0x64e10966, /// 0x788
			 0xc26aeff7, /// 0x78c
			 0xec5ab2b8, /// 0x790
			 0xe66703aa, /// 0x794
			 0xe8d1bd1b, /// 0x798
			 0x54f98b7d, /// 0x79c
			 0x471e5f3d, /// 0x7a0
			 0x9eda8b01, /// 0x7a4
			 0xb0333203, /// 0x7a8
			 0x04bec371, /// 0x7ac
			 0xaf483559, /// 0x7b0
			 0x86210b7a, /// 0x7b4
			 0xdce62342, /// 0x7b8
			 0x2360a34f, /// 0x7bc
			 0x69ec4121, /// 0x7c0
			 0x1db544f4, /// 0x7c4
			 0x881330c3, /// 0x7c8
			 0x29df47e8, /// 0x7cc
			 0xd58f20c6, /// 0x7d0
			 0xdc8a82c9, /// 0x7d4
			 0x1e9e9b1b, /// 0x7d8
			 0xb23199e2, /// 0x7dc
			 0x2e704889, /// 0x7e0
			 0x0972755a, /// 0x7e4
			 0xfc386c92, /// 0x7e8
			 0x3cdd5bd1, /// 0x7ec
			 0x2ee6a589, /// 0x7f0
			 0xcde5e6e8, /// 0x7f4
			 0xf8ab0970, /// 0x7f8
			 0x03c1791a, /// 0x7fc
			 0x1025c81f, /// 0x800
			 0x20a1b5b6, /// 0x804
			 0x9b34bfeb, /// 0x808
			 0xd96b5133, /// 0x80c
			 0xc0c31d31, /// 0x810
			 0x5d38285f, /// 0x814
			 0x0615a50e, /// 0x818
			 0xff2bfc87, /// 0x81c
			 0xc269e1d4, /// 0x820
			 0x7a0c5ae9, /// 0x824
			 0x2ed7c338, /// 0x828
			 0x64b5cb20, /// 0x82c
			 0x6c7705e0, /// 0x830
			 0xac2b8551, /// 0x834
			 0xd7ba7868, /// 0x838
			 0x3f2a7ff7, /// 0x83c
			 0x6aeb051e, /// 0x840
			 0x79aa22b8, /// 0x844
			 0xf18dfd55, /// 0x848
			 0xc3e553a4, /// 0x84c
			 0xd23fd3b7, /// 0x850
			 0x3bf8c756, /// 0x854
			 0x2a33abfc, /// 0x858
			 0xa66a381a, /// 0x85c
			 0x768865ec, /// 0x860
			 0xf623a3d3, /// 0x864
			 0xc800a29b, /// 0x868
			 0xec53a262, /// 0x86c
			 0x7b94d494, /// 0x870
			 0x66dfa074, /// 0x874
			 0xcc6cf039, /// 0x878
			 0xb2aa94a6, /// 0x87c
			 0xcb5d4e4d, /// 0x880
			 0x14f325cf, /// 0x884
			 0x3bacd612, /// 0x888
			 0xddb198ed, /// 0x88c
			 0xf63836a2, /// 0x890
			 0xe18180fb, /// 0x894
			 0xe50655f2, /// 0x898
			 0x289b201c, /// 0x89c
			 0xf0120bd4, /// 0x8a0
			 0xafb68437, /// 0x8a4
			 0x66e05d9c, /// 0x8a8
			 0x7b0d353c, /// 0x8ac
			 0xc0492a51, /// 0x8b0
			 0x1b6bebc6, /// 0x8b4
			 0x49e1ecd5, /// 0x8b8
			 0x8cc3226a, /// 0x8bc
			 0x2d59d926, /// 0x8c0
			 0x06c325fd, /// 0x8c4
			 0x3de36040, /// 0x8c8
			 0xe4375bb4, /// 0x8cc
			 0xbe113ee2, /// 0x8d0
			 0xa3541add, /// 0x8d4
			 0x8e8a5d09, /// 0x8d8
			 0xdab65464, /// 0x8dc
			 0x8adb0097, /// 0x8e0
			 0x8af70bc6, /// 0x8e4
			 0x114ce3cd, /// 0x8e8
			 0xc6f32845, /// 0x8ec
			 0xa3432c58, /// 0x8f0
			 0x75417a12, /// 0x8f4
			 0xbeb77f48, /// 0x8f8
			 0xb0f70370, /// 0x8fc
			 0x007fbdad, /// 0x900
			 0xae7f6f0b, /// 0x904
			 0xa9522f63, /// 0x908
			 0xb5011ae0, /// 0x90c
			 0xc08cba1e, /// 0x910
			 0x13d96afb, /// 0x914
			 0xa061f6f7, /// 0x918
			 0x383cdf1b, /// 0x91c
			 0xc0e996cd, /// 0x920
			 0x7a600e92, /// 0x924
			 0x19045400, /// 0x928
			 0xd983b9a4, /// 0x92c
			 0x920dc0e9, /// 0x930
			 0xd7accd23, /// 0x934
			 0xe86f61fa, /// 0x938
			 0x3f2bd96b, /// 0x93c
			 0x18c46d65, /// 0x940
			 0xeaf6477e, /// 0x944
			 0xd6248dd0, /// 0x948
			 0x33fb186f, /// 0x94c
			 0xcbfb01e1, /// 0x950
			 0xabf7a1c6, /// 0x954
			 0x95780b11, /// 0x958
			 0xa196d8c4, /// 0x95c
			 0xfa0eeee7, /// 0x960
			 0xc3f01791, /// 0x964
			 0xd4b914f9, /// 0x968
			 0x6c6147ac, /// 0x96c
			 0xae7ee027, /// 0x970
			 0x0a7e3e5d, /// 0x974
			 0xb2a17c05, /// 0x978
			 0xe686ba99, /// 0x97c
			 0xab990c91, /// 0x980
			 0xf25ec20e, /// 0x984
			 0x22e5b127, /// 0x988
			 0xce52eaf1, /// 0x98c
			 0xfd1a2217, /// 0x990
			 0x345f9eac, /// 0x994
			 0x36a28267, /// 0x998
			 0x8961568a, /// 0x99c
			 0xd04d1c8b, /// 0x9a0
			 0x4513de35, /// 0x9a4
			 0x43158859, /// 0x9a8
			 0xcca78db9, /// 0x9ac
			 0xda371bfb, /// 0x9b0
			 0xfa98f9b1, /// 0x9b4
			 0x217488d1, /// 0x9b8
			 0xbe493381, /// 0x9bc
			 0x4dde0692, /// 0x9c0
			 0xde3262bd, /// 0x9c4
			 0x8fb900e3, /// 0x9c8
			 0x80ad951a, /// 0x9cc
			 0x3fed3d03, /// 0x9d0
			 0x4e2d9c6b, /// 0x9d4
			 0x71b05aa8, /// 0x9d8
			 0xd44c0e6e, /// 0x9dc
			 0x7657f018, /// 0x9e0
			 0xf1ef5768, /// 0x9e4
			 0xa30fc98a, /// 0x9e8
			 0x88aedcf2, /// 0x9ec
			 0x2a2f3061, /// 0x9f0
			 0xfc6d0b99, /// 0x9f4
			 0x016a9921, /// 0x9f8
			 0x7bff419f, /// 0x9fc
			 0xa2868a47, /// 0xa00
			 0xf17a00eb, /// 0xa04
			 0x9882e89c, /// 0xa08
			 0x0ca6dd65, /// 0xa0c
			 0xb08a0a72, /// 0xa10
			 0x96424b03, /// 0xa14
			 0x4476894a, /// 0xa18
			 0x3c717e80, /// 0xa1c
			 0x976b6918, /// 0xa20
			 0x1827e82e, /// 0xa24
			 0xae86d0b7, /// 0xa28
			 0x0769f711, /// 0xa2c
			 0xe8004052, /// 0xa30
			 0x2855efe7, /// 0xa34
			 0x175df655, /// 0xa38
			 0x09d73e25, /// 0xa3c
			 0x11bad8e9, /// 0xa40
			 0xbd4603e5, /// 0xa44
			 0x4ecb589a, /// 0xa48
			 0x02bbe59a, /// 0xa4c
			 0x75bb003c, /// 0xa50
			 0x99e9415f, /// 0xa54
			 0x325f66d6, /// 0xa58
			 0x2b9966c8, /// 0xa5c
			 0x25a679a0, /// 0xa60
			 0x7f248f53, /// 0xa64
			 0x83a3499e, /// 0xa68
			 0xdb016178, /// 0xa6c
			 0x2f46e8ba, /// 0xa70
			 0xe46ee54d, /// 0xa74
			 0x3cdba2f3, /// 0xa78
			 0x2270920c, /// 0xa7c
			 0xcfa2c724, /// 0xa80
			 0x4f44f3b5, /// 0xa84
			 0x7c1d45d4, /// 0xa88
			 0x2bd276c4, /// 0xa8c
			 0x2f10d2f5, /// 0xa90
			 0xdb837585, /// 0xa94
			 0x4fc750f4, /// 0xa98
			 0xb1748bd6, /// 0xa9c
			 0x5340cc4d, /// 0xaa0
			 0xdcdba08c, /// 0xaa4
			 0xf3606d73, /// 0xaa8
			 0x1a10c85a, /// 0xaac
			 0x7e9c605f, /// 0xab0
			 0x39919b2e, /// 0xab4
			 0xa9daa996, /// 0xab8
			 0xd18d94f0, /// 0xabc
			 0xa9dcf4a4, /// 0xac0
			 0xa47c792d, /// 0xac4
			 0x4956dca0, /// 0xac8
			 0x8691d2c7, /// 0xacc
			 0xd2314a3f, /// 0xad0
			 0x67f9e110, /// 0xad4
			 0x9589eee3, /// 0xad8
			 0x19b078a0, /// 0xadc
			 0x494362bd, /// 0xae0
			 0xdbd18800, /// 0xae4
			 0xf911674d, /// 0xae8
			 0x8209b2f3, /// 0xaec
			 0x82c96731, /// 0xaf0
			 0xbea755d8, /// 0xaf4
			 0x3be8a9d3, /// 0xaf8
			 0x739bf831, /// 0xafc
			 0x6ff946f3, /// 0xb00
			 0xbd6f1218, /// 0xb04
			 0xb8b19bb8, /// 0xb08
			 0x99dd823d, /// 0xb0c
			 0xe7e4cd93, /// 0xb10
			 0x26fbe028, /// 0xb14
			 0x1a12f1d3, /// 0xb18
			 0x6b7d4e01, /// 0xb1c
			 0xef565c24, /// 0xb20
			 0x19709971, /// 0xb24
			 0x894a51c4, /// 0xb28
			 0x3668c4c7, /// 0xb2c
			 0xda844954, /// 0xb30
			 0xe52fb4f5, /// 0xb34
			 0xe45f7ed6, /// 0xb38
			 0x25b9b6ea, /// 0xb3c
			 0x641837d7, /// 0xb40
			 0x24a74265, /// 0xb44
			 0x880bd7e9, /// 0xb48
			 0x5cda37cf, /// 0xb4c
			 0x83be7d09, /// 0xb50
			 0xf8669047, /// 0xb54
			 0xf4d57c95, /// 0xb58
			 0xc06ef679, /// 0xb5c
			 0x80f88b48, /// 0xb60
			 0x9010f364, /// 0xb64
			 0x91772748, /// 0xb68
			 0x89bf2ba4, /// 0xb6c
			 0x8700f4f3, /// 0xb70
			 0x48021972, /// 0xb74
			 0x27960a55, /// 0xb78
			 0x84c96fba, /// 0xb7c
			 0xd837bc67, /// 0xb80
			 0xb8cff264, /// 0xb84
			 0xac154b60, /// 0xb88
			 0x3f2a31f3, /// 0xb8c
			 0x8a5bbba6, /// 0xb90
			 0x5ca07eec, /// 0xb94
			 0x3e031f6d, /// 0xb98
			 0x7cf94059, /// 0xb9c
			 0xdba5eb5d, /// 0xba0
			 0x66b17817, /// 0xba4
			 0xefa9ab5c, /// 0xba8
			 0xad786df4, /// 0xbac
			 0x45a9b414, /// 0xbb0
			 0x5cdbe9b7, /// 0xbb4
			 0xc4543968, /// 0xbb8
			 0xba717b22, /// 0xbbc
			 0xa6d80823, /// 0xbc0
			 0xb10a649d, /// 0xbc4
			 0x92eb111d, /// 0xbc8
			 0x5b0dc08e, /// 0xbcc
			 0x67aa7e01, /// 0xbd0
			 0x3dc1e35f, /// 0xbd4
			 0x6b46e9c4, /// 0xbd8
			 0xd7086b27, /// 0xbdc
			 0x470a613b, /// 0xbe0
			 0xa90cb9aa, /// 0xbe4
			 0x20dcd661, /// 0xbe8
			 0x0abe9569, /// 0xbec
			 0x7c1166f7, /// 0xbf0
			 0xeb0858f4, /// 0xbf4
			 0x2fb0f2df, /// 0xbf8
			 0x8076ca77, /// 0xbfc
			 0xbdb0d9b9, /// 0xc00
			 0xd8b2d8ba, /// 0xc04
			 0x4720d1a9, /// 0xc08
			 0x6e049a73, /// 0xc0c
			 0x0ed836d5, /// 0xc10
			 0x6dccff01, /// 0xc14
			 0x895c045d, /// 0xc18
			 0xfdc7eabe, /// 0xc1c
			 0x56b8eaf1, /// 0xc20
			 0x7d3225a2, /// 0xc24
			 0x0f72eb5f, /// 0xc28
			 0xe9a5c569, /// 0xc2c
			 0xa6cf5a26, /// 0xc30
			 0x0d94845c, /// 0xc34
			 0xbbaad73c, /// 0xc38
			 0x31704ce2, /// 0xc3c
			 0x77935d4e, /// 0xc40
			 0x78611ee9, /// 0xc44
			 0x2ff8b2d1, /// 0xc48
			 0x025f07c3, /// 0xc4c
			 0x9fd84599, /// 0xc50
			 0xa9ec2282, /// 0xc54
			 0xdbf26a9a, /// 0xc58
			 0xfb24a372, /// 0xc5c
			 0xe1652417, /// 0xc60
			 0xa4eb3f61, /// 0xc64
			 0xc2365530, /// 0xc68
			 0x5282355a, /// 0xc6c
			 0xa3297e27, /// 0xc70
			 0x7fe467fa, /// 0xc74
			 0x69831032, /// 0xc78
			 0xc7fb361e, /// 0xc7c
			 0x4c67eeae, /// 0xc80
			 0xb4b4a5eb, /// 0xc84
			 0x511ec841, /// 0xc88
			 0xd3510499, /// 0xc8c
			 0xaaf25c9e, /// 0xc90
			 0x77e4e760, /// 0xc94
			 0xabce6241, /// 0xc98
			 0xac9485db, /// 0xc9c
			 0x9cd2eea8, /// 0xca0
			 0x85a5e212, /// 0xca4
			 0x45ba5b77, /// 0xca8
			 0xb1316889, /// 0xcac
			 0x984163ed, /// 0xcb0
			 0xddd1604c, /// 0xcb4
			 0xe8f50bf5, /// 0xcb8
			 0x57904139, /// 0xcbc
			 0x8b3eb449, /// 0xcc0
			 0x730a71d3, /// 0xcc4
			 0xdeb2b68c, /// 0xcc8
			 0xc594bec7, /// 0xccc
			 0x8ffa9eff, /// 0xcd0
			 0xd56b8637, /// 0xcd4
			 0x63e0dde8, /// 0xcd8
			 0x3ab595bf, /// 0xcdc
			 0x1ea53c27, /// 0xce0
			 0x2a8d6ab0, /// 0xce4
			 0xc9af7eab, /// 0xce8
			 0x06ccf707, /// 0xcec
			 0xe9f785c3, /// 0xcf0
			 0x14b827dd, /// 0xcf4
			 0x1195bf8e, /// 0xcf8
			 0xad5fe86e, /// 0xcfc
			 0xd9127d38, /// 0xd00
			 0x0b69d7ea, /// 0xd04
			 0xd218d4aa, /// 0xd08
			 0x54ff4883, /// 0xd0c
			 0x4367d82e, /// 0xd10
			 0xbffeb3c2, /// 0xd14
			 0xb7dec1c8, /// 0xd18
			 0x3f4f1788, /// 0xd1c
			 0xf76024a4, /// 0xd20
			 0xd548b47e, /// 0xd24
			 0x5d031d16, /// 0xd28
			 0x7cbbea51, /// 0xd2c
			 0x68fe2c81, /// 0xd30
			 0xa4f22647, /// 0xd34
			 0x0a4f0b0e, /// 0xd38
			 0x8d774502, /// 0xd3c
			 0xa4f6349d, /// 0xd40
			 0xab5d94c2, /// 0xd44
			 0x784ddfc0, /// 0xd48
			 0x7f0c45e5, /// 0xd4c
			 0x0df1f083, /// 0xd50
			 0x2ed11c9b, /// 0xd54
			 0xe6703b77, /// 0xd58
			 0xec9c5630, /// 0xd5c
			 0xbceb4522, /// 0xd60
			 0xe6ff6806, /// 0xd64
			 0x017a0c33, /// 0xd68
			 0x63799313, /// 0xd6c
			 0xa2cd9942, /// 0xd70
			 0x3755c940, /// 0xd74
			 0x9c641c4d, /// 0xd78
			 0xe4a7a2f3, /// 0xd7c
			 0xb285ab67, /// 0xd80
			 0x5ce2eeec, /// 0xd84
			 0x20f5f114, /// 0xd88
			 0x024bf8a7, /// 0xd8c
			 0xa285ef23, /// 0xd90
			 0xbe03c6f8, /// 0xd94
			 0x477ffeab, /// 0xd98
			 0x8aef67a5, /// 0xd9c
			 0x5ca0f824, /// 0xda0
			 0x50491a2b, /// 0xda4
			 0x1b302a59, /// 0xda8
			 0xc9a21c4c, /// 0xdac
			 0x66e5f75e, /// 0xdb0
			 0x5af1f61c, /// 0xdb4
			 0x04919612, /// 0xdb8
			 0xa1f48a6e, /// 0xdbc
			 0xc3f2138b, /// 0xdc0
			 0xe0ed7c30, /// 0xdc4
			 0x5a25d7d0, /// 0xdc8
			 0x859f1f18, /// 0xdcc
			 0x51bec8b3, /// 0xdd0
			 0x7dd130fa, /// 0xdd4
			 0x6519a2a4, /// 0xdd8
			 0x31e4d6e5, /// 0xddc
			 0x75443adf, /// 0xde0
			 0xe833f8e3, /// 0xde4
			 0x64985496, /// 0xde8
			 0x6113f73b, /// 0xdec
			 0xe2829a27, /// 0xdf0
			 0x814e42d6, /// 0xdf4
			 0x4eee6ae1, /// 0xdf8
			 0xc8085c4a, /// 0xdfc
			 0x31637c7c, /// 0xe00
			 0xba1d38fb, /// 0xe04
			 0xc4895ce9, /// 0xe08
			 0xee5ec9a3, /// 0xe0c
			 0xbd3382ab, /// 0xe10
			 0xae350491, /// 0xe14
			 0xd84c49b2, /// 0xe18
			 0xb86605ac, /// 0xe1c
			 0x3a920e24, /// 0xe20
			 0x5fd5e862, /// 0xe24
			 0x2b73aac1, /// 0xe28
			 0xc087281f, /// 0xe2c
			 0x986a7124, /// 0xe30
			 0x41d7b9f2, /// 0xe34
			 0x1fbcbcbd, /// 0xe38
			 0xa6f31d2d, /// 0xe3c
			 0x93c6c615, /// 0xe40
			 0xa4c05235, /// 0xe44
			 0xefba8ee0, /// 0xe48
			 0xeb034ea2, /// 0xe4c
			 0x3ec16dcb, /// 0xe50
			 0x677ff370, /// 0xe54
			 0xfcc4ea1d, /// 0xe58
			 0x2aabd1a7, /// 0xe5c
			 0x502931df, /// 0xe60
			 0x6168537d, /// 0xe64
			 0x813a732d, /// 0xe68
			 0x961e8c7e, /// 0xe6c
			 0x7a27e015, /// 0xe70
			 0x42e4c5fc, /// 0xe74
			 0xf866f5bb, /// 0xe78
			 0x72a3e448, /// 0xe7c
			 0x10ca110a, /// 0xe80
			 0xcf241dd6, /// 0xe84
			 0x46037afc, /// 0xe88
			 0xe343395f, /// 0xe8c
			 0x26488356, /// 0xe90
			 0x0d46903f, /// 0xe94
			 0xb6c90bc5, /// 0xe98
			 0x6972467a, /// 0xe9c
			 0x9c4e25f4, /// 0xea0
			 0x297010dd, /// 0xea4
			 0x731e49cf, /// 0xea8
			 0xe8762b67, /// 0xeac
			 0xc6e36bc1, /// 0xeb0
			 0x8383f3cd, /// 0xeb4
			 0x0aa8c157, /// 0xeb8
			 0x11066305, /// 0xebc
			 0x48f9ad4b, /// 0xec0
			 0xcbde62af, /// 0xec4
			 0xe23d73eb, /// 0xec8
			 0xde39a781, /// 0xecc
			 0xc87666f1, /// 0xed0
			 0x5577ec32, /// 0xed4
			 0xe79bf6e5, /// 0xed8
			 0x73e79d5a, /// 0xedc
			 0xa6669577, /// 0xee0
			 0xba14c0a1, /// 0xee4
			 0x5cd0fb3b, /// 0xee8
			 0xd1309706, /// 0xeec
			 0x4bdf0430, /// 0xef0
			 0x2f7f1f6e, /// 0xef4
			 0x5a6e31de, /// 0xef8
			 0x59463b7e, /// 0xefc
			 0x4c54d960, /// 0xf00
			 0x6115d33c, /// 0xf04
			 0xd5ba7d2a, /// 0xf08
			 0x81a1a86d, /// 0xf0c
			 0xb2ef3e66, /// 0xf10
			 0x8124b7be, /// 0xf14
			 0x13c672d9, /// 0xf18
			 0x7c806270, /// 0xf1c
			 0x12dbfc9b, /// 0xf20
			 0x25ccf2b9, /// 0xf24
			 0xbd39af3a, /// 0xf28
			 0x801c1802, /// 0xf2c
			 0x13a2de29, /// 0xf30
			 0x535407d2, /// 0xf34
			 0xacd8c236, /// 0xf38
			 0xcae1c66d, /// 0xf3c
			 0xe2d8f9fb, /// 0xf40
			 0xca6f24d9, /// 0xf44
			 0xdcc2b853, /// 0xf48
			 0x8e3b3aa2, /// 0xf4c
			 0x47b709e3, /// 0xf50
			 0xc2ce41ca, /// 0xf54
			 0xa41ba085, /// 0xf58
			 0x39962fb0, /// 0xf5c
			 0xf4dc101f, /// 0xf60
			 0xcebc9301, /// 0xf64
			 0x3e6edfed, /// 0xf68
			 0x1e68ced9, /// 0xf6c
			 0xa83f1307, /// 0xf70
			 0x4138dc89, /// 0xf74
			 0x160de3ef, /// 0xf78
			 0xb465af17, /// 0xf7c
			 0x367dbf92, /// 0xf80
			 0x07a077ff, /// 0xf84
			 0x95ff8985, /// 0xf88
			 0xe2a148b9, /// 0xf8c
			 0x5ab6469e, /// 0xf90
			 0x5c4cb97e, /// 0xf94
			 0x402bcf05, /// 0xf98
			 0x508fdd51, /// 0xf9c
			 0x3f4657d4, /// 0xfa0
			 0x9db7cd92, /// 0xfa4
			 0x056ddb9d, /// 0xfa8
			 0x9bc235db, /// 0xfac
			 0xf4ca8b35, /// 0xfb0
			 0x3da1026c, /// 0xfb4
			 0xbfdaa9ac, /// 0xfb8
			 0x31f5dbbc, /// 0xfbc
			 0xa055e587, /// 0xfc0
			 0xe1a30d58, /// 0xfc4
			 0xdd7ecd95, /// 0xfc8
			 0x1de5d7d4, /// 0xfcc
			 0x3b38c322, /// 0xfd0
			 0x39841981, /// 0xfd4
			 0x300d2698, /// 0xfd8
			 0xba6e62cc, /// 0xfdc
			 0xe901c2c6, /// 0xfe0
			 0x853bae13, /// 0xfe4
			 0xa920235a, /// 0xfe8
			 0xe7c15294, /// 0xfec
			 0x393984cc, /// 0xff0
			 0xb41a26d7, /// 0xff4
			 0xee0aa35e, /// 0xff8
			 0x05c0cef2 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00004
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00010
			 0x0e000007,                                                  // Trailing 1s:                                /// 00014
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00018
			 0x3f028f5c,                                                  // 0.51                                        /// 0001c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00020
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00024
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00028
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00030
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00044
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00048
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0004c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00050
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0005c
			 0x4b000000,                                                  // 8388608.0                                   /// 00060
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00068
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00070
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x80000000,                                                  // -zero                                       /// 00080
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00088
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0008c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00090
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00094
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00098
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0009c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00000000,                                                  // zero                                        /// 000a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00108
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00110
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00118
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00120
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0012c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00134
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00138
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xff800000,                                                  // -inf                                        /// 00144
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00148
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0014c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00150
			 0x0e000007,                                                  // Trailing 1s:                                /// 00154
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0015c
			 0x55555555,                                                  // 4 random values                             /// 00160
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00164
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00168
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00178
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00184
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0018c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00190
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00194
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00198
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0019c
			 0x4b000000,                                                  // 8388608.0                                   /// 001a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001b0
			 0x80000000,                                                  // -zero                                       /// 001b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x3f028f5c,                                                  // 0.51                                        /// 001c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x55555555,                                                  // 4 random values                             /// 001d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001d8
			 0xbf028f5c,                                                  // -0.51                                       /// 001dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00208
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0020c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00214
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0021c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00224
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x7fc00001,                                                  // signaling NaN                               /// 00234
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00238
			 0x0c780000,                                                  // Leading 1s:                                 /// 0023c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00240
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00244
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00248
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0024c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00250
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00258
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0025c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00268
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00274
			 0x3f028f5c,                                                  // 0.51                                        /// 00278
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00280
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00288
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0028c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00290
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00294
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00298
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00300
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00304
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00308
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0030c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00310
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00314
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x33333333,                                                  // 4 random values                             /// 0031c
			 0x7fc00001,                                                  // signaling NaN                               /// 00320
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00324
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00328
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0032c
			 0xcb000000,                                                  // -8388608.0                                  /// 00330
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x4b000000,                                                  // 8388608.0                                   /// 0033c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0034c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00350
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00354
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0035c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00360
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00364
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00374
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00384
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00390
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00394
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x7f800000,                                                  // inf                                         /// 003d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00400
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00404
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0040c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00410
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00414
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0e000007,                                                  // Trailing 1s:                                /// 0041c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00420
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00424
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00428
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0042c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00430
			 0x0c780000,                                                  // Leading 1s:                                 /// 00434
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00438
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0043c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00440
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00444
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0044c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00450
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00454
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00458
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00460
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00464
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00468
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0046c
			 0x33333333,                                                  // 4 random values                             /// 00470
			 0x3f028f5c,                                                  // 0.51                                        /// 00474
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00478
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0047c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00484
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00488
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80000000,                                                  // -zero                                       /// 00490
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00494
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00498
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00500
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00504
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00508
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00510
			 0x80000000,                                                  // -zero                                       /// 00514
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00518
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0051c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00528
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00530
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00538
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0053c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00540
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00544
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00548
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00550
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0055c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00560
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0c600000,                                                  // Leading 1s:                                 /// 00568
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0056c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00588
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0058c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00590
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00594
			 0xcb000000,                                                  // -8388608.0                                  /// 00598
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0059c
			 0xcb000000,                                                  // -8388608.0                                  /// 005a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005ac
			 0x7f800000,                                                  // inf                                         /// 005b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005e8
			 0x4b000000,                                                  // 8388608.0                                   /// 005ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00600
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00608
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0060c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00610
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00614
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0061c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00624
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00628
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00630
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00638
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0063c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00640
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00644
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0064c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00654
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0065c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00664
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0066c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00670
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00674
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00678
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0067c
			 0x7f800000,                                                  // inf                                         /// 00680
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00688
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00698
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0069c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00704
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00708
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00710
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00714
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00718
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00728
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0072c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00730
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00734
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00738
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00744
			 0xff800000,                                                  // -inf                                        /// 00748
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0074c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00750
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00754
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00758
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00764
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00768
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00774
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00778
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00780
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00784
			 0x3f028f5c,                                                  // 0.51                                        /// 00788
			 0x00011111,                                                  // 9.7958E-41                                  /// 0078c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00790
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00794
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00798
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0079c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007a4
			 0x3f028f5c,                                                  // 0.51                                        /// 007a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b0
			 0x7f800000,                                                  // inf                                         /// 007b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x7fc00001,                                                  // signaling NaN                               /// 007d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007e8
			 0xff800000,                                                  // -inf                                        /// 007ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00800
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00818
			 0x0c780000,                                                  // Leading 1s:                                 /// 0081c
			 0xcb000000,                                                  // -8388608.0                                  /// 00820
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00824
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00828
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00838
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00844
			 0x80011111,                                                  // -9.7958E-41                                 /// 00848
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00854
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00860
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0086c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00870
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00874
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00878
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00880
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00888
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0088c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00894
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00898
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008d0
			 0x55555555,                                                  // 4 random values                             /// 008d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ec
			 0x80000000,                                                  // -zero                                       /// 008f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00900
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00908
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0090c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00910
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0xbf028f5c,                                                  // -0.51                                       /// 00920
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00924
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0093c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00940
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00950
			 0x0c600000,                                                  // Leading 1s:                                 /// 00954
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00958
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0095c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00964
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00968
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00970
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00974
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0c600000,                                                  // Leading 1s:                                 /// 0097c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00990
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0099c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0xff800000,                                                  // -inf                                        /// 009a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f4
			 0x55555555,                                                  // 4 random values                             /// 009f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0xcb000000,                                                  // -8388608.0                                  /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x33333333,                                                  // 4 random values                             /// 00a38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a70
			 0x4b000000,                                                  // 8388608.0                                   /// 00a74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00abc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00acc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ad0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ad8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00adc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00afc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x55555555,                                                  // 4 random values                             /// 00b40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xcb000000,                                                  // -8388608.0                                  /// 00b88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b98
			 0xcb000000,                                                  // -8388608.0                                  /// 00b9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc4
			 0x33333333,                                                  // 4 random values                             /// 00bc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bf0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c14
			 0x80000000,                                                  // -zero                                       /// 00c18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00000000,                                                  // zero                                        /// 00c40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c58
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c9c
			 0x55555555,                                                  // 4 random values                             /// 00ca0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ccc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cd0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ce0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cf0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d44
			 0x55555555,                                                  // 4 random values                             /// 00d48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00da4
			 0xbf028f5c,                                                  // -0.51                                       /// 00da8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00db0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00db8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dbc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ddc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e3c
			 0xff800000,                                                  // -inf                                        /// 00e40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e7c
			 0x33333333,                                                  // 4 random values                             /// 00e80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eac
			 0x00000000,                                                  // zero                                        /// 00eb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ecc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ee0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00000000,                                                  // zero                                        /// 00f98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe4
			 0x55555555,                                                  // 4 random values                             /// 00fe8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ff8
			 0xAAAAAAAA                                                  // 4 random values                             /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00000
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00008
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0000c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00010
			 0xcb000000,                                                  // -8388608.0                                  /// 00014
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00018
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00020
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0002c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00030
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00034
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00038
			 0x0e000003,                                                  // Trailing 1s:                                /// 0003c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00040
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00050
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00054
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00058
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0005c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00064
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00068
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00074
			 0x0e000001,                                                  // Trailing 1s:                                /// 00078
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0007c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00084
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00088
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00090
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00094
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00098
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0009c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000a8
			 0x33333333,                                                  // 4 random values                             /// 000ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c0
			 0xcb000000,                                                  // -8388608.0                                  /// 000c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0010c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00118
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00120
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00124
			 0x55555555,                                                  // 4 random values                             /// 00128
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00130
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0013c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00140
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00148
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0014c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00154
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00164
			 0x4b000000,                                                  // 8388608.0                                   /// 00168
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0016c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00174
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00178
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0017c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00180
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00188
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00190
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0019c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0xbf028f5c,                                                  // -0.51                                       /// 001b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x4b000000,                                                  // 8388608.0                                   /// 001d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e8
			 0x3f028f5c,                                                  // 0.51                                        /// 001ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00200
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0xffc00001,                                                  // -signaling NaN                              /// 00218
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0021c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x7fc00001,                                                  // signaling NaN                               /// 00228
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00234
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00238
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0023c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00240
			 0x0e000001,                                                  // Trailing 1s:                                /// 00244
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0024c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00258
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0025c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00268
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00270
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00274
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0027c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0028c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00294
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00298
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0029c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7fc00001,                                                  // signaling NaN                               /// 002a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xff800000,                                                  // -inf                                        /// 002b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80011111,                                                  // -9.7958E-41                                 /// 00304
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00308
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0030c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00310
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00314
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00320
			 0x4b000000,                                                  // 8388608.0                                   /// 00324
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00328
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00330
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00334
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00338
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0033c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00340
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00348
			 0xbf028f5c,                                                  // -0.51                                       /// 0034c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00354
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0035c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00368
			 0x3f028f5c,                                                  // 0.51                                        /// 0036c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00370
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00378
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0037c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00380
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00384
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00390
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00394
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00398
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0039c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x33333333,                                                  // 4 random values                             /// 003a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00400
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00404
			 0x55555555,                                                  // 4 random values                             /// 00408
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00410
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00414
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00418
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0041c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00424
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00428
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00430
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00434
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0043c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00440
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00444
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00448
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00450
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00454
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0045c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00460
			 0x0c600000,                                                  // Leading 1s:                                 /// 00464
			 0x3f028f5c,                                                  // 0.51                                        /// 00468
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00474
			 0x4b000000,                                                  // 8388608.0                                   /// 00478
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0047c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00480
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00488
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00498
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0049c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00500
			 0x0e000007,                                                  // Trailing 1s:                                /// 00504
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0050c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00510
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00518
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00520
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00524
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00000000,                                                  // zero                                        /// 0052c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00530
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e000001,                                                  // Trailing 1s:                                /// 00544
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00548
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00000000,                                                  // zero                                        /// 00550
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00558
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0055c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00560
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00564
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00568
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0056c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00580
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00584
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00588
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00590
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00598
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005f0
			 0x4b000000,                                                  // 8388608.0                                   /// 005f4
			 0x55555555,                                                  // 4 random values                             /// 005f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00600
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00604
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00610
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00618
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0061c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00620
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00624
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00628
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00634
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00000000,                                                  // zero                                        /// 0063c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00640
			 0x0e000001,                                                  // Trailing 1s:                                /// 00644
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00648
			 0x7fc00001,                                                  // signaling NaN                               /// 0064c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00650
			 0x0c600000,                                                  // Leading 1s:                                 /// 00654
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00658
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0065c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00660
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00678
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0067c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00680
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00684
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00688
			 0xbf028f5c,                                                  // -0.51                                       /// 0068c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00690
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00694
			 0x00011111,                                                  // 9.7958E-41                                  /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006ec
			 0x00000000,                                                  // zero                                        /// 006f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00704
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00708
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0070c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00714
			 0x55555555,                                                  // 4 random values                             /// 00718
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00720
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00724
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00728
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0072c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00730
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00734
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00738
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0073c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00740
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00744
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00748
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0074c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00750
			 0x4b000000,                                                  // 8388608.0                                   /// 00754
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00758
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0075c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00764
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00768
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00770
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0077c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00780
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x7f800000,                                                  // inf                                         /// 0078c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00794
			 0x33333333,                                                  // 4 random values                             /// 00798
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x33333333,                                                  // 4 random values                             /// 007a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x33333333,                                                  // 4 random values                             /// 007b4
			 0x80000000,                                                  // -zero                                       /// 007b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00800
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00808
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0080c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0xcb000000,                                                  // -8388608.0                                  /// 00824
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00830
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0083c
			 0xbf028f5c,                                                  // -0.51                                       /// 00840
			 0xff800000,                                                  // -inf                                        /// 00844
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00848
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0084c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00850
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00854
			 0x0c700000,                                                  // Leading 1s:                                 /// 00858
			 0x80011111,                                                  // -9.7958E-41                                 /// 0085c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00864
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0086c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00870
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00878
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00894
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00898
			 0x0c780000,                                                  // Leading 1s:                                 /// 0089c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d4
			 0x7f800000,                                                  // inf                                         /// 008d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00904
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x55555555,                                                  // 4 random values                             /// 0090c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00914
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00920
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00924
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00928
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0093c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00940
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x7fc00001,                                                  // signaling NaN                               /// 00948
			 0xbf028f5c,                                                  // -0.51                                       /// 0094c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00950
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00954
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00964
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00968
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0096c
			 0xffc00001,                                                  // -signaling NaN                              /// 00970
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00974
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00978
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0097c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00980
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00984
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0e000007,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00994
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 009d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d4
			 0x7f800000,                                                  // inf                                         /// 009d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009ec
			 0x33333333,                                                  // 4 random values                             /// 009f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a2c
			 0x80000000,                                                  // -zero                                       /// 00a30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ab8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00abc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ac4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00acc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00adc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00af4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00afc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bcc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c58
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xbf028f5c,                                                  // -0.51                                       /// 00c8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ccc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000000,                                                  // zero                                        /// 00cd8
			 0x55555555,                                                  // 4 random values                             /// 00cdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cf0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x3f028f5c,                                                  // 0.51                                        /// 00d34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dc0
			 0x7f800000,                                                  // inf                                         /// 00dc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dd4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ddc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00de0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00df8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00000000,                                                  // zero                                        /// 00e50
			 0x7fc00001,                                                  // signaling NaN                               /// 00e54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7f800000,                                                  // inf                                         /// 00e5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e80
			 0xbf028f5c,                                                  // -0.51                                       /// 00e84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e88
			 0xff800000,                                                  // -inf                                        /// 00e8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ea0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ecc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ed0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f800000,                                                  // inf                                         /// 00ee4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ee8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x3f028f5c,                                                  // 0.51                                        /// 00efc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x4b000000,                                                  // 8388608.0                                   /// 00f08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f28
			 0x4b000000,                                                  // 8388608.0                                   /// 00f2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0x00000000,                                                  // zero                                        /// 00fb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fdc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fe8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0f7fffff                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x7747ee6a, /// 0x0
			 0x2fe34be2, /// 0x4
			 0x2dbb16f5, /// 0x8
			 0xd7190fa0, /// 0xc
			 0x653d61ef, /// 0x10
			 0x7259c525, /// 0x14
			 0xea398c11, /// 0x18
			 0xf4e98886, /// 0x1c
			 0x9acb6a0c, /// 0x20
			 0xc801c970, /// 0x24
			 0x5c1af074, /// 0x28
			 0x6da8f1db, /// 0x2c
			 0xa2dd71f0, /// 0x30
			 0x04fdf0f9, /// 0x34
			 0xc6afe620, /// 0x38
			 0xc9b088b9, /// 0x3c
			 0x6513ff04, /// 0x40
			 0xaae69975, /// 0x44
			 0xe8fc15ad, /// 0x48
			 0xb24d9c0a, /// 0x4c
			 0xc47ecebd, /// 0x50
			 0xef1056ea, /// 0x54
			 0x49f43d4a, /// 0x58
			 0xedcde5f5, /// 0x5c
			 0xed54ac2c, /// 0x60
			 0x59937a38, /// 0x64
			 0x7042b5af, /// 0x68
			 0xd2012d4f, /// 0x6c
			 0xb86e8f6b, /// 0x70
			 0xbb216d78, /// 0x74
			 0x8e535738, /// 0x78
			 0xea68b7de, /// 0x7c
			 0x9f44905a, /// 0x80
			 0x7dedc801, /// 0x84
			 0xf6e75f75, /// 0x88
			 0x5ce18915, /// 0x8c
			 0xa1a99c34, /// 0x90
			 0x20db9916, /// 0x94
			 0x73fa1d4d, /// 0x98
			 0xf20830fc, /// 0x9c
			 0x7690c26e, /// 0xa0
			 0x079882fc, /// 0xa4
			 0x966d5785, /// 0xa8
			 0xedc0c140, /// 0xac
			 0xa116d9a2, /// 0xb0
			 0xbc8e4c01, /// 0xb4
			 0xe9d53ead, /// 0xb8
			 0xae32ec35, /// 0xbc
			 0x0b75abb9, /// 0xc0
			 0xc9fa0cbb, /// 0xc4
			 0x0c73c084, /// 0xc8
			 0x7424798a, /// 0xcc
			 0x08ea4538, /// 0xd0
			 0x87f644f4, /// 0xd4
			 0x60b2b4ee, /// 0xd8
			 0x61f3de5e, /// 0xdc
			 0x4dcecaa5, /// 0xe0
			 0x71bdba9b, /// 0xe4
			 0x9ae6e023, /// 0xe8
			 0xc668ff05, /// 0xec
			 0x9e75a7ad, /// 0xf0
			 0x426b9837, /// 0xf4
			 0xfc1d9523, /// 0xf8
			 0x1cc8ddfd, /// 0xfc
			 0x6bb1be12, /// 0x100
			 0x94221fa7, /// 0x104
			 0xc974d0ea, /// 0x108
			 0xe9395e31, /// 0x10c
			 0xde381009, /// 0x110
			 0x84d239db, /// 0x114
			 0x0394f140, /// 0x118
			 0x77724c7e, /// 0x11c
			 0xa85b144a, /// 0x120
			 0xc74612be, /// 0x124
			 0x62067102, /// 0x128
			 0x2da36dd0, /// 0x12c
			 0x8d9b9e5a, /// 0x130
			 0x2eb5ecea, /// 0x134
			 0x5f7e5eda, /// 0x138
			 0x60542dd2, /// 0x13c
			 0x5ac0bd18, /// 0x140
			 0xfa22dbf3, /// 0x144
			 0xc19f4a26, /// 0x148
			 0x5293cd86, /// 0x14c
			 0xec144fa8, /// 0x150
			 0xf179690e, /// 0x154
			 0x046ed56b, /// 0x158
			 0xa4960949, /// 0x15c
			 0x1e1af34f, /// 0x160
			 0x0a4264eb, /// 0x164
			 0x23782f3c, /// 0x168
			 0x23ccee43, /// 0x16c
			 0x9210d427, /// 0x170
			 0xf6a58d1f, /// 0x174
			 0x08c5e299, /// 0x178
			 0xd578d4dc, /// 0x17c
			 0x5e63789a, /// 0x180
			 0x65e7fd66, /// 0x184
			 0x83a91671, /// 0x188
			 0x03f85c74, /// 0x18c
			 0x0693f4b5, /// 0x190
			 0x53aaad3f, /// 0x194
			 0x4f5641ec, /// 0x198
			 0x4775b92c, /// 0x19c
			 0xe22fc106, /// 0x1a0
			 0x97f60e06, /// 0x1a4
			 0xb5aeac55, /// 0x1a8
			 0xfe9d335a, /// 0x1ac
			 0x8f7dd984, /// 0x1b0
			 0xf6669e81, /// 0x1b4
			 0xf9b38385, /// 0x1b8
			 0xd88eaa06, /// 0x1bc
			 0xdc374106, /// 0x1c0
			 0xb5368b76, /// 0x1c4
			 0x1e826a86, /// 0x1c8
			 0x65e76f0e, /// 0x1cc
			 0x0fea5c8d, /// 0x1d0
			 0xd84d8394, /// 0x1d4
			 0xd6262caa, /// 0x1d8
			 0x3d7764c7, /// 0x1dc
			 0xdae032ee, /// 0x1e0
			 0x225e4494, /// 0x1e4
			 0x5db7a027, /// 0x1e8
			 0xbd5dc655, /// 0x1ec
			 0xc0c1e1c4, /// 0x1f0
			 0xf51f2362, /// 0x1f4
			 0x676c65bd, /// 0x1f8
			 0x3936cdd5, /// 0x1fc
			 0xdbdb3a96, /// 0x200
			 0x29fa0562, /// 0x204
			 0x989d5873, /// 0x208
			 0xb2cc29f0, /// 0x20c
			 0x96db97cb, /// 0x210
			 0xebf90b95, /// 0x214
			 0x29a2f29c, /// 0x218
			 0x6615a25c, /// 0x21c
			 0xab4144f9, /// 0x220
			 0x7b525c64, /// 0x224
			 0x6c203799, /// 0x228
			 0xbf62ceda, /// 0x22c
			 0x660f626b, /// 0x230
			 0x29ba48e0, /// 0x234
			 0x48265116, /// 0x238
			 0xad9c3a21, /// 0x23c
			 0x5ca2cc07, /// 0x240
			 0x3103bcd5, /// 0x244
			 0x39b3d575, /// 0x248
			 0x75fb9dec, /// 0x24c
			 0x08738e4b, /// 0x250
			 0xf6e8bccb, /// 0x254
			 0x94280745, /// 0x258
			 0x5509fe63, /// 0x25c
			 0x7db98af7, /// 0x260
			 0x4c3b4581, /// 0x264
			 0xf6766a0a, /// 0x268
			 0x51cb4f86, /// 0x26c
			 0x727fa19a, /// 0x270
			 0xda90ba3a, /// 0x274
			 0x8b92a5f5, /// 0x278
			 0xaa399261, /// 0x27c
			 0xf5580795, /// 0x280
			 0xdb0235bd, /// 0x284
			 0x0250e145, /// 0x288
			 0x3701a625, /// 0x28c
			 0x2010feef, /// 0x290
			 0xf1a58bb2, /// 0x294
			 0x9774826a, /// 0x298
			 0x4632d367, /// 0x29c
			 0x394e39b1, /// 0x2a0
			 0x4056492c, /// 0x2a4
			 0x56db874e, /// 0x2a8
			 0x265419a3, /// 0x2ac
			 0x0d0677af, /// 0x2b0
			 0x357b70d9, /// 0x2b4
			 0x0fbe1b23, /// 0x2b8
			 0xd3172998, /// 0x2bc
			 0x682fa613, /// 0x2c0
			 0xf983bf44, /// 0x2c4
			 0xafe30f84, /// 0x2c8
			 0xf4a37450, /// 0x2cc
			 0x899f15ca, /// 0x2d0
			 0xe18476a9, /// 0x2d4
			 0xe552f5af, /// 0x2d8
			 0xba6a53f0, /// 0x2dc
			 0xccdd8fe8, /// 0x2e0
			 0x7acad179, /// 0x2e4
			 0xcf13b322, /// 0x2e8
			 0xbf9f77c8, /// 0x2ec
			 0x6a186a67, /// 0x2f0
			 0x67e313fb, /// 0x2f4
			 0x2eca121d, /// 0x2f8
			 0xffd5a023, /// 0x2fc
			 0x682e7e5e, /// 0x300
			 0x748ed607, /// 0x304
			 0x86519eae, /// 0x308
			 0x9b3101dd, /// 0x30c
			 0x02ceb3a6, /// 0x310
			 0x54fd3d90, /// 0x314
			 0xad799df9, /// 0x318
			 0x18bf1de3, /// 0x31c
			 0x77ec70fe, /// 0x320
			 0x4b67334b, /// 0x324
			 0xf7648304, /// 0x328
			 0xcbdc9f30, /// 0x32c
			 0xfa7db50a, /// 0x330
			 0x57bf8742, /// 0x334
			 0xa0e4077d, /// 0x338
			 0x9f04ff04, /// 0x33c
			 0xc94b79a9, /// 0x340
			 0x728b9935, /// 0x344
			 0x65efab09, /// 0x348
			 0x34f78c04, /// 0x34c
			 0x00083c96, /// 0x350
			 0x3f6d3ea3, /// 0x354
			 0x719a8c28, /// 0x358
			 0x74047a96, /// 0x35c
			 0x38f661ab, /// 0x360
			 0xbbd522a1, /// 0x364
			 0x628984e6, /// 0x368
			 0x3f3bad7b, /// 0x36c
			 0x30d52f8b, /// 0x370
			 0x7cbfc0c5, /// 0x374
			 0x796a063a, /// 0x378
			 0xc61ddf89, /// 0x37c
			 0x2803163c, /// 0x380
			 0x33816fe0, /// 0x384
			 0x215f6371, /// 0x388
			 0xd7346520, /// 0x38c
			 0x9d1e57e5, /// 0x390
			 0x8e004ca2, /// 0x394
			 0x65012607, /// 0x398
			 0x11f02c9f, /// 0x39c
			 0x00415d1c, /// 0x3a0
			 0xca220440, /// 0x3a4
			 0x4a36718e, /// 0x3a8
			 0xfe259940, /// 0x3ac
			 0xe66f90f0, /// 0x3b0
			 0xaec91882, /// 0x3b4
			 0x1bc38956, /// 0x3b8
			 0x86856bd2, /// 0x3bc
			 0x5e8e0f47, /// 0x3c0
			 0x0c3d3c4f, /// 0x3c4
			 0xd5cb8b13, /// 0x3c8
			 0xec6c6ab5, /// 0x3cc
			 0xddb64332, /// 0x3d0
			 0x9011fc71, /// 0x3d4
			 0x244d8f85, /// 0x3d8
			 0x6e213856, /// 0x3dc
			 0x5e46e4f2, /// 0x3e0
			 0xaeab1f5a, /// 0x3e4
			 0xb3453060, /// 0x3e8
			 0xd5b5b36f, /// 0x3ec
			 0xc4f5756d, /// 0x3f0
			 0x7520f96f, /// 0x3f4
			 0x83a9046a, /// 0x3f8
			 0xaf17bb7f, /// 0x3fc
			 0xa41f1d25, /// 0x400
			 0xd93de70b, /// 0x404
			 0x69b565a5, /// 0x408
			 0xb0311933, /// 0x40c
			 0xafe61dcb, /// 0x410
			 0xec17790a, /// 0x414
			 0x750a493d, /// 0x418
			 0x095c0788, /// 0x41c
			 0x187973e8, /// 0x420
			 0x623371d7, /// 0x424
			 0x50d12a97, /// 0x428
			 0x2c07b6dc, /// 0x42c
			 0x31b86ce1, /// 0x430
			 0x493853f6, /// 0x434
			 0x6d2cceb8, /// 0x438
			 0x88506160, /// 0x43c
			 0x6a4ab8f8, /// 0x440
			 0xee8fdf9e, /// 0x444
			 0x21ecad12, /// 0x448
			 0x00fc5b57, /// 0x44c
			 0x5f28777d, /// 0x450
			 0x44963ebd, /// 0x454
			 0xb61a3300, /// 0x458
			 0x2263339c, /// 0x45c
			 0x08631d1b, /// 0x460
			 0x2793dc01, /// 0x464
			 0xbe4bca18, /// 0x468
			 0x92ee554f, /// 0x46c
			 0x7165df4a, /// 0x470
			 0xe4d29b6f, /// 0x474
			 0x5981d632, /// 0x478
			 0xe901894a, /// 0x47c
			 0x0c689608, /// 0x480
			 0x2b3a6e2c, /// 0x484
			 0xb231de66, /// 0x488
			 0xb77ec1b0, /// 0x48c
			 0x2d7b7742, /// 0x490
			 0xc6fe85e1, /// 0x494
			 0x54a4dab1, /// 0x498
			 0x070c40b6, /// 0x49c
			 0x1f872bac, /// 0x4a0
			 0x7334f21a, /// 0x4a4
			 0xaf7d22f1, /// 0x4a8
			 0xc8c7e36c, /// 0x4ac
			 0x4e79c9b8, /// 0x4b0
			 0xe11871f0, /// 0x4b4
			 0xc92fad37, /// 0x4b8
			 0x609ae9d1, /// 0x4bc
			 0x35c64467, /// 0x4c0
			 0x56556ed9, /// 0x4c4
			 0xd314daad, /// 0x4c8
			 0x4f6784da, /// 0x4cc
			 0x3e412269, /// 0x4d0
			 0x7606edfa, /// 0x4d4
			 0x79ffc00b, /// 0x4d8
			 0x84b66b5c, /// 0x4dc
			 0xa3a2b929, /// 0x4e0
			 0x8ad8bd64, /// 0x4e4
			 0x9b210815, /// 0x4e8
			 0x926605c5, /// 0x4ec
			 0x9801d674, /// 0x4f0
			 0x35dc6a01, /// 0x4f4
			 0x8f03f9cc, /// 0x4f8
			 0xb1c80a9f, /// 0x4fc
			 0x095b0a72, /// 0x500
			 0xa37dd1e0, /// 0x504
			 0x216fbe24, /// 0x508
			 0xb50b987d, /// 0x50c
			 0xa59234d0, /// 0x510
			 0x044c87d8, /// 0x514
			 0x79b1b8b4, /// 0x518
			 0x20db7b25, /// 0x51c
			 0x56d2cfb1, /// 0x520
			 0xac434ea9, /// 0x524
			 0xc03b3c9b, /// 0x528
			 0x460b0fc2, /// 0x52c
			 0xd0ce3714, /// 0x530
			 0x8f70c74d, /// 0x534
			 0x687d7487, /// 0x538
			 0xbf6fe5af, /// 0x53c
			 0x3b26cd75, /// 0x540
			 0xf3561f21, /// 0x544
			 0xea4d5282, /// 0x548
			 0xe7ef5e4d, /// 0x54c
			 0xdcf5c27c, /// 0x550
			 0xc4ead5eb, /// 0x554
			 0xa6c354f0, /// 0x558
			 0x7a862c44, /// 0x55c
			 0x3be2f7e8, /// 0x560
			 0xfae6621d, /// 0x564
			 0x07339eb1, /// 0x568
			 0x871f57ee, /// 0x56c
			 0x17fc751c, /// 0x570
			 0x804300e5, /// 0x574
			 0x571052b6, /// 0x578
			 0x1e116062, /// 0x57c
			 0xbafcead2, /// 0x580
			 0xeb615946, /// 0x584
			 0xb26870bd, /// 0x588
			 0xdb62267e, /// 0x58c
			 0xd02a4db9, /// 0x590
			 0x441970dd, /// 0x594
			 0x3a8ed93d, /// 0x598
			 0xc60a15b5, /// 0x59c
			 0xdefdfe39, /// 0x5a0
			 0x62d7c769, /// 0x5a4
			 0x009263fa, /// 0x5a8
			 0xf00f904a, /// 0x5ac
			 0x41c75f5b, /// 0x5b0
			 0x773c1523, /// 0x5b4
			 0xab64e337, /// 0x5b8
			 0x39e1f651, /// 0x5bc
			 0x03bb1262, /// 0x5c0
			 0x86b3a8d4, /// 0x5c4
			 0x1eede015, /// 0x5c8
			 0x0f8ab1d8, /// 0x5cc
			 0x5a133157, /// 0x5d0
			 0x841fedf5, /// 0x5d4
			 0xdb7a3de0, /// 0x5d8
			 0x8b9e4ebe, /// 0x5dc
			 0x8ef78de4, /// 0x5e0
			 0xb264d28b, /// 0x5e4
			 0xe4060ff9, /// 0x5e8
			 0x4ba7691f, /// 0x5ec
			 0xa5ece830, /// 0x5f0
			 0x1570b70e, /// 0x5f4
			 0x04b0f604, /// 0x5f8
			 0x9f0a85f5, /// 0x5fc
			 0x17cffa55, /// 0x600
			 0x7df31ba4, /// 0x604
			 0xfc4661b1, /// 0x608
			 0xf7d48c92, /// 0x60c
			 0xbed93be8, /// 0x610
			 0xba0d6b7f, /// 0x614
			 0x2484e176, /// 0x618
			 0x90ac5a7e, /// 0x61c
			 0x2241f1dc, /// 0x620
			 0x5bd82dce, /// 0x624
			 0xf5ce0782, /// 0x628
			 0x7ca053a1, /// 0x62c
			 0x7be5483a, /// 0x630
			 0x611e27e9, /// 0x634
			 0x59097921, /// 0x638
			 0xab2614bf, /// 0x63c
			 0xa8c6b283, /// 0x640
			 0x944189d8, /// 0x644
			 0xc92a93be, /// 0x648
			 0xc2ba274b, /// 0x64c
			 0xdab6e618, /// 0x650
			 0x42a73bd2, /// 0x654
			 0x813dc984, /// 0x658
			 0x7d284271, /// 0x65c
			 0x6dc176cf, /// 0x660
			 0x91b0dbcc, /// 0x664
			 0xd219dd78, /// 0x668
			 0x8d0216c2, /// 0x66c
			 0x6341d352, /// 0x670
			 0x6b147afa, /// 0x674
			 0xc0197d16, /// 0x678
			 0x67fbbd25, /// 0x67c
			 0x25ddc40c, /// 0x680
			 0xa86e4da6, /// 0x684
			 0x8d770b1b, /// 0x688
			 0x7a852ed7, /// 0x68c
			 0x128f860c, /// 0x690
			 0x14e598aa, /// 0x694
			 0x23e8b98d, /// 0x698
			 0xb0c3db67, /// 0x69c
			 0x36f1335d, /// 0x6a0
			 0x5854fcab, /// 0x6a4
			 0x61930562, /// 0x6a8
			 0x3807b155, /// 0x6ac
			 0xb11a265c, /// 0x6b0
			 0xb5966f8a, /// 0x6b4
			 0xd93aa960, /// 0x6b8
			 0x67b4d725, /// 0x6bc
			 0xf0c6c307, /// 0x6c0
			 0x0d8ac7af, /// 0x6c4
			 0xe5990daa, /// 0x6c8
			 0x4386c0a1, /// 0x6cc
			 0xa0a6a604, /// 0x6d0
			 0x7838392b, /// 0x6d4
			 0x175666f7, /// 0x6d8
			 0xe4461ab9, /// 0x6dc
			 0x7ea28a58, /// 0x6e0
			 0xb51f64a6, /// 0x6e4
			 0xebf9787f, /// 0x6e8
			 0xc0425b22, /// 0x6ec
			 0xbb78353b, /// 0x6f0
			 0xd8d74520, /// 0x6f4
			 0x987979fc, /// 0x6f8
			 0x06dbdb82, /// 0x6fc
			 0x5412bba1, /// 0x700
			 0x26ec4d81, /// 0x704
			 0xaea1affe, /// 0x708
			 0x6aeb35bf, /// 0x70c
			 0xb98de126, /// 0x710
			 0x3cbe9fbd, /// 0x714
			 0x9fcdde74, /// 0x718
			 0x876bea8b, /// 0x71c
			 0xb04fe042, /// 0x720
			 0x0a07d6f2, /// 0x724
			 0x888bbf7c, /// 0x728
			 0x4e408817, /// 0x72c
			 0xe740ba8b, /// 0x730
			 0xddd66ca7, /// 0x734
			 0xcdbd67cb, /// 0x738
			 0x01d6dfb8, /// 0x73c
			 0x147dd714, /// 0x740
			 0x473d0e74, /// 0x744
			 0x54352aa6, /// 0x748
			 0x6ec1bc7b, /// 0x74c
			 0xe5fb0e2c, /// 0x750
			 0x1e05625a, /// 0x754
			 0x6efe74b9, /// 0x758
			 0x9b250dd0, /// 0x75c
			 0xde597fa8, /// 0x760
			 0x878fab72, /// 0x764
			 0x8e9c6917, /// 0x768
			 0x732f8b7d, /// 0x76c
			 0x2822e120, /// 0x770
			 0x8210aac1, /// 0x774
			 0x1b518c87, /// 0x778
			 0x3eaa717c, /// 0x77c
			 0x27ed7cd5, /// 0x780
			 0x9afc12dc, /// 0x784
			 0x7cac394a, /// 0x788
			 0xedba1970, /// 0x78c
			 0x6bcbd4ba, /// 0x790
			 0x400baa29, /// 0x794
			 0xfae84b58, /// 0x798
			 0xef4fe699, /// 0x79c
			 0x690f3d69, /// 0x7a0
			 0x32fdae0d, /// 0x7a4
			 0x0cbb20e7, /// 0x7a8
			 0xdb8a4ed8, /// 0x7ac
			 0x12996305, /// 0x7b0
			 0xa1774dc5, /// 0x7b4
			 0xb9b800c2, /// 0x7b8
			 0x58adaaf7, /// 0x7bc
			 0x2296b5cc, /// 0x7c0
			 0xb821c686, /// 0x7c4
			 0x2f8f8cc0, /// 0x7c8
			 0x39d4fa3f, /// 0x7cc
			 0x19bb8db0, /// 0x7d0
			 0x4c7fd281, /// 0x7d4
			 0x674545f0, /// 0x7d8
			 0x335668e7, /// 0x7dc
			 0xe21c7296, /// 0x7e0
			 0x518336b0, /// 0x7e4
			 0xe49af720, /// 0x7e8
			 0xb517289a, /// 0x7ec
			 0x2bb4782f, /// 0x7f0
			 0x31544e42, /// 0x7f4
			 0x822b30d9, /// 0x7f8
			 0x45b4fac9, /// 0x7fc
			 0xb11c2a50, /// 0x800
			 0x2a4bbe24, /// 0x804
			 0x2e873ec7, /// 0x808
			 0xfd1726d5, /// 0x80c
			 0x819a572b, /// 0x810
			 0xcfa08c2f, /// 0x814
			 0xbcc98002, /// 0x818
			 0x6128ba29, /// 0x81c
			 0x1dc01c61, /// 0x820
			 0x58c4db0c, /// 0x824
			 0x38fdcc45, /// 0x828
			 0x1367b5ff, /// 0x82c
			 0x0c59db63, /// 0x830
			 0x87a0dc5b, /// 0x834
			 0xd9dae12f, /// 0x838
			 0xd775bcae, /// 0x83c
			 0x6149b7af, /// 0x840
			 0x94272f02, /// 0x844
			 0x04fcb054, /// 0x848
			 0x61094e4d, /// 0x84c
			 0xf6c2856b, /// 0x850
			 0xbb745ed7, /// 0x854
			 0x198e3b05, /// 0x858
			 0xf227b7a1, /// 0x85c
			 0x0f624c85, /// 0x860
			 0x0d15eac6, /// 0x864
			 0x6d8f6085, /// 0x868
			 0xcd862c4a, /// 0x86c
			 0x1aefe979, /// 0x870
			 0x905b7c5e, /// 0x874
			 0x0e6abe14, /// 0x878
			 0x0cfb0750, /// 0x87c
			 0x4d59a1aa, /// 0x880
			 0x5ce7b29a, /// 0x884
			 0x38879fcd, /// 0x888
			 0x85632243, /// 0x88c
			 0xe9146429, /// 0x890
			 0xff423b49, /// 0x894
			 0xe9468b4d, /// 0x898
			 0x07082474, /// 0x89c
			 0x6f4fcdbb, /// 0x8a0
			 0x17533541, /// 0x8a4
			 0xcdedf48a, /// 0x8a8
			 0xf1236a42, /// 0x8ac
			 0xd74d201e, /// 0x8b0
			 0xcfaa43e2, /// 0x8b4
			 0x372cfd18, /// 0x8b8
			 0x624eaa45, /// 0x8bc
			 0xeaa6f8b7, /// 0x8c0
			 0xdf1bb1de, /// 0x8c4
			 0x58f3043b, /// 0x8c8
			 0x7ab24b73, /// 0x8cc
			 0xc2d0d95c, /// 0x8d0
			 0xab33a998, /// 0x8d4
			 0x486851de, /// 0x8d8
			 0xeac57119, /// 0x8dc
			 0x66df64ec, /// 0x8e0
			 0x97a20d5f, /// 0x8e4
			 0x3ce9d4f3, /// 0x8e8
			 0xbc584c0e, /// 0x8ec
			 0x22f222f4, /// 0x8f0
			 0x6c7f2af0, /// 0x8f4
			 0x4be9967c, /// 0x8f8
			 0xf1cfcdcc, /// 0x8fc
			 0x603ce86f, /// 0x900
			 0x15cacad0, /// 0x904
			 0x59032c6c, /// 0x908
			 0x4fdc61d9, /// 0x90c
			 0x4540f100, /// 0x910
			 0xa4408e69, /// 0x914
			 0xc73810b4, /// 0x918
			 0xc2ec05c7, /// 0x91c
			 0xce9bdbd4, /// 0x920
			 0x65be2e19, /// 0x924
			 0xc303ba11, /// 0x928
			 0xdad7b60d, /// 0x92c
			 0xa947d1b1, /// 0x930
			 0xc057948b, /// 0x934
			 0xf1839c9a, /// 0x938
			 0x53b4d1fa, /// 0x93c
			 0x889aca59, /// 0x940
			 0xd78378e6, /// 0x944
			 0x792e7923, /// 0x948
			 0x7a79740d, /// 0x94c
			 0x82bea156, /// 0x950
			 0xfd71c636, /// 0x954
			 0xa991e220, /// 0x958
			 0x63241d0a, /// 0x95c
			 0xf6eb344d, /// 0x960
			 0x4ca17ee6, /// 0x964
			 0xf3278baa, /// 0x968
			 0x7f35b35e, /// 0x96c
			 0x1e8cd349, /// 0x970
			 0x01de2acf, /// 0x974
			 0x9d309eab, /// 0x978
			 0x6113666d, /// 0x97c
			 0xc9068131, /// 0x980
			 0xffc4d3fa, /// 0x984
			 0x75ae2dff, /// 0x988
			 0xe58e329c, /// 0x98c
			 0xb6c54151, /// 0x990
			 0xfe6a0192, /// 0x994
			 0xf6d1cdfa, /// 0x998
			 0xbc1a7d35, /// 0x99c
			 0x6f50a879, /// 0x9a0
			 0x5c9288ca, /// 0x9a4
			 0xb46d0bdf, /// 0x9a8
			 0x754b58ce, /// 0x9ac
			 0x150a4d4b, /// 0x9b0
			 0xf12f44a4, /// 0x9b4
			 0x1d5c88ec, /// 0x9b8
			 0x510180af, /// 0x9bc
			 0x72d9ce37, /// 0x9c0
			 0x4f7ad6fd, /// 0x9c4
			 0xe55357a4, /// 0x9c8
			 0xa849b5ff, /// 0x9cc
			 0xa8ed97dd, /// 0x9d0
			 0x14b378b9, /// 0x9d4
			 0x0b479a4e, /// 0x9d8
			 0xbd108fd0, /// 0x9dc
			 0xc3df47b2, /// 0x9e0
			 0xa40812aa, /// 0x9e4
			 0xc7d190e5, /// 0x9e8
			 0xd96f4f44, /// 0x9ec
			 0x503f10ae, /// 0x9f0
			 0x8bd235fb, /// 0x9f4
			 0xc6ec6b33, /// 0x9f8
			 0x6a3a2ac1, /// 0x9fc
			 0x2e10d134, /// 0xa00
			 0xf570d009, /// 0xa04
			 0x2c984ec5, /// 0xa08
			 0xcaf82a08, /// 0xa0c
			 0x55adf11b, /// 0xa10
			 0x933c4e82, /// 0xa14
			 0x00fb302e, /// 0xa18
			 0x1e9685b7, /// 0xa1c
			 0x68e24907, /// 0xa20
			 0x632c831c, /// 0xa24
			 0xe465a55f, /// 0xa28
			 0x8da13fa3, /// 0xa2c
			 0xbe158699, /// 0xa30
			 0x07c54635, /// 0xa34
			 0xe322c3ed, /// 0xa38
			 0x42c5196b, /// 0xa3c
			 0x3b3c47fa, /// 0xa40
			 0xf8a848a4, /// 0xa44
			 0xc6f08591, /// 0xa48
			 0x2a2505b3, /// 0xa4c
			 0x663af77d, /// 0xa50
			 0x2f7f257a, /// 0xa54
			 0x2456bdfd, /// 0xa58
			 0x6b836424, /// 0xa5c
			 0xaf9e5211, /// 0xa60
			 0xebb3ecf7, /// 0xa64
			 0x8b1779ca, /// 0xa68
			 0x80e4b75a, /// 0xa6c
			 0x0df9c486, /// 0xa70
			 0xf5c0ea84, /// 0xa74
			 0x74fed661, /// 0xa78
			 0x4731aa28, /// 0xa7c
			 0xb77e89ef, /// 0xa80
			 0xbf386fa9, /// 0xa84
			 0x6fd3bbc5, /// 0xa88
			 0x19b2c835, /// 0xa8c
			 0x17d5f45c, /// 0xa90
			 0x9a2a93cf, /// 0xa94
			 0x3ff0363f, /// 0xa98
			 0x09fe3db7, /// 0xa9c
			 0x350faef2, /// 0xaa0
			 0x4727a241, /// 0xaa4
			 0x28bfb2c0, /// 0xaa8
			 0x19acb131, /// 0xaac
			 0x262aecbd, /// 0xab0
			 0xb3c0b06a, /// 0xab4
			 0x51b6a817, /// 0xab8
			 0x341840a4, /// 0xabc
			 0xb1f57aef, /// 0xac0
			 0x5a99527e, /// 0xac4
			 0x2dfff5de, /// 0xac8
			 0xabfe6912, /// 0xacc
			 0xc769a81b, /// 0xad0
			 0xf1e4c42e, /// 0xad4
			 0x18f07cf6, /// 0xad8
			 0x3ab10a19, /// 0xadc
			 0x3b3907e7, /// 0xae0
			 0x105cf727, /// 0xae4
			 0xd2d06372, /// 0xae8
			 0x0e1763cd, /// 0xaec
			 0x7faea386, /// 0xaf0
			 0xf62c702f, /// 0xaf4
			 0x089214c4, /// 0xaf8
			 0x7ffe68bf, /// 0xafc
			 0x5891abfc, /// 0xb00
			 0xa783af47, /// 0xb04
			 0xc32f0a06, /// 0xb08
			 0x58e9b424, /// 0xb0c
			 0x87a24ffb, /// 0xb10
			 0x8ce5f4b5, /// 0xb14
			 0xa129072d, /// 0xb18
			 0xa3da03a9, /// 0xb1c
			 0xacb3965e, /// 0xb20
			 0xcf84ed6b, /// 0xb24
			 0xcd9f158f, /// 0xb28
			 0xd0eed9a5, /// 0xb2c
			 0x862ee3e8, /// 0xb30
			 0x43ed829b, /// 0xb34
			 0x0ea245c2, /// 0xb38
			 0xe5682c2f, /// 0xb3c
			 0x98f83eef, /// 0xb40
			 0x86abbbfb, /// 0xb44
			 0x2afbf673, /// 0xb48
			 0x11ca8a42, /// 0xb4c
			 0x95912255, /// 0xb50
			 0x23d520e3, /// 0xb54
			 0xb7d1d5da, /// 0xb58
			 0xbec3ef70, /// 0xb5c
			 0x891e5073, /// 0xb60
			 0x31cd3985, /// 0xb64
			 0x0fbeffd9, /// 0xb68
			 0xf397831b, /// 0xb6c
			 0x6769248f, /// 0xb70
			 0x9794d01e, /// 0xb74
			 0x1668a775, /// 0xb78
			 0x40af1243, /// 0xb7c
			 0x899968c7, /// 0xb80
			 0x11fc0dbc, /// 0xb84
			 0xef1a8bd0, /// 0xb88
			 0x282781a6, /// 0xb8c
			 0xfdfd912f, /// 0xb90
			 0xc8681e76, /// 0xb94
			 0xbed47d49, /// 0xb98
			 0xa60c9399, /// 0xb9c
			 0x8f6fbff7, /// 0xba0
			 0x15c1b531, /// 0xba4
			 0xa1e28a46, /// 0xba8
			 0x3f2aa138, /// 0xbac
			 0x8870e1c8, /// 0xbb0
			 0xd48cd4d2, /// 0xbb4
			 0xc261cc46, /// 0xbb8
			 0xf4908611, /// 0xbbc
			 0x22d7da6a, /// 0xbc0
			 0xe37c092d, /// 0xbc4
			 0xaf9488be, /// 0xbc8
			 0x8fe679be, /// 0xbcc
			 0x53d0a317, /// 0xbd0
			 0x8aa407b7, /// 0xbd4
			 0x58ad8199, /// 0xbd8
			 0x7e73eed8, /// 0xbdc
			 0x6a8a7087, /// 0xbe0
			 0xf531d420, /// 0xbe4
			 0x1c3bd11b, /// 0xbe8
			 0x615521ae, /// 0xbec
			 0xe8f0f6d3, /// 0xbf0
			 0x3703ff0e, /// 0xbf4
			 0xa702e085, /// 0xbf8
			 0xbdbae1f7, /// 0xbfc
			 0x2e8b305e, /// 0xc00
			 0xeb34555b, /// 0xc04
			 0x582d4f23, /// 0xc08
			 0xdbdb50e6, /// 0xc0c
			 0x95af0280, /// 0xc10
			 0xfb2fef75, /// 0xc14
			 0xa33c364e, /// 0xc18
			 0x811bfeaf, /// 0xc1c
			 0x9cc4a065, /// 0xc20
			 0xc3bcffcd, /// 0xc24
			 0xa7afeee6, /// 0xc28
			 0x26d7d631, /// 0xc2c
			 0x2668001e, /// 0xc30
			 0x5e821bff, /// 0xc34
			 0xfa78094c, /// 0xc38
			 0x93a2afc0, /// 0xc3c
			 0xe5e2621c, /// 0xc40
			 0x9526689a, /// 0xc44
			 0xf79297a8, /// 0xc48
			 0x5988851f, /// 0xc4c
			 0x03fd8dbd, /// 0xc50
			 0x4b9a9938, /// 0xc54
			 0x142f59e3, /// 0xc58
			 0xccdc4371, /// 0xc5c
			 0x956d5034, /// 0xc60
			 0x1609e482, /// 0xc64
			 0xb71f4630, /// 0xc68
			 0x8ff42daf, /// 0xc6c
			 0xfe18109b, /// 0xc70
			 0x0a969324, /// 0xc74
			 0x71070b18, /// 0xc78
			 0x8197d09f, /// 0xc7c
			 0xf96b8916, /// 0xc80
			 0xbc51b3b4, /// 0xc84
			 0x090c2cb9, /// 0xc88
			 0x739106d3, /// 0xc8c
			 0xc881a21b, /// 0xc90
			 0x1d022fe3, /// 0xc94
			 0x1615bfe2, /// 0xc98
			 0x41d4135c, /// 0xc9c
			 0xe8405ace, /// 0xca0
			 0xcf86986a, /// 0xca4
			 0x8fc3c0d3, /// 0xca8
			 0x5773d724, /// 0xcac
			 0xda63e315, /// 0xcb0
			 0xe361a1b4, /// 0xcb4
			 0xb17999ce, /// 0xcb8
			 0x2314a412, /// 0xcbc
			 0xcdaa2c9c, /// 0xcc0
			 0xa27ad42c, /// 0xcc4
			 0x548dbcee, /// 0xcc8
			 0x3ebc8549, /// 0xccc
			 0xbc80bf72, /// 0xcd0
			 0xbcc820ed, /// 0xcd4
			 0x239b9fab, /// 0xcd8
			 0xe966ca7a, /// 0xcdc
			 0x8859ce7c, /// 0xce0
			 0x31d6ebbc, /// 0xce4
			 0xb6f1fdb2, /// 0xce8
			 0xfbed15d0, /// 0xcec
			 0xc6c3ac34, /// 0xcf0
			 0x7a872b6b, /// 0xcf4
			 0xac9bd1ce, /// 0xcf8
			 0x1e14e845, /// 0xcfc
			 0x02935b39, /// 0xd00
			 0x45c1ebff, /// 0xd04
			 0x27f277fd, /// 0xd08
			 0x39ab22e4, /// 0xd0c
			 0xe37b6d5b, /// 0xd10
			 0x6b6fa141, /// 0xd14
			 0x513c790a, /// 0xd18
			 0xace285cd, /// 0xd1c
			 0x8b0222da, /// 0xd20
			 0x925d6aa5, /// 0xd24
			 0xa37a6a0b, /// 0xd28
			 0xae3d9466, /// 0xd2c
			 0xb391b2e5, /// 0xd30
			 0xdb5cba36, /// 0xd34
			 0x833a013a, /// 0xd38
			 0x8bdab645, /// 0xd3c
			 0xcab3a72c, /// 0xd40
			 0xeb3bc0c5, /// 0xd44
			 0x4d6f0f66, /// 0xd48
			 0xced6bae2, /// 0xd4c
			 0xdbd6d189, /// 0xd50
			 0xbdf702de, /// 0xd54
			 0xf729cfda, /// 0xd58
			 0x4daeca6c, /// 0xd5c
			 0x57f2dcf4, /// 0xd60
			 0x9e5ea256, /// 0xd64
			 0x00f02bc4, /// 0xd68
			 0x37db76f0, /// 0xd6c
			 0x97eae653, /// 0xd70
			 0x5b87f5d8, /// 0xd74
			 0x570a9c9d, /// 0xd78
			 0xdf10e47f, /// 0xd7c
			 0xc291bbd2, /// 0xd80
			 0x18be705c, /// 0xd84
			 0xd40c84be, /// 0xd88
			 0xa2082c44, /// 0xd8c
			 0x0d3fc483, /// 0xd90
			 0x661694da, /// 0xd94
			 0xefec2141, /// 0xd98
			 0x7a302b01, /// 0xd9c
			 0xfca2abec, /// 0xda0
			 0xd401b6b5, /// 0xda4
			 0x3976fffd, /// 0xda8
			 0xe8d4dbf1, /// 0xdac
			 0xec79f61f, /// 0xdb0
			 0xdaa62b4d, /// 0xdb4
			 0x70d98608, /// 0xdb8
			 0x46f308c6, /// 0xdbc
			 0xf9bc998f, /// 0xdc0
			 0x38cd072e, /// 0xdc4
			 0xda8dd92b, /// 0xdc8
			 0x763c12e2, /// 0xdcc
			 0x9b6d0515, /// 0xdd0
			 0x1a13bfa3, /// 0xdd4
			 0x870396c0, /// 0xdd8
			 0xff3f6620, /// 0xddc
			 0x6a67b751, /// 0xde0
			 0xfba81dbf, /// 0xde4
			 0xefecb400, /// 0xde8
			 0x02c89aa1, /// 0xdec
			 0xab7f156c, /// 0xdf0
			 0x3601fe34, /// 0xdf4
			 0xc21a5ab3, /// 0xdf8
			 0x85c1c57a, /// 0xdfc
			 0xc3cbb176, /// 0xe00
			 0xab3ca88f, /// 0xe04
			 0xf4ada3b0, /// 0xe08
			 0x0a9de63a, /// 0xe0c
			 0xe34193d0, /// 0xe10
			 0x60b290b3, /// 0xe14
			 0xba058b36, /// 0xe18
			 0x5e9e5b1a, /// 0xe1c
			 0x058963e4, /// 0xe20
			 0x2b43a1ff, /// 0xe24
			 0x9555b23e, /// 0xe28
			 0xa5a73757, /// 0xe2c
			 0xa57ad06b, /// 0xe30
			 0x7892c6f7, /// 0xe34
			 0x416fc8dd, /// 0xe38
			 0x7ef29e7a, /// 0xe3c
			 0x9dfbf007, /// 0xe40
			 0x5bc171b7, /// 0xe44
			 0xb72d0609, /// 0xe48
			 0x534b4af4, /// 0xe4c
			 0x0c82e599, /// 0xe50
			 0x8bf35749, /// 0xe54
			 0x0d568628, /// 0xe58
			 0xc2ce44e9, /// 0xe5c
			 0xe8aae7f3, /// 0xe60
			 0x5b1c4316, /// 0xe64
			 0xc53884fe, /// 0xe68
			 0xdd43fb2d, /// 0xe6c
			 0x63bf35ba, /// 0xe70
			 0x12d5f541, /// 0xe74
			 0x9fbda3d4, /// 0xe78
			 0x9484b54e, /// 0xe7c
			 0x5e93dfbf, /// 0xe80
			 0x3cad982b, /// 0xe84
			 0x9f2c66f2, /// 0xe88
			 0xdd84a60c, /// 0xe8c
			 0x95e5cb1a, /// 0xe90
			 0xd2af3672, /// 0xe94
			 0x790e87dc, /// 0xe98
			 0x518d53a9, /// 0xe9c
			 0x59e92e51, /// 0xea0
			 0xf1e4a1f8, /// 0xea4
			 0x6c637839, /// 0xea8
			 0x0088b19e, /// 0xeac
			 0xb8b2cd2d, /// 0xeb0
			 0x30351c6a, /// 0xeb4
			 0x405edfbc, /// 0xeb8
			 0x856aeae6, /// 0xebc
			 0xae53bdd1, /// 0xec0
			 0x0216bb3d, /// 0xec4
			 0x64d5d08c, /// 0xec8
			 0x91edb8bd, /// 0xecc
			 0xdde4162a, /// 0xed0
			 0xeb48bd20, /// 0xed4
			 0x13f03be4, /// 0xed8
			 0xcaf4bb80, /// 0xedc
			 0x2a5cd1be, /// 0xee0
			 0x9e7fdb79, /// 0xee4
			 0x434783cf, /// 0xee8
			 0xa9e7bc6b, /// 0xeec
			 0xed31496a, /// 0xef0
			 0x9e3cdd20, /// 0xef4
			 0x913a9988, /// 0xef8
			 0x458f4f80, /// 0xefc
			 0xfc98e9b7, /// 0xf00
			 0x46ed2f9b, /// 0xf04
			 0xa70fa136, /// 0xf08
			 0xa97a4a6c, /// 0xf0c
			 0x543211b2, /// 0xf10
			 0x1f03392f, /// 0xf14
			 0xa4a3bdcd, /// 0xf18
			 0x49e4e9cb, /// 0xf1c
			 0xc67f3bda, /// 0xf20
			 0x38f9b407, /// 0xf24
			 0x7b0d2ca7, /// 0xf28
			 0x6d8fabee, /// 0xf2c
			 0xd6948f05, /// 0xf30
			 0xc041f2b0, /// 0xf34
			 0x76032205, /// 0xf38
			 0x364c3224, /// 0xf3c
			 0x3e88a01f, /// 0xf40
			 0xdeae7083, /// 0xf44
			 0xdf373634, /// 0xf48
			 0x9e84cc90, /// 0xf4c
			 0x3c39de9f, /// 0xf50
			 0xfaf9d1e7, /// 0xf54
			 0x5752406a, /// 0xf58
			 0x8e2ceaf5, /// 0xf5c
			 0xa99e0e90, /// 0xf60
			 0x98097586, /// 0xf64
			 0x8f6afef1, /// 0xf68
			 0x1bc2f33d, /// 0xf6c
			 0xf9311ca8, /// 0xf70
			 0x601abeee, /// 0xf74
			 0xab3da5f6, /// 0xf78
			 0x78aeedd2, /// 0xf7c
			 0x57e0b635, /// 0xf80
			 0x6d7ab6c9, /// 0xf84
			 0x0bd9bae4, /// 0xf88
			 0xa66ca15c, /// 0xf8c
			 0xa7b5a876, /// 0xf90
			 0x29831941, /// 0xf94
			 0x9622d63e, /// 0xf98
			 0x261db053, /// 0xf9c
			 0x3698acd0, /// 0xfa0
			 0x4c1857fe, /// 0xfa4
			 0x79d90840, /// 0xfa8
			 0x3a3d8cbb, /// 0xfac
			 0x1fee833c, /// 0xfb0
			 0xef38e71b, /// 0xfb4
			 0xfcf269a9, /// 0xfb8
			 0x55376b22, /// 0xfbc
			 0xab07218d, /// 0xfc0
			 0xc99341c3, /// 0xfc4
			 0x3059c7bc, /// 0xfc8
			 0xddb93003, /// 0xfcc
			 0x28554ea1, /// 0xfd0
			 0x16250289, /// 0xfd4
			 0x4439df05, /// 0xfd8
			 0xa9d6a278, /// 0xfdc
			 0xf4d001fe, /// 0xfe0
			 0x7f633703, /// 0xfe4
			 0x16e1b582, /// 0xfe8
			 0xe5598fbe, /// 0xfec
			 0x61e92ef2, /// 0xff0
			 0x279fc234, /// 0xff4
			 0x65523233, /// 0xff8
			 0x0498fead /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00000
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00008
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00010
			 0x0c780000,                                                  // Leading 1s:                                 /// 00014
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00018
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0001c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0002c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00030
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00038
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00044
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00050
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00054
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00058
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0005c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00060
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00070
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00074
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00080
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00088
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00090
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00098
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0009c
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xbf028f5c,                                                  // -0.51                                       /// 000c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00104
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00108
			 0x33333333,                                                  // 4 random values                             /// 0010c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00110
			 0x0e000007,                                                  // Trailing 1s:                                /// 00114
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00118
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00124
			 0x00000000,                                                  // zero                                        /// 00128
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00130
			 0x0e000003,                                                  // Trailing 1s:                                /// 00134
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0013c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00140
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00148
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0014c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00150
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00154
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0015c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00164
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0016c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00180
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00188
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0xff800000,                                                  // -inf                                        /// 00198
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0019c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0xbf028f5c,                                                  // -0.51                                       /// 001c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00200
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00204
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00208
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00214
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00218
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00220
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00228
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0022c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00230
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00234
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0024c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00250
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00254
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00258
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0025c
			 0x7fc00001,                                                  // signaling NaN                               /// 00260
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00264
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00268
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0026c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00270
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00274
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0027c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00280
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0c780000,                                                  // Leading 1s:                                 /// 00288
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0029c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c4
			 0x33333333,                                                  // 4 random values                             /// 002c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 002f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00300
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00304
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00308
			 0x0c400000,                                                  // Leading 1s:                                 /// 0030c
			 0xff800000,                                                  // -inf                                        /// 00310
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00318
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00320
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00324
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00328
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0032c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00330
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00338
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0033c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00340
			 0x0e000001,                                                  // Trailing 1s:                                /// 00344
			 0x0c780000,                                                  // Leading 1s:                                 /// 00348
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0034c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00350
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00354
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00360
			 0x80011111,                                                  // -9.7958E-41                                 /// 00364
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x4b000000,                                                  // 8388608.0                                   /// 0036c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00370
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00378
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00380
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00384
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00390
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00394
			 0x7f800000,                                                  // inf                                         /// 00398
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0039c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000000,                                                  // zero                                        /// 003ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003d4
			 0xffc00001,                                                  // -signaling NaN                              /// 003d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xffc00001,                                                  // -signaling NaN                              /// 003e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003f0
			 0x3f028f5c,                                                  // 0.51                                        /// 003f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00400
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0040c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00414
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00420
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00428
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0042c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00430
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00434
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0043c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00440
			 0x0c600000,                                                  // Leading 1s:                                 /// 00444
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00448
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0044c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00450
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00454
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00458
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0045c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00464
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00468
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0046c
			 0x4b000000,                                                  // 8388608.0                                   /// 00470
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00478
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0047c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x0e000007,                                                  // Trailing 1s:                                /// 0048c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00490
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00494
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00498
			 0xcb000000,                                                  // -8388608.0                                  /// 0049c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004a0
			 0xcb000000,                                                  // -8388608.0                                  /// 004a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xbf028f5c,                                                  // -0.51                                       /// 004ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00504
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00508
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0050c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00510
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00514
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00518
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00524
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00528
			 0x0e000001,                                                  // Trailing 1s:                                /// 0052c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0xcb000000,                                                  // -8388608.0                                  /// 00534
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0053c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00540
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00544
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00548
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0054c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00550
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00554
			 0x0c600000,                                                  // Leading 1s:                                 /// 00558
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0055c
			 0x00000000,                                                  // zero                                        /// 00560
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00570
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0057c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00580
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00584
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0058c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00594
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00598
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0059c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f4
			 0x7f800000,                                                  // inf                                         /// 005f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0060c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00614
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00618
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0061c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00620
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00628
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0062c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00630
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00634
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0063c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00644
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00648
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0064c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00650
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00658
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00664
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00668
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0066c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00670
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00674
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0067c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00680
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00684
			 0x0c600000,                                                  // Leading 1s:                                 /// 00688
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0068c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00690
			 0x55555555,                                                  // 4 random values                             /// 00694
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00698
			 0x0e000001,                                                  // Trailing 1s:                                /// 0069c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ac
			 0x33333333,                                                  // 4 random values                             /// 006b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00700
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00704
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00710
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00718
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0071c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00720
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00724
			 0x00000000,                                                  // zero                                        /// 00728
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0072c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00734
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x7f800000,                                                  // inf                                         /// 00744
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00748
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00750
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00754
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00758
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0075c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00760
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00764
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00768
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00778
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00780
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00788
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00790
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00794
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00798
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0079c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00800
			 0x0c600000,                                                  // Leading 1s:                                 /// 00804
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00808
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00810
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00814
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00818
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00820
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00824
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00828
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0082c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00830
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00834
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00838
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0083c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00844
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00848
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0084c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00850
			 0xbf028f5c,                                                  // -0.51                                       /// 00854
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0086c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00870
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00874
			 0x7f800000,                                                  // inf                                         /// 00878
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0087c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00880
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00884
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00888
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00890
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00894
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00898
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0089c
			 0x4b000000,                                                  // 8388608.0                                   /// 008a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7fc00001,                                                  // signaling NaN                               /// 008c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00904
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00908
			 0x4b000000,                                                  // 8388608.0                                   /// 0090c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00910
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00918
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00920
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00930
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00934
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00938
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00940
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00944
			 0x55555555,                                                  // 4 random values                             /// 00948
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0094c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0095c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00964
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0e000007,                                                  // Trailing 1s:                                /// 00974
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x7f800000,                                                  // inf                                         /// 00984
			 0xcb000000,                                                  // -8388608.0                                  /// 00988
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0098c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00990
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00994
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00998
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0099c
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x3f028f5c,                                                  // 0.51                                        /// 00a08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a1c
			 0x55555555,                                                  // 4 random values                             /// 00a20
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a24
			 0xcb000000,                                                  // -8388608.0                                  /// 00a28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x7f800000,                                                  // inf                                         /// 00a7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x33333333,                                                  // 4 random values                             /// 00aa0
			 0xff800000,                                                  // -inf                                        /// 00aa4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ab4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00abc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ac4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ae4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x7f800000,                                                  // inf                                         /// 00b10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ba0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ba4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00be8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0xff800000,                                                  // -inf                                        /// 00c60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca0
			 0x55555555,                                                  // 4 random values                             /// 00ca4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cb4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ccc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ce0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ce8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf8
			 0xff800000,                                                  // -inf                                        /// 00cfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x7fc00001,                                                  // signaling NaN                               /// 00d18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d60
			 0x80000000,                                                  // -zero                                       /// 00d64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ddc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00de0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x33333333,                                                  // 4 random values                             /// 00e14
			 0x7fc00001,                                                  // signaling NaN                               /// 00e18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00000000,                                                  // zero                                        /// 00e4c
			 0x7f800000,                                                  // inf                                         /// 00e50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e70
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e74
			 0x4b000000,                                                  // 8388608.0                                   /// 00e78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e8c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ea8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec4
			 0x00000000,                                                  // zero                                        /// 00ec8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ed0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ed4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00edc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ee8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00efc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f20
			 0xff800000,                                                  // -inf                                        /// 00f24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x55555555,                                                  // 4 random values                             /// 00f48
			 0xff800000,                                                  // -inf                                        /// 00f4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fa4
			 0x33333333,                                                  // 4 random values                             /// 00fa8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fe4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ff0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ff4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7fc000                                                  // Leading 1s:                                 /// last
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
			 0x000005e0,
			 0x000007c4,
			 0x00000010,
			 0x00000104,
			 0x00000498,
			 0x000000d4,
			 0x000002d4,
			 0x00000488,

			 /// vpu register f2
			 0x40a00000,
			 0x41200000,
			 0x41d00000,
			 0x41a00000,
			 0x41e00000,
			 0x41f00000,
			 0x41000000,
			 0x41f80000,

			 /// vpu register f3
			 0x41300000,
			 0x41700000,
			 0x41200000,
			 0x41100000,
			 0x41900000,
			 0x41300000,
			 0x41900000,
			 0x41100000,

			 /// vpu register f4
			 0x42000000,
			 0x41e80000,
			 0x3f800000,
			 0x41c00000,
			 0x40400000,
			 0x41980000,
			 0x41200000,
			 0x41800000,

			 /// vpu register f5
			 0x41f00000,
			 0x41500000,
			 0x3f800000,
			 0x40c00000,
			 0x40a00000,
			 0x3f800000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f6
			 0x41e80000,
			 0x41000000,
			 0x41500000,
			 0x41500000,
			 0x41e00000,
			 0x40e00000,
			 0x41c00000,
			 0x40a00000,

			 /// vpu register f7
			 0x40c00000,
			 0x41000000,
			 0x41a00000,
			 0x41980000,
			 0x41900000,
			 0x41880000,
			 0x41e80000,
			 0x40e00000,

			 /// vpu register f8
			 0x41000000,
			 0x40800000,
			 0x41900000,
			 0x41f00000,
			 0x40000000,
			 0x41000000,
			 0x41e00000,
			 0x41d80000,

			 /// vpu register f9
			 0x41800000,
			 0x41000000,
			 0x41b80000,
			 0x41b80000,
			 0x41880000,
			 0x41e80000,
			 0x41a80000,
			 0x41e00000,

			 /// vpu register f10
			 0x41200000,
			 0x41a80000,
			 0x40800000,
			 0x41a80000,
			 0x41e00000,
			 0x41300000,
			 0x41600000,
			 0x41500000,

			 /// vpu register f11
			 0x41900000,
			 0x41100000,
			 0x41600000,
			 0x42000000,
			 0x41200000,
			 0x41e80000,
			 0x41e00000,
			 0x41900000,

			 /// vpu register f12
			 0x41200000,
			 0x40a00000,
			 0x41300000,
			 0x41300000,
			 0x41b00000,
			 0x41000000,
			 0x41980000,
			 0x41000000,

			 /// vpu register f13
			 0x41c80000,
			 0x41880000,
			 0x41200000,
			 0x41a00000,
			 0x41c80000,
			 0x41600000,
			 0x41b80000,
			 0x41100000,

			 /// vpu register f14
			 0x40e00000,
			 0x41500000,
			 0x41d80000,
			 0x41f80000,
			 0x41c00000,
			 0x41e00000,
			 0x41800000,
			 0x41f00000,

			 /// vpu register f15
			 0x41a00000,
			 0x41500000,
			 0x41c80000,
			 0x41100000,
			 0x41600000,
			 0x41d00000,
			 0x41500000,
			 0x41b00000,

			 /// vpu register f16
			 0x42000000,
			 0x41c00000,
			 0x41b80000,
			 0x40c00000,
			 0x41f00000,
			 0x40a00000,
			 0x41d80000,
			 0x41d80000,

			 /// vpu register f17
			 0x41f80000,
			 0x3f800000,
			 0x40a00000,
			 0x40a00000,
			 0x41880000,
			 0x41900000,
			 0x41800000,
			 0x41f00000,

			 /// vpu register f18
			 0x41a00000,
			 0x41c00000,
			 0x41a80000,
			 0x40c00000,
			 0x40000000,
			 0x41a00000,
			 0x41900000,
			 0x40000000,

			 /// vpu register f19
			 0x41a00000,
			 0x41880000,
			 0x41f00000,
			 0x40a00000,
			 0x40c00000,
			 0x41b00000,
			 0x41c80000,
			 0x41f00000,

			 /// vpu register f20
			 0x41200000,
			 0x41400000,
			 0x41b80000,
			 0x40000000,
			 0x40400000,
			 0x41e80000,
			 0x41500000,
			 0x41f00000,

			 /// vpu register f21
			 0x40800000,
			 0x41980000,
			 0x41c00000,
			 0x41e80000,
			 0x41e80000,
			 0x41700000,
			 0x41600000,
			 0x41200000,

			 /// vpu register f22
			 0x40000000,
			 0x40400000,
			 0x41b00000,
			 0x41300000,
			 0x41000000,
			 0x41400000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f23
			 0x40c00000,
			 0x41e80000,
			 0x41b80000,
			 0x41d00000,
			 0x41c00000,
			 0x40800000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f24
			 0x41880000,
			 0x41e00000,
			 0x3f800000,
			 0x41300000,
			 0x41a00000,
			 0x41900000,
			 0x41c00000,
			 0x42000000,

			 /// vpu register f25
			 0x40400000,
			 0x41800000,
			 0x41a80000,
			 0x41d00000,
			 0x41900000,
			 0x41f00000,
			 0x41b00000,
			 0x41f00000,

			 /// vpu register f26
			 0x40a00000,
			 0x40000000,
			 0x41800000,
			 0x41000000,
			 0x41e80000,
			 0x41500000,
			 0x42000000,
			 0x40800000,

			 /// vpu register f27
			 0x41200000,
			 0x41d00000,
			 0x41300000,
			 0x41a80000,
			 0x41300000,
			 0x41c80000,
			 0x41980000,
			 0x41a80000,

			 /// vpu register f28
			 0x40a00000,
			 0x41400000,
			 0x41700000,
			 0x41a00000,
			 0x41d80000,
			 0x41d80000,
			 0x41300000,
			 0x41900000,

			 /// vpu register f29
			 0x41b80000,
			 0x41400000,
			 0x41300000,
			 0x41500000,
			 0x41500000,
			 0x40000000,
			 0x40400000,
			 0x41c00000,

			 /// vpu register f30
			 0x40000000,
			 0x41f80000,
			 0x41200000,
			 0x41880000,
			 0x40e00000,
			 0x40a00000,
			 0x40000000,
			 0x41500000,

			 /// vpu register f31
			 0x41f00000,
			 0x41800000,
			 0x40a00000,
			 0x41a80000,
			 0x41300000,
			 0x41c80000,
			 0x41200000,
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
		"famomaxug.pi f7, f1 (x11)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f29, f1 (x14)\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f26, f1 (x14)\n"                        ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f9, f1 (x15)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f22, f1 (x14)\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f18, f1 (x12)\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f11, f1 (x14)\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f10, f1 (x12)\n"                        ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f23, f1 (x13)\n"                        ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f27, f1 (x14)\n"                        ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f11, f1 (x13)\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f16, f1 (x12)\n"                        ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f18, f1 (x15)\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f7, f1 (x11)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f30, f1 (x12)\n"                        ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f20, f1 (x13)\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f3, f1 (x12)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f12, f1 (x14)\n"                        ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f16, f1 (x12)\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f4, f1 (x13)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f19, f1 (x13)\n"                        ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f6, f1 (x12)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f9, f1 (x13)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f12, f1 (x14)\n"                        ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f14, f1 (x13)\n"                        ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f27, f1 (x14)\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f9, f1 (x13)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f20, f1 (x11)\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f10, f1 (x13)\n"                        ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f29, f1 (x12)\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f24, f1 (x11)\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f27, f1 (x15)\n"                        ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f17, f1 (x13)\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f4, f1 (x14)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f7, f1 (x14)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f23, f1 (x12)\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f20, f1 (x14)\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f15, f1 (x12)\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f17, f1 (x15)\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f5, f1 (x14)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f2, f1 (x15)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f5, f1 (x13)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f4, f1 (x13)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f28, f1 (x15)\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f22, f1 (x11)\n"                        ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f19, f1 (x14)\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f18, f1 (x12)\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f25, f1 (x15)\n"                        ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f10, f1 (x14)\n"                        ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f8, f1 (x15)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f17, f1 (x15)\n"                        ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f26, f1 (x13)\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f7, f1 (x15)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f28, f1 (x15)\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f9, f1 (x13)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f29, f1 (x14)\n"                        ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f6, f1 (x14)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f11, f1 (x11)\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f17, f1 (x11)\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f27, f1 (x15)\n"                        ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f27, f1 (x11)\n"                        ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f11, f1 (x13)\n"                        ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f14, f1 (x13)\n"                        ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f23, f1 (x13)\n"                        ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f26, f1 (x11)\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f19, f1 (x13)\n"                        ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f15, f1 (x12)\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f13, f1 (x15)\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f25, f1 (x12)\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f5, f1 (x11)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f28, f1 (x13)\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f17, f1 (x11)\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f7, f1 (x14)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f11, f1 (x14)\n"                        ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f15, f1 (x15)\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f27, f1 (x14)\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f15, f1 (x14)\n"                        ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f2, f1 (x14)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f8, f1 (x13)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f22, f1 (x15)\n"                        ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f15, f1 (x12)\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f10, f1 (x12)\n"                        ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f19, f1 (x14)\n"                        ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f26, f1 (x11)\n"                        ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f13, f1 (x15)\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f10, f1 (x13)\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f30, f1 (x13)\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f8, f1 (x11)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f23, f1 (x11)\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f17, f1 (x14)\n"                        ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f7, f1 (x12)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f29, f1 (x14)\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f14, f1 (x15)\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f5, f1 (x15)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f13, f1 (x12)\n"                        ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f30, f1 (x14)\n"                        ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f8, f1 (x15)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f14, f1 (x13)\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxug.pi f16, f1 (x12)\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
