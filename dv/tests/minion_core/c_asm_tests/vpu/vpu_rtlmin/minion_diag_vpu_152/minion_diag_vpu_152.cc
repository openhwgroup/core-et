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
			 0x44b4cb2c, /// 0x0
			 0x1e46f6de, /// 0x4
			 0x2ee37e45, /// 0x8
			 0x38a27c63, /// 0xc
			 0x9dd78c08, /// 0x10
			 0x3ec36d75, /// 0x14
			 0x363d410c, /// 0x18
			 0x1dc3821e, /// 0x1c
			 0xcb8dea47, /// 0x20
			 0xbe914870, /// 0x24
			 0xaa4acb1c, /// 0x28
			 0xf74a0d51, /// 0x2c
			 0xcb532908, /// 0x30
			 0x827c2d16, /// 0x34
			 0x3f63cafa, /// 0x38
			 0x1fa0430f, /// 0x3c
			 0xa8cfd648, /// 0x40
			 0xbfd95b69, /// 0x44
			 0xd3c20070, /// 0x48
			 0x740ff083, /// 0x4c
			 0xde7115d6, /// 0x50
			 0xd0e27c3f, /// 0x54
			 0x67b3ef0f, /// 0x58
			 0x1a14ad9c, /// 0x5c
			 0x017f7f99, /// 0x60
			 0xcc707870, /// 0x64
			 0x96285e2e, /// 0x68
			 0x782b7dbd, /// 0x6c
			 0x71f05620, /// 0x70
			 0x1f7fdfa1, /// 0x74
			 0xfaf26063, /// 0x78
			 0xacdaa887, /// 0x7c
			 0xb1950a29, /// 0x80
			 0x7959274f, /// 0x84
			 0x37df24ec, /// 0x88
			 0x4d2a7d4f, /// 0x8c
			 0xfcee3bae, /// 0x90
			 0x625db4d2, /// 0x94
			 0xa34fdd5f, /// 0x98
			 0x2341022c, /// 0x9c
			 0x8033eac1, /// 0xa0
			 0x6b111b35, /// 0xa4
			 0xf00895fb, /// 0xa8
			 0xf6377583, /// 0xac
			 0x990ee5a3, /// 0xb0
			 0x2931dd77, /// 0xb4
			 0x6e6e11f2, /// 0xb8
			 0x2a5dc78e, /// 0xbc
			 0xcca52e8b, /// 0xc0
			 0x2b7dced1, /// 0xc4
			 0x580c9bc1, /// 0xc8
			 0x4306e3e0, /// 0xcc
			 0xdfc8a361, /// 0xd0
			 0x29b073d6, /// 0xd4
			 0x022ee30c, /// 0xd8
			 0xcd0d20e4, /// 0xdc
			 0xe7c1ed75, /// 0xe0
			 0xeeea2d82, /// 0xe4
			 0x5b743427, /// 0xe8
			 0xffbeda23, /// 0xec
			 0xc40b7571, /// 0xf0
			 0xb3dc02ba, /// 0xf4
			 0xe2131546, /// 0xf8
			 0x08a06ac8, /// 0xfc
			 0x63e0920b, /// 0x100
			 0x212a8ddb, /// 0x104
			 0x80826c35, /// 0x108
			 0x7c57868a, /// 0x10c
			 0xdd870485, /// 0x110
			 0x37d7bb80, /// 0x114
			 0x7ddf6288, /// 0x118
			 0x28e38dba, /// 0x11c
			 0xdeca9be5, /// 0x120
			 0x616d6d2a, /// 0x124
			 0x483e7dac, /// 0x128
			 0xa828b1e1, /// 0x12c
			 0x46c33798, /// 0x130
			 0x8a81e5e0, /// 0x134
			 0xac21e546, /// 0x138
			 0x108d34c4, /// 0x13c
			 0x55b7b529, /// 0x140
			 0xb77083e1, /// 0x144
			 0x909a57b6, /// 0x148
			 0x7c52eee0, /// 0x14c
			 0xe2989478, /// 0x150
			 0x425822f3, /// 0x154
			 0x7ef4b4f1, /// 0x158
			 0xc6666a40, /// 0x15c
			 0xff26f307, /// 0x160
			 0x34d3ccfb, /// 0x164
			 0x5ce77c94, /// 0x168
			 0x859efc74, /// 0x16c
			 0x2fe0e522, /// 0x170
			 0xdec4b2f9, /// 0x174
			 0x25a238fa, /// 0x178
			 0xd8940ac8, /// 0x17c
			 0xae3c415f, /// 0x180
			 0xa93a50e8, /// 0x184
			 0x6e238cf0, /// 0x188
			 0xb210c827, /// 0x18c
			 0xa676be43, /// 0x190
			 0x94b3516d, /// 0x194
			 0x48f612b6, /// 0x198
			 0x5ff581ca, /// 0x19c
			 0x4fcf8545, /// 0x1a0
			 0xaff6e23b, /// 0x1a4
			 0x1fba83eb, /// 0x1a8
			 0x66aeb84f, /// 0x1ac
			 0xfde71a2f, /// 0x1b0
			 0xebc64185, /// 0x1b4
			 0xc7fbc1f3, /// 0x1b8
			 0x4e6bef21, /// 0x1bc
			 0xcde81f05, /// 0x1c0
			 0x29e881f3, /// 0x1c4
			 0x79a26478, /// 0x1c8
			 0xd76980ad, /// 0x1cc
			 0xa8788d9a, /// 0x1d0
			 0x684252db, /// 0x1d4
			 0x837866c0, /// 0x1d8
			 0xab80146c, /// 0x1dc
			 0xddba01d7, /// 0x1e0
			 0x1c7ee7b9, /// 0x1e4
			 0x62082d0b, /// 0x1e8
			 0x469829c2, /// 0x1ec
			 0x860a391a, /// 0x1f0
			 0xb0671770, /// 0x1f4
			 0x5e984987, /// 0x1f8
			 0x744e2d20, /// 0x1fc
			 0xc521eae8, /// 0x200
			 0x6a270730, /// 0x204
			 0x08dbf7a8, /// 0x208
			 0xcb5ea9a4, /// 0x20c
			 0x90080ab3, /// 0x210
			 0x23e2cc16, /// 0x214
			 0xc6c36090, /// 0x218
			 0x87ac6860, /// 0x21c
			 0x73919b91, /// 0x220
			 0xeec39a37, /// 0x224
			 0x4844dc2f, /// 0x228
			 0xba959f18, /// 0x22c
			 0xc55f5e0c, /// 0x230
			 0xef83ff9d, /// 0x234
			 0xc3c4d0ed, /// 0x238
			 0x03a1ea47, /// 0x23c
			 0x517f8867, /// 0x240
			 0xb66af208, /// 0x244
			 0x02c4144a, /// 0x248
			 0x44981c86, /// 0x24c
			 0x65c4e62f, /// 0x250
			 0x538c9f10, /// 0x254
			 0x7f46f350, /// 0x258
			 0x9d6ea5f8, /// 0x25c
			 0xfbea1976, /// 0x260
			 0x7944627f, /// 0x264
			 0x04cf86fa, /// 0x268
			 0x8e19df42, /// 0x26c
			 0xc5923c27, /// 0x270
			 0xafbbd26a, /// 0x274
			 0x700a3965, /// 0x278
			 0x195e146f, /// 0x27c
			 0xbe190a6f, /// 0x280
			 0xd31544e0, /// 0x284
			 0x77d2fb24, /// 0x288
			 0x6ed7d149, /// 0x28c
			 0xb0df5a24, /// 0x290
			 0x697047d4, /// 0x294
			 0x34e8e43b, /// 0x298
			 0x84704569, /// 0x29c
			 0xa43d722f, /// 0x2a0
			 0xe16b2d76, /// 0x2a4
			 0x064db5b2, /// 0x2a8
			 0x9b8dfc4c, /// 0x2ac
			 0xf99331eb, /// 0x2b0
			 0x0ea4d543, /// 0x2b4
			 0x1d00641b, /// 0x2b8
			 0x1266c361, /// 0x2bc
			 0x924c3a06, /// 0x2c0
			 0x59bcf499, /// 0x2c4
			 0x2f210a63, /// 0x2c8
			 0xe07563ea, /// 0x2cc
			 0x0bfce07c, /// 0x2d0
			 0x3be4e29a, /// 0x2d4
			 0x6a1c3532, /// 0x2d8
			 0x2e87f82b, /// 0x2dc
			 0xba6c932c, /// 0x2e0
			 0xcd0b415d, /// 0x2e4
			 0xeff9eec7, /// 0x2e8
			 0xd84572be, /// 0x2ec
			 0x7821d018, /// 0x2f0
			 0xdafa47cc, /// 0x2f4
			 0x0796a8cb, /// 0x2f8
			 0xbe5c35ff, /// 0x2fc
			 0x07d2e6b3, /// 0x300
			 0xb9f6132a, /// 0x304
			 0x53fceb3f, /// 0x308
			 0x1709a3dc, /// 0x30c
			 0xfee7d873, /// 0x310
			 0xd4452139, /// 0x314
			 0xb573e959, /// 0x318
			 0x1c0dde36, /// 0x31c
			 0x7a055325, /// 0x320
			 0xa0dd3c8a, /// 0x324
			 0xaa946c8e, /// 0x328
			 0x89ec5d3b, /// 0x32c
			 0x12f6facc, /// 0x330
			 0x3ef17756, /// 0x334
			 0x9f675b23, /// 0x338
			 0xb8c5fd94, /// 0x33c
			 0x5f196c53, /// 0x340
			 0xcbafd062, /// 0x344
			 0xf5667d0a, /// 0x348
			 0x33434d3e, /// 0x34c
			 0x3d6153f8, /// 0x350
			 0x44650fed, /// 0x354
			 0xa867a74f, /// 0x358
			 0x869f68f5, /// 0x35c
			 0x761f00ee, /// 0x360
			 0xbde2b38c, /// 0x364
			 0x03d36153, /// 0x368
			 0x876aac1e, /// 0x36c
			 0x8ab46046, /// 0x370
			 0x211d1032, /// 0x374
			 0x085c9215, /// 0x378
			 0x71b1db6d, /// 0x37c
			 0xaa9f244d, /// 0x380
			 0xbff5d08a, /// 0x384
			 0xce627aae, /// 0x388
			 0x3b435445, /// 0x38c
			 0x2c30ef22, /// 0x390
			 0xdfb67edc, /// 0x394
			 0xeae6bf76, /// 0x398
			 0x8f7446ad, /// 0x39c
			 0x5a3d4800, /// 0x3a0
			 0xf16ce2dc, /// 0x3a4
			 0x2b46dfac, /// 0x3a8
			 0x6de02443, /// 0x3ac
			 0x1d3641dc, /// 0x3b0
			 0x778e2ae9, /// 0x3b4
			 0xb8851b5a, /// 0x3b8
			 0x8df5ec3b, /// 0x3bc
			 0x72f264d4, /// 0x3c0
			 0x07a63b05, /// 0x3c4
			 0x2a8f2879, /// 0x3c8
			 0x5b68fcd3, /// 0x3cc
			 0x7cfd141d, /// 0x3d0
			 0x844ad973, /// 0x3d4
			 0xc5348593, /// 0x3d8
			 0x22fea994, /// 0x3dc
			 0x5e803eab, /// 0x3e0
			 0x5b8b03ad, /// 0x3e4
			 0x52e554c8, /// 0x3e8
			 0x5c6f358e, /// 0x3ec
			 0x76adeefa, /// 0x3f0
			 0x883c3682, /// 0x3f4
			 0xb92a375c, /// 0x3f8
			 0xa4670db6, /// 0x3fc
			 0xfa386c5e, /// 0x400
			 0xb5e6b0a5, /// 0x404
			 0xead15d09, /// 0x408
			 0x6f7a0c53, /// 0x40c
			 0x7b64ffe5, /// 0x410
			 0x8d371ad0, /// 0x414
			 0x04e3fc11, /// 0x418
			 0x6aba0c74, /// 0x41c
			 0xd310f650, /// 0x420
			 0xd3ddc6fa, /// 0x424
			 0x6110fc6d, /// 0x428
			 0x79ca30de, /// 0x42c
			 0xc87ecd64, /// 0x430
			 0x688230fd, /// 0x434
			 0x091cf603, /// 0x438
			 0x05e629f2, /// 0x43c
			 0xd39c110e, /// 0x440
			 0xbf406777, /// 0x444
			 0x53f3e560, /// 0x448
			 0xbea9b834, /// 0x44c
			 0xcd465fcf, /// 0x450
			 0x826c0035, /// 0x454
			 0x94b0ad0d, /// 0x458
			 0x46603b9e, /// 0x45c
			 0xfc753fd2, /// 0x460
			 0x74d3ae2b, /// 0x464
			 0x28508866, /// 0x468
			 0x5c7ca8d5, /// 0x46c
			 0xa7daf183, /// 0x470
			 0xa1451b1c, /// 0x474
			 0x19948c75, /// 0x478
			 0x759ce07d, /// 0x47c
			 0x68c23885, /// 0x480
			 0xb2a2d102, /// 0x484
			 0x35596e41, /// 0x488
			 0x884fd054, /// 0x48c
			 0x9dc502b6, /// 0x490
			 0x8f827f60, /// 0x494
			 0x0939364a, /// 0x498
			 0x519021d6, /// 0x49c
			 0xf8f1e84e, /// 0x4a0
			 0x0f044b8a, /// 0x4a4
			 0x2e040ae3, /// 0x4a8
			 0x0173ab81, /// 0x4ac
			 0x1a868f60, /// 0x4b0
			 0x28c36733, /// 0x4b4
			 0x6da98a3c, /// 0x4b8
			 0x0baac4b1, /// 0x4bc
			 0x3f18f842, /// 0x4c0
			 0x4b4be1e2, /// 0x4c4
			 0x507e1f59, /// 0x4c8
			 0x84909399, /// 0x4cc
			 0x2a9cd622, /// 0x4d0
			 0x325b8fc9, /// 0x4d4
			 0x763db9f3, /// 0x4d8
			 0x79320eaa, /// 0x4dc
			 0x49c3174c, /// 0x4e0
			 0x8d7f014f, /// 0x4e4
			 0x8961118c, /// 0x4e8
			 0x39ff7ed1, /// 0x4ec
			 0x2273b0a8, /// 0x4f0
			 0x45af25ec, /// 0x4f4
			 0x0fd09c64, /// 0x4f8
			 0xe00eb818, /// 0x4fc
			 0x649fc6aa, /// 0x500
			 0x1413cf9c, /// 0x504
			 0xabe50b3e, /// 0x508
			 0x74c0c8cd, /// 0x50c
			 0x973a255a, /// 0x510
			 0xe8d8b49f, /// 0x514
			 0xbd15dc47, /// 0x518
			 0x33d73765, /// 0x51c
			 0x0c3d7a80, /// 0x520
			 0x7ca77e5f, /// 0x524
			 0xdb2fff92, /// 0x528
			 0xba01bee6, /// 0x52c
			 0x32a5aa61, /// 0x530
			 0x72b2947d, /// 0x534
			 0x9356a04c, /// 0x538
			 0x17d7f867, /// 0x53c
			 0x323ad160, /// 0x540
			 0x489eaaad, /// 0x544
			 0x1853fd18, /// 0x548
			 0x43aecf79, /// 0x54c
			 0x088947be, /// 0x550
			 0x57028ca8, /// 0x554
			 0x4eeb9e23, /// 0x558
			 0xec2f08cb, /// 0x55c
			 0x0c0ccf88, /// 0x560
			 0x0f112e43, /// 0x564
			 0xf1e73409, /// 0x568
			 0x57b6ec14, /// 0x56c
			 0x00a2297b, /// 0x570
			 0x3450090d, /// 0x574
			 0x93c45ad3, /// 0x578
			 0x0919b9d3, /// 0x57c
			 0x4e45f10e, /// 0x580
			 0xc0eef377, /// 0x584
			 0x13da0e43, /// 0x588
			 0x7dd149af, /// 0x58c
			 0x3dbe82ce, /// 0x590
			 0xee611eca, /// 0x594
			 0xa973d008, /// 0x598
			 0xaee94d4e, /// 0x59c
			 0xec7ef87f, /// 0x5a0
			 0x69025d99, /// 0x5a4
			 0x85190799, /// 0x5a8
			 0x151a11a0, /// 0x5ac
			 0x481945fe, /// 0x5b0
			 0x275e3841, /// 0x5b4
			 0x76c4ffad, /// 0x5b8
			 0x32827c87, /// 0x5bc
			 0xc7e2fc17, /// 0x5c0
			 0x9fe1d075, /// 0x5c4
			 0xeee0a8eb, /// 0x5c8
			 0xa365efe3, /// 0x5cc
			 0xb719c629, /// 0x5d0
			 0xe3f3deed, /// 0x5d4
			 0x56870a1e, /// 0x5d8
			 0xcee8fc83, /// 0x5dc
			 0x8b325281, /// 0x5e0
			 0x49d63ccb, /// 0x5e4
			 0xc303e453, /// 0x5e8
			 0x9441d57b, /// 0x5ec
			 0xd382f27a, /// 0x5f0
			 0x2b2c4ad9, /// 0x5f4
			 0xd28e8f23, /// 0x5f8
			 0x51a44c31, /// 0x5fc
			 0x4ea0a3bd, /// 0x600
			 0xd355cc1d, /// 0x604
			 0xc2eff83b, /// 0x608
			 0x36381542, /// 0x60c
			 0xf5d16b69, /// 0x610
			 0x24957843, /// 0x614
			 0x21b01f4f, /// 0x618
			 0x0ca2626c, /// 0x61c
			 0xb23d8323, /// 0x620
			 0x300da5c6, /// 0x624
			 0x49eff30d, /// 0x628
			 0xae9ef466, /// 0x62c
			 0x22974284, /// 0x630
			 0x5f31f698, /// 0x634
			 0xdcf49e11, /// 0x638
			 0xe80beee6, /// 0x63c
			 0x420d935b, /// 0x640
			 0xbc7fc58d, /// 0x644
			 0xd904b1c9, /// 0x648
			 0xf7e92e0c, /// 0x64c
			 0x395f59ae, /// 0x650
			 0x8fd8af09, /// 0x654
			 0xf507811d, /// 0x658
			 0xed9ca92c, /// 0x65c
			 0x2c092966, /// 0x660
			 0x721facec, /// 0x664
			 0x0098d236, /// 0x668
			 0x2507d8b9, /// 0x66c
			 0x0935c740, /// 0x670
			 0x8e718816, /// 0x674
			 0x1ad89e46, /// 0x678
			 0xba13ffa6, /// 0x67c
			 0x08e88e37, /// 0x680
			 0x0dc9a2ba, /// 0x684
			 0x2cd88e12, /// 0x688
			 0x512be5c3, /// 0x68c
			 0x74c06f67, /// 0x690
			 0xf0262ae3, /// 0x694
			 0xecfe5535, /// 0x698
			 0x3621621d, /// 0x69c
			 0xd51ff05f, /// 0x6a0
			 0x89d1f168, /// 0x6a4
			 0xc3348a6f, /// 0x6a8
			 0x1cb50917, /// 0x6ac
			 0xeaf667db, /// 0x6b0
			 0xdf368697, /// 0x6b4
			 0xb5e3c122, /// 0x6b8
			 0x28faaf8d, /// 0x6bc
			 0xecafa60e, /// 0x6c0
			 0x22f533e3, /// 0x6c4
			 0xeaeebe6a, /// 0x6c8
			 0xd4bc1efc, /// 0x6cc
			 0xc6778722, /// 0x6d0
			 0xa724b81e, /// 0x6d4
			 0x73f83713, /// 0x6d8
			 0xd6525d8e, /// 0x6dc
			 0x0faad735, /// 0x6e0
			 0xdd36d22f, /// 0x6e4
			 0x20382a9f, /// 0x6e8
			 0x11d67186, /// 0x6ec
			 0xc7f19552, /// 0x6f0
			 0xc5dc2f09, /// 0x6f4
			 0xc7782435, /// 0x6f8
			 0x96decfb6, /// 0x6fc
			 0x1874b7b4, /// 0x700
			 0xcd4f329f, /// 0x704
			 0xf79aef52, /// 0x708
			 0x1c19525e, /// 0x70c
			 0x4d53216c, /// 0x710
			 0x56e87f17, /// 0x714
			 0xdcdfe9ae, /// 0x718
			 0x0b38a1e7, /// 0x71c
			 0x68629dbe, /// 0x720
			 0xcc5f512f, /// 0x724
			 0xad820043, /// 0x728
			 0xb1542e59, /// 0x72c
			 0x1db19f0e, /// 0x730
			 0xd42d2870, /// 0x734
			 0xa5c8b75a, /// 0x738
			 0xa68a28b6, /// 0x73c
			 0x3f84430c, /// 0x740
			 0x0d2376e2, /// 0x744
			 0x176bc69c, /// 0x748
			 0xb3df1e03, /// 0x74c
			 0x3d8d6f42, /// 0x750
			 0xd3da3e98, /// 0x754
			 0x7c7ba703, /// 0x758
			 0x86b6058a, /// 0x75c
			 0x249e316f, /// 0x760
			 0x47ae7029, /// 0x764
			 0xb73058cc, /// 0x768
			 0xb1f019dc, /// 0x76c
			 0x0d5c95e4, /// 0x770
			 0xdc363766, /// 0x774
			 0xdff26c9d, /// 0x778
			 0x7894dd10, /// 0x77c
			 0xb86f3ec7, /// 0x780
			 0xc86000dd, /// 0x784
			 0x63651acf, /// 0x788
			 0x2661976f, /// 0x78c
			 0x7ff21e4b, /// 0x790
			 0x9308faa7, /// 0x794
			 0x0e2fe3fb, /// 0x798
			 0x838b0622, /// 0x79c
			 0xdf942a2d, /// 0x7a0
			 0xc8a4aa3d, /// 0x7a4
			 0xefdacf0c, /// 0x7a8
			 0xb09e2269, /// 0x7ac
			 0x25de8005, /// 0x7b0
			 0x8ae9afa9, /// 0x7b4
			 0x34eafd0b, /// 0x7b8
			 0x23e9d117, /// 0x7bc
			 0x6b10d8a2, /// 0x7c0
			 0xbfeab572, /// 0x7c4
			 0x85ccdccf, /// 0x7c8
			 0x23e1b6fe, /// 0x7cc
			 0x69acc771, /// 0x7d0
			 0x35a80a4c, /// 0x7d4
			 0xda9a7932, /// 0x7d8
			 0x736e0d5b, /// 0x7dc
			 0x0c9b068d, /// 0x7e0
			 0x28369c40, /// 0x7e4
			 0x739b0e55, /// 0x7e8
			 0xed389177, /// 0x7ec
			 0xcd92c3fa, /// 0x7f0
			 0x3147d1c7, /// 0x7f4
			 0x4d79f30a, /// 0x7f8
			 0x9a554530, /// 0x7fc
			 0x4af956ec, /// 0x800
			 0x40628adb, /// 0x804
			 0xc6fa32c7, /// 0x808
			 0xa4694c8f, /// 0x80c
			 0x7ea77931, /// 0x810
			 0x05e9392c, /// 0x814
			 0x1941db10, /// 0x818
			 0x09585500, /// 0x81c
			 0xff27aee9, /// 0x820
			 0xe77c8143, /// 0x824
			 0x8e04f4cc, /// 0x828
			 0x1ad7eb4d, /// 0x82c
			 0xa01f6501, /// 0x830
			 0x83dfbbb0, /// 0x834
			 0xf7a0258e, /// 0x838
			 0xe1349958, /// 0x83c
			 0x800fdd85, /// 0x840
			 0x93d43f72, /// 0x844
			 0xd86a7a83, /// 0x848
			 0xe50e5248, /// 0x84c
			 0x0add44cf, /// 0x850
			 0x1fe1d4e4, /// 0x854
			 0x469f1a88, /// 0x858
			 0x85aeb8b0, /// 0x85c
			 0x9e1b08e5, /// 0x860
			 0x4ab3a66d, /// 0x864
			 0x29c9236b, /// 0x868
			 0x3f324e78, /// 0x86c
			 0x03f13572, /// 0x870
			 0x2af8470a, /// 0x874
			 0x95a7c053, /// 0x878
			 0x3eaab335, /// 0x87c
			 0x9c65128d, /// 0x880
			 0x2473835b, /// 0x884
			 0x703f3b31, /// 0x888
			 0xca1a7db6, /// 0x88c
			 0x29c50bf3, /// 0x890
			 0x6d5f2537, /// 0x894
			 0xcab1ea0b, /// 0x898
			 0x8badcba7, /// 0x89c
			 0x0b868be1, /// 0x8a0
			 0x9a686e0e, /// 0x8a4
			 0xa61214c2, /// 0x8a8
			 0x0c5d142e, /// 0x8ac
			 0x1d97f877, /// 0x8b0
			 0x6795b1d2, /// 0x8b4
			 0xebd3191c, /// 0x8b8
			 0x2a1dff59, /// 0x8bc
			 0x198fd36a, /// 0x8c0
			 0xe2cf7097, /// 0x8c4
			 0xd906135a, /// 0x8c8
			 0x93579051, /// 0x8cc
			 0xcfff5c75, /// 0x8d0
			 0x06ae9b48, /// 0x8d4
			 0x99e23cac, /// 0x8d8
			 0x6087fe42, /// 0x8dc
			 0xbd6078eb, /// 0x8e0
			 0x9a7af28f, /// 0x8e4
			 0x6aec48f4, /// 0x8e8
			 0x1a8f4017, /// 0x8ec
			 0x43cbe02b, /// 0x8f0
			 0xd53b9e45, /// 0x8f4
			 0x4a4cbaf9, /// 0x8f8
			 0xe0d51aac, /// 0x8fc
			 0xc691c871, /// 0x900
			 0x906a74f7, /// 0x904
			 0x8e1c8d78, /// 0x908
			 0xf794a1f6, /// 0x90c
			 0x0a190a75, /// 0x910
			 0x95b1127f, /// 0x914
			 0x726f5a8c, /// 0x918
			 0x3107ad56, /// 0x91c
			 0x849e2e08, /// 0x920
			 0x7d89597b, /// 0x924
			 0x4b34f88b, /// 0x928
			 0xa1ef1d03, /// 0x92c
			 0x98acbb6b, /// 0x930
			 0x4e0c63c0, /// 0x934
			 0x458d8f2d, /// 0x938
			 0xe90bc14d, /// 0x93c
			 0xfd68f4fd, /// 0x940
			 0xd112f5f9, /// 0x944
			 0x6d9cb998, /// 0x948
			 0xeb12ab09, /// 0x94c
			 0xe7148d4c, /// 0x950
			 0x06ffb548, /// 0x954
			 0xcfe0cbaf, /// 0x958
			 0x7d54033e, /// 0x95c
			 0x4bdb4c32, /// 0x960
			 0x00772ca1, /// 0x964
			 0x57347464, /// 0x968
			 0x76ffabaa, /// 0x96c
			 0x05939223, /// 0x970
			 0x8b00a850, /// 0x974
			 0xc25c012e, /// 0x978
			 0x235bec74, /// 0x97c
			 0x9e2986c4, /// 0x980
			 0x97093c06, /// 0x984
			 0x9f1c7783, /// 0x988
			 0x2da3a8a1, /// 0x98c
			 0xbb8cc8e4, /// 0x990
			 0x76c5243f, /// 0x994
			 0x4f90f8a5, /// 0x998
			 0xc394ac7a, /// 0x99c
			 0x8933bb05, /// 0x9a0
			 0x6a46706b, /// 0x9a4
			 0x2e047ff4, /// 0x9a8
			 0xc0675d93, /// 0x9ac
			 0xd3e1e956, /// 0x9b0
			 0x1e5e92c9, /// 0x9b4
			 0x01b12196, /// 0x9b8
			 0xa94f246b, /// 0x9bc
			 0x085545a5, /// 0x9c0
			 0x5418ce07, /// 0x9c4
			 0xd14f61fe, /// 0x9c8
			 0x0729eb0f, /// 0x9cc
			 0xb9dc6857, /// 0x9d0
			 0x105f23c8, /// 0x9d4
			 0x19b03038, /// 0x9d8
			 0x23245d79, /// 0x9dc
			 0xb4822e12, /// 0x9e0
			 0xdd0fabca, /// 0x9e4
			 0xf73a5058, /// 0x9e8
			 0xbc00bbf7, /// 0x9ec
			 0x3df1690a, /// 0x9f0
			 0x6aafd4f2, /// 0x9f4
			 0xb7485124, /// 0x9f8
			 0x7244ff8b, /// 0x9fc
			 0x059d0620, /// 0xa00
			 0xd83434f5, /// 0xa04
			 0x41292301, /// 0xa08
			 0xe4840ff4, /// 0xa0c
			 0xe005b00e, /// 0xa10
			 0x43ceea7d, /// 0xa14
			 0x45c0a186, /// 0xa18
			 0xcddd7d62, /// 0xa1c
			 0x0e01264a, /// 0xa20
			 0x33e02754, /// 0xa24
			 0x893851a0, /// 0xa28
			 0x1517778a, /// 0xa2c
			 0x615ce1e8, /// 0xa30
			 0x0794e7fc, /// 0xa34
			 0xb883d8d6, /// 0xa38
			 0xca39c20b, /// 0xa3c
			 0x9b00a224, /// 0xa40
			 0x1befd17d, /// 0xa44
			 0x825be425, /// 0xa48
			 0x95047244, /// 0xa4c
			 0x1b93ad6f, /// 0xa50
			 0xc5a76094, /// 0xa54
			 0x97903037, /// 0xa58
			 0x135e0b64, /// 0xa5c
			 0xd60e43b2, /// 0xa60
			 0x0287c5ce, /// 0xa64
			 0xf17f79b6, /// 0xa68
			 0xa47df4e8, /// 0xa6c
			 0xfea776a9, /// 0xa70
			 0x060191bd, /// 0xa74
			 0x93397c0f, /// 0xa78
			 0x37aa02f9, /// 0xa7c
			 0x5f22042f, /// 0xa80
			 0x79581a75, /// 0xa84
			 0x77fb9406, /// 0xa88
			 0xcc84fa1c, /// 0xa8c
			 0x5caf5959, /// 0xa90
			 0x8bc91818, /// 0xa94
			 0x33d79a98, /// 0xa98
			 0xc06c413e, /// 0xa9c
			 0x37e6f9e9, /// 0xaa0
			 0xf6d9e4f5, /// 0xaa4
			 0xdb5283d7, /// 0xaa8
			 0xab5125a7, /// 0xaac
			 0x5be65897, /// 0xab0
			 0x02837bcb, /// 0xab4
			 0xd4ec4956, /// 0xab8
			 0x82b05682, /// 0xabc
			 0x372b09ff, /// 0xac0
			 0xdf911d11, /// 0xac4
			 0xe88fab2e, /// 0xac8
			 0x11cea5f5, /// 0xacc
			 0xed95f894, /// 0xad0
			 0x5fc607ed, /// 0xad4
			 0x79e2cad3, /// 0xad8
			 0x31dffe70, /// 0xadc
			 0xd22a122c, /// 0xae0
			 0x8c6e1f79, /// 0xae4
			 0xdc1eb07a, /// 0xae8
			 0x25dad5fc, /// 0xaec
			 0xacc25511, /// 0xaf0
			 0x67bdbef5, /// 0xaf4
			 0xb45d9dc6, /// 0xaf8
			 0xebe5b914, /// 0xafc
			 0x1cc0325d, /// 0xb00
			 0xdca42391, /// 0xb04
			 0xb98ade2a, /// 0xb08
			 0x877fa84d, /// 0xb0c
			 0x5b4965f4, /// 0xb10
			 0x69b131b1, /// 0xb14
			 0xfcac20af, /// 0xb18
			 0xc2c67646, /// 0xb1c
			 0xaba832cb, /// 0xb20
			 0x472e9b28, /// 0xb24
			 0xc1198470, /// 0xb28
			 0x6e595831, /// 0xb2c
			 0x8a60cd6f, /// 0xb30
			 0xe9191453, /// 0xb34
			 0x0c971297, /// 0xb38
			 0x2839ee52, /// 0xb3c
			 0x4ba99ce9, /// 0xb40
			 0xfd00bd1a, /// 0xb44
			 0x27a720bc, /// 0xb48
			 0xcd592537, /// 0xb4c
			 0x8866c390, /// 0xb50
			 0x0e1cca3a, /// 0xb54
			 0xeb3a5d6b, /// 0xb58
			 0x3241fc90, /// 0xb5c
			 0xa596d899, /// 0xb60
			 0x6611b8a8, /// 0xb64
			 0x74cfa3bd, /// 0xb68
			 0x942b6087, /// 0xb6c
			 0x2788091b, /// 0xb70
			 0x89031fcb, /// 0xb74
			 0x47818afa, /// 0xb78
			 0x485c6b9b, /// 0xb7c
			 0x080b4a48, /// 0xb80
			 0x889839c9, /// 0xb84
			 0x78349cfc, /// 0xb88
			 0x68bc320b, /// 0xb8c
			 0x9535ae97, /// 0xb90
			 0xfdfcb052, /// 0xb94
			 0x13c3433a, /// 0xb98
			 0xda382054, /// 0xb9c
			 0x7c6e3c6e, /// 0xba0
			 0xd440a68d, /// 0xba4
			 0xc713156c, /// 0xba8
			 0xc3946e8d, /// 0xbac
			 0xd5ac75e1, /// 0xbb0
			 0xacffc08b, /// 0xbb4
			 0xb2d2c7b3, /// 0xbb8
			 0x1d2790d8, /// 0xbbc
			 0x48024863, /// 0xbc0
			 0xe75983e8, /// 0xbc4
			 0x2a824728, /// 0xbc8
			 0xad6a208c, /// 0xbcc
			 0x93a7d1f6, /// 0xbd0
			 0xee1573e7, /// 0xbd4
			 0x64e8f216, /// 0xbd8
			 0xca919f4a, /// 0xbdc
			 0xdb4698db, /// 0xbe0
			 0xfaa42d88, /// 0xbe4
			 0x2532fc8b, /// 0xbe8
			 0xb5af7416, /// 0xbec
			 0xe2d3d24c, /// 0xbf0
			 0x0fd97ddb, /// 0xbf4
			 0xfbecd5ba, /// 0xbf8
			 0x0d279f2a, /// 0xbfc
			 0x0a981352, /// 0xc00
			 0x7bd38714, /// 0xc04
			 0x0df0e544, /// 0xc08
			 0x39a1ae95, /// 0xc0c
			 0xa17b7f79, /// 0xc10
			 0x814ce65b, /// 0xc14
			 0x365d6650, /// 0xc18
			 0x139a123f, /// 0xc1c
			 0xe45f5a6c, /// 0xc20
			 0x4e071c4a, /// 0xc24
			 0x26b6962d, /// 0xc28
			 0x427d2600, /// 0xc2c
			 0x495a125e, /// 0xc30
			 0xac766bfc, /// 0xc34
			 0xbdde6367, /// 0xc38
			 0x50a5425c, /// 0xc3c
			 0xedba1dbc, /// 0xc40
			 0x1ad67c82, /// 0xc44
			 0xd23ecf9f, /// 0xc48
			 0x488c0190, /// 0xc4c
			 0x0f060be8, /// 0xc50
			 0x9b95548a, /// 0xc54
			 0x6d4597c6, /// 0xc58
			 0x022ede08, /// 0xc5c
			 0x264803e9, /// 0xc60
			 0x7837c31b, /// 0xc64
			 0xb995d6fd, /// 0xc68
			 0x9e4a4aa6, /// 0xc6c
			 0x64e98320, /// 0xc70
			 0x5754625e, /// 0xc74
			 0x919f8735, /// 0xc78
			 0xeb13bcb5, /// 0xc7c
			 0xee36e06e, /// 0xc80
			 0x84b58401, /// 0xc84
			 0xdf21d539, /// 0xc88
			 0xf6a1c12f, /// 0xc8c
			 0x856a129b, /// 0xc90
			 0xf6415eed, /// 0xc94
			 0x8f683462, /// 0xc98
			 0x16f601ea, /// 0xc9c
			 0x0a5f6060, /// 0xca0
			 0x5f109bdf, /// 0xca4
			 0x6fba75f5, /// 0xca8
			 0x8bfb28d7, /// 0xcac
			 0x8db376fb, /// 0xcb0
			 0x87330715, /// 0xcb4
			 0xa7feb943, /// 0xcb8
			 0x7ea19e8b, /// 0xcbc
			 0xa2d5171b, /// 0xcc0
			 0x0f5662ef, /// 0xcc4
			 0x5ce82a30, /// 0xcc8
			 0xccba0cbf, /// 0xccc
			 0xed6919a5, /// 0xcd0
			 0x562aa69f, /// 0xcd4
			 0xa591fcf3, /// 0xcd8
			 0xf8b494d0, /// 0xcdc
			 0xe4a9e8bf, /// 0xce0
			 0x212ec202, /// 0xce4
			 0x92dc8b0f, /// 0xce8
			 0xeddf7732, /// 0xcec
			 0x1e1a1b16, /// 0xcf0
			 0x3be3143e, /// 0xcf4
			 0x074aabd8, /// 0xcf8
			 0x1a64beb4, /// 0xcfc
			 0xaa98b36a, /// 0xd00
			 0xe596575f, /// 0xd04
			 0xb17dc42b, /// 0xd08
			 0x2814b972, /// 0xd0c
			 0xed5ddf22, /// 0xd10
			 0x6bb246ca, /// 0xd14
			 0xc96d12e7, /// 0xd18
			 0x9125b387, /// 0xd1c
			 0x591d4081, /// 0xd20
			 0x9d786f38, /// 0xd24
			 0x1be5bb96, /// 0xd28
			 0xfee6bb1f, /// 0xd2c
			 0xac1245cb, /// 0xd30
			 0xb8cfc6cf, /// 0xd34
			 0x3ac9a202, /// 0xd38
			 0xedb9802d, /// 0xd3c
			 0xb4a53711, /// 0xd40
			 0x38bb7e16, /// 0xd44
			 0x93551c4e, /// 0xd48
			 0xf497860c, /// 0xd4c
			 0xc07845ab, /// 0xd50
			 0xdb048a13, /// 0xd54
			 0x79f428df, /// 0xd58
			 0xde102fb9, /// 0xd5c
			 0x76f9c36d, /// 0xd60
			 0xedaebe35, /// 0xd64
			 0xcbe56d37, /// 0xd68
			 0xb16ff996, /// 0xd6c
			 0xfab36953, /// 0xd70
			 0x39acff30, /// 0xd74
			 0x71d9d468, /// 0xd78
			 0x1cd63992, /// 0xd7c
			 0xfbf65ba8, /// 0xd80
			 0xf627c35f, /// 0xd84
			 0x7b20c5dc, /// 0xd88
			 0x74cbdfb5, /// 0xd8c
			 0x220223bd, /// 0xd90
			 0xa962cd68, /// 0xd94
			 0xd798a44a, /// 0xd98
			 0xdb1de65a, /// 0xd9c
			 0xbeba85f3, /// 0xda0
			 0xf31e53bc, /// 0xda4
			 0x82972dab, /// 0xda8
			 0xc9888e05, /// 0xdac
			 0x1319315c, /// 0xdb0
			 0x521cec1e, /// 0xdb4
			 0x423f5b54, /// 0xdb8
			 0x84bb7d19, /// 0xdbc
			 0x8d9df3de, /// 0xdc0
			 0x1e96aee1, /// 0xdc4
			 0x0d0d29e5, /// 0xdc8
			 0xb1709973, /// 0xdcc
			 0x2a7f7eaa, /// 0xdd0
			 0x6268d33b, /// 0xdd4
			 0x3e005b02, /// 0xdd8
			 0x35f7a8a6, /// 0xddc
			 0x43a158e5, /// 0xde0
			 0x342750a3, /// 0xde4
			 0x4274cbcd, /// 0xde8
			 0xef28ff77, /// 0xdec
			 0x7c151d0e, /// 0xdf0
			 0x67681c93, /// 0xdf4
			 0xee0407b7, /// 0xdf8
			 0xe25812a7, /// 0xdfc
			 0xe8fda42e, /// 0xe00
			 0x7447902d, /// 0xe04
			 0x2c630866, /// 0xe08
			 0x72f1392b, /// 0xe0c
			 0x43350cea, /// 0xe10
			 0x297c0e30, /// 0xe14
			 0x7cc1b9fe, /// 0xe18
			 0xe6bb4e68, /// 0xe1c
			 0x81590aae, /// 0xe20
			 0x882b8a08, /// 0xe24
			 0x12dd1c27, /// 0xe28
			 0x16137283, /// 0xe2c
			 0x7d915fae, /// 0xe30
			 0xdd69f9e8, /// 0xe34
			 0x72e542e5, /// 0xe38
			 0xeb570974, /// 0xe3c
			 0x69b53ed9, /// 0xe40
			 0x80ce52cf, /// 0xe44
			 0xf909c9e8, /// 0xe48
			 0x23e07229, /// 0xe4c
			 0xca93636d, /// 0xe50
			 0x8d47b321, /// 0xe54
			 0x63d9bd2c, /// 0xe58
			 0xf89d128a, /// 0xe5c
			 0xc57c8d72, /// 0xe60
			 0x97b94c9e, /// 0xe64
			 0x684e8c02, /// 0xe68
			 0x133cf6ba, /// 0xe6c
			 0x1605df95, /// 0xe70
			 0x8063ab5b, /// 0xe74
			 0x4350bf31, /// 0xe78
			 0xd4c607ca, /// 0xe7c
			 0xf4169a9e, /// 0xe80
			 0x48980e30, /// 0xe84
			 0x072cee46, /// 0xe88
			 0x10cee2c0, /// 0xe8c
			 0x5bb0a4b7, /// 0xe90
			 0xdddef6dc, /// 0xe94
			 0x8c4b14da, /// 0xe98
			 0x439541cc, /// 0xe9c
			 0xda5b0679, /// 0xea0
			 0x919fd28a, /// 0xea4
			 0x8a813764, /// 0xea8
			 0x3af9bd7d, /// 0xeac
			 0x8b97acb5, /// 0xeb0
			 0x0c44c38d, /// 0xeb4
			 0x5755c812, /// 0xeb8
			 0xc01aaf92, /// 0xebc
			 0xac82a189, /// 0xec0
			 0x1f97bf3a, /// 0xec4
			 0x21081744, /// 0xec8
			 0x0a48829c, /// 0xecc
			 0xfdab4086, /// 0xed0
			 0x54f0ff74, /// 0xed4
			 0xf015a9f3, /// 0xed8
			 0xc98bf599, /// 0xedc
			 0x416b7b5e, /// 0xee0
			 0x5c42d371, /// 0xee4
			 0x0c80c38e, /// 0xee8
			 0x3a237946, /// 0xeec
			 0x3ef66680, /// 0xef0
			 0x6c425fb2, /// 0xef4
			 0xd85f79d4, /// 0xef8
			 0xabfa7c37, /// 0xefc
			 0x6bfa1390, /// 0xf00
			 0xb754151b, /// 0xf04
			 0x2ad64025, /// 0xf08
			 0x83834d27, /// 0xf0c
			 0xb08c18e0, /// 0xf10
			 0x526fb88d, /// 0xf14
			 0xd066ef75, /// 0xf18
			 0xddc4ab8c, /// 0xf1c
			 0x835f2236, /// 0xf20
			 0x7dc3f25b, /// 0xf24
			 0xc4768a7d, /// 0xf28
			 0x4a1e7e26, /// 0xf2c
			 0x778c614f, /// 0xf30
			 0x09377314, /// 0xf34
			 0x2f8ef8fc, /// 0xf38
			 0xde7fabd2, /// 0xf3c
			 0x724d8b42, /// 0xf40
			 0x8bd91c50, /// 0xf44
			 0x34ecaaf3, /// 0xf48
			 0x2b6975f4, /// 0xf4c
			 0x4c0b011d, /// 0xf50
			 0x9c0aa656, /// 0xf54
			 0xe6b853c1, /// 0xf58
			 0xaee45acc, /// 0xf5c
			 0xf20ff809, /// 0xf60
			 0x0e80d691, /// 0xf64
			 0xacfa35b7, /// 0xf68
			 0x3b28bbb6, /// 0xf6c
			 0xdbb135e4, /// 0xf70
			 0x5b624ae2, /// 0xf74
			 0x23ce6378, /// 0xf78
			 0x38b80abb, /// 0xf7c
			 0x58887301, /// 0xf80
			 0xdbf0f806, /// 0xf84
			 0xb66a56ba, /// 0xf88
			 0x6a4ef764, /// 0xf8c
			 0xbbd7758f, /// 0xf90
			 0x0a14ea7d, /// 0xf94
			 0x6f798886, /// 0xf98
			 0x42005163, /// 0xf9c
			 0x2cc5d573, /// 0xfa0
			 0x16035bb5, /// 0xfa4
			 0x1761c0e9, /// 0xfa8
			 0xe11b3cd4, /// 0xfac
			 0x61e4d904, /// 0xfb0
			 0x7c9ac0f6, /// 0xfb4
			 0x2908c639, /// 0xfb8
			 0xccb67958, /// 0xfbc
			 0x3ac69707, /// 0xfc0
			 0xbaf0b2a4, /// 0xfc4
			 0xe7c1e69e, /// 0xfc8
			 0x50527345, /// 0xfcc
			 0x2bd926f1, /// 0xfd0
			 0xb447fb01, /// 0xfd4
			 0x38a90247, /// 0xfd8
			 0x9fd8c769, /// 0xfdc
			 0xdfe63ab1, /// 0xfe0
			 0x5639b86c, /// 0xfe4
			 0xb84ff4e2, /// 0xfe8
			 0x6a6426cd, /// 0xfec
			 0xcbc99f34, /// 0xff0
			 0x083408f0, /// 0xff4
			 0x4ca391b6, /// 0xff8
			 0xeffdc8f0 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00000
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00004
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0xffc00001,                                                  // -signaling NaN                              /// 0000c
			 0x3f028f5c,                                                  // 0.51                                        /// 00010
			 0x0c700000,                                                  // Leading 1s:                                 /// 00014
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0001c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00030
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00034
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00038
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00040
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0004c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0005c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00060
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00064
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0006c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00070
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00074
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00078
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00080
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00084
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00088
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0008c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a8
			 0x55555555,                                                  // 4 random values                             /// 000ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00100
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00104
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x00000000,                                                  // zero                                        /// 00110
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0011c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80011111,                                                  // -9.7958E-41                                 /// 00124
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0012c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00130
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00134
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00138
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0013c
			 0x55555555,                                                  // 4 random values                             /// 00140
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00148
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00160
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00164
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00168
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00178
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0017c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00180
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00184
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00188
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00194
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00208
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00210
			 0x0c600000,                                                  // Leading 1s:                                 /// 00214
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00220
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00224
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0022c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00238
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00240
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00244
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0024c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80011111,                                                  // -9.7958E-41                                 /// 00258
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0025c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x4b000000,                                                  // 8388608.0                                   /// 0026c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x80011111,                                                  // -9.7958E-41                                 /// 00274
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00280
			 0x7fc00001,                                                  // signaling NaN                               /// 00284
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x7fc00001,                                                  // signaling NaN                               /// 00294
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a0
			 0x7f800000,                                                  // inf                                         /// 002a4
			 0x7f800000,                                                  // inf                                         /// 002a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00300
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00308
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0030c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00310
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00314
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00318
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00320
			 0x0c600000,                                                  // Leading 1s:                                 /// 00324
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0032c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00330
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00334
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0033c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00340
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00344
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00348
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00358
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00360
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00370
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00374
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00378
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00380
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00384
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00390
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00394
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00398
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0039c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a8
			 0xcb000000,                                                  // -8388608.0                                  /// 003ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e0
			 0xff800000,                                                  // -inf                                        /// 003e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80011111,                                                  // -9.7958E-41                                 /// 00404
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x7f800000,                                                  // inf                                         /// 00414
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00418
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00420
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00424
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00428
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00430
			 0x7fc00001,                                                  // signaling NaN                               /// 00434
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00440
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00444
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0044c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00450
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00460
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00464
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0046c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00470
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00474
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0047c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00480
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00484
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00494
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0049c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004a4
			 0xffc00001,                                                  // -signaling NaN                              /// 004a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e8
			 0xbf028f5c,                                                  // -0.51                                       /// 004ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00500
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xffc00001,                                                  // -signaling NaN                              /// 0050c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00510
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00514
			 0x4b000000,                                                  // 8388608.0                                   /// 00518
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0051c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00528
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00530
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00538
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0053c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00540
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00544
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0054c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00554
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00558
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0055c
			 0x80000000,                                                  // -zero                                       /// 00560
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0056c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00570
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0057c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00580
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00584
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00588
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00590
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00594
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x7fc00001,                                                  // signaling NaN                               /// 005a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a8
			 0xcb000000,                                                  // -8388608.0                                  /// 005ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00604
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00608
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0060c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00610
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00614
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0061c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00620
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00624
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00628
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00630
			 0x0e000003,                                                  // Trailing 1s:                                /// 00634
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0063c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00644
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00648
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00658
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0065c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x4b000000,                                                  // 8388608.0                                   /// 00664
			 0x80011111,                                                  // -9.7958E-41                                 /// 00668
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00670
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0067c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00680
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00684
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0068c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00690
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00694
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00698
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x55555555,                                                  // 4 random values                             /// 006b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00000000,                                                  // zero                                        /// 006d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 006e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e4
			 0x33333333,                                                  // 4 random values                             /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00700
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00708
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00710
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00718
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0071c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00720
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00724
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00728
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00730
			 0x55555555,                                                  // 4 random values                             /// 00734
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00738
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0073c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00740
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0074c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00750
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00760
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00764
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00770
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00774
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00778
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0077c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00780
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00784
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80000000,                                                  // -zero                                       /// 0078c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00794
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00798
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00000000,                                                  // zero                                        /// 007a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c8
			 0xbf028f5c,                                                  // -0.51                                       /// 007cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00804
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00810
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00814
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0081c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00828
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00830
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00834
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00838
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0083c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00840
			 0x0e000001,                                                  // Trailing 1s:                                /// 00844
			 0x0e000003,                                                  // Trailing 1s:                                /// 00848
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0xbf028f5c,                                                  // -0.51                                       /// 0085c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0086c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00870
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00874
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0087c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00880
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00884
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00888
			 0x0e000003,                                                  // Trailing 1s:                                /// 0088c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00890
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00898
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0089c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c4
			 0xbf028f5c,                                                  // -0.51                                       /// 008c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80000000,                                                  // -zero                                       /// 008f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00900
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00904
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00908
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00910
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00914
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0091c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00920
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00924
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00928
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0092c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0093c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00940
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00944
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0094c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00950
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0095c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00964
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00970
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00978
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0097c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0c780000,                                                  // Leading 1s:                                 /// 00984
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0098c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x0c600000,                                                  // Leading 1s:                                 /// 0099c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x3f028f5c,                                                  // 0.51                                        /// 009b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009d0
			 0x33333333,                                                  // 4 random values                             /// 009d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00a00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a08
			 0xbf028f5c,                                                  // -0.51                                       /// 00a0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a70
			 0xbf028f5c,                                                  // -0.51                                       /// 00a74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a78
			 0x80000000,                                                  // -zero                                       /// 00a7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ab4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00abc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ac4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00acc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00adc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00af0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xffc00001,                                                  // -signaling NaN                              /// 00af8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00afc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000000,                                                  // zero                                        /// 00b10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b30
			 0x7f800000,                                                  // inf                                         /// 00b34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ba4
			 0x55555555,                                                  // 4 random values                             /// 00ba8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bc0
			 0x33333333,                                                  // 4 random values                             /// 00bc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd4
			 0xff800000,                                                  // -inf                                        /// 00bd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c08
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0xffc00001,                                                  // -signaling NaN                              /// 00c2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c98
			 0xbf028f5c,                                                  // -0.51                                       /// 00c9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ca4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ca8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cac
			 0xff800000,                                                  // -inf                                        /// 00cb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ccc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ce8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cf8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cfc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d64
			 0x33333333,                                                  // 4 random values                             /// 00d68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x33333333,                                                  // 4 random values                             /// 00d8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00da4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dbc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00de4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00de8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dfc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e40
			 0xffc00001,                                                  // -signaling NaN                              /// 00e44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x3f028f5c,                                                  // 0.51                                        /// 00e58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x3f028f5c,                                                  // 0.51                                        /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e84
			 0x7f800000,                                                  // inf                                         /// 00e88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ea4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ecc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ed0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00edc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ee0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x00000000,                                                  // zero                                        /// 00ef8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x7f800000,                                                  // inf                                         /// 00f7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f90
			 0x3f028f5c,                                                  // 0.51                                        /// 00f94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fe0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c700000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00004
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00010
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00014
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00018
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00020
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00024
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00028
			 0x80000000,                                                  // -zero                                       /// 0002c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00030
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00034
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0003c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00044
			 0x33333333,                                                  // 4 random values                             /// 00048
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0004c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x0c400000,                                                  // Leading 1s:                                 /// 00054
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00058
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0005c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00060
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00068
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0006c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00074
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00080
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00084
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0008c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00090
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00100
			 0x0e000001,                                                  // Trailing 1s:                                /// 00104
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00110
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00114
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00118
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0011c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00124
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x55555555,                                                  // 4 random values                             /// 00130
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00138
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00144
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00148
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0014c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00154
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00158
			 0x00000000,                                                  // zero                                        /// 0015c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00160
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00164
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00168
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0016c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x33333333,                                                  // 4 random values                             /// 00174
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0017c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00180
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00184
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0018c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00194
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0019c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x3f028f5c,                                                  // 0.51                                        /// 001e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00200
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0020c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00214
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e000007,                                                  // Trailing 1s:                                /// 0021c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00224
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00230
			 0x0c780000,                                                  // Leading 1s:                                 /// 00234
			 0x55555555,                                                  // 4 random values                             /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00240
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00244
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00248
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0024c
			 0x33333333,                                                  // 4 random values                             /// 00250
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x33333333,                                                  // 4 random values                             /// 00258
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0025c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00260
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00264
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00268
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00270
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00274
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0027c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00280
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00284
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00288
			 0x7fc00001,                                                  // signaling NaN                               /// 0028c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00298
			 0x0c780000,                                                  // Leading 1s:                                 /// 0029c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a4
			 0xcb000000,                                                  // -8388608.0                                  /// 002a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00300
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00308
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0030c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00310
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00318
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0031c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00328
			 0x7fc00001,                                                  // signaling NaN                               /// 0032c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00330
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00334
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00338
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00340
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00344
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00348
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0034c
			 0xcb000000,                                                  // -8388608.0                                  /// 00350
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00358
			 0x4b000000,                                                  // 8388608.0                                   /// 0035c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00364
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00378
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00380
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00384
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00388
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00390
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0xff800000,                                                  // -inf                                        /// 003cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00400
			 0x33333333,                                                  // 4 random values                             /// 00404
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0040c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00410
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00414
			 0xbf028f5c,                                                  // -0.51                                       /// 00418
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00424
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0042c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00430
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00434
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00438
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0043c
			 0x3f028f5c,                                                  // 0.51                                        /// 00440
			 0x3f028f5c,                                                  // 0.51                                        /// 00444
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00448
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00450
			 0x0c780000,                                                  // Leading 1s:                                 /// 00454
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00458
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00460
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00468
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0046c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00470
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00474
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00478
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0047c
			 0x3f028f5c,                                                  // 0.51                                        /// 00480
			 0x0c600000,                                                  // Leading 1s:                                 /// 00484
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0048c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00494
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00498
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0049c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00500
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00504
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00510
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00514
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00518
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0051c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00524
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00528
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0052c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00530
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00534
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00538
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00544
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00548
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00550
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0055c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00560
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00564
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0056c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00570
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00578
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00588
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00594
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0059c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00604
			 0x0e000001,                                                  // Trailing 1s:                                /// 00608
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0060c
			 0x3f028f5c,                                                  // 0.51                                        /// 00610
			 0x7fc00001,                                                  // signaling NaN                               /// 00614
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0061c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00620
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00624
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00638
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00648
			 0x4b000000,                                                  // 8388608.0                                   /// 0064c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00660
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00664
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00674
			 0xbf028f5c,                                                  // -0.51                                       /// 00678
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0067c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00680
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00684
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00688
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0068c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00690
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00694
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00698
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0069c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c8
			 0xbf028f5c,                                                  // -0.51                                       /// 006cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d4
			 0x55555555,                                                  // 4 random values                             /// 006d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e4
			 0xbf028f5c,                                                  // -0.51                                       /// 006e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006f8
			 0x7fc00001,                                                  // signaling NaN                               /// 006fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00700
			 0x55555555,                                                  // 4 random values                             /// 00704
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0070c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00710
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x80011111,                                                  // -9.7958E-41                                 /// 00718
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00720
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0072c
			 0x80000000,                                                  // -zero                                       /// 00730
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00734
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0c400000,                                                  // Leading 1s:                                 /// 00744
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00748
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00754
			 0x0e000003,                                                  // Trailing 1s:                                /// 00758
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0075c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00760
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00764
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00768
			 0x0e000003,                                                  // Trailing 1s:                                /// 0076c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00770
			 0x7f800000,                                                  // inf                                         /// 00774
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00780
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00784
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00011111,                                                  // 9.7958E-41                                  /// 0078c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00790
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0079c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007b8
			 0x7fc00001,                                                  // signaling NaN                               /// 007bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x4b000000,                                                  // 8388608.0                                   /// 007e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x55555555,                                                  // 4 random values                             /// 007fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00800
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00808
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0080c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00810
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00814
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00818
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0081c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00820
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00824
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0e000007,                                                  // Trailing 1s:                                /// 0082c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00834
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x80000000,                                                  // -zero                                       /// 0083c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00848
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0084c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00850
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00858
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00860
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00864
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00870
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00874
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00878
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00884
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0088c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00898
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0089c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c4
			 0x00000000,                                                  // zero                                        /// 008c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00900
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00908
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00910
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00918
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0091c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00920
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0092c
			 0x00000000,                                                  // zero                                        /// 00930
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00934
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0093c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00940
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00944
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0094c
			 0x7f800000,                                                  // inf                                         /// 00950
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00954
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00958
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00968
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00970
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00974
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00978
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0097c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00980
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00984
			 0x0c700000,                                                  // Leading 1s:                                 /// 00988
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0098c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00990
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00994
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00998
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0099c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d4
			 0x55555555,                                                  // 4 random values                             /// 009d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xcb000000,                                                  // -8388608.0                                  /// 00a58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ac4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ac8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00acc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00adc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ae4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00af0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b14
			 0x3f028f5c,                                                  // 0.51                                        /// 00b18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b74
			 0x7f800000,                                                  // inf                                         /// 00b78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bb4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc8
			 0x00000000,                                                  // zero                                        /// 00bcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00be8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bf4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c0c
			 0x7f800000,                                                  // inf                                         /// 00c10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c24
			 0x7fc00001,                                                  // signaling NaN                               /// 00c28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x55555555,                                                  // 4 random values                             /// 00c8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ce8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x33333333,                                                  // 4 random values                             /// 00d40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x33333333,                                                  // 4 random values                             /// 00d88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00da0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00da4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00db0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dcc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ddc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x4b000000,                                                  // 8388608.0                                   /// 00df0
			 0x55555555,                                                  // 4 random values                             /// 00df4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e20
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e38
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00eac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00eb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ebc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ec8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ecc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ed0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee0
			 0x33333333,                                                  // 4 random values                             /// 00ee4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ef4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ef8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fa8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fb4
			 0x3f028f5c,                                                  // 0.51                                        /// 00fb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fe0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ff0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00000200                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00000
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00008
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00014
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00018
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00020
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00024
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00030
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00034
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00038
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00040
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0004c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x33333333,                                                  // 4 random values                             /// 00058
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0005c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00060
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00064
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00068
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0006c
			 0xff800000,                                                  // -inf                                        /// 00070
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0007c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00080
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00090
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00098
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0009c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000ec
			 0x80000000,                                                  // -zero                                       /// 000f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0010c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00110
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00114
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00118
			 0x4b000000,                                                  // 8388608.0                                   /// 0011c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00128
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00130
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00134
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00138
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0013c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00144
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0014c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0c780000,                                                  // Leading 1s:                                 /// 00154
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00158
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0015c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00160
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00168
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00170
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00194
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x33333333,                                                  // 4 random values                             /// 001a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00200
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00204
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00208
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0020c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x33333333,                                                  // 4 random values                             /// 00214
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00218
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00220
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00228
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0022c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00230
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00234
			 0x0c600000,                                                  // Leading 1s:                                 /// 00238
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0023c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00240
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x7fc00001,                                                  // signaling NaN                               /// 00248
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0024c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00258
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0025c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80000000,                                                  // -zero                                       /// 00264
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0026c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00270
			 0x3f028f5c,                                                  // 0.51                                        /// 00274
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0027c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00280
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00284
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0028c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00290
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00300
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00308
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00310
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0031c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00320
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00328
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0032c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00338
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00340
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00344
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0034c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00350
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00354
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00358
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00370
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00374
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00380
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00384
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00390
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00394
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00398
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b0
			 0xff800000,                                                  // -inf                                        /// 003b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f0
			 0x80000000,                                                  // -zero                                       /// 003f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0xffc00001,                                                  // -signaling NaN                              /// 00404
			 0xffc00001,                                                  // -signaling NaN                              /// 00408
			 0x4b000000,                                                  // 8388608.0                                   /// 0040c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00414
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00418
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0041c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00420
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00424
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00428
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0042c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00430
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00434
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00438
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0043c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00444
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00448
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0044c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00450
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0045c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00468
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00470
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00478
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0047c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00484
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00488
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00490
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xffc00001,                                                  // -signaling NaN                              /// 00498
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0049c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004e8
			 0xff800000,                                                  // -inf                                        /// 004ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0050c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0051c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00520
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0053c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00544
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00548
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0c400000,                                                  // Leading 1s:                                 /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x80000000,                                                  // -zero                                       /// 00560
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00564
			 0x55555555,                                                  // 4 random values                             /// 00568
			 0xff800000,                                                  // -inf                                        /// 0056c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00570
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00574
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00584
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00588
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0058c
			 0x4b000000,                                                  // 8388608.0                                   /// 00590
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00594
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00598
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0x7f800000,                                                  // inf                                         /// 005a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x7f800000,                                                  // inf                                         /// 005ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00600
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xcb000000,                                                  // -8388608.0                                  /// 00608
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00610
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00614
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00620
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0062c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00630
			 0x3f028f5c,                                                  // 0.51                                        /// 00634
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00640
			 0xffc00001,                                                  // -signaling NaN                              /// 00644
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0064c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00650
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00660
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00664
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00668
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00674
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0067c
			 0xcb000000,                                                  // -8388608.0                                  /// 00680
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00684
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00688
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00690
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00694
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c0
			 0x80000000,                                                  // -zero                                       /// 006c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d4
			 0x7f800000,                                                  // inf                                         /// 006d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0xbf028f5c,                                                  // -0.51                                       /// 006e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006f4
			 0xffc00001,                                                  // -signaling NaN                              /// 006f8
			 0xff800000,                                                  // -inf                                        /// 006fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00708
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0070c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00714
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00718
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0xff800000,                                                  // -inf                                        /// 0072c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00734
			 0x7f800000,                                                  // inf                                         /// 00738
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0073c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00744
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0074c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00750
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00754
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00758
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0075c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00768
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0076c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00778
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0077c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00784
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0xff800000,                                                  // -inf                                        /// 00790
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00794
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0079c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x7f800000,                                                  // inf                                         /// 007ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xbf028f5c,                                                  // -0.51                                       /// 007fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00800
			 0x0e000001,                                                  // Trailing 1s:                                /// 00804
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00808
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00810
			 0x00011111,                                                  // 9.7958E-41                                  /// 00814
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00818
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00820
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00824
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0082c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00834
			 0x55555555,                                                  // 4 random values                             /// 00838
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0083c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00840
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00848
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0084c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00850
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0085c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00860
			 0x0c400000,                                                  // Leading 1s:                                 /// 00864
			 0x0e000003,                                                  // Trailing 1s:                                /// 00868
			 0x0c600000,                                                  // Leading 1s:                                 /// 0086c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00870
			 0x0c400000,                                                  // Leading 1s:                                 /// 00874
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x3f028f5c,                                                  // 0.51                                        /// 0087c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00884
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00888
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0088c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00890
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b4
			 0x55555555,                                                  // 4 random values                             /// 008b8
			 0x7f800000,                                                  // inf                                         /// 008bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 008ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00904
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00908
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0090c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00910
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00918
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00928
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0092c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00930
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00934
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00938
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0093c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00940
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00944
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0xbf028f5c,                                                  // -0.51                                       /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00958
			 0x0e000003,                                                  // Trailing 1s:                                /// 0095c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00960
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00974
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00980
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00988
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00994
			 0x0c400000,                                                  // Leading 1s:                                 /// 00998
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0099c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e0
			 0x3f028f5c,                                                  // 0.51                                        /// 009e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a44
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ab4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00acc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00adc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xffc00001,                                                  // -signaling NaN                              /// 00af8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00afc
			 0x3f028f5c,                                                  // 0.51                                        /// 00b00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0xffc00001,                                                  // -signaling NaN                              /// 00b14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000000,                                                  // zero                                        /// 00b50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ba0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bf0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c00
			 0x7fc00001,                                                  // signaling NaN                               /// 00c04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ccc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cdc
			 0xff800000,                                                  // -inf                                        /// 00ce0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d3c
			 0x7f800000,                                                  // inf                                         /// 00d40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xcb000000,                                                  // -8388608.0                                  /// 00d48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00da0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00da4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0xffc00001,                                                  // -signaling NaN                              /// 00dac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ddc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00de4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00de8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df0
			 0xffc00001,                                                  // -signaling NaN                              /// 00df4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x80000000,                                                  // -zero                                       /// 00e7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e90
			 0xcb000000,                                                  // -8388608.0                                  /// 00e94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0xff800000,                                                  // -inf                                        /// 00ea0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ebc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ed8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00edc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ee8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00000000,                                                  // zero                                        /// 00ef4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00efc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80000000,                                                  // -zero                                       /// 00f10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x3f028f5c,                                                  // 0.51                                        /// 00f2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fa8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00000000,                                                  // zero                                        /// 00fc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fe8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ff8
			 0x00400000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00004
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0000c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00010
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00014
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00018
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0001c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0002c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00030
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00034
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00038
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x3f028f5c,                                                  // 0.51                                        /// 00040
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00044
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00058
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0005c
			 0x33333333,                                                  // 4 random values                             /// 00060
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00064
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00070
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00074
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00078
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0007c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00080
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00084
			 0x00000000,                                                  // zero                                        /// 00088
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0009c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 000e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00100
			 0xffc00001,                                                  // -signaling NaN                              /// 00104
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00108
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0010c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00114
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00124
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00128
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0012c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x7fc00001,                                                  // signaling NaN                               /// 00134
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0013c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00000000,                                                  // zero                                        /// 00144
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00148
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0014c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00150
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00158
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00164
			 0x0c700000,                                                  // Leading 1s:                                 /// 00168
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0016c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00174
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00184
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00188
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0018c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00198
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0019c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e0
			 0x33333333,                                                  // 4 random values                             /// 001e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00200
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00204
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00208
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0020c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00210
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00218
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0022c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00230
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00234
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00238
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0023c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00240
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00244
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00248
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0024c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00258
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00260
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00264
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00268
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0026c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00270
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00274
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00278
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0027c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00280
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0028c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00290
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00294
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00298
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0029c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002dc
			 0x80000000,                                                  // -zero                                       /// 002e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00300
			 0x0e000001,                                                  // Trailing 1s:                                /// 00304
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0030c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00314
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00318
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0031c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00328
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00330
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0033c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00340
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00348
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00350
			 0xffc00001,                                                  // -signaling NaN                              /// 00354
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00358
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0035c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00360
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00364
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0036c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00374
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0037c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00384
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00388
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00394
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00398
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0039c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003ac
			 0xbf028f5c,                                                  // -0.51                                       /// 003b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xffc00001,                                                  // -signaling NaN                              /// 003e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003ec
			 0xff800000,                                                  // -inf                                        /// 003f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f8
			 0x33333333,                                                  // 4 random values                             /// 003fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00404
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00408
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0040c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00410
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00414
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00418
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00420
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00424
			 0x0c780000,                                                  // Leading 1s:                                 /// 00428
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0042c
			 0x7f800000,                                                  // inf                                         /// 00430
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0xff800000,                                                  // -inf                                        /// 00438
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00440
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00444
			 0x7fc00001,                                                  // signaling NaN                               /// 00448
			 0x7fc00001,                                                  // signaling NaN                               /// 0044c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00454
			 0x7fc00001,                                                  // signaling NaN                               /// 00458
			 0x80011111,                                                  // -9.7958E-41                                 /// 0045c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00460
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00464
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0046c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00470
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00478
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0047c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00488
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0048c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00490
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e000003,                                                  // Trailing 1s:                                /// 00498
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00500
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00508
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00510
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0051c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00520
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00524
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0052c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00530
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00534
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00544
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xbf028f5c,                                                  // -0.51                                       /// 0054c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00550
			 0x4b000000,                                                  // 8388608.0                                   /// 00554
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0055c
			 0x00000000,                                                  // zero                                        /// 00560
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00568
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00570
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00578
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0057c
			 0x33333333,                                                  // 4 random values                             /// 00580
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0058c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00590
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00598
			 0x7f800000,                                                  // inf                                         /// 0059c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0xbf028f5c,                                                  // -0.51                                       /// 005ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffc00001,                                                  // -signaling NaN                              /// 005b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x4b000000,                                                  // 8388608.0                                   /// 005c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005e8
			 0x55555555,                                                  // 4 random values                             /// 005ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00604
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0060c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00618
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0061c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00620
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00624
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0c700000,                                                  // Leading 1s:                                 /// 0062c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x33333333,                                                  // 4 random values                             /// 00638
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00648
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0065c
			 0x7fc00001,                                                  // signaling NaN                               /// 00660
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00664
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00668
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00670
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x7fc00001,                                                  // signaling NaN                               /// 0067c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00680
			 0x0e000003,                                                  // Trailing 1s:                                /// 00684
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00688
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00690
			 0x0c780000,                                                  // Leading 1s:                                 /// 00694
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0e000001,                                                  // Trailing 1s:                                /// 006a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00700
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00704
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00708
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0070c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00710
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00718
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00720
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00724
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00728
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00734
			 0x55555555,                                                  // 4 random values                             /// 00738
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00740
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00748
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0074c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00754
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00758
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0075c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00768
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00778
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xffc00001,                                                  // -signaling NaN                              /// 00784
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00788
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0078c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00790
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00794
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00798
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x33333333,                                                  // 4 random values                             /// 007ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00804
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0080c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00810
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00814
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00818
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0081c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00824
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00828
			 0x33333333,                                                  // 4 random values                             /// 0082c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0084c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00850
			 0x0e000003,                                                  // Trailing 1s:                                /// 00854
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00864
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00868
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00870
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00874
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00878
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00880
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00884
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00890
			 0x0c600000,                                                  // Leading 1s:                                 /// 00894
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00898
			 0x0e000001,                                                  // Trailing 1s:                                /// 0089c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008c4
			 0xcb000000,                                                  // -8388608.0                                  /// 008c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00900
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00904
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00910
			 0x80011111,                                                  // -9.7958E-41                                 /// 00914
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x7fc00001,                                                  // signaling NaN                               /// 00920
			 0x00000000,                                                  // zero                                        /// 00924
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00928
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0092c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00934
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00938
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00940
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00948
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0094c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00964
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00968
			 0x4b000000,                                                  // 8388608.0                                   /// 0096c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00970
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00978
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00980
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0098c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00990
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00994
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00998
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009b4
			 0xffc00001,                                                  // -signaling NaN                              /// 009b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a0c
			 0x33333333,                                                  // 4 random values                             /// 00a10
			 0x7fc00001,                                                  // signaling NaN                               /// 00a14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aa8
			 0x7f800000,                                                  // inf                                         /// 00aac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00abc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ac4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00acc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00adc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00afc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bd8
			 0x80000000,                                                  // -zero                                       /// 00bdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00be0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0x33333333,                                                  // 4 random values                             /// 00bf4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c04
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x55555555,                                                  // 4 random values                             /// 00c20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x33333333,                                                  // 4 random values                             /// 00c40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c8c
			 0x00000000,                                                  // zero                                        /// 00c90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xffc00001,                                                  // -signaling NaN                              /// 00c98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0xff800000,                                                  // -inf                                        /// 00ca4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ccc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce4
			 0x00000000,                                                  // zero                                        /// 00ce8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d30
			 0x3f028f5c,                                                  // 0.51                                        /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xcb000000,                                                  // -8388608.0                                  /// 00d48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d60
			 0x7fc00001,                                                  // signaling NaN                               /// 00d64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d68
			 0x55555555,                                                  // 4 random values                             /// 00d6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00db0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dc8
			 0x33333333,                                                  // 4 random values                             /// 00dcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ddc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00de0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00de8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00df4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e94
			 0x55555555,                                                  // 4 random values                             /// 00e98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ea4
			 0x33333333,                                                  // 4 random values                             /// 00ea8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ebc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ec4
			 0xff800000,                                                  // -inf                                        /// 00ec8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ed8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00edc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ef0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fa8
			 0xff800000,                                                  // -inf                                        /// 00fac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff8
			 0xbf028f5c                                                  // -0.51                                       /// last
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
			 0x00000088,
			 0x00000718,
			 0x00000620,
			 0x0000011c,
			 0x0000012c,
			 0x000001d4,
			 0x00000158,
			 0x00000208,

			 /// vpu register f2
			 0x40400000,
			 0x41700000,
			 0x41500000,
			 0x41600000,
			 0x41a00000,
			 0x41600000,
			 0x41300000,
			 0x41600000,

			 /// vpu register f3
			 0x41f80000,
			 0x41500000,
			 0x41900000,
			 0x41000000,
			 0x41980000,
			 0x41880000,
			 0x41c00000,
			 0x41500000,

			 /// vpu register f4
			 0x41700000,
			 0x41400000,
			 0x41b80000,
			 0x41a00000,
			 0x41980000,
			 0x41880000,
			 0x41b00000,
			 0x41100000,

			 /// vpu register f5
			 0x41700000,
			 0x41a00000,
			 0x40400000,
			 0x41c80000,
			 0x41400000,
			 0x41d00000,
			 0x40400000,
			 0x41a80000,

			 /// vpu register f6
			 0x40c00000,
			 0x42000000,
			 0x40c00000,
			 0x41c80000,
			 0x41600000,
			 0x40800000,
			 0x40c00000,
			 0x40000000,

			 /// vpu register f7
			 0x41f00000,
			 0x40800000,
			 0x40c00000,
			 0x40400000,
			 0x41d80000,
			 0x41980000,
			 0x40e00000,
			 0x41600000,

			 /// vpu register f8
			 0x41800000,
			 0x41800000,
			 0x41800000,
			 0x41e00000,
			 0x41500000,
			 0x41b80000,
			 0x41e00000,
			 0x41100000,

			 /// vpu register f9
			 0x41880000,
			 0x40c00000,
			 0x41c80000,
			 0x41900000,
			 0x41e80000,
			 0x41000000,
			 0x41b80000,
			 0x41d00000,

			 /// vpu register f10
			 0x40800000,
			 0x41e80000,
			 0x41b80000,
			 0x3f800000,
			 0x40800000,
			 0x41c80000,
			 0x41700000,
			 0x41600000,

			 /// vpu register f11
			 0x41980000,
			 0x41700000,
			 0x41b80000,
			 0x41a80000,
			 0x41300000,
			 0x40800000,
			 0x41e80000,
			 0x41a00000,

			 /// vpu register f12
			 0x41b80000,
			 0x41400000,
			 0x41f00000,
			 0x41d80000,
			 0x40e00000,
			 0x41c80000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f13
			 0x41b00000,
			 0x41f80000,
			 0x41400000,
			 0x41800000,
			 0x40c00000,
			 0x40000000,
			 0x3f800000,
			 0x3f800000,

			 /// vpu register f14
			 0x41e00000,
			 0x41980000,
			 0x40e00000,
			 0x41100000,
			 0x41a00000,
			 0x41d00000,
			 0x3f800000,
			 0x41880000,

			 /// vpu register f15
			 0x41d80000,
			 0x41b80000,
			 0x41e00000,
			 0x41000000,
			 0x41b80000,
			 0x41100000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f16
			 0x41a80000,
			 0x41f00000,
			 0x41400000,
			 0x41600000,
			 0x41980000,
			 0x41800000,
			 0x41500000,
			 0x40a00000,

			 /// vpu register f17
			 0x40000000,
			 0x41200000,
			 0x42000000,
			 0x41d00000,
			 0x41300000,
			 0x41d80000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f18
			 0x41100000,
			 0x41800000,
			 0x3f800000,
			 0x41980000,
			 0x41200000,
			 0x42000000,
			 0x41c80000,
			 0x41f80000,

			 /// vpu register f19
			 0x40000000,
			 0x41a80000,
			 0x41d00000,
			 0x41e00000,
			 0x40000000,
			 0x40e00000,
			 0x41c80000,
			 0x41a80000,

			 /// vpu register f20
			 0x41880000,
			 0x41600000,
			 0x41c80000,
			 0x41e00000,
			 0x41700000,
			 0x41a80000,
			 0x40c00000,
			 0x41800000,

			 /// vpu register f21
			 0x41700000,
			 0x41880000,
			 0x41a80000,
			 0x41000000,
			 0x3f800000,
			 0x41000000,
			 0x41d00000,
			 0x41b00000,

			 /// vpu register f22
			 0x41d00000,
			 0x3f800000,
			 0x41300000,
			 0x41f80000,
			 0x41f80000,
			 0x41800000,
			 0x40a00000,
			 0x41a00000,

			 /// vpu register f23
			 0x41a80000,
			 0x41980000,
			 0x41d80000,
			 0x40800000,
			 0x41a80000,
			 0x41e00000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f24
			 0x40a00000,
			 0x41880000,
			 0x41900000,
			 0x41a80000,
			 0x40400000,
			 0x41880000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f25
			 0x41a80000,
			 0x41000000,
			 0x41a80000,
			 0x41000000,
			 0x41e00000,
			 0x40000000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f26
			 0x40800000,
			 0x41980000,
			 0x41400000,
			 0x41400000,
			 0x40000000,
			 0x41200000,
			 0x41600000,
			 0x41200000,

			 /// vpu register f27
			 0x41d00000,
			 0x41e80000,
			 0x41d80000,
			 0x41e80000,
			 0x41700000,
			 0x41f80000,
			 0x41a80000,
			 0x41880000,

			 /// vpu register f28
			 0x41b00000,
			 0x41a00000,
			 0x40e00000,
			 0x41900000,
			 0x42000000,
			 0x41d80000,
			 0x41980000,
			 0x41600000,

			 /// vpu register f29
			 0x41d80000,
			 0x41300000,
			 0x41d80000,
			 0x41a80000,
			 0x41880000,
			 0x41300000,
			 0x41d80000,
			 0x41880000,

			 /// vpu register f30
			 0x41700000,
			 0x42000000,
			 0x42000000,
			 0x41f00000,
			 0x41800000,
			 0x41600000,
			 0x41000000,
			 0x41c00000,

			 /// vpu register f31
			 0x41f80000,
			 0x40800000,
			 0x41980000,
			 0x41f00000,
			 0x41880000,
			 0x41800000,
			 0x41f80000,
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
		"fsrl.pi f27, f4, f17\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f18, f4, f14\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f12, f2, f14\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f16, f10, f11\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f27, f27, f23\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f12, f1, f23\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f3, f19\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f24, f6, f6\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f13, f20, f20\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f25, f17, f12\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f3, f9, f30\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f24, f26, f23\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f21, f6, f19\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f24, f11, f29\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f29, f28, f30\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f6, f8, f26\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f28, f18, f27\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f21, f30, f17\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f8, f1, f29\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f10, f15, f29\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f24, f27, f18\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f19, f21, f5\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f14, f3, f20\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f28, f25, f28\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f16, f0, f19\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f26, f10, f17\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f11, f18\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f17, f7, f20\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f15, f27, f8\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f22, f8, f8\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f26, f3, f15\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f6, f12, f26\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f11, f21, f29\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f16, f16, f7\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f16, f9, f11\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f15, f8, f4\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f27, f11, f18\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f19, f7, f4\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f10, f4, f11\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f13, f20, f22\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f29, f3, f22\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f21, f10, f3\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f17, f14, f2\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f11, f11, f1\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f15, f21, f3\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f13, f25, f1\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f20, f20, f6\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f23, f19, f26\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f16, f20, f12\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f6, f3, f18\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f5, f9, f21\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f3, f14, f17\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f2, f4, f8\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f18, f15, f8\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f18, f8, f25\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f19, f0, f3\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f8, f22, f8\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f26, f11, f5\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f21, f1, f19\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f11, f9, f15\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f15, f14\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f11, f9, f16\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f12, f24, f2\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f23, f5, f23\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f4, f29, f14\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f29, f29, f7\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f5, f16, f1\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f28, f2, f16\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f13, f22, f15\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f14, f0, f0\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f2, f18, f13\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f16, f0, f26\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f7, f7, f11\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f19, f7, f12\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f23, f3, f10\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f28, f1, f23\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f21, f28, f6\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f26, f7\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f8, f27, f3\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f6, f21, f24\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f5, f24, f11\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f25, f17, f5\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f27, f15, f13\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f28, f23, f13\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f8, f30, f13\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f25, f0\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f22, f8, f29\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f23, f8, f18\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f3, f20, f13\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f23, f23, f2\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f12, f7, f22\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f14, f13, f30\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f15, f12\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f9, f3, f28\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f13, f20, f9\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f30, f29, f8\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f30, f14, f5\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f4, f13, f12\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrl.pi f19, f23, f22\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
