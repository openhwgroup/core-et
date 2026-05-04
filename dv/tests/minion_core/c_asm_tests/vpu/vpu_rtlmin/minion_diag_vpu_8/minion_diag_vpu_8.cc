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
			 0xab301401, /// 0x0
			 0xf18d10d3, /// 0x4
			 0xdd51f869, /// 0x8
			 0xa7087b94, /// 0xc
			 0xf01576ad, /// 0x10
			 0xd3ef6eb5, /// 0x14
			 0x2226e421, /// 0x18
			 0x3c16a13b, /// 0x1c
			 0xc095dc56, /// 0x20
			 0x2c196d01, /// 0x24
			 0xee770308, /// 0x28
			 0xad5a8817, /// 0x2c
			 0xe4fd34e9, /// 0x30
			 0x413d832d, /// 0x34
			 0x98739c88, /// 0x38
			 0x0fb3ef35, /// 0x3c
			 0x3a9b0da6, /// 0x40
			 0xc7daee5f, /// 0x44
			 0xf3e0fbc5, /// 0x48
			 0x552e567f, /// 0x4c
			 0xb6bea7c1, /// 0x50
			 0xbf1cfa5b, /// 0x54
			 0x66351177, /// 0x58
			 0x0b3736c8, /// 0x5c
			 0xe2eaadd9, /// 0x60
			 0x5cfa7bcc, /// 0x64
			 0x3f35fcdd, /// 0x68
			 0x20d1a96b, /// 0x6c
			 0xbc2877f6, /// 0x70
			 0x6fdf9ac6, /// 0x74
			 0x22956ce4, /// 0x78
			 0xc507a2fd, /// 0x7c
			 0xdaeb6a4c, /// 0x80
			 0x8f959590, /// 0x84
			 0x5911e9c7, /// 0x88
			 0x8982ab76, /// 0x8c
			 0xad6d3e4c, /// 0x90
			 0xe500007f, /// 0x94
			 0xe2f95a14, /// 0x98
			 0x16458ad5, /// 0x9c
			 0x416832b4, /// 0xa0
			 0x17a58cbe, /// 0xa4
			 0xe2494866, /// 0xa8
			 0x08cfc2c5, /// 0xac
			 0xfe0dba30, /// 0xb0
			 0x7fa80dd1, /// 0xb4
			 0xf8d0765a, /// 0xb8
			 0xaae2f86c, /// 0xbc
			 0xa9c69c68, /// 0xc0
			 0x86909a85, /// 0xc4
			 0xf4411819, /// 0xc8
			 0x408c44d0, /// 0xcc
			 0x5c95bcfa, /// 0xd0
			 0x698137d6, /// 0xd4
			 0xb1052f7e, /// 0xd8
			 0xf46f6eaf, /// 0xdc
			 0x6e47f581, /// 0xe0
			 0x12dba0f8, /// 0xe4
			 0xe1b2aa6c, /// 0xe8
			 0xcbe1490a, /// 0xec
			 0x3ad8c951, /// 0xf0
			 0x3a2b20ba, /// 0xf4
			 0x5c675069, /// 0xf8
			 0xffda3954, /// 0xfc
			 0xe00cd1cd, /// 0x100
			 0x4577685e, /// 0x104
			 0x2e697069, /// 0x108
			 0xdf70d273, /// 0x10c
			 0xbd9b1664, /// 0x110
			 0xe9323ee8, /// 0x114
			 0x478d4a75, /// 0x118
			 0x31f8dc85, /// 0x11c
			 0x24626ea3, /// 0x120
			 0xae97d7c9, /// 0x124
			 0xdc57df84, /// 0x128
			 0x32713bc7, /// 0x12c
			 0x104323c1, /// 0x130
			 0x4e563770, /// 0x134
			 0xcd725fa2, /// 0x138
			 0xb74cebbf, /// 0x13c
			 0xdccf406b, /// 0x140
			 0x8be2b4d3, /// 0x144
			 0x9b3975d8, /// 0x148
			 0x59677fcb, /// 0x14c
			 0xbc09efc6, /// 0x150
			 0x219629d0, /// 0x154
			 0x31911323, /// 0x158
			 0xb0052ff7, /// 0x15c
			 0x25e64362, /// 0x160
			 0x193aea47, /// 0x164
			 0x12dbb2e8, /// 0x168
			 0x314faa76, /// 0x16c
			 0x29effcd4, /// 0x170
			 0xcdeca6a0, /// 0x174
			 0x4d520589, /// 0x178
			 0x273fbf30, /// 0x17c
			 0x887e7973, /// 0x180
			 0xe0b387fa, /// 0x184
			 0xf670a45c, /// 0x188
			 0x1b841061, /// 0x18c
			 0x035d5dcf, /// 0x190
			 0x93f3305d, /// 0x194
			 0xc0157d92, /// 0x198
			 0xe55234a2, /// 0x19c
			 0xa561eaf9, /// 0x1a0
			 0xf62bb603, /// 0x1a4
			 0xa83cd6d0, /// 0x1a8
			 0x28b2e5f3, /// 0x1ac
			 0xdb0e508d, /// 0x1b0
			 0x8bb5b117, /// 0x1b4
			 0x4f7e4e0b, /// 0x1b8
			 0xdb6e3d95, /// 0x1bc
			 0x57a9009e, /// 0x1c0
			 0xe13c5843, /// 0x1c4
			 0x403d9557, /// 0x1c8
			 0xeef5e4d0, /// 0x1cc
			 0xda284c13, /// 0x1d0
			 0x4c7e6590, /// 0x1d4
			 0x7106755b, /// 0x1d8
			 0x1dcf7f58, /// 0x1dc
			 0xfc600477, /// 0x1e0
			 0xedbcd15a, /// 0x1e4
			 0xa5e44b6a, /// 0x1e8
			 0x81385feb, /// 0x1ec
			 0x97c3cfa2, /// 0x1f0
			 0xd6ec36cf, /// 0x1f4
			 0x9a4d8d8a, /// 0x1f8
			 0x16772454, /// 0x1fc
			 0x93216639, /// 0x200
			 0xb2e29a0f, /// 0x204
			 0xaaa1ccbd, /// 0x208
			 0xed1e8541, /// 0x20c
			 0xdf42e611, /// 0x210
			 0xb99d655c, /// 0x214
			 0x9e9b7bf4, /// 0x218
			 0x3b4d6a08, /// 0x21c
			 0xac93798f, /// 0x220
			 0xdde81f03, /// 0x224
			 0x4e472ed5, /// 0x228
			 0x673bcd60, /// 0x22c
			 0xde936121, /// 0x230
			 0x398e9e87, /// 0x234
			 0x6b13330e, /// 0x238
			 0x28848745, /// 0x23c
			 0xd9e88c41, /// 0x240
			 0x0701db50, /// 0x244
			 0x9d2c8fc0, /// 0x248
			 0x87c31333, /// 0x24c
			 0xf4c9f238, /// 0x250
			 0xebb34874, /// 0x254
			 0x69daa4a0, /// 0x258
			 0x9434dc59, /// 0x25c
			 0xeeb4e01b, /// 0x260
			 0xf3bf52ce, /// 0x264
			 0x4201924a, /// 0x268
			 0x438d59fa, /// 0x26c
			 0x9ea461bb, /// 0x270
			 0xd2d4c6d1, /// 0x274
			 0xe2b3738f, /// 0x278
			 0x4742f7e5, /// 0x27c
			 0x06937acd, /// 0x280
			 0xeed42fcd, /// 0x284
			 0xe0f0b1cb, /// 0x288
			 0x38923ce4, /// 0x28c
			 0x7477c192, /// 0x290
			 0x28e24c61, /// 0x294
			 0x444ab2fe, /// 0x298
			 0x5f07b7fe, /// 0x29c
			 0xeb4a845a, /// 0x2a0
			 0xdbfa6b92, /// 0x2a4
			 0x82f9730d, /// 0x2a8
			 0xa43370a8, /// 0x2ac
			 0x87fc0e74, /// 0x2b0
			 0xbc4d95d0, /// 0x2b4
			 0xea29827d, /// 0x2b8
			 0x79fd8d53, /// 0x2bc
			 0x8c580b34, /// 0x2c0
			 0x9d1c130a, /// 0x2c4
			 0xbcdca89a, /// 0x2c8
			 0xb1f21692, /// 0x2cc
			 0x88c933e8, /// 0x2d0
			 0x85e2692a, /// 0x2d4
			 0x2ef96c5a, /// 0x2d8
			 0x7ee074ee, /// 0x2dc
			 0x54c6705f, /// 0x2e0
			 0x5a5334d0, /// 0x2e4
			 0x5ac6cc61, /// 0x2e8
			 0x117d0ced, /// 0x2ec
			 0x59bb1b1f, /// 0x2f0
			 0xf6890f6a, /// 0x2f4
			 0xecf86f92, /// 0x2f8
			 0xf33f21ba, /// 0x2fc
			 0x3f05298d, /// 0x300
			 0xda91663c, /// 0x304
			 0x6a29f311, /// 0x308
			 0x5a735d6e, /// 0x30c
			 0x16ffee7f, /// 0x310
			 0x92d3eb5c, /// 0x314
			 0x43432e20, /// 0x318
			 0x3c8374f3, /// 0x31c
			 0xcf21d88e, /// 0x320
			 0x80a13fa4, /// 0x324
			 0x9ec92db7, /// 0x328
			 0x23dc5014, /// 0x32c
			 0x3d89aa52, /// 0x330
			 0x5010af0e, /// 0x334
			 0xb94247e9, /// 0x338
			 0xe69b8560, /// 0x33c
			 0x7d25883b, /// 0x340
			 0x52a52f58, /// 0x344
			 0x9e30b134, /// 0x348
			 0x1d6108cc, /// 0x34c
			 0x62578337, /// 0x350
			 0x91b78e46, /// 0x354
			 0xbcc2c019, /// 0x358
			 0x315cf920, /// 0x35c
			 0xe14a4b48, /// 0x360
			 0x57746d5f, /// 0x364
			 0x098e7878, /// 0x368
			 0xa5f5b97f, /// 0x36c
			 0xdfe7202f, /// 0x370
			 0x1b78eff1, /// 0x374
			 0xd84115dd, /// 0x378
			 0x21732352, /// 0x37c
			 0xb4a4cec7, /// 0x380
			 0xcdc2f7a9, /// 0x384
			 0xd8f9cef3, /// 0x388
			 0x9f223732, /// 0x38c
			 0x23179e69, /// 0x390
			 0xd6a98053, /// 0x394
			 0x7f3e117d, /// 0x398
			 0x5c7da939, /// 0x39c
			 0x0a891f74, /// 0x3a0
			 0xdabd7ec8, /// 0x3a4
			 0xfa0d225e, /// 0x3a8
			 0xa409374a, /// 0x3ac
			 0xa0e2540d, /// 0x3b0
			 0x78a1e946, /// 0x3b4
			 0xa6a6258a, /// 0x3b8
			 0xf64201e7, /// 0x3bc
			 0xb1f4cbe2, /// 0x3c0
			 0x795e997f, /// 0x3c4
			 0x4b8e5d25, /// 0x3c8
			 0x8f8bc81c, /// 0x3cc
			 0x65c1d2a1, /// 0x3d0
			 0x2d39f502, /// 0x3d4
			 0x4bcc73f7, /// 0x3d8
			 0x8c32e885, /// 0x3dc
			 0x26e95fbd, /// 0x3e0
			 0x1eda255c, /// 0x3e4
			 0x30d2b560, /// 0x3e8
			 0x76bad739, /// 0x3ec
			 0x98c9ecba, /// 0x3f0
			 0x80026f90, /// 0x3f4
			 0xe56cbe7e, /// 0x3f8
			 0xab3fa762, /// 0x3fc
			 0xe675197f, /// 0x400
			 0x9a1a64fb, /// 0x404
			 0xf5f1b59f, /// 0x408
			 0x863a8138, /// 0x40c
			 0x09afda44, /// 0x410
			 0x1eee18ee, /// 0x414
			 0xcf4e260c, /// 0x418
			 0x84b0c824, /// 0x41c
			 0xf262b003, /// 0x420
			 0x39483567, /// 0x424
			 0x84c2fd16, /// 0x428
			 0x0bc3618a, /// 0x42c
			 0xcca14a5e, /// 0x430
			 0xd4cf93b8, /// 0x434
			 0x77ddbe1a, /// 0x438
			 0x5a518aac, /// 0x43c
			 0xef187add, /// 0x440
			 0x338dd8f0, /// 0x444
			 0x474335d7, /// 0x448
			 0xf1d6bb66, /// 0x44c
			 0x4bee2390, /// 0x450
			 0xd74c5b59, /// 0x454
			 0xdff52c5d, /// 0x458
			 0xd71f6459, /// 0x45c
			 0xc1216ac4, /// 0x460
			 0x9a5279e3, /// 0x464
			 0x80d7b72c, /// 0x468
			 0x2dac5b95, /// 0x46c
			 0xfe6ad224, /// 0x470
			 0x01c10332, /// 0x474
			 0xa6d9d986, /// 0x478
			 0xbbbc8aba, /// 0x47c
			 0x566521f8, /// 0x480
			 0xdd060932, /// 0x484
			 0xb06b0e24, /// 0x488
			 0x70b40192, /// 0x48c
			 0xf9f77d9a, /// 0x490
			 0xfe795cd4, /// 0x494
			 0xbd14bf3f, /// 0x498
			 0x8829f13e, /// 0x49c
			 0xb4659ee3, /// 0x4a0
			 0xb4590706, /// 0x4a4
			 0x5f0878cd, /// 0x4a8
			 0x3fc71d93, /// 0x4ac
			 0x33491dfc, /// 0x4b0
			 0x26c49ae6, /// 0x4b4
			 0x01ca621b, /// 0x4b8
			 0x8253cf32, /// 0x4bc
			 0x4431512a, /// 0x4c0
			 0xf73c64bd, /// 0x4c4
			 0xfcb2b7bb, /// 0x4c8
			 0xb13f4f45, /// 0x4cc
			 0x6b5f9052, /// 0x4d0
			 0x3bc528e0, /// 0x4d4
			 0x5aa39dea, /// 0x4d8
			 0x12228d3a, /// 0x4dc
			 0x6eace95f, /// 0x4e0
			 0xe8004c3d, /// 0x4e4
			 0xeda20e19, /// 0x4e8
			 0x1a0e50ed, /// 0x4ec
			 0xdcfd7e8e, /// 0x4f0
			 0xa2c9ce81, /// 0x4f4
			 0xf168abf5, /// 0x4f8
			 0xf6e6965d, /// 0x4fc
			 0xfef45a77, /// 0x500
			 0x913942d7, /// 0x504
			 0xb010a82a, /// 0x508
			 0x1f41ba82, /// 0x50c
			 0x10741a0c, /// 0x510
			 0x70bc4132, /// 0x514
			 0x09992cbd, /// 0x518
			 0x17bf3131, /// 0x51c
			 0x4bb5f004, /// 0x520
			 0x41f007ac, /// 0x524
			 0xb9a433f0, /// 0x528
			 0xf605c02a, /// 0x52c
			 0xaf04774e, /// 0x530
			 0x3f2b4c33, /// 0x534
			 0xd7416ac9, /// 0x538
			 0x54b2fdc0, /// 0x53c
			 0xf121e28e, /// 0x540
			 0xf7825d5f, /// 0x544
			 0x4b2a470b, /// 0x548
			 0x4b28a15a, /// 0x54c
			 0x33a8321b, /// 0x550
			 0xa04cdf02, /// 0x554
			 0x7df27c61, /// 0x558
			 0x68c05684, /// 0x55c
			 0xdec2bb6e, /// 0x560
			 0x34cd298b, /// 0x564
			 0x6fc07e5e, /// 0x568
			 0xa91b7cf4, /// 0x56c
			 0x3d57deba, /// 0x570
			 0xd3c65714, /// 0x574
			 0x7bd6488a, /// 0x578
			 0x470da704, /// 0x57c
			 0x87fe56c1, /// 0x580
			 0x72a8cc7c, /// 0x584
			 0x43210d82, /// 0x588
			 0x1ee6f727, /// 0x58c
			 0xb52c5a19, /// 0x590
			 0x1cf0f956, /// 0x594
			 0x283d90ff, /// 0x598
			 0x7c180d29, /// 0x59c
			 0xed1647ae, /// 0x5a0
			 0xb07f3fc3, /// 0x5a4
			 0x6b4c9da6, /// 0x5a8
			 0x02cbe9de, /// 0x5ac
			 0xa30f50ba, /// 0x5b0
			 0xbabeef72, /// 0x5b4
			 0xd85680ed, /// 0x5b8
			 0x82de5168, /// 0x5bc
			 0xeb80fffd, /// 0x5c0
			 0x768133b6, /// 0x5c4
			 0x778d503d, /// 0x5c8
			 0x4331fd9b, /// 0x5cc
			 0x56507913, /// 0x5d0
			 0x36eb0b56, /// 0x5d4
			 0xeb1d957f, /// 0x5d8
			 0x3cea60cc, /// 0x5dc
			 0xa50166da, /// 0x5e0
			 0x37f4b54e, /// 0x5e4
			 0xaf8a5a19, /// 0x5e8
			 0x444e18e2, /// 0x5ec
			 0x9684bb18, /// 0x5f0
			 0xb9e0683f, /// 0x5f4
			 0xc55407a2, /// 0x5f8
			 0xfce6c53e, /// 0x5fc
			 0xf26c47c4, /// 0x600
			 0xa0b54c1d, /// 0x604
			 0x1401f1cb, /// 0x608
			 0xed3aa519, /// 0x60c
			 0x29b05f6e, /// 0x610
			 0x883ba883, /// 0x614
			 0xd8f19a27, /// 0x618
			 0x176f1fcc, /// 0x61c
			 0xaaa0a394, /// 0x620
			 0x1bd03af8, /// 0x624
			 0xb1d8b9ec, /// 0x628
			 0xe8e11b2f, /// 0x62c
			 0x2efaa2c3, /// 0x630
			 0x51f290b8, /// 0x634
			 0x55921ba2, /// 0x638
			 0x2d0ff0d5, /// 0x63c
			 0x55acc318, /// 0x640
			 0xc6963207, /// 0x644
			 0xfd22fff1, /// 0x648
			 0xb7d7c3ac, /// 0x64c
			 0xb0f93564, /// 0x650
			 0x418c0266, /// 0x654
			 0xe0c92c90, /// 0x658
			 0x2ff61582, /// 0x65c
			 0x3bd2dd5d, /// 0x660
			 0x456d6e66, /// 0x664
			 0x1a4701c0, /// 0x668
			 0x39248cf1, /// 0x66c
			 0xa5794a16, /// 0x670
			 0x6fd59d84, /// 0x674
			 0x63769af7, /// 0x678
			 0x368ae1b2, /// 0x67c
			 0xd782e940, /// 0x680
			 0xfd7b1e6f, /// 0x684
			 0x870af0e5, /// 0x688
			 0x9729e8f2, /// 0x68c
			 0x47f25b25, /// 0x690
			 0xfd7ea9ee, /// 0x694
			 0xa34331f5, /// 0x698
			 0x981a4395, /// 0x69c
			 0xc38f0612, /// 0x6a0
			 0x770c7fa3, /// 0x6a4
			 0x5deafc00, /// 0x6a8
			 0xb6a99887, /// 0x6ac
			 0xf73c461e, /// 0x6b0
			 0xa04e8f60, /// 0x6b4
			 0x81e5e38c, /// 0x6b8
			 0x79dd8b72, /// 0x6bc
			 0x04ad310d, /// 0x6c0
			 0x6b810b11, /// 0x6c4
			 0xb816ca37, /// 0x6c8
			 0x03835731, /// 0x6cc
			 0x406db36d, /// 0x6d0
			 0xf660f80a, /// 0x6d4
			 0x140bffa0, /// 0x6d8
			 0x091df216, /// 0x6dc
			 0x2813f89d, /// 0x6e0
			 0x8020a2a5, /// 0x6e4
			 0x57d5e779, /// 0x6e8
			 0x79263539, /// 0x6ec
			 0xe29d4fe1, /// 0x6f0
			 0xe396bed8, /// 0x6f4
			 0xfaaeee5e, /// 0x6f8
			 0x7c8d8dcc, /// 0x6fc
			 0xb9cefc4a, /// 0x700
			 0xca532e19, /// 0x704
			 0x149f143b, /// 0x708
			 0xa97f2b4f, /// 0x70c
			 0xfbcceedc, /// 0x710
			 0xdd83446c, /// 0x714
			 0x806c1474, /// 0x718
			 0xca3744b3, /// 0x71c
			 0x698ea3ca, /// 0x720
			 0x237513fb, /// 0x724
			 0xc61b58f2, /// 0x728
			 0x5f73ea23, /// 0x72c
			 0x1450a291, /// 0x730
			 0xfec8cdbc, /// 0x734
			 0x23bb308e, /// 0x738
			 0x383bd2d2, /// 0x73c
			 0xebe55ba8, /// 0x740
			 0x2332f8e6, /// 0x744
			 0xc98d966e, /// 0x748
			 0xf2fe9568, /// 0x74c
			 0x9e0cc3a7, /// 0x750
			 0xe8a55547, /// 0x754
			 0x0eb8a14d, /// 0x758
			 0x8e645e23, /// 0x75c
			 0xc05bb199, /// 0x760
			 0x8e45e76a, /// 0x764
			 0x4d90bf73, /// 0x768
			 0xfd78928e, /// 0x76c
			 0x4d6be861, /// 0x770
			 0xf8b0d09a, /// 0x774
			 0xb218333a, /// 0x778
			 0xb8a9681c, /// 0x77c
			 0x90416ee9, /// 0x780
			 0x8bd5ba4a, /// 0x784
			 0xb30c3293, /// 0x788
			 0x446fcfd4, /// 0x78c
			 0xc687b628, /// 0x790
			 0x5c19c3bd, /// 0x794
			 0x48cda851, /// 0x798
			 0x5776de7a, /// 0x79c
			 0xb1e8de6d, /// 0x7a0
			 0x03ee2c0b, /// 0x7a4
			 0x04a6f328, /// 0x7a8
			 0x6d7df21e, /// 0x7ac
			 0xd1ab0757, /// 0x7b0
			 0x1c7ea36a, /// 0x7b4
			 0xfd432a74, /// 0x7b8
			 0x3d4d4e80, /// 0x7bc
			 0x93ebe48e, /// 0x7c0
			 0x7a9fb115, /// 0x7c4
			 0xbfc8569c, /// 0x7c8
			 0xc566f10e, /// 0x7cc
			 0xf4c1c0e0, /// 0x7d0
			 0xfa01d754, /// 0x7d4
			 0xfea9fa16, /// 0x7d8
			 0x3708282c, /// 0x7dc
			 0x5f6aa261, /// 0x7e0
			 0xaacf2c8a, /// 0x7e4
			 0xde690e27, /// 0x7e8
			 0x1c067ddc, /// 0x7ec
			 0x8b1b7de9, /// 0x7f0
			 0x1fba4291, /// 0x7f4
			 0x4e3ea846, /// 0x7f8
			 0x0b7e56d5, /// 0x7fc
			 0x5b176e5c, /// 0x800
			 0x8f931708, /// 0x804
			 0xf2ec186e, /// 0x808
			 0xe007684e, /// 0x80c
			 0x9819ff7c, /// 0x810
			 0x101ea11a, /// 0x814
			 0x5600f51d, /// 0x818
			 0x99ed3873, /// 0x81c
			 0xb3b93a7c, /// 0x820
			 0xba52637d, /// 0x824
			 0xb8bd911b, /// 0x828
			 0x4b3bfe4e, /// 0x82c
			 0x59cf6c41, /// 0x830
			 0x3e5c1778, /// 0x834
			 0xc243e969, /// 0x838
			 0x8559a918, /// 0x83c
			 0xf51d22ee, /// 0x840
			 0x6f3f6af4, /// 0x844
			 0x65f2efd9, /// 0x848
			 0x9e8da745, /// 0x84c
			 0xa1f3a170, /// 0x850
			 0x5fb869e3, /// 0x854
			 0xe1156883, /// 0x858
			 0xe7b898d8, /// 0x85c
			 0x227031c4, /// 0x860
			 0xbad0c2d4, /// 0x864
			 0xcd011da4, /// 0x868
			 0x893b1bcc, /// 0x86c
			 0x1c56d52f, /// 0x870
			 0x0d7b9ece, /// 0x874
			 0xcc993c8b, /// 0x878
			 0x8999b785, /// 0x87c
			 0xc6ed80b8, /// 0x880
			 0x91961c7c, /// 0x884
			 0x38966615, /// 0x888
			 0x40941c30, /// 0x88c
			 0xc79997a1, /// 0x890
			 0x74e66a38, /// 0x894
			 0x762d8fc8, /// 0x898
			 0x12e7160a, /// 0x89c
			 0x543bea04, /// 0x8a0
			 0xad049ea7, /// 0x8a4
			 0xb6a448a1, /// 0x8a8
			 0x1aeb91e4, /// 0x8ac
			 0x75c5d373, /// 0x8b0
			 0x9c6ed1bb, /// 0x8b4
			 0xb7eb93ef, /// 0x8b8
			 0x14bfb440, /// 0x8bc
			 0x87bc5653, /// 0x8c0
			 0x9325e2d8, /// 0x8c4
			 0x47b586d9, /// 0x8c8
			 0xb04d9959, /// 0x8cc
			 0x9e7f8a0d, /// 0x8d0
			 0x17598e70, /// 0x8d4
			 0x5322d2aa, /// 0x8d8
			 0xa4034ead, /// 0x8dc
			 0x5c54b007, /// 0x8e0
			 0x953ab245, /// 0x8e4
			 0x981d4982, /// 0x8e8
			 0xffd98556, /// 0x8ec
			 0x070182cc, /// 0x8f0
			 0x23eca4a5, /// 0x8f4
			 0xbeb35296, /// 0x8f8
			 0x97720467, /// 0x8fc
			 0x070cc137, /// 0x900
			 0x9bf1562b, /// 0x904
			 0x1a964163, /// 0x908
			 0x8f3f975c, /// 0x90c
			 0x0fc1415e, /// 0x910
			 0xd90e74db, /// 0x914
			 0xfea0caf7, /// 0x918
			 0x2abdd323, /// 0x91c
			 0x985520c5, /// 0x920
			 0x85700f75, /// 0x924
			 0xa6d7763b, /// 0x928
			 0x37702b5d, /// 0x92c
			 0x5659c2ee, /// 0x930
			 0xb683f569, /// 0x934
			 0x81b75d94, /// 0x938
			 0x7eb79946, /// 0x93c
			 0x29b023f7, /// 0x940
			 0x1eb38345, /// 0x944
			 0xbe846a1e, /// 0x948
			 0x9c9f9973, /// 0x94c
			 0x07479ed3, /// 0x950
			 0xa1137259, /// 0x954
			 0x7e5de4a6, /// 0x958
			 0x06bde6ba, /// 0x95c
			 0x6073211e, /// 0x960
			 0xf5ce0c12, /// 0x964
			 0xfe38f714, /// 0x968
			 0x60e0b24d, /// 0x96c
			 0x0ad7296c, /// 0x970
			 0x6b38d2e1, /// 0x974
			 0xfd71bfcd, /// 0x978
			 0x49c135e0, /// 0x97c
			 0x25122f71, /// 0x980
			 0xb08e317b, /// 0x984
			 0x0cf3c857, /// 0x988
			 0x82c43408, /// 0x98c
			 0xd339553f, /// 0x990
			 0x5ba7b404, /// 0x994
			 0xf1ac2bc5, /// 0x998
			 0x5e058747, /// 0x99c
			 0xeed0c58a, /// 0x9a0
			 0xdd70c351, /// 0x9a4
			 0xf8b91e33, /// 0x9a8
			 0x5728b4bf, /// 0x9ac
			 0x7c246030, /// 0x9b0
			 0xf0a9e695, /// 0x9b4
			 0x68655865, /// 0x9b8
			 0x73abb74d, /// 0x9bc
			 0x1f413c4e, /// 0x9c0
			 0x4fc7131a, /// 0x9c4
			 0x0594a226, /// 0x9c8
			 0xd4bc8fd0, /// 0x9cc
			 0x1728fba8, /// 0x9d0
			 0xa0db04c7, /// 0x9d4
			 0xffb73097, /// 0x9d8
			 0xfc48c976, /// 0x9dc
			 0xbcdaeb39, /// 0x9e0
			 0x7ab06c59, /// 0x9e4
			 0xfa5b8606, /// 0x9e8
			 0x449ca0a0, /// 0x9ec
			 0xb2628ca9, /// 0x9f0
			 0xfe7e528a, /// 0x9f4
			 0xcf1d6b48, /// 0x9f8
			 0x9440f339, /// 0x9fc
			 0xeaaf4d21, /// 0xa00
			 0xe497c0e4, /// 0xa04
			 0xc80d410b, /// 0xa08
			 0x808d688b, /// 0xa0c
			 0x91adb497, /// 0xa10
			 0x4fd12728, /// 0xa14
			 0xce7ce9ad, /// 0xa18
			 0x3795ccb5, /// 0xa1c
			 0xaa79ba79, /// 0xa20
			 0x28c2bd94, /// 0xa24
			 0xcc32653d, /// 0xa28
			 0xac334a32, /// 0xa2c
			 0x0392b133, /// 0xa30
			 0x052061e5, /// 0xa34
			 0x9a3f8c57, /// 0xa38
			 0xd316de37, /// 0xa3c
			 0xc8cf566d, /// 0xa40
			 0x39406bdc, /// 0xa44
			 0xec95179e, /// 0xa48
			 0xc352625b, /// 0xa4c
			 0x8a85de67, /// 0xa50
			 0xeac0a14d, /// 0xa54
			 0xd6aba54a, /// 0xa58
			 0x22c5b1b1, /// 0xa5c
			 0x79ef3143, /// 0xa60
			 0x419bdac8, /// 0xa64
			 0x9ba4e3bc, /// 0xa68
			 0x8d225dc0, /// 0xa6c
			 0xc1c7faa6, /// 0xa70
			 0xfa7a288c, /// 0xa74
			 0xb30aba51, /// 0xa78
			 0x9f421605, /// 0xa7c
			 0x319e0f3f, /// 0xa80
			 0x7b46c3f6, /// 0xa84
			 0x82a7fee4, /// 0xa88
			 0x65af2513, /// 0xa8c
			 0xc4ee8c78, /// 0xa90
			 0xb890db11, /// 0xa94
			 0x1e935781, /// 0xa98
			 0x65f5e808, /// 0xa9c
			 0x362e157b, /// 0xaa0
			 0xfd4efc16, /// 0xaa4
			 0xa2d5cc3d, /// 0xaa8
			 0x66b7e179, /// 0xaac
			 0xac3a9b7e, /// 0xab0
			 0x05706d1c, /// 0xab4
			 0x756f1f2c, /// 0xab8
			 0x31aa1417, /// 0xabc
			 0x2e74a498, /// 0xac0
			 0x720e96bf, /// 0xac4
			 0xf2ff9f8b, /// 0xac8
			 0x51076cbf, /// 0xacc
			 0xbe79aac2, /// 0xad0
			 0xbef9d60a, /// 0xad4
			 0xe68f5820, /// 0xad8
			 0x9f3fcff8, /// 0xadc
			 0x69812433, /// 0xae0
			 0x628b5435, /// 0xae4
			 0xdb3b4992, /// 0xae8
			 0x07d0e7f8, /// 0xaec
			 0x3a3c8413, /// 0xaf0
			 0xbac34887, /// 0xaf4
			 0x6a656f83, /// 0xaf8
			 0xe0eceae1, /// 0xafc
			 0x1b06d976, /// 0xb00
			 0x03c9de81, /// 0xb04
			 0xc682ef17, /// 0xb08
			 0x2ab663de, /// 0xb0c
			 0x8171434e, /// 0xb10
			 0x2092d679, /// 0xb14
			 0xf95e8a15, /// 0xb18
			 0xc0b35cf0, /// 0xb1c
			 0x987711df, /// 0xb20
			 0xcb1da4b1, /// 0xb24
			 0xac457ba1, /// 0xb28
			 0xcdc23beb, /// 0xb2c
			 0xda4e75f2, /// 0xb30
			 0x3142ce91, /// 0xb34
			 0xdb701af9, /// 0xb38
			 0xfece094f, /// 0xb3c
			 0x5540e651, /// 0xb40
			 0xc5b86ba1, /// 0xb44
			 0xc7da821f, /// 0xb48
			 0xc2664f95, /// 0xb4c
			 0x4442739d, /// 0xb50
			 0x615f66c7, /// 0xb54
			 0x25ff0cb1, /// 0xb58
			 0x9757a5a8, /// 0xb5c
			 0x8efeb637, /// 0xb60
			 0x3997eed2, /// 0xb64
			 0x48f95866, /// 0xb68
			 0xd8474888, /// 0xb6c
			 0x1289f04b, /// 0xb70
			 0x0aa6c9b2, /// 0xb74
			 0x299a4109, /// 0xb78
			 0xf8e538d6, /// 0xb7c
			 0x7dfce90b, /// 0xb80
			 0xa7985e53, /// 0xb84
			 0xcb8a6053, /// 0xb88
			 0xab9475a0, /// 0xb8c
			 0xd977c213, /// 0xb90
			 0xc992ccd4, /// 0xb94
			 0x859a3df4, /// 0xb98
			 0x91298d70, /// 0xb9c
			 0x0ceef236, /// 0xba0
			 0xb40d82e1, /// 0xba4
			 0x91ace80f, /// 0xba8
			 0xb3aaa54a, /// 0xbac
			 0x04e76313, /// 0xbb0
			 0x2ace2590, /// 0xbb4
			 0x78e8fb0f, /// 0xbb8
			 0x8304e9ad, /// 0xbbc
			 0x913246dd, /// 0xbc0
			 0x07b24c62, /// 0xbc4
			 0x0389497a, /// 0xbc8
			 0x6ef6a500, /// 0xbcc
			 0xb10fdae6, /// 0xbd0
			 0x5cfe9a63, /// 0xbd4
			 0xe64cfb5f, /// 0xbd8
			 0x9f42986f, /// 0xbdc
			 0xf79d9971, /// 0xbe0
			 0xb8d9dcb1, /// 0xbe4
			 0xab360386, /// 0xbe8
			 0xd6621987, /// 0xbec
			 0x3a79794e, /// 0xbf0
			 0x8908b386, /// 0xbf4
			 0xab555401, /// 0xbf8
			 0x724159ee, /// 0xbfc
			 0xbaf79835, /// 0xc00
			 0x914401cf, /// 0xc04
			 0x3f42b1d4, /// 0xc08
			 0xb75d2837, /// 0xc0c
			 0x15404756, /// 0xc10
			 0x4c37fca2, /// 0xc14
			 0x2a52e0c7, /// 0xc18
			 0x815e5df5, /// 0xc1c
			 0xe790367d, /// 0xc20
			 0xec911894, /// 0xc24
			 0x32b1b4ba, /// 0xc28
			 0xfb946c50, /// 0xc2c
			 0xf6290e1a, /// 0xc30
			 0x3dc13ba1, /// 0xc34
			 0xecefc71a, /// 0xc38
			 0x2eddbd24, /// 0xc3c
			 0xba0e0c46, /// 0xc40
			 0xa7de880d, /// 0xc44
			 0xcc4db794, /// 0xc48
			 0x5555bae1, /// 0xc4c
			 0x7f9d88c7, /// 0xc50
			 0xdc37eb2a, /// 0xc54
			 0x3ea704d0, /// 0xc58
			 0xc50c3ba2, /// 0xc5c
			 0xb1c1fcb7, /// 0xc60
			 0xf8075fa8, /// 0xc64
			 0xf7a710e1, /// 0xc68
			 0x71135d95, /// 0xc6c
			 0x579803ac, /// 0xc70
			 0x444c412a, /// 0xc74
			 0xd07f4321, /// 0xc78
			 0xf4968069, /// 0xc7c
			 0xab2eb519, /// 0xc80
			 0x0dc659c9, /// 0xc84
			 0x7c0a5bb5, /// 0xc88
			 0xd0649b51, /// 0xc8c
			 0xa3a6b5c5, /// 0xc90
			 0x2044b483, /// 0xc94
			 0xd92a7f0d, /// 0xc98
			 0x318f7c5c, /// 0xc9c
			 0x01368413, /// 0xca0
			 0x1ceaf2e8, /// 0xca4
			 0xfa5d4077, /// 0xca8
			 0xc1c34e37, /// 0xcac
			 0x23b37ea8, /// 0xcb0
			 0x2ef5019b, /// 0xcb4
			 0x78c44c77, /// 0xcb8
			 0x67ac1ce4, /// 0xcbc
			 0xb79e1457, /// 0xcc0
			 0x5d7a7380, /// 0xcc4
			 0xb2f3ab6c, /// 0xcc8
			 0xd6d9988d, /// 0xccc
			 0x774982f6, /// 0xcd0
			 0xb0595c0b, /// 0xcd4
			 0xaa5c66e6, /// 0xcd8
			 0x0630fa91, /// 0xcdc
			 0x23f2a1bd, /// 0xce0
			 0x2c583a6c, /// 0xce4
			 0xf347130e, /// 0xce8
			 0xa33e9552, /// 0xcec
			 0xf8df3591, /// 0xcf0
			 0x89777489, /// 0xcf4
			 0x79ce42ac, /// 0xcf8
			 0xb541deb6, /// 0xcfc
			 0x79c9b3b6, /// 0xd00
			 0xeaf4cb4a, /// 0xd04
			 0xe3e34370, /// 0xd08
			 0x7fa4bdf8, /// 0xd0c
			 0x8602e9e1, /// 0xd10
			 0xe90e64ed, /// 0xd14
			 0x67d5a817, /// 0xd18
			 0xb28f6968, /// 0xd1c
			 0x76f1400b, /// 0xd20
			 0x1d0a0e0f, /// 0xd24
			 0xd9be98cb, /// 0xd28
			 0x299bf471, /// 0xd2c
			 0x16417c93, /// 0xd30
			 0xdbeb248c, /// 0xd34
			 0xeb995d19, /// 0xd38
			 0xbb713227, /// 0xd3c
			 0x6a96bb3e, /// 0xd40
			 0x316988c6, /// 0xd44
			 0x11651fad, /// 0xd48
			 0xe8604266, /// 0xd4c
			 0x83e6ce70, /// 0xd50
			 0xf1e5ea79, /// 0xd54
			 0xf2eb9fa1, /// 0xd58
			 0xc8f6edf5, /// 0xd5c
			 0x21bc4fb7, /// 0xd60
			 0x3e469554, /// 0xd64
			 0x14a953ee, /// 0xd68
			 0x1a1ca006, /// 0xd6c
			 0xac153cdd, /// 0xd70
			 0x127e3ca9, /// 0xd74
			 0x87faa4f9, /// 0xd78
			 0xfb325216, /// 0xd7c
			 0xcb5ecfe9, /// 0xd80
			 0x51e2aada, /// 0xd84
			 0x2486894c, /// 0xd88
			 0xa8b2c8ad, /// 0xd8c
			 0x947ffbc0, /// 0xd90
			 0xa54bf314, /// 0xd94
			 0x79d106f8, /// 0xd98
			 0x2dbaf441, /// 0xd9c
			 0xb82d5c45, /// 0xda0
			 0x4fc0d89e, /// 0xda4
			 0x81d190ae, /// 0xda8
			 0x279f83d8, /// 0xdac
			 0xb2af8b08, /// 0xdb0
			 0xd6a80695, /// 0xdb4
			 0x2d343b10, /// 0xdb8
			 0xc0c77458, /// 0xdbc
			 0xaf42064b, /// 0xdc0
			 0x13692a08, /// 0xdc4
			 0x15754a0a, /// 0xdc8
			 0x4abb1d5f, /// 0xdcc
			 0xda5f3faa, /// 0xdd0
			 0x64a78cea, /// 0xdd4
			 0xb75fca92, /// 0xdd8
			 0xf038135f, /// 0xddc
			 0xc58faf72, /// 0xde0
			 0xbed026aa, /// 0xde4
			 0x31e676ed, /// 0xde8
			 0xf23017d8, /// 0xdec
			 0x653b0fc8, /// 0xdf0
			 0x51662a68, /// 0xdf4
			 0x06fc981f, /// 0xdf8
			 0xa9896dc3, /// 0xdfc
			 0xb8285c69, /// 0xe00
			 0x007de508, /// 0xe04
			 0xfba74562, /// 0xe08
			 0xbe25994a, /// 0xe0c
			 0xc98d0e75, /// 0xe10
			 0x9334fc7c, /// 0xe14
			 0xcd8f390e, /// 0xe18
			 0xb5c1bb33, /// 0xe1c
			 0xff4f49e6, /// 0xe20
			 0x239cf90e, /// 0xe24
			 0xbb35f880, /// 0xe28
			 0xf90f930d, /// 0xe2c
			 0x951531b9, /// 0xe30
			 0x963f283d, /// 0xe34
			 0xd79c763b, /// 0xe38
			 0x81ec0e26, /// 0xe3c
			 0x34b2f353, /// 0xe40
			 0x2bb804ff, /// 0xe44
			 0x71b8b843, /// 0xe48
			 0x9da8f46d, /// 0xe4c
			 0x8b918cc0, /// 0xe50
			 0x8efa92e8, /// 0xe54
			 0x1e005ef5, /// 0xe58
			 0xaf0a14fd, /// 0xe5c
			 0x92e45993, /// 0xe60
			 0x29ca1b5d, /// 0xe64
			 0x890b3281, /// 0xe68
			 0xb2e9cc15, /// 0xe6c
			 0xc2ce54fc, /// 0xe70
			 0x77018934, /// 0xe74
			 0x5b136ecb, /// 0xe78
			 0xbbd5d4cc, /// 0xe7c
			 0xdb68a178, /// 0xe80
			 0x1475b0c2, /// 0xe84
			 0xc38bd250, /// 0xe88
			 0xec45f76c, /// 0xe8c
			 0x6e1f4aa1, /// 0xe90
			 0x0dfe222e, /// 0xe94
			 0xf2c36494, /// 0xe98
			 0xcbe65f06, /// 0xe9c
			 0x5ff66b60, /// 0xea0
			 0xb9e0806f, /// 0xea4
			 0x39ea42d0, /// 0xea8
			 0x64768302, /// 0xeac
			 0x408cbdc7, /// 0xeb0
			 0xa21a35d2, /// 0xeb4
			 0xd819a395, /// 0xeb8
			 0xb2886226, /// 0xebc
			 0x506e271c, /// 0xec0
			 0x24c301c7, /// 0xec4
			 0x0b5c76e9, /// 0xec8
			 0x74f3f80d, /// 0xecc
			 0x36c1857b, /// 0xed0
			 0x56b2ad42, /// 0xed4
			 0x3e65f80d, /// 0xed8
			 0x1ab80b14, /// 0xedc
			 0x04d62b6b, /// 0xee0
			 0x60f0c958, /// 0xee4
			 0xe5f19be6, /// 0xee8
			 0x4a4bc571, /// 0xeec
			 0x4f0b6f2f, /// 0xef0
			 0x5e76d223, /// 0xef4
			 0x521ed04b, /// 0xef8
			 0x48048417, /// 0xefc
			 0x59e0b9eb, /// 0xf00
			 0xb95c28f7, /// 0xf04
			 0x3abb8e2d, /// 0xf08
			 0x0f56692c, /// 0xf0c
			 0x5d5ce03b, /// 0xf10
			 0xe46c16d9, /// 0xf14
			 0xec76adb0, /// 0xf18
			 0x728abd8d, /// 0xf1c
			 0x395ced58, /// 0xf20
			 0x88bda7ff, /// 0xf24
			 0xd52ae9b9, /// 0xf28
			 0xae4a9531, /// 0xf2c
			 0x3c6421d7, /// 0xf30
			 0x480d9292, /// 0xf34
			 0xac15074e, /// 0xf38
			 0x0605b66b, /// 0xf3c
			 0xab2b32f5, /// 0xf40
			 0x8049197a, /// 0xf44
			 0xbd6930fe, /// 0xf48
			 0x436d89a5, /// 0xf4c
			 0xfdf1da66, /// 0xf50
			 0xa2b4e22d, /// 0xf54
			 0x07b7f416, /// 0xf58
			 0x09508fe1, /// 0xf5c
			 0x05c8bea2, /// 0xf60
			 0xc11c2e91, /// 0xf64
			 0x67966971, /// 0xf68
			 0x88e77bf8, /// 0xf6c
			 0x0b9534d8, /// 0xf70
			 0xd0acfdcb, /// 0xf74
			 0xa64c6efc, /// 0xf78
			 0xa2c8fb23, /// 0xf7c
			 0x00fbc5e9, /// 0xf80
			 0xfd3c78d7, /// 0xf84
			 0x8fb6c1d2, /// 0xf88
			 0xba557bb6, /// 0xf8c
			 0x0eab6a9a, /// 0xf90
			 0x152a32b8, /// 0xf94
			 0xb56d5464, /// 0xf98
			 0x31c2b94a, /// 0xf9c
			 0x453a6c44, /// 0xfa0
			 0x205fb078, /// 0xfa4
			 0x94437384, /// 0xfa8
			 0xa31c181a, /// 0xfac
			 0xd3e8115c, /// 0xfb0
			 0x4e8a7bd9, /// 0xfb4
			 0x5e53c973, /// 0xfb8
			 0x115de024, /// 0xfbc
			 0xb93c16aa, /// 0xfc0
			 0x94d1b553, /// 0xfc4
			 0xf9a43a98, /// 0xfc8
			 0x70d3c1dc, /// 0xfcc
			 0x2558e9e0, /// 0xfd0
			 0xa4d4c6c0, /// 0xfd4
			 0x6a93e362, /// 0xfd8
			 0x5860d935, /// 0xfdc
			 0xa5c2f76d, /// 0xfe0
			 0xe60ee5f3, /// 0xfe4
			 0x0658ed4c, /// 0xfe8
			 0xa8d75df2, /// 0xfec
			 0xce6aa06a, /// 0xff0
			 0x346f4fef, /// 0xff4
			 0x79ce72a9, /// 0xff8
			 0x138cedee /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x3f028f5c,                                                  // 0.51                                        /// 00000
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00004
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00008
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0000c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00010
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00014
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00018
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0001c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00020
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00024
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0xff800000,                                                  // -inf                                        /// 0002c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00030
			 0x33333333,                                                  // 4 random values                             /// 00034
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00038
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0003c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00044
			 0x33333333,                                                  // 4 random values                             /// 00048
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0004c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00054
			 0xcb000000,                                                  // -8388608.0                                  /// 00058
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0xff800000,                                                  // -inf                                        /// 00060
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00064
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00068
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00070
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00074
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00078
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0007c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00084
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00090
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0c400000,                                                  // Leading 1s:                                 /// 00098
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 000b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x7fc00001,                                                  // signaling NaN                               /// 000c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ec
			 0x7f800000,                                                  // inf                                         /// 000f0
			 0x55555555,                                                  // 4 random values                             /// 000f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00100
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x80000000,                                                  // -zero                                       /// 00108
			 0xbf028f5c,                                                  // -0.51                                       /// 0010c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00110
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00114
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00118
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0011c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00120
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00124
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00128
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0012c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00130
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00134
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00138
			 0x0c400000,                                                  // Leading 1s:                                 /// 0013c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00140
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00144
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0014c
			 0x7f800000,                                                  // inf                                         /// 00150
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00154
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00158
			 0x80011111,                                                  // -9.7958E-41                                 /// 0015c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0016c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00170
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00174
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00178
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00184
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00188
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00190
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00194
			 0x00000000,                                                  // zero                                        /// 00198
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0019c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001b4
			 0xcb000000,                                                  // -8388608.0                                  /// 001b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x7f800000,                                                  // inf                                         /// 001f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00200
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00208
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00210
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00220
			 0x0c700000,                                                  // Leading 1s:                                 /// 00224
			 0x0c700000,                                                  // Leading 1s:                                 /// 00228
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0022c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00230
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0023c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c600000,                                                  // Leading 1s:                                 /// 00248
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0024c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00250
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00254
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x55555555,                                                  // 4 random values                             /// 0025c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00260
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00264
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00270
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0027c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00280
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00288
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00294
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00298
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x0c780000,                                                  // Leading 1s:                                 /// 002a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f800000,                                                  // inf                                         /// 002bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00300
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00304
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00308
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0030c
			 0xbf028f5c,                                                  // -0.51                                       /// 00310
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00314
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00320
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00324
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00328
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0032c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff800000,                                                  // -inf                                        /// 00334
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00338
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00340
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00348
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00350
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00360
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00370
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0037c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00380
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00384
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00388
			 0x55555555,                                                  // 4 random values                             /// 0038c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x7fc00001,                                                  // signaling NaN                               /// 00394
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00398
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0039c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00400
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00408
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0040c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00410
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00420
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00428
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00430
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00434
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00438
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0043c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00444
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00448
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00454
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00458
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00464
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00468
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0046c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00470
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00474
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0047c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00484
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00488
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00490
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00494
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00498
			 0xcb000000,                                                  // -8388608.0                                  /// 0049c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x7f800000,                                                  // inf                                         /// 004b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x33333333,                                                  // 4 random values                             /// 004c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004e0
			 0xcb000000,                                                  // -8388608.0                                  /// 004e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x33333333,                                                  // 4 random values                             /// 004fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00500
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00504
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0050c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00510
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00514
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00518
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0051c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00520
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00528
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00530
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00534
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00538
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0053c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00540
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00548
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00550
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0055c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00560
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00564
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c780000,                                                  // Leading 1s:                                 /// 0056c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00570
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00574
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00578
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00580
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x3f028f5c,                                                  // 0.51                                        /// 00588
			 0x55555555,                                                  // 4 random values                             /// 0058c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00594
			 0x0c780000,                                                  // Leading 1s:                                 /// 00598
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xff800000,                                                  // -inf                                        /// 005b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7fc00001,                                                  // signaling NaN                               /// 005c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f4
			 0xcb000000,                                                  // -8388608.0                                  /// 005f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00600
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00608
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00610
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00614
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00618
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0061c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00620
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x7fc00001,                                                  // signaling NaN                               /// 0062c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00630
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00634
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xcb000000,                                                  // -8388608.0                                  /// 0063c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00648
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00658
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0065c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0066c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00670
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00678
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0067c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00680
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00684
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00690
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00694
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0069c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b0
			 0xffc00001,                                                  // -signaling NaN                              /// 006b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x3f028f5c,                                                  // 0.51                                        /// 006c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006e0
			 0xbf028f5c,                                                  // -0.51                                       /// 006e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f8
			 0x7fc00001,                                                  // signaling NaN                               /// 006fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00700
			 0x00011111,                                                  // 9.7958E-41                                  /// 00704
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0070c
			 0x00000000,                                                  // zero                                        /// 00710
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00714
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0071c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00728
			 0x00011111,                                                  // 9.7958E-41                                  /// 0072c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00730
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00734
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00738
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00740
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00744
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00748
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0074c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00750
			 0x80000000,                                                  // -zero                                       /// 00754
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00758
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0075c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00760
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00764
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00768
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0076c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00774
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00778
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0077c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0078c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00790
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00794
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00798
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d0
			 0x4b000000,                                                  // 8388608.0                                   /// 007d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00800
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00804
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00808
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0080c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00810
			 0x80011111,                                                  // -9.7958E-41                                 /// 00814
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00818
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00820
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00824
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0082c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00838
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0083c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c400000,                                                  // Leading 1s:                                 /// 00844
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00848
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00850
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00858
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00860
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00868
			 0xcb000000,                                                  // -8388608.0                                  /// 0086c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00870
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00874
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0088c
			 0x33333333,                                                  // 4 random values                             /// 00890
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00898
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x7f800000,                                                  // inf                                         /// 008c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008ec
			 0x4b000000,                                                  // 8388608.0                                   /// 008f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008fc
			 0x00000000,                                                  // zero                                        /// 00900
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0090c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00914
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00918
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0091c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xffc00001,                                                  // -signaling NaN                              /// 00928
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0092c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00930
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00938
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0093c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00940
			 0x0e000003,                                                  // Trailing 1s:                                /// 00944
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00950
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00954
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00958
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00968
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x33333333,                                                  // 4 random values                             /// 00974
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x3f028f5c,                                                  // 0.51                                        /// 0097c
			 0xff800000,                                                  // -inf                                        /// 00980
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0098c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00994
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00998
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0099c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x4b000000,                                                  // 8388608.0                                   /// 009bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x33333333,                                                  // 4 random values                             /// 009cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0xcb000000,                                                  // -8388608.0                                  /// 009f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00acc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00adc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae0
			 0x33333333,                                                  // 4 random values                             /// 00ae4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0xcb000000,                                                  // -8388608.0                                  /// 00af4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000000,                                                  // -zero                                       /// 00b28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x55555555,                                                  // 4 random values                             /// 00b44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b64
			 0xff800000,                                                  // -inf                                        /// 00b68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ba0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ba4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bcc
			 0x7f800000,                                                  // inf                                         /// 00bd0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bec
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bfc
			 0xcb000000,                                                  // -8388608.0                                  /// 00c00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c24
			 0x80000000,                                                  // -zero                                       /// 00c28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c54
			 0x7f800000,                                                  // inf                                         /// 00c58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0xcb000000,                                                  // -8388608.0                                  /// 00c9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ca8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc8
			 0x00000000,                                                  // zero                                        /// 00ccc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cdc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cfc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d40
			 0xff800000,                                                  // -inf                                        /// 00d44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d4c
			 0x7f800000,                                                  // inf                                         /// 00d50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00da0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd0
			 0xffc00001,                                                  // -signaling NaN                              /// 00dd4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00de4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ea4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb0
			 0x33333333,                                                  // 4 random values                             /// 00eb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xff800000,                                                  // -inf                                        /// 00ec0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ecc
			 0x7f800000,                                                  // inf                                         /// 00ed0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ed8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00edc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ee8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ef0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ef8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00efc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f20
			 0x33333333,                                                  // 4 random values                             /// 00f24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fdc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ff8
			 0x0d00fff0                                                  // Set of 1s                                   /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00000
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00004
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x7f800000,                                                  // inf                                         /// 00010
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00014
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00020
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00024
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00028
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00030
			 0xbf028f5c,                                                  // -0.51                                       /// 00034
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00038
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00044
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00048
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x80011111,                                                  // -9.7958E-41                                 /// 00054
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00058
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00064
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00068
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0006c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00070
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0007c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00094
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00098
			 0xffc00001,                                                  // -signaling NaN                              /// 0009c
			 0x55555555,                                                  // 4 random values                             /// 000a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xbf028f5c,                                                  // -0.51                                       /// 000c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00104
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0010c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00114
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00120
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0012c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00130
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00138
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x55555555,                                                  // 4 random values                             /// 00144
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0014c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00154
			 0x7f800000,                                                  // inf                                         /// 00158
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00160
			 0x0c780000,                                                  // Leading 1s:                                 /// 00164
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00168
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0016c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00174
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00000000,                                                  // zero                                        /// 0017c
			 0x55555555,                                                  // 4 random values                             /// 00180
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00184
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0018c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00190
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00194
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00198
			 0x3f028f5c,                                                  // 0.51                                        /// 0019c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00000000,                                                  // zero                                        /// 001a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x4b000000,                                                  // 8388608.0                                   /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001e8
			 0x4b000000,                                                  // 8388608.0                                   /// 001ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00200
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0020c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00210
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0xff800000,                                                  // -inf                                        /// 0021c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00224
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00228
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0023c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00240
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00244
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00254
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0025c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00260
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00264
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0026c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00270
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00278
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00280
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00284
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0028c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00290
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0029c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xbf028f5c,                                                  // -0.51                                       /// 002a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x0c780000,                                                  // Leading 1s:                                 /// 00308
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00314
			 0x7fc00001,                                                  // signaling NaN                               /// 00318
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00324
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0032c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00330
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00334
			 0x80000000,                                                  // -zero                                       /// 00338
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00344
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00348
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0034c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00350
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00358
			 0x0c700000,                                                  // Leading 1s:                                 /// 0035c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00360
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00364
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00378
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0037c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00380
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00384
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00388
			 0x0c600000,                                                  // Leading 1s:                                 /// 0038c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00394
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00398
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0039c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00400
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00404
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00408
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00418
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0041c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0042c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00434
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80011111,                                                  // -9.7958E-41                                 /// 0043c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00440
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00444
			 0x0c780000,                                                  // Leading 1s:                                 /// 00448
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0044c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00450
			 0x0e000007,                                                  // Trailing 1s:                                /// 00454
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00458
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0045c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00460
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00464
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00468
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00470
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00474
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00478
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00484
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00488
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00490
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00494
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0049c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0xffc00001,                                                  // -signaling NaN                              /// 004a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x33333333,                                                  // 4 random values                             /// 004ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 004c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 004d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0050c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00510
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00514
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00518
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x4b000000,                                                  // 8388608.0                                   /// 00520
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00524
			 0x0c780000,                                                  // Leading 1s:                                 /// 00528
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0052c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00530
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00534
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00538
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00540
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00544
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0054c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00550
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00554
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00558
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0055c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00560
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00568
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0056c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c700000,                                                  // Leading 1s:                                 /// 00578
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0057c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00580
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00588
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0058c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00590
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00594
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0059c
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00600
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00604
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00608
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00614
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0061c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00620
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00624
			 0x00000000,                                                  // zero                                        /// 00628
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00630
			 0xbf028f5c,                                                  // -0.51                                       /// 00634
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00638
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0063c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00640
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00644
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00648
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0064c
			 0x3f028f5c,                                                  // 0.51                                        /// 00650
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00654
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00658
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0065c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00660
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00664
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x3f028f5c,                                                  // 0.51                                        /// 0066c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00670
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00678
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00680
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00684
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00688
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0068c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00694
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00698
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0069c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xffc00001,                                                  // -signaling NaN                              /// 006d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c600000,                                                  // Leading 1s:                                 /// 00704
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00708
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0070c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x0e000003,                                                  // Trailing 1s:                                /// 00718
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0071c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00724
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00728
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00730
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00734
			 0x4b000000,                                                  // 8388608.0                                   /// 00738
			 0x0e000001,                                                  // Trailing 1s:                                /// 0073c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00740
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00744
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00748
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0074c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00750
			 0x0c780000,                                                  // Leading 1s:                                 /// 00754
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00758
			 0x0c400000,                                                  // Leading 1s:                                 /// 0075c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00764
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00774
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00778
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0077c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00788
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0078c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00790
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00794
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00798
			 0x4b000000,                                                  // 8388608.0                                   /// 0079c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e8
			 0xcb000000,                                                  // -8388608.0                                  /// 007ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00800
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00808
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0080c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00810
			 0x00011111,                                                  // 9.7958E-41                                  /// 00814
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00818
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0081c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00820
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00824
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0082c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0083c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00840
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00844
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00850
			 0xcb000000,                                                  // -8388608.0                                  /// 00854
			 0xffc00001,                                                  // -signaling NaN                              /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0085c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00864
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x4b000000,                                                  // 8388608.0                                   /// 00870
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00874
			 0x0e000001,                                                  // Trailing 1s:                                /// 00878
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0087c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00888
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0088c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00894
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7fc00001,                                                  // signaling NaN                               /// 008c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00900
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00914
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00918
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0091c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00920
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00924
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00928
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00930
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00934
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00938
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0093c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00944
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00948
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00958
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0095c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00968
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0096c
			 0x7fc00001,                                                  // signaling NaN                               /// 00970
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00974
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0097c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00984
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00988
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00998
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0099c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x33333333,                                                  // 4 random values                             /// 009b8
			 0x3f028f5c,                                                  // 0.51                                        /// 009bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e8
			 0xbf028f5c,                                                  // -0.51                                       /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x55555555,                                                  // 4 random values                             /// 00a08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a18
			 0x33333333,                                                  // 4 random values                             /// 00a1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x7fc00001,                                                  // signaling NaN                               /// 00a2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x7f800000,                                                  // inf                                         /// 00a40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a78
			 0xffc00001,                                                  // -signaling NaN                              /// 00a7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a84
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aa8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ac8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ad0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ae0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b30
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b38
			 0x55555555,                                                  // 4 random values                             /// 00b3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ba0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00000000,                                                  // zero                                        /// 00bb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bf8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c54
			 0xff800000,                                                  // -inf                                        /// 00c58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff800000,                                                  // -inf                                        /// 00c6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cb8
			 0x33333333,                                                  // 4 random values                             /// 00cbc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0x7f800000,                                                  // inf                                         /// 00cdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x00000000,                                                  // zero                                        /// 00cec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cfc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d38
			 0xbf028f5c,                                                  // -0.51                                       /// 00d3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d94
			 0x55555555,                                                  // 4 random values                             /// 00d98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00da8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dc8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd8
			 0xff800000,                                                  // -inf                                        /// 00ddc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00de8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df4
			 0x80000000,                                                  // -zero                                       /// 00df8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x4b000000,                                                  // 8388608.0                                   /// 00e68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ebc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ec4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ecc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ed0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00edc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ee4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00efc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f3c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f74
			 0x55555555,                                                  // 4 random values                             /// 00f78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fa8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb0
			 0x55555555,                                                  // 4 random values                             /// 00fb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00fc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fdc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff8
			 0x80080000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xbce76140, /// 0x0
			 0x8abe087d, /// 0x4
			 0xd4fd2e68, /// 0x8
			 0xc5cdb04b, /// 0xc
			 0xc9988b9d, /// 0x10
			 0x5641e4ea, /// 0x14
			 0x80b9f88b, /// 0x18
			 0xaaf8abe5, /// 0x1c
			 0x790f0455, /// 0x20
			 0xd8047c48, /// 0x24
			 0xfd7a1b5b, /// 0x28
			 0xa539cfbf, /// 0x2c
			 0xfcd1b8c1, /// 0x30
			 0xe2373286, /// 0x34
			 0xd073badf, /// 0x38
			 0xa9a94bbe, /// 0x3c
			 0x690257f4, /// 0x40
			 0x4fc24738, /// 0x44
			 0xcda56e47, /// 0x48
			 0x64283a9d, /// 0x4c
			 0xc6731dd9, /// 0x50
			 0xe318582e, /// 0x54
			 0xf526ff71, /// 0x58
			 0xb4fe73c0, /// 0x5c
			 0x35980a76, /// 0x60
			 0x77f9cd4e, /// 0x64
			 0xf100adb5, /// 0x68
			 0x64b02abd, /// 0x6c
			 0xe4414543, /// 0x70
			 0x06ac57de, /// 0x74
			 0x07a36848, /// 0x78
			 0x240daffb, /// 0x7c
			 0x8b07f860, /// 0x80
			 0xa6c72ba9, /// 0x84
			 0x8350b1fd, /// 0x88
			 0x6964ea19, /// 0x8c
			 0x7d21f949, /// 0x90
			 0x12b08035, /// 0x94
			 0x34f70ff5, /// 0x98
			 0xff9b8b06, /// 0x9c
			 0xdf8cd1c4, /// 0xa0
			 0x0019e91e, /// 0xa4
			 0xb1ac1a53, /// 0xa8
			 0x0dfd2484, /// 0xac
			 0x063d289c, /// 0xb0
			 0x6904b659, /// 0xb4
			 0xe878fb57, /// 0xb8
			 0xcf27eae9, /// 0xbc
			 0x1dd01a03, /// 0xc0
			 0x8c06ac8e, /// 0xc4
			 0xa6e85702, /// 0xc8
			 0x7fd2b2ee, /// 0xcc
			 0xe8825f25, /// 0xd0
			 0x7531b56e, /// 0xd4
			 0x44e7bf82, /// 0xd8
			 0x00c60d4d, /// 0xdc
			 0xafa3bd39, /// 0xe0
			 0x1e92eac4, /// 0xe4
			 0x809668ad, /// 0xe8
			 0xb780c0ab, /// 0xec
			 0x83a0a01c, /// 0xf0
			 0x9cddd73a, /// 0xf4
			 0x53c13747, /// 0xf8
			 0xcd1310e1, /// 0xfc
			 0xf30ca283, /// 0x100
			 0x97ac46d4, /// 0x104
			 0xdc94a3c3, /// 0x108
			 0xd1869292, /// 0x10c
			 0x95a424ec, /// 0x110
			 0xf6d3c3c0, /// 0x114
			 0x719c3562, /// 0x118
			 0x32d5d7af, /// 0x11c
			 0x591ca7ec, /// 0x120
			 0xa8bae359, /// 0x124
			 0x0f2c2cb0, /// 0x128
			 0x0e68e39e, /// 0x12c
			 0xeb75a58d, /// 0x130
			 0x6572429b, /// 0x134
			 0x93c83794, /// 0x138
			 0x9291522e, /// 0x13c
			 0x3acac22a, /// 0x140
			 0x4f7356c1, /// 0x144
			 0xac989d01, /// 0x148
			 0x944faeac, /// 0x14c
			 0x83246a3e, /// 0x150
			 0xbd160377, /// 0x154
			 0xb6eb4d95, /// 0x158
			 0xbb7f4e86, /// 0x15c
			 0xfcb22f9b, /// 0x160
			 0x33f69627, /// 0x164
			 0x67675c4c, /// 0x168
			 0x9dc086ef, /// 0x16c
			 0xd2a6bdc9, /// 0x170
			 0x152c7267, /// 0x174
			 0xd9cccf2b, /// 0x178
			 0x03ffd058, /// 0x17c
			 0x17a743ff, /// 0x180
			 0x42d2e3ec, /// 0x184
			 0x7cc683a3, /// 0x188
			 0xe4809126, /// 0x18c
			 0x7ca5579d, /// 0x190
			 0x2d422817, /// 0x194
			 0xfe1b42a7, /// 0x198
			 0x951ff957, /// 0x19c
			 0x13d75103, /// 0x1a0
			 0x243b381f, /// 0x1a4
			 0x3e2226bd, /// 0x1a8
			 0x9f672b3c, /// 0x1ac
			 0x9ed2f517, /// 0x1b0
			 0xa805e7ae, /// 0x1b4
			 0x4b1c296c, /// 0x1b8
			 0xd377d962, /// 0x1bc
			 0xdf43efc7, /// 0x1c0
			 0xbd5b62a3, /// 0x1c4
			 0x63fa03a8, /// 0x1c8
			 0x53ea5e9a, /// 0x1cc
			 0x0fc67522, /// 0x1d0
			 0xbb99a54f, /// 0x1d4
			 0x3ba0c991, /// 0x1d8
			 0x9a792fb4, /// 0x1dc
			 0x3ee687ed, /// 0x1e0
			 0x4749c42c, /// 0x1e4
			 0xe2a4bdcb, /// 0x1e8
			 0x3e10ac85, /// 0x1ec
			 0xd71571b7, /// 0x1f0
			 0xc95b858a, /// 0x1f4
			 0x0ca948d8, /// 0x1f8
			 0x73245b10, /// 0x1fc
			 0x593e718d, /// 0x200
			 0x6787d02b, /// 0x204
			 0x4c0b4147, /// 0x208
			 0x8adf3309, /// 0x20c
			 0x2efa6f49, /// 0x210
			 0x93ccad4b, /// 0x214
			 0xf5ba4a0b, /// 0x218
			 0x2b914a85, /// 0x21c
			 0x72b9cdd2, /// 0x220
			 0x9984b4a9, /// 0x224
			 0xe9d845dc, /// 0x228
			 0xf24e495a, /// 0x22c
			 0xecab0b74, /// 0x230
			 0x234032e1, /// 0x234
			 0x6b7fd118, /// 0x238
			 0x1784b4e3, /// 0x23c
			 0xdd16afac, /// 0x240
			 0x3516c49e, /// 0x244
			 0x3dd05dad, /// 0x248
			 0x84f96715, /// 0x24c
			 0x45c6f587, /// 0x250
			 0x6555273b, /// 0x254
			 0x9503a09a, /// 0x258
			 0x1c9c85fe, /// 0x25c
			 0x830e7cde, /// 0x260
			 0xc8b32d5e, /// 0x264
			 0x5ed5d0f3, /// 0x268
			 0x736e78aa, /// 0x26c
			 0xa35c40ea, /// 0x270
			 0x3146d4c8, /// 0x274
			 0x0cc23e1c, /// 0x278
			 0xa785ce55, /// 0x27c
			 0xf8a2b54b, /// 0x280
			 0x39f8608c, /// 0x284
			 0x37f1bfd8, /// 0x288
			 0xd5bb39b4, /// 0x28c
			 0x95b3b18d, /// 0x290
			 0x05dd051c, /// 0x294
			 0xa30fa62e, /// 0x298
			 0xd33ff79e, /// 0x29c
			 0x945f4e6e, /// 0x2a0
			 0xb9792c5a, /// 0x2a4
			 0x01bb53e4, /// 0x2a8
			 0x3700e783, /// 0x2ac
			 0xc740cd79, /// 0x2b0
			 0x3e39c577, /// 0x2b4
			 0xdb1e1267, /// 0x2b8
			 0x6fcb2aab, /// 0x2bc
			 0x1dc838a1, /// 0x2c0
			 0xa7fcd3c6, /// 0x2c4
			 0x0abe8cb4, /// 0x2c8
			 0x2eb8bab2, /// 0x2cc
			 0x81d9b7ca, /// 0x2d0
			 0xb1ae0394, /// 0x2d4
			 0x460b39e9, /// 0x2d8
			 0x2f143a68, /// 0x2dc
			 0x1d863b7f, /// 0x2e0
			 0x0b1b2da7, /// 0x2e4
			 0xd3834d22, /// 0x2e8
			 0x72731015, /// 0x2ec
			 0xe0fed29b, /// 0x2f0
			 0xa987f3c9, /// 0x2f4
			 0x31863a52, /// 0x2f8
			 0x7ced3ea1, /// 0x2fc
			 0xeacb5a8d, /// 0x300
			 0x9e7ce8fb, /// 0x304
			 0xa36368be, /// 0x308
			 0x29165af4, /// 0x30c
			 0x5e726813, /// 0x310
			 0xceb1ad33, /// 0x314
			 0x44ec4966, /// 0x318
			 0x24984232, /// 0x31c
			 0x0403e6f6, /// 0x320
			 0xeaa9904c, /// 0x324
			 0xc3600416, /// 0x328
			 0x91a9c787, /// 0x32c
			 0xc32db58e, /// 0x330
			 0x0256e69d, /// 0x334
			 0xc7d89e5f, /// 0x338
			 0xeb0921e3, /// 0x33c
			 0xa8e742d7, /// 0x340
			 0x6b3f04a5, /// 0x344
			 0x7fee94a2, /// 0x348
			 0x494c1a2a, /// 0x34c
			 0x9bbc9ae5, /// 0x350
			 0xbf5599e6, /// 0x354
			 0x5d894d75, /// 0x358
			 0xfc279ef8, /// 0x35c
			 0x5a6b47e1, /// 0x360
			 0x5f82b002, /// 0x364
			 0x324405f7, /// 0x368
			 0x9557b794, /// 0x36c
			 0xc9b760e8, /// 0x370
			 0x2f218ffb, /// 0x374
			 0x71a98498, /// 0x378
			 0x6bf52dde, /// 0x37c
			 0xea85c8da, /// 0x380
			 0x4342ba9b, /// 0x384
			 0xb1100ccc, /// 0x388
			 0x4a2caf96, /// 0x38c
			 0x17cad362, /// 0x390
			 0x9e21c084, /// 0x394
			 0x801a3be9, /// 0x398
			 0x9060b393, /// 0x39c
			 0x0ef03fac, /// 0x3a0
			 0xf1288a0c, /// 0x3a4
			 0x06e73f80, /// 0x3a8
			 0x17fcaea8, /// 0x3ac
			 0xd65994f5, /// 0x3b0
			 0xf6baa287, /// 0x3b4
			 0xd8dbd781, /// 0x3b8
			 0x8a72d3b2, /// 0x3bc
			 0xcc662b1c, /// 0x3c0
			 0x5d7ca806, /// 0x3c4
			 0xb7cfd8bc, /// 0x3c8
			 0xfd3014f6, /// 0x3cc
			 0x7a3b7d5c, /// 0x3d0
			 0x12f7a3a8, /// 0x3d4
			 0x46c880fb, /// 0x3d8
			 0xa6cae725, /// 0x3dc
			 0x056be27d, /// 0x3e0
			 0xe548dd98, /// 0x3e4
			 0xa4d1a6f4, /// 0x3e8
			 0x20d43a8c, /// 0x3ec
			 0xf68da258, /// 0x3f0
			 0x6a5b5eeb, /// 0x3f4
			 0xda004139, /// 0x3f8
			 0x57d26867, /// 0x3fc
			 0x04469c45, /// 0x400
			 0xf54d1fbe, /// 0x404
			 0x8c5b9a24, /// 0x408
			 0xceffc195, /// 0x40c
			 0xc80ac029, /// 0x410
			 0xc84ebb46, /// 0x414
			 0x5d1781fb, /// 0x418
			 0x9d76f09f, /// 0x41c
			 0x94233568, /// 0x420
			 0x6b489da9, /// 0x424
			 0x1c8e2ac7, /// 0x428
			 0x6660fefc, /// 0x42c
			 0x2452816c, /// 0x430
			 0xc4cc9adf, /// 0x434
			 0x905c7b41, /// 0x438
			 0xe9eeb231, /// 0x43c
			 0x73674299, /// 0x440
			 0xf5178eaf, /// 0x444
			 0x8f64daca, /// 0x448
			 0xb6a607c1, /// 0x44c
			 0x28bfa9d8, /// 0x450
			 0xdc7417a3, /// 0x454
			 0xebb380df, /// 0x458
			 0x0e12e84d, /// 0x45c
			 0xad82e7ec, /// 0x460
			 0x7708341c, /// 0x464
			 0xdd8552fb, /// 0x468
			 0x95fcbf7c, /// 0x46c
			 0xba6c141f, /// 0x470
			 0x76c7d169, /// 0x474
			 0x0d5bfd64, /// 0x478
			 0x6caf0683, /// 0x47c
			 0x71a1dd55, /// 0x480
			 0x6374eba9, /// 0x484
			 0xb026cdd8, /// 0x488
			 0x19511db6, /// 0x48c
			 0x099c36b1, /// 0x490
			 0xa5d64410, /// 0x494
			 0x33a9bd1f, /// 0x498
			 0xa6ef7e92, /// 0x49c
			 0xa9a8578e, /// 0x4a0
			 0xce0763f5, /// 0x4a4
			 0x224b1e81, /// 0x4a8
			 0x8935ae94, /// 0x4ac
			 0xfa3b0165, /// 0x4b0
			 0x3da1b96e, /// 0x4b4
			 0x4ea6f310, /// 0x4b8
			 0x791d831d, /// 0x4bc
			 0x38bbcbac, /// 0x4c0
			 0xc8d28710, /// 0x4c4
			 0xf3e8014d, /// 0x4c8
			 0x034d3ce6, /// 0x4cc
			 0x1442b1b7, /// 0x4d0
			 0x6e745359, /// 0x4d4
			 0xe46bfa8e, /// 0x4d8
			 0x4cb1b96c, /// 0x4dc
			 0x77697e9c, /// 0x4e0
			 0xbb3afa78, /// 0x4e4
			 0x43bd4e75, /// 0x4e8
			 0xcfb6457a, /// 0x4ec
			 0xe1efa3f6, /// 0x4f0
			 0xf0b2437c, /// 0x4f4
			 0x4c78fa06, /// 0x4f8
			 0xebc1ab25, /// 0x4fc
			 0x57b6dfb5, /// 0x500
			 0xd2d7909d, /// 0x504
			 0xf67c913d, /// 0x508
			 0xaeeb3582, /// 0x50c
			 0x83ef294b, /// 0x510
			 0x2f3ef52e, /// 0x514
			 0x127bac46, /// 0x518
			 0x6d69cb46, /// 0x51c
			 0x1ca10b26, /// 0x520
			 0xc945533d, /// 0x524
			 0x191cc21c, /// 0x528
			 0xde6ca2ff, /// 0x52c
			 0x2714f6d0, /// 0x530
			 0xaf52282e, /// 0x534
			 0x5e796a7e, /// 0x538
			 0xd477c952, /// 0x53c
			 0x9bd5e67e, /// 0x540
			 0x8bd09ea0, /// 0x544
			 0xf28658a2, /// 0x548
			 0xd1bfc4d8, /// 0x54c
			 0xc25543d3, /// 0x550
			 0x682961f9, /// 0x554
			 0xa6afb035, /// 0x558
			 0xfa512e3c, /// 0x55c
			 0x839ffb82, /// 0x560
			 0x6193cd0a, /// 0x564
			 0x78b2a96a, /// 0x568
			 0xfb282ca4, /// 0x56c
			 0xb36e1cad, /// 0x570
			 0x36dbaf0e, /// 0x574
			 0xbf04598b, /// 0x578
			 0x4e3c8ccf, /// 0x57c
			 0xef2ae790, /// 0x580
			 0x78770ea6, /// 0x584
			 0x3579ea75, /// 0x588
			 0xa5bd4e30, /// 0x58c
			 0xded2271e, /// 0x590
			 0xa1c64930, /// 0x594
			 0x563709b0, /// 0x598
			 0x57a09901, /// 0x59c
			 0xeb4ee469, /// 0x5a0
			 0x4faaf83c, /// 0x5a4
			 0x7385515d, /// 0x5a8
			 0x914cca44, /// 0x5ac
			 0x57da91f2, /// 0x5b0
			 0x136e07ed, /// 0x5b4
			 0xf1db8461, /// 0x5b8
			 0x9fadb14e, /// 0x5bc
			 0x3eb734df, /// 0x5c0
			 0x8cc3eeb1, /// 0x5c4
			 0xad38e14c, /// 0x5c8
			 0xdc9675c8, /// 0x5cc
			 0xa5929877, /// 0x5d0
			 0x07f3c754, /// 0x5d4
			 0x5b35dc2b, /// 0x5d8
			 0x65685765, /// 0x5dc
			 0x9c985e4b, /// 0x5e0
			 0xffcc77c8, /// 0x5e4
			 0xd47d157a, /// 0x5e8
			 0xb9fca173, /// 0x5ec
			 0x2fb7f904, /// 0x5f0
			 0xa354a354, /// 0x5f4
			 0xd4f85476, /// 0x5f8
			 0xc6bf76a7, /// 0x5fc
			 0x043847b3, /// 0x600
			 0xec13f05e, /// 0x604
			 0x89d4f476, /// 0x608
			 0xac9decdb, /// 0x60c
			 0xd074d82d, /// 0x610
			 0xb3dc8d32, /// 0x614
			 0xb44a0708, /// 0x618
			 0xa20d0845, /// 0x61c
			 0xae4b58f1, /// 0x620
			 0x01636c09, /// 0x624
			 0x8a79dd64, /// 0x628
			 0x038462d3, /// 0x62c
			 0x854c4801, /// 0x630
			 0xb0d60dfe, /// 0x634
			 0x6dc32246, /// 0x638
			 0x5e9894cb, /// 0x63c
			 0x8d1f6039, /// 0x640
			 0xc0bdc759, /// 0x644
			 0x296b7cef, /// 0x648
			 0xa661e534, /// 0x64c
			 0x7abcb74e, /// 0x650
			 0x42f96c4b, /// 0x654
			 0x4c9a2037, /// 0x658
			 0xb152a2fe, /// 0x65c
			 0x083923a1, /// 0x660
			 0xa03f17a4, /// 0x664
			 0xe115e8a8, /// 0x668
			 0x1d4ad879, /// 0x66c
			 0x92fe9517, /// 0x670
			 0x1ca2d7c1, /// 0x674
			 0x39c64c4c, /// 0x678
			 0x65262865, /// 0x67c
			 0xc4cdec94, /// 0x680
			 0x76636f1e, /// 0x684
			 0xc68aea13, /// 0x688
			 0x8bf5aa43, /// 0x68c
			 0x43ba455d, /// 0x690
			 0xa0dce921, /// 0x694
			 0x35662786, /// 0x698
			 0x5105f8e3, /// 0x69c
			 0xc5725568, /// 0x6a0
			 0x7d1ddf02, /// 0x6a4
			 0xe085b12e, /// 0x6a8
			 0x520764b6, /// 0x6ac
			 0xa92df0b8, /// 0x6b0
			 0x2ce134c1, /// 0x6b4
			 0x805389de, /// 0x6b8
			 0xce57583f, /// 0x6bc
			 0xb6b0e03b, /// 0x6c0
			 0x1ffc2e40, /// 0x6c4
			 0xb9e485b3, /// 0x6c8
			 0xcfb1bb1a, /// 0x6cc
			 0x66fde89e, /// 0x6d0
			 0x769812b3, /// 0x6d4
			 0x250ff4e9, /// 0x6d8
			 0x15619b8c, /// 0x6dc
			 0x2fbfd304, /// 0x6e0
			 0x035a2949, /// 0x6e4
			 0xbf2fb8a5, /// 0x6e8
			 0x37947d82, /// 0x6ec
			 0xded773e0, /// 0x6f0
			 0xc9da4114, /// 0x6f4
			 0x4925b77e, /// 0x6f8
			 0x84dd6cc2, /// 0x6fc
			 0xa6ab2d93, /// 0x700
			 0xfc29fc1c, /// 0x704
			 0xa80f381b, /// 0x708
			 0x0076f624, /// 0x70c
			 0x5953cd27, /// 0x710
			 0x177b07af, /// 0x714
			 0x279490bf, /// 0x718
			 0x7360c3a4, /// 0x71c
			 0x4326b06f, /// 0x720
			 0xd83eb379, /// 0x724
			 0x14e09d5f, /// 0x728
			 0x8352f7d4, /// 0x72c
			 0x806724be, /// 0x730
			 0x913baab0, /// 0x734
			 0xf5ecd094, /// 0x738
			 0x22446c3d, /// 0x73c
			 0xb9b394f4, /// 0x740
			 0x2cd4d8e5, /// 0x744
			 0xecc3e959, /// 0x748
			 0x7c983b8f, /// 0x74c
			 0x8e0e6d86, /// 0x750
			 0xcfcf6bd0, /// 0x754
			 0xaf847802, /// 0x758
			 0xbf0240f0, /// 0x75c
			 0x1c7bfceb, /// 0x760
			 0x0654a37a, /// 0x764
			 0x1f662b51, /// 0x768
			 0xe69925ca, /// 0x76c
			 0xf66669dd, /// 0x770
			 0x9badc7fd, /// 0x774
			 0x46222442, /// 0x778
			 0xe4dc80b0, /// 0x77c
			 0x4324ddba, /// 0x780
			 0x1b87e399, /// 0x784
			 0x38406bab, /// 0x788
			 0x775fca8f, /// 0x78c
			 0x6a4232c4, /// 0x790
			 0xb144e928, /// 0x794
			 0x83fab0bd, /// 0x798
			 0x6aa11e36, /// 0x79c
			 0x3fbd3eb3, /// 0x7a0
			 0x7ec38618, /// 0x7a4
			 0x54ba7b7e, /// 0x7a8
			 0x2b35dfd4, /// 0x7ac
			 0x73c3e9a4, /// 0x7b0
			 0xdcc38728, /// 0x7b4
			 0x592a9f78, /// 0x7b8
			 0xe6aa3a93, /// 0x7bc
			 0xb7883a0f, /// 0x7c0
			 0x3060d7fe, /// 0x7c4
			 0x84d0257b, /// 0x7c8
			 0x35a4e62e, /// 0x7cc
			 0x92a62683, /// 0x7d0
			 0x102c5c27, /// 0x7d4
			 0xc93571d6, /// 0x7d8
			 0x835b8a7b, /// 0x7dc
			 0xc84452db, /// 0x7e0
			 0x6f252e7e, /// 0x7e4
			 0x63b88c18, /// 0x7e8
			 0x28621890, /// 0x7ec
			 0x07af3629, /// 0x7f0
			 0xf7d4b867, /// 0x7f4
			 0x96c5f78c, /// 0x7f8
			 0x8b88cf73, /// 0x7fc
			 0xc15f2420, /// 0x800
			 0x82e3b5d7, /// 0x804
			 0x7cf8028b, /// 0x808
			 0xffb610e1, /// 0x80c
			 0x4049e4e9, /// 0x810
			 0xbd6f284a, /// 0x814
			 0xeffd90bc, /// 0x818
			 0xc50c0431, /// 0x81c
			 0x7e63dda3, /// 0x820
			 0x76d1a953, /// 0x824
			 0xff5f2e8a, /// 0x828
			 0x6171d2d7, /// 0x82c
			 0x353539e0, /// 0x830
			 0xc5da4799, /// 0x834
			 0xa400b1b1, /// 0x838
			 0x4fe324b6, /// 0x83c
			 0x12eb86fe, /// 0x840
			 0x084059b7, /// 0x844
			 0x3f5a860a, /// 0x848
			 0x0afb3964, /// 0x84c
			 0x0fad1d41, /// 0x850
			 0xa214927b, /// 0x854
			 0x7d4809ef, /// 0x858
			 0x9cddaa41, /// 0x85c
			 0x5f0b113f, /// 0x860
			 0x24ceaa11, /// 0x864
			 0x28f3f846, /// 0x868
			 0x0c96b67d, /// 0x86c
			 0xf568328a, /// 0x870
			 0x22feff2c, /// 0x874
			 0x2e193250, /// 0x878
			 0x7b762f5d, /// 0x87c
			 0xa02552b1, /// 0x880
			 0x0d7fe607, /// 0x884
			 0xb928c464, /// 0x888
			 0xbd614703, /// 0x88c
			 0x844882b3, /// 0x890
			 0x190f5bd4, /// 0x894
			 0x5a91564f, /// 0x898
			 0x714bb6a2, /// 0x89c
			 0x34d0b957, /// 0x8a0
			 0x784da335, /// 0x8a4
			 0x9076d510, /// 0x8a8
			 0x8f2ce83f, /// 0x8ac
			 0x7b7209ab, /// 0x8b0
			 0xf5c0f44a, /// 0x8b4
			 0xf4340790, /// 0x8b8
			 0x0b204d32, /// 0x8bc
			 0x66e30c9a, /// 0x8c0
			 0xb2306019, /// 0x8c4
			 0xfd579cad, /// 0x8c8
			 0xc68f1a40, /// 0x8cc
			 0x093185e8, /// 0x8d0
			 0x0640303d, /// 0x8d4
			 0xcdce7a33, /// 0x8d8
			 0x94fc9c81, /// 0x8dc
			 0xa6dc6747, /// 0x8e0
			 0x1db78c5b, /// 0x8e4
			 0x6d00f9cf, /// 0x8e8
			 0x25abcb37, /// 0x8ec
			 0xd4284865, /// 0x8f0
			 0xed83feb6, /// 0x8f4
			 0x8acf4906, /// 0x8f8
			 0xb0b8d734, /// 0x8fc
			 0xc90a49b7, /// 0x900
			 0x50c72bfd, /// 0x904
			 0xfbca9bce, /// 0x908
			 0x83911606, /// 0x90c
			 0x538f331e, /// 0x910
			 0x22d907f8, /// 0x914
			 0xd338acea, /// 0x918
			 0x9549bbd9, /// 0x91c
			 0x22b18c60, /// 0x920
			 0xa2d6988d, /// 0x924
			 0x633a43fc, /// 0x928
			 0x1d8ec671, /// 0x92c
			 0xaaa83c7a, /// 0x930
			 0xda6643f0, /// 0x934
			 0x007b4eda, /// 0x938
			 0xe5195a6f, /// 0x93c
			 0x15fcc9ee, /// 0x940
			 0x79b7df45, /// 0x944
			 0x7687c0e2, /// 0x948
			 0xf18f100a, /// 0x94c
			 0xcd468f6a, /// 0x950
			 0xc3833db4, /// 0x954
			 0x27c5245e, /// 0x958
			 0x26c95468, /// 0x95c
			 0x60dba2f0, /// 0x960
			 0x0e8ce277, /// 0x964
			 0xc2ef48a7, /// 0x968
			 0x739f0976, /// 0x96c
			 0xde4727c0, /// 0x970
			 0xf90def01, /// 0x974
			 0x91502716, /// 0x978
			 0xcce13f3e, /// 0x97c
			 0x323a47fb, /// 0x980
			 0xfbdcd54f, /// 0x984
			 0xfe001c0b, /// 0x988
			 0xe8ec71d0, /// 0x98c
			 0x91b42143, /// 0x990
			 0x9eff03a5, /// 0x994
			 0xd41c52d2, /// 0x998
			 0xe17a0dcd, /// 0x99c
			 0xd0ddd570, /// 0x9a0
			 0xb182217f, /// 0x9a4
			 0x2b1d1b65, /// 0x9a8
			 0x6ce34fda, /// 0x9ac
			 0x1758d762, /// 0x9b0
			 0x513c58b8, /// 0x9b4
			 0x32b0e09e, /// 0x9b8
			 0x2d3e6f00, /// 0x9bc
			 0xc22f533b, /// 0x9c0
			 0x14c982cc, /// 0x9c4
			 0x90d3c37b, /// 0x9c8
			 0x5a35694a, /// 0x9cc
			 0xfea5cf05, /// 0x9d0
			 0x594f8307, /// 0x9d4
			 0x44a27db3, /// 0x9d8
			 0x9ab58c6e, /// 0x9dc
			 0x96ee39fe, /// 0x9e0
			 0x646c4fab, /// 0x9e4
			 0x5140f296, /// 0x9e8
			 0x86a3dc14, /// 0x9ec
			 0x8fa03e97, /// 0x9f0
			 0x164ba004, /// 0x9f4
			 0x365e802c, /// 0x9f8
			 0x24554290, /// 0x9fc
			 0x67141960, /// 0xa00
			 0x5242d192, /// 0xa04
			 0x70aa81be, /// 0xa08
			 0xa111b9fb, /// 0xa0c
			 0xe2baa634, /// 0xa10
			 0xd52e74a9, /// 0xa14
			 0xa136d183, /// 0xa18
			 0x84ace5ab, /// 0xa1c
			 0xc34925c1, /// 0xa20
			 0xa2bd543b, /// 0xa24
			 0xfcaa007a, /// 0xa28
			 0xf674895b, /// 0xa2c
			 0x1b88cecb, /// 0xa30
			 0x1d6cde55, /// 0xa34
			 0xf68c16c3, /// 0xa38
			 0x57abad39, /// 0xa3c
			 0xd232d152, /// 0xa40
			 0xc7ed8fec, /// 0xa44
			 0xb9281ad2, /// 0xa48
			 0xf4cf4732, /// 0xa4c
			 0x352da104, /// 0xa50
			 0x1f46a562, /// 0xa54
			 0x4d7377c7, /// 0xa58
			 0x4cb026ff, /// 0xa5c
			 0xa4a178ee, /// 0xa60
			 0x5e3d0ed2, /// 0xa64
			 0x927641c2, /// 0xa68
			 0xe8250c35, /// 0xa6c
			 0xaea7f188, /// 0xa70
			 0x0171f61f, /// 0xa74
			 0x7363fb11, /// 0xa78
			 0xfc35d5ec, /// 0xa7c
			 0x4c125469, /// 0xa80
			 0xb2ac9d43, /// 0xa84
			 0x976efd71, /// 0xa88
			 0xbc14a464, /// 0xa8c
			 0x671c012b, /// 0xa90
			 0xb815f187, /// 0xa94
			 0x5d04ae48, /// 0xa98
			 0x32a856fd, /// 0xa9c
			 0xd9a7cf5a, /// 0xaa0
			 0xc75db4ed, /// 0xaa4
			 0x7e1be0cc, /// 0xaa8
			 0x6d3d24d7, /// 0xaac
			 0x35bd72ae, /// 0xab0
			 0xcbf88b1d, /// 0xab4
			 0x0f0d67f5, /// 0xab8
			 0xf0e11446, /// 0xabc
			 0xfc14f486, /// 0xac0
			 0x93f17112, /// 0xac4
			 0xedb1cbf3, /// 0xac8
			 0x27494c02, /// 0xacc
			 0xdac3ab65, /// 0xad0
			 0x91361e57, /// 0xad4
			 0xb1891d11, /// 0xad8
			 0xf1063e41, /// 0xadc
			 0x7478b087, /// 0xae0
			 0xa925b9dc, /// 0xae4
			 0xd777a0cb, /// 0xae8
			 0x7a50f4a7, /// 0xaec
			 0x70806a47, /// 0xaf0
			 0xc85d265a, /// 0xaf4
			 0x2b562cad, /// 0xaf8
			 0xf3f2f152, /// 0xafc
			 0xdc558e64, /// 0xb00
			 0xee9dab09, /// 0xb04
			 0xac772b3e, /// 0xb08
			 0xcec101e8, /// 0xb0c
			 0xf2ba8f1a, /// 0xb10
			 0xd76daff1, /// 0xb14
			 0x66a92596, /// 0xb18
			 0x6794aedc, /// 0xb1c
			 0x85197256, /// 0xb20
			 0xaf1140bd, /// 0xb24
			 0xe86a4cf8, /// 0xb28
			 0x3f9471a0, /// 0xb2c
			 0xb7df763e, /// 0xb30
			 0xbdfb9646, /// 0xb34
			 0x06d0ff88, /// 0xb38
			 0xa303fcd6, /// 0xb3c
			 0xf983e468, /// 0xb40
			 0x53f84e15, /// 0xb44
			 0x6f37636e, /// 0xb48
			 0xa16965d4, /// 0xb4c
			 0x2557bc33, /// 0xb50
			 0xfad386f2, /// 0xb54
			 0xf00b0439, /// 0xb58
			 0x67389aeb, /// 0xb5c
			 0x19404750, /// 0xb60
			 0x17e82d53, /// 0xb64
			 0x1d81de87, /// 0xb68
			 0xa16682b5, /// 0xb6c
			 0xa5af400d, /// 0xb70
			 0xee1b7ecf, /// 0xb74
			 0xe517143d, /// 0xb78
			 0xab0b7b6c, /// 0xb7c
			 0xf21b2891, /// 0xb80
			 0x8c268465, /// 0xb84
			 0x895d22e3, /// 0xb88
			 0xde315d96, /// 0xb8c
			 0x56140eac, /// 0xb90
			 0x28f713da, /// 0xb94
			 0x8a1ba725, /// 0xb98
			 0x54df2838, /// 0xb9c
			 0x3df360f5, /// 0xba0
			 0x7d07bfc5, /// 0xba4
			 0xb42f82ec, /// 0xba8
			 0x6259eac8, /// 0xbac
			 0x2e4f12ca, /// 0xbb0
			 0x1af45c16, /// 0xbb4
			 0xa6597a63, /// 0xbb8
			 0x3625395e, /// 0xbbc
			 0xb17d332c, /// 0xbc0
			 0x76811ccb, /// 0xbc4
			 0xde9598bf, /// 0xbc8
			 0x05af29be, /// 0xbcc
			 0x6df27738, /// 0xbd0
			 0x4ad1c83f, /// 0xbd4
			 0x56d31252, /// 0xbd8
			 0x2d76a72a, /// 0xbdc
			 0x28d74eac, /// 0xbe0
			 0xa474a832, /// 0xbe4
			 0xc7458850, /// 0xbe8
			 0x7eede7d4, /// 0xbec
			 0x69deb23b, /// 0xbf0
			 0x3bca6e8d, /// 0xbf4
			 0x1e6521f1, /// 0xbf8
			 0xaa5208f1, /// 0xbfc
			 0x5e315ccf, /// 0xc00
			 0x62f403ba, /// 0xc04
			 0x2c218b45, /// 0xc08
			 0x36bc1690, /// 0xc0c
			 0xaea8035f, /// 0xc10
			 0x62e23e6a, /// 0xc14
			 0x7c263adc, /// 0xc18
			 0x1fb1a95a, /// 0xc1c
			 0x77b1084a, /// 0xc20
			 0x09bc93df, /// 0xc24
			 0x17d93eb4, /// 0xc28
			 0x03133bcd, /// 0xc2c
			 0x12a9e0aa, /// 0xc30
			 0xb31ec00a, /// 0xc34
			 0xa484342e, /// 0xc38
			 0xc8221acb, /// 0xc3c
			 0x1e75a5a8, /// 0xc40
			 0x500c732d, /// 0xc44
			 0x095aac01, /// 0xc48
			 0x3bb7429e, /// 0xc4c
			 0x5e0e5285, /// 0xc50
			 0x85cd986b, /// 0xc54
			 0xb3b40264, /// 0xc58
			 0xf6bc6330, /// 0xc5c
			 0x619f6d9a, /// 0xc60
			 0x1345ccde, /// 0xc64
			 0xa6288d8b, /// 0xc68
			 0x129e5fce, /// 0xc6c
			 0x43b43102, /// 0xc70
			 0x8495f6b6, /// 0xc74
			 0xc3914842, /// 0xc78
			 0x8466db71, /// 0xc7c
			 0xd44789e0, /// 0xc80
			 0xef8ad3d5, /// 0xc84
			 0x30faafe2, /// 0xc88
			 0xa111d648, /// 0xc8c
			 0x81773ec9, /// 0xc90
			 0xe2530a70, /// 0xc94
			 0x9ece2163, /// 0xc98
			 0x9d44bb18, /// 0xc9c
			 0x712a5317, /// 0xca0
			 0xaa483c05, /// 0xca4
			 0x2dd85121, /// 0xca8
			 0xa2ae1a55, /// 0xcac
			 0xae1e9a6d, /// 0xcb0
			 0xe4e5b6ae, /// 0xcb4
			 0x661f3d7c, /// 0xcb8
			 0x88dd37cf, /// 0xcbc
			 0x1c194947, /// 0xcc0
			 0x342d7f78, /// 0xcc4
			 0xbd71f94d, /// 0xcc8
			 0xc04a6dd3, /// 0xccc
			 0xe9fc6185, /// 0xcd0
			 0x3d50172d, /// 0xcd4
			 0x7503384e, /// 0xcd8
			 0xea8bb15e, /// 0xcdc
			 0xa1baf025, /// 0xce0
			 0x387a0436, /// 0xce4
			 0x5360a838, /// 0xce8
			 0x50873dd5, /// 0xcec
			 0x9b766ab9, /// 0xcf0
			 0x8386f7d0, /// 0xcf4
			 0xfd00dd11, /// 0xcf8
			 0x0ea999f7, /// 0xcfc
			 0x79b951ed, /// 0xd00
			 0xad89d286, /// 0xd04
			 0xc24112b1, /// 0xd08
			 0x22ecac3b, /// 0xd0c
			 0x37aa0be6, /// 0xd10
			 0xa7b916a4, /// 0xd14
			 0x2d0009ea, /// 0xd18
			 0xf107296b, /// 0xd1c
			 0x1dc8bcfe, /// 0xd20
			 0xcc8f5000, /// 0xd24
			 0x99137ed7, /// 0xd28
			 0x89bc3b0a, /// 0xd2c
			 0x21a20d01, /// 0xd30
			 0x6ab17a54, /// 0xd34
			 0x0a3ec173, /// 0xd38
			 0xee8ad9f6, /// 0xd3c
			 0x19084ce6, /// 0xd40
			 0x86144184, /// 0xd44
			 0xe70e017a, /// 0xd48
			 0x3a4f55f5, /// 0xd4c
			 0x1379a408, /// 0xd50
			 0x63008b88, /// 0xd54
			 0x353b044b, /// 0xd58
			 0xa5ced43c, /// 0xd5c
			 0x28264fb4, /// 0xd60
			 0xf7b3d9ca, /// 0xd64
			 0x2fa0c6a3, /// 0xd68
			 0xffacc96a, /// 0xd6c
			 0xdfc3b0bd, /// 0xd70
			 0x17038bf9, /// 0xd74
			 0x38e025f6, /// 0xd78
			 0x6964a0ae, /// 0xd7c
			 0x92b44b21, /// 0xd80
			 0x05fbbbd9, /// 0xd84
			 0xf1d60382, /// 0xd88
			 0x4ad79221, /// 0xd8c
			 0x35a33ae3, /// 0xd90
			 0x3ad5593d, /// 0xd94
			 0x681fd6b4, /// 0xd98
			 0xb13714d8, /// 0xd9c
			 0xc2f7cfab, /// 0xda0
			 0x1f26d361, /// 0xda4
			 0x84f43fe7, /// 0xda8
			 0x982ef6d2, /// 0xdac
			 0xf8a8059d, /// 0xdb0
			 0xc72a65f4, /// 0xdb4
			 0x5e7cb80a, /// 0xdb8
			 0xac53df62, /// 0xdbc
			 0x9433f117, /// 0xdc0
			 0x1f00d7a9, /// 0xdc4
			 0x2290fceb, /// 0xdc8
			 0x84f3a71b, /// 0xdcc
			 0x36dd3817, /// 0xdd0
			 0x4ba05dd2, /// 0xdd4
			 0x01375ba1, /// 0xdd8
			 0xbacfaca0, /// 0xddc
			 0x727489f5, /// 0xde0
			 0xc4014407, /// 0xde4
			 0x1288497f, /// 0xde8
			 0xf8612a93, /// 0xdec
			 0xe20081ed, /// 0xdf0
			 0x061466f0, /// 0xdf4
			 0x74ae2c39, /// 0xdf8
			 0xb9222c0b, /// 0xdfc
			 0xe26fd817, /// 0xe00
			 0x6d38e5f5, /// 0xe04
			 0x325a435b, /// 0xe08
			 0x162ffc3a, /// 0xe0c
			 0xac6c2d09, /// 0xe10
			 0x10f03fc0, /// 0xe14
			 0x0dddf40d, /// 0xe18
			 0xe545d455, /// 0xe1c
			 0xc317f503, /// 0xe20
			 0xd21d2432, /// 0xe24
			 0x48bfa732, /// 0xe28
			 0x03202fb8, /// 0xe2c
			 0x3d323043, /// 0xe30
			 0xa9c2ffba, /// 0xe34
			 0x555e88f9, /// 0xe38
			 0x6fc501ae, /// 0xe3c
			 0x3b1d513d, /// 0xe40
			 0x8890656e, /// 0xe44
			 0x7c59005a, /// 0xe48
			 0xa2193253, /// 0xe4c
			 0x96b78cb5, /// 0xe50
			 0x91a70383, /// 0xe54
			 0x9ab615fc, /// 0xe58
			 0x4f24fc95, /// 0xe5c
			 0x8f177cf1, /// 0xe60
			 0x450d3b0d, /// 0xe64
			 0x6f609a10, /// 0xe68
			 0xb4851ac6, /// 0xe6c
			 0x2eab7299, /// 0xe70
			 0xa3b0a759, /// 0xe74
			 0xadc33280, /// 0xe78
			 0x4ed81216, /// 0xe7c
			 0x23dbcb01, /// 0xe80
			 0xa751a008, /// 0xe84
			 0x4e30fea0, /// 0xe88
			 0x9d2da39b, /// 0xe8c
			 0x58ed5bf7, /// 0xe90
			 0xe07f37d4, /// 0xe94
			 0xa04a0d73, /// 0xe98
			 0xe764d12b, /// 0xe9c
			 0xa9a89d7c, /// 0xea0
			 0x02a70d38, /// 0xea4
			 0x5be52c30, /// 0xea8
			 0x45496fa6, /// 0xeac
			 0x9ea99333, /// 0xeb0
			 0xdcf4c9d0, /// 0xeb4
			 0x6b40f6d2, /// 0xeb8
			 0x73ccf2bc, /// 0xebc
			 0x1c7b4b1d, /// 0xec0
			 0xf6d5d1ef, /// 0xec4
			 0x8ba41050, /// 0xec8
			 0x770ef8a2, /// 0xecc
			 0x7169778e, /// 0xed0
			 0x8b2eed64, /// 0xed4
			 0xde31db3a, /// 0xed8
			 0x8d1ae968, /// 0xedc
			 0x3bc6c06d, /// 0xee0
			 0x560c21e8, /// 0xee4
			 0x1a6c550e, /// 0xee8
			 0xf766b051, /// 0xeec
			 0x3e50e28e, /// 0xef0
			 0xdd0aaeb4, /// 0xef4
			 0xb47cbdb4, /// 0xef8
			 0x857615f0, /// 0xefc
			 0xa8eb97e5, /// 0xf00
			 0xdea9269b, /// 0xf04
			 0x684ee82d, /// 0xf08
			 0x45224031, /// 0xf0c
			 0xa7d2a836, /// 0xf10
			 0x8910faf9, /// 0xf14
			 0x8189dd59, /// 0xf18
			 0x8d852654, /// 0xf1c
			 0x2d66bed5, /// 0xf20
			 0x92a73885, /// 0xf24
			 0x86250ed9, /// 0xf28
			 0x3d411b92, /// 0xf2c
			 0x2be9dea5, /// 0xf30
			 0xc4fd0c85, /// 0xf34
			 0x3163a0cc, /// 0xf38
			 0xd9368cff, /// 0xf3c
			 0x92b4874e, /// 0xf40
			 0x4dcb7b69, /// 0xf44
			 0x3f7d734c, /// 0xf48
			 0x2ea6232f, /// 0xf4c
			 0x604a4df0, /// 0xf50
			 0xc8bd357e, /// 0xf54
			 0x85186d9a, /// 0xf58
			 0xcfc0bd29, /// 0xf5c
			 0x6a72034d, /// 0xf60
			 0xb49fce71, /// 0xf64
			 0xd16122e0, /// 0xf68
			 0x0e0e962d, /// 0xf6c
			 0xd69e39f9, /// 0xf70
			 0x5ae76106, /// 0xf74
			 0x925f96d3, /// 0xf78
			 0x3d1cdcb8, /// 0xf7c
			 0x6c9cee3d, /// 0xf80
			 0x8b9947e0, /// 0xf84
			 0x5f433632, /// 0xf88
			 0xac69ed3c, /// 0xf8c
			 0x45f117f4, /// 0xf90
			 0x63dcee7b, /// 0xf94
			 0xc0ccf600, /// 0xf98
			 0xf00dec03, /// 0xf9c
			 0xc8532dec, /// 0xfa0
			 0x23453f72, /// 0xfa4
			 0x5ed22894, /// 0xfa8
			 0x5d677791, /// 0xfac
			 0xea5e0d86, /// 0xfb0
			 0xee267025, /// 0xfb4
			 0xa0b11880, /// 0xfb8
			 0x56cad3cb, /// 0xfbc
			 0x91897635, /// 0xfc0
			 0xaacc2be5, /// 0xfc4
			 0x0e415897, /// 0xfc8
			 0x6d95b490, /// 0xfcc
			 0x16ac4538, /// 0xfd0
			 0xed7575d0, /// 0xfd4
			 0x576df786, /// 0xfd8
			 0x5382c2ae, /// 0xfdc
			 0xe726b62c, /// 0xfe0
			 0x9101d880, /// 0xfe4
			 0x5c371440, /// 0xfe8
			 0xa301f7f3, /// 0xfec
			 0x901a641a, /// 0xff0
			 0xbd44a472, /// 0xff4
			 0x6784be11, /// 0xff8
			 0x1b19cf78 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xef2ed8d4, /// 0x0
			 0x4e8eca81, /// 0x4
			 0x4a2aec26, /// 0x8
			 0xcd0b1270, /// 0xc
			 0x504fc2d3, /// 0x10
			 0x5a513518, /// 0x14
			 0xa06f7b93, /// 0x18
			 0x32218243, /// 0x1c
			 0xe84a5dd1, /// 0x20
			 0x103a0620, /// 0x24
			 0x5d611315, /// 0x28
			 0x5bcda25b, /// 0x2c
			 0x914fe712, /// 0x30
			 0xb46f0905, /// 0x34
			 0xaa7ca022, /// 0x38
			 0x3de4e2d7, /// 0x3c
			 0x2bf8b65d, /// 0x40
			 0xfd981cb7, /// 0x44
			 0x85897621, /// 0x48
			 0xa582b3ca, /// 0x4c
			 0x8ae4281a, /// 0x50
			 0x6213bc40, /// 0x54
			 0x4868b90c, /// 0x58
			 0x6217ec71, /// 0x5c
			 0x7f69eac4, /// 0x60
			 0x31d2b46a, /// 0x64
			 0xd1c77482, /// 0x68
			 0x25c81d29, /// 0x6c
			 0xf3a2f21b, /// 0x70
			 0xa36d1375, /// 0x74
			 0xa00ee959, /// 0x78
			 0xc2ed6b13, /// 0x7c
			 0xd4cac995, /// 0x80
			 0xecd9e021, /// 0x84
			 0xbc8483c3, /// 0x88
			 0xd2137fb0, /// 0x8c
			 0x7fc7e7ae, /// 0x90
			 0xaeba54da, /// 0x94
			 0x0619e204, /// 0x98
			 0xc055523e, /// 0x9c
			 0x4d121d55, /// 0xa0
			 0x840de640, /// 0xa4
			 0x71c6e662, /// 0xa8
			 0xbbea524b, /// 0xac
			 0xda2e862a, /// 0xb0
			 0xbb0e3b20, /// 0xb4
			 0x357f7c74, /// 0xb8
			 0x1941516e, /// 0xbc
			 0xd4067e98, /// 0xc0
			 0x1362bdcf, /// 0xc4
			 0x9019b798, /// 0xc8
			 0xc9bf77a2, /// 0xcc
			 0x83043790, /// 0xd0
			 0x39d14cbd, /// 0xd4
			 0x879c8844, /// 0xd8
			 0x3490b3bc, /// 0xdc
			 0xfd3fb2d8, /// 0xe0
			 0xd6ff9b21, /// 0xe4
			 0xa6f36951, /// 0xe8
			 0xce1659cd, /// 0xec
			 0xe27e58a4, /// 0xf0
			 0xa92f577e, /// 0xf4
			 0x8445ea30, /// 0xf8
			 0x29f74998, /// 0xfc
			 0x12ed06b9, /// 0x100
			 0x8f238242, /// 0x104
			 0x85bf9eaa, /// 0x108
			 0x0285c251, /// 0x10c
			 0x192f9466, /// 0x110
			 0x57f886d3, /// 0x114
			 0xe6e148f8, /// 0x118
			 0x96c980e3, /// 0x11c
			 0xb4956667, /// 0x120
			 0x6654ff1e, /// 0x124
			 0x059330b0, /// 0x128
			 0xe2e039d5, /// 0x12c
			 0x6752e827, /// 0x130
			 0x57315a2b, /// 0x134
			 0x73d16023, /// 0x138
			 0xf581b94c, /// 0x13c
			 0x00692c46, /// 0x140
			 0xebadc0a1, /// 0x144
			 0x73f4d5e1, /// 0x148
			 0x63d7712f, /// 0x14c
			 0xca869c86, /// 0x150
			 0x391edffa, /// 0x154
			 0x6e8af09f, /// 0x158
			 0x118c85f1, /// 0x15c
			 0x85e4505b, /// 0x160
			 0x504fd6a9, /// 0x164
			 0x734a73ad, /// 0x168
			 0x20881aac, /// 0x16c
			 0x115ace3a, /// 0x170
			 0x92d4cfda, /// 0x174
			 0x783992ac, /// 0x178
			 0x6f6ebd81, /// 0x17c
			 0x45919dc4, /// 0x180
			 0xc9c380e7, /// 0x184
			 0xdd56b591, /// 0x188
			 0x91bd1f59, /// 0x18c
			 0x9343922b, /// 0x190
			 0xd719309f, /// 0x194
			 0x4541cd10, /// 0x198
			 0xc07fbfca, /// 0x19c
			 0xf0ad24ec, /// 0x1a0
			 0x9eb4075f, /// 0x1a4
			 0x4bfcd081, /// 0x1a8
			 0x8bf886e6, /// 0x1ac
			 0x7f8a7211, /// 0x1b0
			 0xe7b2c36f, /// 0x1b4
			 0xd6dca85c, /// 0x1b8
			 0x2eedce9a, /// 0x1bc
			 0x6af87700, /// 0x1c0
			 0xf3b74fd1, /// 0x1c4
			 0x62f341b2, /// 0x1c8
			 0x2d8376b2, /// 0x1cc
			 0x3601d6be, /// 0x1d0
			 0xde23751f, /// 0x1d4
			 0x3735b112, /// 0x1d8
			 0x44045991, /// 0x1dc
			 0x69009565, /// 0x1e0
			 0x15c29a2a, /// 0x1e4
			 0x5c7612c0, /// 0x1e8
			 0x721cb394, /// 0x1ec
			 0x82749504, /// 0x1f0
			 0x5e3f17b7, /// 0x1f4
			 0x23fcde4d, /// 0x1f8
			 0x540b0184, /// 0x1fc
			 0x0bc5b96d, /// 0x200
			 0x60ae155c, /// 0x204
			 0x2382b6b5, /// 0x208
			 0x8b0c6c65, /// 0x20c
			 0x768ef2ab, /// 0x210
			 0xacff851d, /// 0x214
			 0x06faa4e7, /// 0x218
			 0x7f1f5d7a, /// 0x21c
			 0xcf541e0f, /// 0x220
			 0x8013f405, /// 0x224
			 0x18d03017, /// 0x228
			 0xd033962b, /// 0x22c
			 0x0429234e, /// 0x230
			 0x57d4e5da, /// 0x234
			 0x52005a29, /// 0x238
			 0x82799ccc, /// 0x23c
			 0x196087fb, /// 0x240
			 0x0fdc754c, /// 0x244
			 0x39682b55, /// 0x248
			 0x373050a1, /// 0x24c
			 0x49bb57a6, /// 0x250
			 0x0917ce1c, /// 0x254
			 0x254159d9, /// 0x258
			 0x900175e1, /// 0x25c
			 0x45646ed0, /// 0x260
			 0x593e2e22, /// 0x264
			 0xbf595be1, /// 0x268
			 0x56103dae, /// 0x26c
			 0x81ca1135, /// 0x270
			 0xb235f200, /// 0x274
			 0x145d956c, /// 0x278
			 0xff4b91b0, /// 0x27c
			 0xb2e50169, /// 0x280
			 0x492f965c, /// 0x284
			 0x6a994aad, /// 0x288
			 0xc929e422, /// 0x28c
			 0x48ea84f9, /// 0x290
			 0x44f204e6, /// 0x294
			 0x3513ce2e, /// 0x298
			 0x3de77414, /// 0x29c
			 0x8befcc5d, /// 0x2a0
			 0x027730fb, /// 0x2a4
			 0x90a8038b, /// 0x2a8
			 0xa9753507, /// 0x2ac
			 0x71df2e61, /// 0x2b0
			 0xbf89484d, /// 0x2b4
			 0x6517d7e9, /// 0x2b8
			 0x9186480d, /// 0x2bc
			 0x91109301, /// 0x2c0
			 0xb294d9b4, /// 0x2c4
			 0xe15f9a78, /// 0x2c8
			 0x67a62046, /// 0x2cc
			 0x254901c9, /// 0x2d0
			 0xc92984fc, /// 0x2d4
			 0xdbbf3eed, /// 0x2d8
			 0x344a4720, /// 0x2dc
			 0x549f9507, /// 0x2e0
			 0xe0b95ae8, /// 0x2e4
			 0x87ca7b2b, /// 0x2e8
			 0x13d49fff, /// 0x2ec
			 0x01f34fc0, /// 0x2f0
			 0x00687f51, /// 0x2f4
			 0x54f2c979, /// 0x2f8
			 0xa80f2def, /// 0x2fc
			 0xe7303cfb, /// 0x300
			 0x373fc67e, /// 0x304
			 0x11572bba, /// 0x308
			 0xf776c2a6, /// 0x30c
			 0x2b15a6be, /// 0x310
			 0x7538e18e, /// 0x314
			 0xe2463780, /// 0x318
			 0x1cd2758f, /// 0x31c
			 0x483ad7f6, /// 0x320
			 0x02158de2, /// 0x324
			 0x938e062f, /// 0x328
			 0x4c6cc642, /// 0x32c
			 0x7e2e583b, /// 0x330
			 0x3daad22f, /// 0x334
			 0xb204ef47, /// 0x338
			 0xedeb3a47, /// 0x33c
			 0xbf3eddd1, /// 0x340
			 0x4ae91537, /// 0x344
			 0xea787bb0, /// 0x348
			 0xae0bc4a5, /// 0x34c
			 0x2c4fe504, /// 0x350
			 0x8384769c, /// 0x354
			 0x7d5a6644, /// 0x358
			 0x290b279f, /// 0x35c
			 0x1f77a9d9, /// 0x360
			 0x06283d64, /// 0x364
			 0xf814e991, /// 0x368
			 0x895c51d4, /// 0x36c
			 0x149a66d7, /// 0x370
			 0x202aeb73, /// 0x374
			 0xbc0f9709, /// 0x378
			 0x2592ba56, /// 0x37c
			 0x3225ced8, /// 0x380
			 0x44e45e77, /// 0x384
			 0x1677bc12, /// 0x388
			 0x7a9511b7, /// 0x38c
			 0x2638ea76, /// 0x390
			 0x46811cca, /// 0x394
			 0xc88ab62f, /// 0x398
			 0x4edc9a29, /// 0x39c
			 0xca84357a, /// 0x3a0
			 0x0699b5cf, /// 0x3a4
			 0xae388e07, /// 0x3a8
			 0xdeff9f26, /// 0x3ac
			 0xd62e8742, /// 0x3b0
			 0x2d1e3c83, /// 0x3b4
			 0x6343dce2, /// 0x3b8
			 0xe8e4f1df, /// 0x3bc
			 0x1e025218, /// 0x3c0
			 0x6b591275, /// 0x3c4
			 0x370884da, /// 0x3c8
			 0xea67c218, /// 0x3cc
			 0xcd8a77b1, /// 0x3d0
			 0x00f6ef12, /// 0x3d4
			 0x88ceaf5d, /// 0x3d8
			 0x73c4e4a3, /// 0x3dc
			 0xbb2d71f4, /// 0x3e0
			 0x939eb10c, /// 0x3e4
			 0xd075e32d, /// 0x3e8
			 0x57a5d817, /// 0x3ec
			 0x86391cff, /// 0x3f0
			 0xad7d6566, /// 0x3f4
			 0x263865d4, /// 0x3f8
			 0x693d3337, /// 0x3fc
			 0x55a11365, /// 0x400
			 0x7fa8af42, /// 0x404
			 0xd6caf7c5, /// 0x408
			 0xf0414d9f, /// 0x40c
			 0x536ee87f, /// 0x410
			 0xd0edf669, /// 0x414
			 0x50dadbcf, /// 0x418
			 0xd9d53898, /// 0x41c
			 0x60b219d3, /// 0x420
			 0x952ae453, /// 0x424
			 0x7cf2f806, /// 0x428
			 0x270aba9b, /// 0x42c
			 0xbaf5911a, /// 0x430
			 0xfcdd649b, /// 0x434
			 0x0ced35a5, /// 0x438
			 0x72c995e3, /// 0x43c
			 0x35f42485, /// 0x440
			 0xe24e7cfe, /// 0x444
			 0x0f0e2f3e, /// 0x448
			 0x3a14cbfb, /// 0x44c
			 0xd1c08528, /// 0x450
			 0xdc9df0ec, /// 0x454
			 0xacc2da1b, /// 0x458
			 0x9cd5d9a0, /// 0x45c
			 0x78a4f0c4, /// 0x460
			 0x12625337, /// 0x464
			 0x83df8639, /// 0x468
			 0x19587330, /// 0x46c
			 0x7918ffdf, /// 0x470
			 0x498599a2, /// 0x474
			 0xfbfe761f, /// 0x478
			 0xf18b8b46, /// 0x47c
			 0x0003185a, /// 0x480
			 0x873a894e, /// 0x484
			 0xc9c187dd, /// 0x488
			 0x8e88e077, /// 0x48c
			 0x3f4b5691, /// 0x490
			 0x7b41af13, /// 0x494
			 0xb8fb354f, /// 0x498
			 0x8702a10c, /// 0x49c
			 0x781891ed, /// 0x4a0
			 0x495513bd, /// 0x4a4
			 0x50fd7bd5, /// 0x4a8
			 0x64fcf902, /// 0x4ac
			 0x10fc8ec6, /// 0x4b0
			 0x658a9fbd, /// 0x4b4
			 0x0a7e0e8e, /// 0x4b8
			 0x0a96993b, /// 0x4bc
			 0xd65983f7, /// 0x4c0
			 0x69b7a805, /// 0x4c4
			 0xd21326c8, /// 0x4c8
			 0x6e7bff9f, /// 0x4cc
			 0xb1bea993, /// 0x4d0
			 0x6daa1098, /// 0x4d4
			 0x4d364898, /// 0x4d8
			 0xb830d097, /// 0x4dc
			 0xb0838b1f, /// 0x4e0
			 0xb830bcfc, /// 0x4e4
			 0x73da340f, /// 0x4e8
			 0xba6eb536, /// 0x4ec
			 0xd618ef25, /// 0x4f0
			 0xa09e11f2, /// 0x4f4
			 0xb3d4c946, /// 0x4f8
			 0xe8a61b4a, /// 0x4fc
			 0x902d330b, /// 0x500
			 0x91c42b39, /// 0x504
			 0x0d631a39, /// 0x508
			 0xcc3007cd, /// 0x50c
			 0x2fa57864, /// 0x510
			 0xcbdaa1f6, /// 0x514
			 0x3798be89, /// 0x518
			 0x59f273c4, /// 0x51c
			 0x8981c884, /// 0x520
			 0x09ade8ce, /// 0x524
			 0xd6e727b9, /// 0x528
			 0xd6a86078, /// 0x52c
			 0x618af8c4, /// 0x530
			 0x43789df9, /// 0x534
			 0xa7dedee1, /// 0x538
			 0x853402fc, /// 0x53c
			 0x2989ca88, /// 0x540
			 0xea1f7eeb, /// 0x544
			 0x002e4709, /// 0x548
			 0x7aabb361, /// 0x54c
			 0x9ee7b367, /// 0x550
			 0x6242d872, /// 0x554
			 0xb5d61c68, /// 0x558
			 0xec72dde6, /// 0x55c
			 0x1dc9d72a, /// 0x560
			 0x6486623e, /// 0x564
			 0x3ff76e1a, /// 0x568
			 0x8ddc7359, /// 0x56c
			 0xd27ccc74, /// 0x570
			 0xab203992, /// 0x574
			 0xb1e6d187, /// 0x578
			 0x07d8fc86, /// 0x57c
			 0xf1faa145, /// 0x580
			 0x88b216a1, /// 0x584
			 0x40e8863c, /// 0x588
			 0xf8993038, /// 0x58c
			 0x4e4cdf26, /// 0x590
			 0xdc863610, /// 0x594
			 0x01706006, /// 0x598
			 0x548f52d3, /// 0x59c
			 0x1ed23222, /// 0x5a0
			 0x258b7343, /// 0x5a4
			 0x2ed576b9, /// 0x5a8
			 0x29fea1e7, /// 0x5ac
			 0x1cc0086e, /// 0x5b0
			 0x1f1de3d5, /// 0x5b4
			 0xbbef0812, /// 0x5b8
			 0x9ae5acf0, /// 0x5bc
			 0x5aed5185, /// 0x5c0
			 0xa5d39bc8, /// 0x5c4
			 0x4e21c74a, /// 0x5c8
			 0x4dceda7b, /// 0x5cc
			 0x053a71d3, /// 0x5d0
			 0x65dfb7ca, /// 0x5d4
			 0x34f021d5, /// 0x5d8
			 0x1e5c8bc1, /// 0x5dc
			 0x98a95b7e, /// 0x5e0
			 0x24b6a29c, /// 0x5e4
			 0xa3b8c01d, /// 0x5e8
			 0x0ecf3912, /// 0x5ec
			 0x46ec1f75, /// 0x5f0
			 0x23848e35, /// 0x5f4
			 0x702bff3f, /// 0x5f8
			 0xb9914e94, /// 0x5fc
			 0x2c07d79f, /// 0x600
			 0xcc4a29c6, /// 0x604
			 0x5138500b, /// 0x608
			 0xab58523c, /// 0x60c
			 0x615a5189, /// 0x610
			 0xf4030056, /// 0x614
			 0xd41c0508, /// 0x618
			 0x5faf1ea8, /// 0x61c
			 0x831e9fb8, /// 0x620
			 0xd6575bdd, /// 0x624
			 0x8f081c2b, /// 0x628
			 0x03b6cbc9, /// 0x62c
			 0x192d5da7, /// 0x630
			 0x7aec8f97, /// 0x634
			 0x00bf263f, /// 0x638
			 0xcb6434a3, /// 0x63c
			 0xd4413d7b, /// 0x640
			 0x7cb00ecd, /// 0x644
			 0x7f33c50c, /// 0x648
			 0xbe8f3dd3, /// 0x64c
			 0x3489747a, /// 0x650
			 0xc4ac6282, /// 0x654
			 0x9ad7a69a, /// 0x658
			 0xe461a098, /// 0x65c
			 0xb296b4d3, /// 0x660
			 0x96acfdb6, /// 0x664
			 0x835a17e4, /// 0x668
			 0xd785003b, /// 0x66c
			 0xb16771e2, /// 0x670
			 0x2cb45a94, /// 0x674
			 0x45ec5fe0, /// 0x678
			 0x7d97eda0, /// 0x67c
			 0xf7a2bb26, /// 0x680
			 0xc73bae8f, /// 0x684
			 0xd2c7bdcb, /// 0x688
			 0x6fcfe47a, /// 0x68c
			 0x6b5433c1, /// 0x690
			 0x4f7666f2, /// 0x694
			 0xbd5d1b45, /// 0x698
			 0x1f361aaa, /// 0x69c
			 0x6c979a92, /// 0x6a0
			 0xb1bb269d, /// 0x6a4
			 0x3bcb0570, /// 0x6a8
			 0x9287c1a3, /// 0x6ac
			 0xd10ba55c, /// 0x6b0
			 0xef8ebd8f, /// 0x6b4
			 0xd90a828a, /// 0x6b8
			 0x85f90930, /// 0x6bc
			 0x3c63d585, /// 0x6c0
			 0x3e77069c, /// 0x6c4
			 0x49df6b28, /// 0x6c8
			 0x5a888d96, /// 0x6cc
			 0x9db2bc49, /// 0x6d0
			 0x5628f302, /// 0x6d4
			 0xd3271da0, /// 0x6d8
			 0x7b53f5f3, /// 0x6dc
			 0x3d117b30, /// 0x6e0
			 0xe0a95e86, /// 0x6e4
			 0x297bad0a, /// 0x6e8
			 0x53d5be02, /// 0x6ec
			 0x279358bf, /// 0x6f0
			 0xada3e2ed, /// 0x6f4
			 0xe2c14112, /// 0x6f8
			 0xc94f2602, /// 0x6fc
			 0x06ec0ae8, /// 0x700
			 0xe53684b0, /// 0x704
			 0x9b0d48b9, /// 0x708
			 0x1e57b73b, /// 0x70c
			 0x93a5b67b, /// 0x710
			 0x7a663ba8, /// 0x714
			 0x9f452592, /// 0x718
			 0x842023b4, /// 0x71c
			 0xc7b0ce85, /// 0x720
			 0xfaf031a1, /// 0x724
			 0xb46af9de, /// 0x728
			 0x8a21c61d, /// 0x72c
			 0x6050b230, /// 0x730
			 0xbfadc5d6, /// 0x734
			 0x6b0c0364, /// 0x738
			 0x48bf01c9, /// 0x73c
			 0x2178d119, /// 0x740
			 0xb18a2d85, /// 0x744
			 0x3ee82a9f, /// 0x748
			 0x7dd5502a, /// 0x74c
			 0x99f38757, /// 0x750
			 0x4591ccff, /// 0x754
			 0x35809c6e, /// 0x758
			 0x9cb7fde3, /// 0x75c
			 0x6efde3b2, /// 0x760
			 0x2017d854, /// 0x764
			 0x842b4a7c, /// 0x768
			 0xe50388d0, /// 0x76c
			 0x3386e1da, /// 0x770
			 0xb8a8afbc, /// 0x774
			 0xbed28d9e, /// 0x778
			 0x7c78f849, /// 0x77c
			 0xe7699649, /// 0x780
			 0x3d32b81b, /// 0x784
			 0x4e90f408, /// 0x788
			 0x4d09e861, /// 0x78c
			 0x9f338a06, /// 0x790
			 0x506838ea, /// 0x794
			 0xb00e7269, /// 0x798
			 0xdaf161dd, /// 0x79c
			 0x798ec2b2, /// 0x7a0
			 0x86b75183, /// 0x7a4
			 0x5c84e320, /// 0x7a8
			 0x0f072933, /// 0x7ac
			 0x6856a277, /// 0x7b0
			 0xbac0066f, /// 0x7b4
			 0x6413cf0b, /// 0x7b8
			 0x24a80ad4, /// 0x7bc
			 0x77045a49, /// 0x7c0
			 0xe6ba38a1, /// 0x7c4
			 0xf6c99e4d, /// 0x7c8
			 0x3e02c17b, /// 0x7cc
			 0xe141980e, /// 0x7d0
			 0x22899955, /// 0x7d4
			 0x64c4bbe8, /// 0x7d8
			 0x141bf87b, /// 0x7dc
			 0x14382564, /// 0x7e0
			 0x47ed2bf8, /// 0x7e4
			 0xb49d2752, /// 0x7e8
			 0x382d6c01, /// 0x7ec
			 0x49f008fe, /// 0x7f0
			 0x2e51b9f4, /// 0x7f4
			 0xd66eaded, /// 0x7f8
			 0x440ace9f, /// 0x7fc
			 0xe84d00c5, /// 0x800
			 0x338e970c, /// 0x804
			 0x4f8ffc66, /// 0x808
			 0xa76340d9, /// 0x80c
			 0xf97decd5, /// 0x810
			 0xbec8e823, /// 0x814
			 0x0fc42e04, /// 0x818
			 0x8ccb7fae, /// 0x81c
			 0xcb773d47, /// 0x820
			 0x9f1141e4, /// 0x824
			 0xecb9a10e, /// 0x828
			 0x3ce98c1b, /// 0x82c
			 0x7808e407, /// 0x830
			 0x35fdf8eb, /// 0x834
			 0x7b6914af, /// 0x838
			 0xeb7070f9, /// 0x83c
			 0x28a13fad, /// 0x840
			 0xdf050503, /// 0x844
			 0x8be0f75c, /// 0x848
			 0xec532062, /// 0x84c
			 0xd916eef2, /// 0x850
			 0x4d5c3512, /// 0x854
			 0x1f7b4237, /// 0x858
			 0x7e51e792, /// 0x85c
			 0x14913d97, /// 0x860
			 0x35169499, /// 0x864
			 0x7b65b2df, /// 0x868
			 0x50a3a2b6, /// 0x86c
			 0xae5fa609, /// 0x870
			 0xeb229938, /// 0x874
			 0x2d2b8c0c, /// 0x878
			 0x39def90f, /// 0x87c
			 0x4b09b64e, /// 0x880
			 0x26e6efd9, /// 0x884
			 0xfad09e0f, /// 0x888
			 0x4c57a624, /// 0x88c
			 0x6931d06a, /// 0x890
			 0x78e5bd9d, /// 0x894
			 0xd2ec114f, /// 0x898
			 0xbfaf23c3, /// 0x89c
			 0x9de6b464, /// 0x8a0
			 0x5ffbe605, /// 0x8a4
			 0xafcfb7ca, /// 0x8a8
			 0x5718ce16, /// 0x8ac
			 0x0c290a3b, /// 0x8b0
			 0x0227c4ee, /// 0x8b4
			 0x37cef832, /// 0x8b8
			 0x7431d3cf, /// 0x8bc
			 0x79b162dc, /// 0x8c0
			 0xaeca43a3, /// 0x8c4
			 0xfdf6cd21, /// 0x8c8
			 0x6d53cc8f, /// 0x8cc
			 0xe9b7ed2d, /// 0x8d0
			 0x8caf2d1b, /// 0x8d4
			 0xbf00611d, /// 0x8d8
			 0x42e6b087, /// 0x8dc
			 0xff807900, /// 0x8e0
			 0x48aae5ae, /// 0x8e4
			 0xf484061d, /// 0x8e8
			 0xf0a45b17, /// 0x8ec
			 0x0e706de1, /// 0x8f0
			 0x96a6a550, /// 0x8f4
			 0x12d8ea86, /// 0x8f8
			 0x80ff856a, /// 0x8fc
			 0xeb04759d, /// 0x900
			 0x47553e6e, /// 0x904
			 0x39bbb4cd, /// 0x908
			 0x60b6821e, /// 0x90c
			 0x8103cd30, /// 0x910
			 0x4c487880, /// 0x914
			 0xdd62b3a7, /// 0x918
			 0xd9f51b00, /// 0x91c
			 0xf995acfa, /// 0x920
			 0xdc4d77bc, /// 0x924
			 0xc59cd624, /// 0x928
			 0xdf3f387b, /// 0x92c
			 0x86c1ede4, /// 0x930
			 0x56cbac4e, /// 0x934
			 0x3a0db470, /// 0x938
			 0x611ab444, /// 0x93c
			 0x599b8e71, /// 0x940
			 0x7e2d3452, /// 0x944
			 0x5a5845c3, /// 0x948
			 0xf3d55c9a, /// 0x94c
			 0x9f9836e1, /// 0x950
			 0x5eccb447, /// 0x954
			 0x0ed587c8, /// 0x958
			 0xe5197ca7, /// 0x95c
			 0x43302680, /// 0x960
			 0x2113408d, /// 0x964
			 0x31603a26, /// 0x968
			 0xc38d9620, /// 0x96c
			 0x9a221b05, /// 0x970
			 0xf066886f, /// 0x974
			 0xa7a211bd, /// 0x978
			 0x561b0c4b, /// 0x97c
			 0x3e0e03e8, /// 0x980
			 0xeb30ecd2, /// 0x984
			 0x853b8b9a, /// 0x988
			 0x7833d2f2, /// 0x98c
			 0x7888145f, /// 0x990
			 0x6f056770, /// 0x994
			 0xb5195685, /// 0x998
			 0xd5789957, /// 0x99c
			 0x830e4073, /// 0x9a0
			 0x4ed7fd04, /// 0x9a4
			 0xc52ff292, /// 0x9a8
			 0x0f2500e2, /// 0x9ac
			 0x56f0d648, /// 0x9b0
			 0x007e2670, /// 0x9b4
			 0x5df28203, /// 0x9b8
			 0x50df82a5, /// 0x9bc
			 0xccee01c3, /// 0x9c0
			 0xa6a147cf, /// 0x9c4
			 0x0a838534, /// 0x9c8
			 0x2a1919fb, /// 0x9cc
			 0x40547ac8, /// 0x9d0
			 0x8ec70e46, /// 0x9d4
			 0xc5e9f847, /// 0x9d8
			 0xba5db8ce, /// 0x9dc
			 0x938dceef, /// 0x9e0
			 0x9ad4c8d9, /// 0x9e4
			 0x2ca17ae8, /// 0x9e8
			 0x576df57e, /// 0x9ec
			 0xfbe44f7b, /// 0x9f0
			 0xff34e9d2, /// 0x9f4
			 0xa9a6d037, /// 0x9f8
			 0x52a5fc21, /// 0x9fc
			 0x16462ce0, /// 0xa00
			 0xeac8b31d, /// 0xa04
			 0x773c5e8d, /// 0xa08
			 0xdb757d08, /// 0xa0c
			 0xe333467d, /// 0xa10
			 0x29e644c4, /// 0xa14
			 0x5a0e9104, /// 0xa18
			 0x5c2615c1, /// 0xa1c
			 0x4d23d3fb, /// 0xa20
			 0xe5b612df, /// 0xa24
			 0x9c7bbe65, /// 0xa28
			 0x5d7c3e36, /// 0xa2c
			 0xba3446e9, /// 0xa30
			 0x244cedd7, /// 0xa34
			 0xdb44486a, /// 0xa38
			 0xdac714f9, /// 0xa3c
			 0xb41a8b44, /// 0xa40
			 0xa0e229bd, /// 0xa44
			 0xf2c92598, /// 0xa48
			 0xc363ecf1, /// 0xa4c
			 0x03c49870, /// 0xa50
			 0x8271f242, /// 0xa54
			 0x69f67cdb, /// 0xa58
			 0xef90ee59, /// 0xa5c
			 0xdde4399b, /// 0xa60
			 0xfd74730a, /// 0xa64
			 0x4c603afa, /// 0xa68
			 0x4429dc25, /// 0xa6c
			 0x453bf1cb, /// 0xa70
			 0xb7304c44, /// 0xa74
			 0xef265b85, /// 0xa78
			 0xc9e373c1, /// 0xa7c
			 0xb6250a38, /// 0xa80
			 0x490de1fa, /// 0xa84
			 0x77cec5f1, /// 0xa88
			 0x20b583b6, /// 0xa8c
			 0x0f3c40ab, /// 0xa90
			 0xb3cab638, /// 0xa94
			 0x4b271f51, /// 0xa98
			 0xf058ca93, /// 0xa9c
			 0xabb4fdf1, /// 0xaa0
			 0x03544d43, /// 0xaa4
			 0x81c97cc9, /// 0xaa8
			 0xb6c31c48, /// 0xaac
			 0x06bed217, /// 0xab0
			 0x5047666b, /// 0xab4
			 0xda48871f, /// 0xab8
			 0xbbbac72a, /// 0xabc
			 0xefbc3cce, /// 0xac0
			 0xf110ce9b, /// 0xac4
			 0xa763309a, /// 0xac8
			 0x2420cea8, /// 0xacc
			 0x02a7d096, /// 0xad0
			 0x83978d31, /// 0xad4
			 0xd5c92065, /// 0xad8
			 0xe3a348f1, /// 0xadc
			 0x36d01898, /// 0xae0
			 0xfe8aadf5, /// 0xae4
			 0x67cfe69a, /// 0xae8
			 0x62622a6b, /// 0xaec
			 0xed232398, /// 0xaf0
			 0xe35ae87a, /// 0xaf4
			 0x45fea653, /// 0xaf8
			 0xf1eda06a, /// 0xafc
			 0x8ad04006, /// 0xb00
			 0xcabc4e42, /// 0xb04
			 0xdd9d0248, /// 0xb08
			 0x0145e447, /// 0xb0c
			 0x2437a463, /// 0xb10
			 0xe3a9616b, /// 0xb14
			 0x13eb8558, /// 0xb18
			 0xd72a5ca8, /// 0xb1c
			 0xcf049e31, /// 0xb20
			 0xbc768bc1, /// 0xb24
			 0xa9697bb9, /// 0xb28
			 0x76f41c73, /// 0xb2c
			 0xbefb9d87, /// 0xb30
			 0xab92e051, /// 0xb34
			 0xf5f4ab51, /// 0xb38
			 0x3d53905d, /// 0xb3c
			 0xe8ccee22, /// 0xb40
			 0x01830ad6, /// 0xb44
			 0x2220e251, /// 0xb48
			 0x28268072, /// 0xb4c
			 0x48e88954, /// 0xb50
			 0x0d4972c6, /// 0xb54
			 0xfd7603d2, /// 0xb58
			 0x3d2270e4, /// 0xb5c
			 0xf083ebfd, /// 0xb60
			 0xfdc779ed, /// 0xb64
			 0x1137875e, /// 0xb68
			 0xc18484a6, /// 0xb6c
			 0x717854ab, /// 0xb70
			 0x8f807efb, /// 0xb74
			 0xda7fb9d6, /// 0xb78
			 0x44b41712, /// 0xb7c
			 0x734a1752, /// 0xb80
			 0xa0ca8013, /// 0xb84
			 0x7abe6750, /// 0xb88
			 0xaa49e981, /// 0xb8c
			 0xf84b84c0, /// 0xb90
			 0x93c4f6c5, /// 0xb94
			 0xe1be0005, /// 0xb98
			 0x95b5f577, /// 0xb9c
			 0xfd23685f, /// 0xba0
			 0xe5ff88cc, /// 0xba4
			 0x6a0b27c1, /// 0xba8
			 0x1b8bb2b0, /// 0xbac
			 0x18250683, /// 0xbb0
			 0x63acdca7, /// 0xbb4
			 0xcc9ef06e, /// 0xbb8
			 0x01b3f649, /// 0xbbc
			 0xf5273ff4, /// 0xbc0
			 0x45aa9fce, /// 0xbc4
			 0x2a7dc515, /// 0xbc8
			 0x9b490372, /// 0xbcc
			 0x478a6371, /// 0xbd0
			 0xd1024021, /// 0xbd4
			 0x9207c2c4, /// 0xbd8
			 0x5dcec264, /// 0xbdc
			 0xd72dd33f, /// 0xbe0
			 0x42af2cb6, /// 0xbe4
			 0xc6bf9123, /// 0xbe8
			 0x5fc46a6d, /// 0xbec
			 0x37f5a15f, /// 0xbf0
			 0x0535a942, /// 0xbf4
			 0x56162e6c, /// 0xbf8
			 0x9ef98496, /// 0xbfc
			 0xb64206c7, /// 0xc00
			 0x68602e93, /// 0xc04
			 0x46b76ec9, /// 0xc08
			 0xe64c506e, /// 0xc0c
			 0xebf4af89, /// 0xc10
			 0xa19a6476, /// 0xc14
			 0x065e825d, /// 0xc18
			 0xc8927bd4, /// 0xc1c
			 0xcc718aab, /// 0xc20
			 0x4a2f9ce3, /// 0xc24
			 0xf27e4986, /// 0xc28
			 0xf34ad319, /// 0xc2c
			 0x6736a02f, /// 0xc30
			 0x9b14fa35, /// 0xc34
			 0x234b73b8, /// 0xc38
			 0xc175e7d1, /// 0xc3c
			 0x7d6e0540, /// 0xc40
			 0xd302fdf5, /// 0xc44
			 0xfac5a3c7, /// 0xc48
			 0x4a18eaf0, /// 0xc4c
			 0xfd42ebd9, /// 0xc50
			 0xf33439d4, /// 0xc54
			 0x12fdba98, /// 0xc58
			 0x7624191e, /// 0xc5c
			 0x9a545f58, /// 0xc60
			 0x30403641, /// 0xc64
			 0x0a482c98, /// 0xc68
			 0xf6bcc06f, /// 0xc6c
			 0x080a0f4e, /// 0xc70
			 0x351e7fed, /// 0xc74
			 0xbdaa7feb, /// 0xc78
			 0x7a09ea0c, /// 0xc7c
			 0xd63cad4e, /// 0xc80
			 0xcdd510fc, /// 0xc84
			 0xdcfc86f1, /// 0xc88
			 0x5fc7cc44, /// 0xc8c
			 0xa43d3e5a, /// 0xc90
			 0xe8f40f2b, /// 0xc94
			 0xc7ec5c1e, /// 0xc98
			 0x6c2d9b32, /// 0xc9c
			 0x269fa8c4, /// 0xca0
			 0xf9952288, /// 0xca4
			 0x6fdd7449, /// 0xca8
			 0xa8e1ab6d, /// 0xcac
			 0x0b2ed954, /// 0xcb0
			 0xe8c4078b, /// 0xcb4
			 0x6d01c723, /// 0xcb8
			 0xa139129a, /// 0xcbc
			 0x9b4a48f0, /// 0xcc0
			 0x53a40b7a, /// 0xcc4
			 0x88395b9e, /// 0xcc8
			 0xf570df16, /// 0xccc
			 0xd5154696, /// 0xcd0
			 0x6c8918de, /// 0xcd4
			 0xe1bc49b7, /// 0xcd8
			 0x151a7e0c, /// 0xcdc
			 0xbf2c2794, /// 0xce0
			 0xdc8adeb8, /// 0xce4
			 0xc12ae9b1, /// 0xce8
			 0xc3ab9029, /// 0xcec
			 0x981e22bc, /// 0xcf0
			 0xbfd85971, /// 0xcf4
			 0x266fb7bf, /// 0xcf8
			 0xe56ca8af, /// 0xcfc
			 0x51e50e27, /// 0xd00
			 0xc0e3fcaa, /// 0xd04
			 0x29408424, /// 0xd08
			 0x363171d5, /// 0xd0c
			 0xe17cf208, /// 0xd10
			 0x9fc11194, /// 0xd14
			 0xd8d46b19, /// 0xd18
			 0xcc478506, /// 0xd1c
			 0x0a506560, /// 0xd20
			 0x283ca996, /// 0xd24
			 0x939bb212, /// 0xd28
			 0xd28c6f0c, /// 0xd2c
			 0xa411e0ef, /// 0xd30
			 0xd8a82aed, /// 0xd34
			 0xa2e6995f, /// 0xd38
			 0x854979d6, /// 0xd3c
			 0xbb170430, /// 0xd40
			 0x84bcdedd, /// 0xd44
			 0x84838719, /// 0xd48
			 0x6d91d6e3, /// 0xd4c
			 0xb6d360cc, /// 0xd50
			 0x58789f38, /// 0xd54
			 0xc051df10, /// 0xd58
			 0x880275ea, /// 0xd5c
			 0x8c11d649, /// 0xd60
			 0xad9385d5, /// 0xd64
			 0x4ed4f10f, /// 0xd68
			 0x5e8e3c50, /// 0xd6c
			 0x4465abc6, /// 0xd70
			 0x9492b1d4, /// 0xd74
			 0x642555bb, /// 0xd78
			 0x2ed5e3b7, /// 0xd7c
			 0x8375d057, /// 0xd80
			 0x884615b6, /// 0xd84
			 0x987c59ee, /// 0xd88
			 0x866ad91d, /// 0xd8c
			 0x6efd28d2, /// 0xd90
			 0xbe592a1d, /// 0xd94
			 0x3dc07999, /// 0xd98
			 0x92e5db15, /// 0xd9c
			 0x91a5c24f, /// 0xda0
			 0xbeeaa305, /// 0xda4
			 0x8a941206, /// 0xda8
			 0x5819c955, /// 0xdac
			 0x5032df7c, /// 0xdb0
			 0xc0a0e436, /// 0xdb4
			 0x608d035a, /// 0xdb8
			 0xcefb380c, /// 0xdbc
			 0xa674ac87, /// 0xdc0
			 0xcb86c211, /// 0xdc4
			 0x4611db1a, /// 0xdc8
			 0xb688f45e, /// 0xdcc
			 0xef128e0d, /// 0xdd0
			 0x85f6b4db, /// 0xdd4
			 0xc53d3c70, /// 0xdd8
			 0xccbe04f3, /// 0xddc
			 0xae75ed72, /// 0xde0
			 0x960fd63b, /// 0xde4
			 0x68657a1a, /// 0xde8
			 0xbb8591df, /// 0xdec
			 0x587aac66, /// 0xdf0
			 0x0353d2b4, /// 0xdf4
			 0xa365e59d, /// 0xdf8
			 0x0bba5ae8, /// 0xdfc
			 0xe77c06c5, /// 0xe00
			 0xebe95504, /// 0xe04
			 0x6ad824c1, /// 0xe08
			 0x96d5b446, /// 0xe0c
			 0xca55c814, /// 0xe10
			 0x3420e34a, /// 0xe14
			 0x6659dcb7, /// 0xe18
			 0xbf487678, /// 0xe1c
			 0xf929cd9c, /// 0xe20
			 0x9cb54432, /// 0xe24
			 0xa23117d0, /// 0xe28
			 0x80d45506, /// 0xe2c
			 0x64e203da, /// 0xe30
			 0x5dd57c5b, /// 0xe34
			 0x959c0294, /// 0xe38
			 0x2e964ba1, /// 0xe3c
			 0x32f8e7a5, /// 0xe40
			 0x4bc5beb4, /// 0xe44
			 0xde401d92, /// 0xe48
			 0x8a6027f7, /// 0xe4c
			 0x5d2550c0, /// 0xe50
			 0xee15378b, /// 0xe54
			 0x14cf6fea, /// 0xe58
			 0x99b828f3, /// 0xe5c
			 0xfe8f5df0, /// 0xe60
			 0xad222070, /// 0xe64
			 0xc95a9942, /// 0xe68
			 0x7974e877, /// 0xe6c
			 0xa1f1ff89, /// 0xe70
			 0xe77d6a76, /// 0xe74
			 0x7eba13b0, /// 0xe78
			 0xd7730cc4, /// 0xe7c
			 0x436700b4, /// 0xe80
			 0x0e290e60, /// 0xe84
			 0x8c4a4f51, /// 0xe88
			 0x56bf203f, /// 0xe8c
			 0x4222c457, /// 0xe90
			 0x65c92938, /// 0xe94
			 0xd560e51d, /// 0xe98
			 0x95afcc06, /// 0xe9c
			 0x2c0d339f, /// 0xea0
			 0x6527b685, /// 0xea4
			 0x1dd86193, /// 0xea8
			 0xfcd07ba4, /// 0xeac
			 0xdb770dcc, /// 0xeb0
			 0x2d7afd56, /// 0xeb4
			 0xdf958d55, /// 0xeb8
			 0x66b15e53, /// 0xebc
			 0x0158d773, /// 0xec0
			 0x54596a4a, /// 0xec4
			 0x0c613f2b, /// 0xec8
			 0xef52b5cb, /// 0xecc
			 0xf16f0041, /// 0xed0
			 0xfcfe35fe, /// 0xed4
			 0x6d5eec9c, /// 0xed8
			 0x1f9482ff, /// 0xedc
			 0x913de8e9, /// 0xee0
			 0x39019dcf, /// 0xee4
			 0xf580dd53, /// 0xee8
			 0x74296eee, /// 0xeec
			 0x8521a5ae, /// 0xef0
			 0xe1cd825c, /// 0xef4
			 0x0470192e, /// 0xef8
			 0xbaf625a8, /// 0xefc
			 0x53e8e2b8, /// 0xf00
			 0x762ab720, /// 0xf04
			 0x2c21df36, /// 0xf08
			 0x0efd1f5a, /// 0xf0c
			 0x086a5dc5, /// 0xf10
			 0xd0ca7bba, /// 0xf14
			 0x2dba2c01, /// 0xf18
			 0x2e02bc74, /// 0xf1c
			 0x3135bc01, /// 0xf20
			 0x74da9107, /// 0xf24
			 0xee70b262, /// 0xf28
			 0x2f3efa9a, /// 0xf2c
			 0x48db93cb, /// 0xf30
			 0x8a810194, /// 0xf34
			 0x1f36de82, /// 0xf38
			 0x10712788, /// 0xf3c
			 0x6da29026, /// 0xf40
			 0x136ae32b, /// 0xf44
			 0x51a3e9d6, /// 0xf48
			 0x1b26101f, /// 0xf4c
			 0xef885c6e, /// 0xf50
			 0x27192f2f, /// 0xf54
			 0xd193abdf, /// 0xf58
			 0xe63f738c, /// 0xf5c
			 0x0a7a2921, /// 0xf60
			 0x5a5ec1f1, /// 0xf64
			 0x5642dc74, /// 0xf68
			 0x9c6caf2a, /// 0xf6c
			 0x61e4d39d, /// 0xf70
			 0x080f52a6, /// 0xf74
			 0x481e8d85, /// 0xf78
			 0x051140f0, /// 0xf7c
			 0xb6d0f8cb, /// 0xf80
			 0x658911a7, /// 0xf84
			 0xebbfea92, /// 0xf88
			 0x793453ab, /// 0xf8c
			 0x549d7833, /// 0xf90
			 0x05c6036c, /// 0xf94
			 0x2a29d3e5, /// 0xf98
			 0x807f5798, /// 0xf9c
			 0x097e1c24, /// 0xfa0
			 0x45338031, /// 0xfa4
			 0xb4a6dda7, /// 0xfa8
			 0xfe2e71cf, /// 0xfac
			 0x2488406b, /// 0xfb0
			 0x41b03c3c, /// 0xfb4
			 0x072cf2da, /// 0xfb8
			 0x079cef7f, /// 0xfbc
			 0x3ab04395, /// 0xfc0
			 0xb6ed0f15, /// 0xfc4
			 0xcfe79072, /// 0xfc8
			 0x49df7be0, /// 0xfcc
			 0xf23baf5f, /// 0xfd0
			 0x857ae419, /// 0xfd4
			 0xd738e718, /// 0xfd8
			 0x429e59b1, /// 0xfdc
			 0x9244ccec, /// 0xfe0
			 0xebcbefb0, /// 0xfe4
			 0x300cd45b, /// 0xfe8
			 0x0c35e5b1, /// 0xfec
			 0x93d9d3f2, /// 0xff0
			 0x7fef9787, /// 0xff4
			 0x118235a0, /// 0xff8
			 0x20f11917 /// last
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
			 0x0000059c,
			 0x00000550,
			 0x0000072c,
			 0x000005d8,
			 0x0000033c,
			 0x00000614,
			 0x000003bc,
			 0x0000049c,

			 /// vpu register f2
			 0x41d00000,
			 0x41980000,
			 0x41100000,
			 0x42000000,
			 0x40800000,
			 0x41a80000,
			 0x41300000,
			 0x40c00000,

			 /// vpu register f3
			 0x41f80000,
			 0x42000000,
			 0x42000000,
			 0x41e00000,
			 0x41880000,
			 0x41880000,
			 0x41a80000,
			 0x41d80000,

			 /// vpu register f4
			 0x40a00000,
			 0x41a00000,
			 0x41100000,
			 0x40a00000,
			 0x41c00000,
			 0x41000000,
			 0x40e00000,
			 0x41a80000,

			 /// vpu register f5
			 0x41400000,
			 0x41a00000,
			 0x41800000,
			 0x41900000,
			 0x41d00000,
			 0x41100000,
			 0x41800000,
			 0x42000000,

			 /// vpu register f6
			 0x41d80000,
			 0x40000000,
			 0x41e00000,
			 0x41980000,
			 0x40800000,
			 0x41880000,
			 0x40a00000,
			 0x41300000,

			 /// vpu register f7
			 0x40400000,
			 0x41b80000,
			 0x40800000,
			 0x41000000,
			 0x41b80000,
			 0x40800000,
			 0x41100000,
			 0x41880000,

			 /// vpu register f8
			 0x41b00000,
			 0x41900000,
			 0x41000000,
			 0x40000000,
			 0x41b00000,
			 0x41500000,
			 0x41c80000,
			 0x40800000,

			 /// vpu register f9
			 0x42000000,
			 0x41a80000,
			 0x41b00000,
			 0x40c00000,
			 0x41e00000,
			 0x41a00000,
			 0x40c00000,
			 0x41e00000,

			 /// vpu register f10
			 0x41e80000,
			 0x41800000,
			 0x41900000,
			 0x41b80000,
			 0x41e80000,
			 0x41900000,
			 0x41d00000,
			 0x40400000,

			 /// vpu register f11
			 0x3f800000,
			 0x42000000,
			 0x40c00000,
			 0x40000000,
			 0x40c00000,
			 0x40000000,
			 0x40a00000,
			 0x40400000,

			 /// vpu register f12
			 0x41700000,
			 0x41b00000,
			 0x41200000,
			 0x41b80000,
			 0x41f80000,
			 0x41600000,
			 0x41d80000,
			 0x41000000,

			 /// vpu register f13
			 0x40800000,
			 0x41880000,
			 0x41e80000,
			 0x41500000,
			 0x41c00000,
			 0x41100000,
			 0x41880000,
			 0x40c00000,

			 /// vpu register f14
			 0x41900000,
			 0x40000000,
			 0x41100000,
			 0x41200000,
			 0x40800000,
			 0x41e80000,
			 0x41e00000,
			 0x41700000,

			 /// vpu register f15
			 0x41a00000,
			 0x42000000,
			 0x41880000,
			 0x41c80000,
			 0x41000000,
			 0x3f800000,
			 0x41c80000,
			 0x41980000,

			 /// vpu register f16
			 0x3f800000,
			 0x40a00000,
			 0x41880000,
			 0x41700000,
			 0x41e00000,
			 0x41600000,
			 0x41a00000,
			 0x41900000,

			 /// vpu register f17
			 0x40000000,
			 0x41600000,
			 0x41980000,
			 0x40800000,
			 0x40000000,
			 0x41d00000,
			 0x40e00000,
			 0x41700000,

			 /// vpu register f18
			 0x41d00000,
			 0x41b00000,
			 0x40e00000,
			 0x3f800000,
			 0x41400000,
			 0x41e00000,
			 0x41880000,
			 0x41800000,

			 /// vpu register f19
			 0x41200000,
			 0x40000000,
			 0x41200000,
			 0x41700000,
			 0x41900000,
			 0x41000000,
			 0x40e00000,
			 0x41b00000,

			 /// vpu register f20
			 0x41400000,
			 0x40000000,
			 0x41b00000,
			 0x41300000,
			 0x41900000,
			 0x41200000,
			 0x41b00000,
			 0x41300000,

			 /// vpu register f21
			 0x41e00000,
			 0x41a00000,
			 0x40000000,
			 0x41b00000,
			 0x41e00000,
			 0x3f800000,
			 0x41900000,
			 0x41f00000,

			 /// vpu register f22
			 0x41600000,
			 0x41c00000,
			 0x40c00000,
			 0x40e00000,
			 0x41600000,
			 0x41b80000,
			 0x41a80000,
			 0x41f80000,

			 /// vpu register f23
			 0x41e00000,
			 0x41b00000,
			 0x41b00000,
			 0x42000000,
			 0x41d80000,
			 0x40000000,
			 0x41200000,
			 0x41b80000,

			 /// vpu register f24
			 0x41300000,
			 0x41e00000,
			 0x41200000,
			 0x41300000,
			 0x41d00000,
			 0x41200000,
			 0x41c00000,
			 0x40000000,

			 /// vpu register f25
			 0x41880000,
			 0x42000000,
			 0x41400000,
			 0x41c00000,
			 0x41f00000,
			 0x40c00000,
			 0x41400000,
			 0x41000000,

			 /// vpu register f26
			 0x41c80000,
			 0x41600000,
			 0x41c00000,
			 0x41c80000,
			 0x41700000,
			 0x41500000,
			 0x41d00000,
			 0x42000000,

			 /// vpu register f27
			 0x40a00000,
			 0x41a80000,
			 0x40e00000,
			 0x41100000,
			 0x40e00000,
			 0x41c80000,
			 0x41c80000,
			 0x40c00000,

			 /// vpu register f28
			 0x41a00000,
			 0x41f80000,
			 0x41d00000,
			 0x41a00000,
			 0x41980000,
			 0x41500000,
			 0x40e00000,
			 0x41d00000,

			 /// vpu register f29
			 0x41000000,
			 0x41900000,
			 0x41f00000,
			 0x41900000,
			 0x41200000,
			 0x41e00000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f30
			 0x40000000,
			 0x42000000,
			 0x40a00000,
			 0x40400000,
			 0x41d80000,
			 0x41880000,
			 0x41e80000,
			 0x41300000,

			 /// vpu register f31
			 0x41700000,
			 0x41f00000,
			 0x41500000,
			 0x41d80000,
			 0x40a00000,
			 0x40800000,
			 0x41a80000,
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
		"maskpopcz x3, m3\n"                                  ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m2\n"                                  ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m2\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m4\n"                                  ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m7\n"                                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m0\n"                                  ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m2\n"                                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m4\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m6\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m0\n"                                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m2\n"                                  ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m3\n"                                  ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m4\n"                                  ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m2\n"                                  ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m2\n"                                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m3\n"                                  ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m4\n"                                  ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m6\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m2\n"                                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m1\n"                                  ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m5\n"                                  ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m7\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m7\n"                                  ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m7\n"                                  ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m5\n"                                  ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m0\n"                                  ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m7\n"                                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m7\n"                                  ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m2\n"                                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m4\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m1\n"                                  ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m3\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m0\n"                                  ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m1\n"                                  ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m5\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m2\n"                                  ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m6\n"                                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m6\n"                                  ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m6\n"                                  ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m5\n"                                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m7\n"                                  ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m3\n"                                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m6\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m1\n"                                  ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m3\n"                                  ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m4\n"                                  ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m6\n"                                  ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m3\n"                                  ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m7\n"                                  ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m3\n"                                  ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m1\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m5\n"                                  ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m5\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m5\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m6\n"                                  ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m4\n"                                  ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m0\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m3\n"                                  ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m1\n"                                  ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m3\n"                                  ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m6\n"                                  ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m1\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m0\n"                                  ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m6\n"                                  ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m7\n"                                  ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m7\n"                                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m2\n"                                  ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m5\n"                                  ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m3\n"                                  ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m5\n"                                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m0\n"                                  ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m1\n"                                  ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m4\n"                                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m5\n"                                  ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m5\n"                                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m0\n"                                  ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m3\n"                                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m6\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m5\n"                                  ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m6\n"                                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m2\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m4\n"                                  ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m5\n"                                  ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m6\n"                                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m7\n"                                  ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m4\n"                                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m0\n"                                  ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m5\n"                                  ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m3\n"                                  ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x7, m6\n"                                  ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m1\n"                                  ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m1\n"                                  ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x9, m0\n"                                  ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x6, m7\n"                                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x4, m0\n"                                  ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m2\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m6\n"                                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x5, m5\n"                                  ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopcz x3, m6\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
