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
			 0x9ca43508, /// 0x0
			 0x878d1bf5, /// 0x4
			 0xdb887c4f, /// 0x8
			 0xd5d36f65, /// 0xc
			 0x20819a4f, /// 0x10
			 0x7b71fc19, /// 0x14
			 0x695844b2, /// 0x18
			 0xf95fe7f7, /// 0x1c
			 0x76a4cc45, /// 0x20
			 0xb2809b12, /// 0x24
			 0x7f934320, /// 0x28
			 0xd1ef6ef6, /// 0x2c
			 0x98f1349d, /// 0x30
			 0x7509335b, /// 0x34
			 0x2adb6844, /// 0x38
			 0x9d612eba, /// 0x3c
			 0x095dbe50, /// 0x40
			 0xae72e140, /// 0x44
			 0x52ccf896, /// 0x48
			 0x4aeb7298, /// 0x4c
			 0x6631ff58, /// 0x50
			 0x1f525310, /// 0x54
			 0x703ca287, /// 0x58
			 0x54ea6980, /// 0x5c
			 0xe74d442f, /// 0x60
			 0xd488f55e, /// 0x64
			 0x1125520b, /// 0x68
			 0xc92038e2, /// 0x6c
			 0x1ce7bc9a, /// 0x70
			 0xc8d461c4, /// 0x74
			 0xb41ad260, /// 0x78
			 0x91a4821d, /// 0x7c
			 0xf8eadd41, /// 0x80
			 0x569a9933, /// 0x84
			 0x858b2dbe, /// 0x88
			 0xc008a605, /// 0x8c
			 0xd754c4b2, /// 0x90
			 0xca4319d7, /// 0x94
			 0x5539e6e9, /// 0x98
			 0xad29e665, /// 0x9c
			 0xe7d02d7b, /// 0xa0
			 0x967b359e, /// 0xa4
			 0xc70c2b4b, /// 0xa8
			 0xf21416a3, /// 0xac
			 0x8f507f33, /// 0xb0
			 0x626f4cbf, /// 0xb4
			 0xc272bfac, /// 0xb8
			 0x4199e6ca, /// 0xbc
			 0xb56a3a62, /// 0xc0
			 0x312857cc, /// 0xc4
			 0xbda0d300, /// 0xc8
			 0x3948d151, /// 0xcc
			 0x62f255c7, /// 0xd0
			 0x003f387b, /// 0xd4
			 0x6b1b6ab5, /// 0xd8
			 0x3d5714c0, /// 0xdc
			 0x9c6d8cdf, /// 0xe0
			 0xaa7347f9, /// 0xe4
			 0xdaccf73a, /// 0xe8
			 0xe51aa6a5, /// 0xec
			 0xa9f96a04, /// 0xf0
			 0x6aa690d7, /// 0xf4
			 0x34c4bf21, /// 0xf8
			 0xf8342370, /// 0xfc
			 0x5fb47159, /// 0x100
			 0x225963c4, /// 0x104
			 0x54cd8e54, /// 0x108
			 0x702b1e77, /// 0x10c
			 0x36dd9a62, /// 0x110
			 0x75bf16be, /// 0x114
			 0x83303aa7, /// 0x118
			 0x12e8db5b, /// 0x11c
			 0x51b59832, /// 0x120
			 0xbc96a7ac, /// 0x124
			 0xe480ef21, /// 0x128
			 0xe7e1b43f, /// 0x12c
			 0x1b3858e5, /// 0x130
			 0x6e1722d2, /// 0x134
			 0x99965b29, /// 0x138
			 0x05357418, /// 0x13c
			 0x0f94812c, /// 0x140
			 0x8335be68, /// 0x144
			 0x6f3bb93c, /// 0x148
			 0xe4b0d2f8, /// 0x14c
			 0x84887b84, /// 0x150
			 0x2f523b47, /// 0x154
			 0xe7ec189f, /// 0x158
			 0xaa5b82b1, /// 0x15c
			 0x13097f79, /// 0x160
			 0x1c5b2299, /// 0x164
			 0xd5a960a0, /// 0x168
			 0x1ff86a76, /// 0x16c
			 0xaf12a239, /// 0x170
			 0x018457aa, /// 0x174
			 0x64d2087e, /// 0x178
			 0x88e440f8, /// 0x17c
			 0x0954d909, /// 0x180
			 0xf574aacc, /// 0x184
			 0x163d07ec, /// 0x188
			 0x1a6f7e68, /// 0x18c
			 0xc81a327c, /// 0x190
			 0xd309314a, /// 0x194
			 0xe21cfab9, /// 0x198
			 0x8ee1024c, /// 0x19c
			 0x71b76444, /// 0x1a0
			 0x5e4f114a, /// 0x1a4
			 0x9988fb3b, /// 0x1a8
			 0x6105602e, /// 0x1ac
			 0xd2557e95, /// 0x1b0
			 0xd4c05b0e, /// 0x1b4
			 0xc1da5aa2, /// 0x1b8
			 0x1326b008, /// 0x1bc
			 0xa8847cbe, /// 0x1c0
			 0x141bc751, /// 0x1c4
			 0x72133de7, /// 0x1c8
			 0x07cfd75a, /// 0x1cc
			 0x61336ce8, /// 0x1d0
			 0x2646939f, /// 0x1d4
			 0x61a189c6, /// 0x1d8
			 0xec2b8edb, /// 0x1dc
			 0xb48dfe5d, /// 0x1e0
			 0x2294af4e, /// 0x1e4
			 0xb6cf0d7b, /// 0x1e8
			 0xf5a6667b, /// 0x1ec
			 0xf0c792d7, /// 0x1f0
			 0x17c8ccf2, /// 0x1f4
			 0x8485502c, /// 0x1f8
			 0xed1063c8, /// 0x1fc
			 0x8ee12748, /// 0x200
			 0x6bde067c, /// 0x204
			 0x28d07930, /// 0x208
			 0xef3769fc, /// 0x20c
			 0xd0a60871, /// 0x210
			 0x88b3e702, /// 0x214
			 0xbf5f69c9, /// 0x218
			 0x897ba9d1, /// 0x21c
			 0x1a56d0c2, /// 0x220
			 0x641e233a, /// 0x224
			 0xb786aeb3, /// 0x228
			 0x07d39b93, /// 0x22c
			 0x2104ed39, /// 0x230
			 0x0586534a, /// 0x234
			 0x4d58b0f4, /// 0x238
			 0x661131f0, /// 0x23c
			 0xed5ae36c, /// 0x240
			 0x6a80a2de, /// 0x244
			 0x30c1e5e2, /// 0x248
			 0x43f26030, /// 0x24c
			 0x02012ac2, /// 0x250
			 0x8afa0b00, /// 0x254
			 0xa18d72c6, /// 0x258
			 0xa6132dc0, /// 0x25c
			 0x26b76f8b, /// 0x260
			 0x95b5de1f, /// 0x264
			 0xe4335d31, /// 0x268
			 0x75d7c5ad, /// 0x26c
			 0x06045e3c, /// 0x270
			 0xa07f187a, /// 0x274
			 0x071d4124, /// 0x278
			 0x909a2560, /// 0x27c
			 0x3b1a1d7f, /// 0x280
			 0x16647847, /// 0x284
			 0xf03b6e8a, /// 0x288
			 0xd866abda, /// 0x28c
			 0x14af9767, /// 0x290
			 0x1a5f7d8f, /// 0x294
			 0x5f60a04c, /// 0x298
			 0x838b1bef, /// 0x29c
			 0x88fdbc19, /// 0x2a0
			 0x97b8c21d, /// 0x2a4
			 0xf593a3ef, /// 0x2a8
			 0xdf6dcce0, /// 0x2ac
			 0x69932a52, /// 0x2b0
			 0xdb415822, /// 0x2b4
			 0x2ffa979b, /// 0x2b8
			 0xb92228f4, /// 0x2bc
			 0x66c43e28, /// 0x2c0
			 0xb2d47071, /// 0x2c4
			 0x4dbb3781, /// 0x2c8
			 0xbb861eef, /// 0x2cc
			 0x0b163b3b, /// 0x2d0
			 0xaf6baaa0, /// 0x2d4
			 0xa342c804, /// 0x2d8
			 0xf68a8c11, /// 0x2dc
			 0xba15a59d, /// 0x2e0
			 0xd2b5048d, /// 0x2e4
			 0x475b2f58, /// 0x2e8
			 0x28012110, /// 0x2ec
			 0x677a8b37, /// 0x2f0
			 0xfc96f4b2, /// 0x2f4
			 0xf5720331, /// 0x2f8
			 0x18a9ea2b, /// 0x2fc
			 0xf3cbfbff, /// 0x300
			 0x93172159, /// 0x304
			 0x9b723784, /// 0x308
			 0x028613e3, /// 0x30c
			 0xf0c9cb69, /// 0x310
			 0x1755e4d2, /// 0x314
			 0x41c3da23, /// 0x318
			 0x30f4d51b, /// 0x31c
			 0xa63b151f, /// 0x320
			 0x2e2a8bbe, /// 0x324
			 0x65fc92b9, /// 0x328
			 0x0a2099e7, /// 0x32c
			 0xb5900ad0, /// 0x330
			 0xaab2e985, /// 0x334
			 0xa6318ced, /// 0x338
			 0x49cf4656, /// 0x33c
			 0x15f0e4c0, /// 0x340
			 0x179f7c9b, /// 0x344
			 0xb43abf31, /// 0x348
			 0x292c7ac1, /// 0x34c
			 0x2270df8d, /// 0x350
			 0xace31a19, /// 0x354
			 0xad55669a, /// 0x358
			 0x67b1cdd9, /// 0x35c
			 0x6be8d8b5, /// 0x360
			 0x7c8ada60, /// 0x364
			 0x19108488, /// 0x368
			 0x314c2d39, /// 0x36c
			 0x71eb91a1, /// 0x370
			 0xba8ffe51, /// 0x374
			 0x5501ef30, /// 0x378
			 0xe9577be6, /// 0x37c
			 0x0e77bff2, /// 0x380
			 0x00cc2f59, /// 0x384
			 0x0d980b33, /// 0x388
			 0xa9ed6938, /// 0x38c
			 0x04f9f53c, /// 0x390
			 0xc43284de, /// 0x394
			 0xa4ae75ae, /// 0x398
			 0xec87e9b2, /// 0x39c
			 0xca38327f, /// 0x3a0
			 0x2d470312, /// 0x3a4
			 0x3044c033, /// 0x3a8
			 0x8dc10c73, /// 0x3ac
			 0xcf67b2c2, /// 0x3b0
			 0xe7c1312c, /// 0x3b4
			 0xdb8f916e, /// 0x3b8
			 0xbfe392af, /// 0x3bc
			 0x0ebd0d8f, /// 0x3c0
			 0x57e68603, /// 0x3c4
			 0x9bce6e61, /// 0x3c8
			 0xafb34ea6, /// 0x3cc
			 0xc554a5f8, /// 0x3d0
			 0x785d51a0, /// 0x3d4
			 0xfe7a6259, /// 0x3d8
			 0xeb68c899, /// 0x3dc
			 0x47bc8e5d, /// 0x3e0
			 0x038aa9b7, /// 0x3e4
			 0xa210381f, /// 0x3e8
			 0x33d2f891, /// 0x3ec
			 0x6be663d1, /// 0x3f0
			 0xbdf5aab3, /// 0x3f4
			 0x47bd524c, /// 0x3f8
			 0x575eeba8, /// 0x3fc
			 0x9d52d0ec, /// 0x400
			 0x037b40d5, /// 0x404
			 0xd14a14e9, /// 0x408
			 0xe185e50c, /// 0x40c
			 0x38326ac4, /// 0x410
			 0x10aeea11, /// 0x414
			 0x2d7561d5, /// 0x418
			 0x03006092, /// 0x41c
			 0x80a5e54e, /// 0x420
			 0x9aab8c90, /// 0x424
			 0x4ce8775a, /// 0x428
			 0x73744ced, /// 0x42c
			 0x36be2f60, /// 0x430
			 0x45babb68, /// 0x434
			 0x63eff00f, /// 0x438
			 0xf27b77a6, /// 0x43c
			 0x0781c6be, /// 0x440
			 0xd0423f56, /// 0x444
			 0xc1b08d99, /// 0x448
			 0xc87dae4c, /// 0x44c
			 0xf0bbc7ed, /// 0x450
			 0xcf16ac40, /// 0x454
			 0x74a46dd5, /// 0x458
			 0xc22b716a, /// 0x45c
			 0xc5ec8c96, /// 0x460
			 0x9f488487, /// 0x464
			 0xc1151e51, /// 0x468
			 0x2d2f26a3, /// 0x46c
			 0x9a1251a0, /// 0x470
			 0x7c0069e1, /// 0x474
			 0xbec3ddb2, /// 0x478
			 0x5e91b3c6, /// 0x47c
			 0x1145b0fb, /// 0x480
			 0xdf356fa7, /// 0x484
			 0xdd5c6fcf, /// 0x488
			 0x3734b704, /// 0x48c
			 0xcf3176bc, /// 0x490
			 0xe97bf4bc, /// 0x494
			 0x06617483, /// 0x498
			 0xc7e5f24c, /// 0x49c
			 0x4896c0e3, /// 0x4a0
			 0x3a51b27e, /// 0x4a4
			 0x3a7ca04d, /// 0x4a8
			 0x1dfde25b, /// 0x4ac
			 0xfb8ab7be, /// 0x4b0
			 0xbc21807c, /// 0x4b4
			 0xc12444f8, /// 0x4b8
			 0x9fec8b45, /// 0x4bc
			 0xdf0ac833, /// 0x4c0
			 0x53caf5f1, /// 0x4c4
			 0x86469d07, /// 0x4c8
			 0x4fbd2bb5, /// 0x4cc
			 0xad0e43b7, /// 0x4d0
			 0x20a06d73, /// 0x4d4
			 0x7149cee1, /// 0x4d8
			 0xb4964889, /// 0x4dc
			 0x7d55472b, /// 0x4e0
			 0x9a6ad4fc, /// 0x4e4
			 0xe83691e9, /// 0x4e8
			 0xb3466e04, /// 0x4ec
			 0xf88aa178, /// 0x4f0
			 0x06816f5a, /// 0x4f4
			 0x8a9c8c5e, /// 0x4f8
			 0xb2f50f91, /// 0x4fc
			 0xe6b1997b, /// 0x500
			 0xca1be909, /// 0x504
			 0xa2baf511, /// 0x508
			 0xf39b35db, /// 0x50c
			 0xee505587, /// 0x510
			 0x5e9a8008, /// 0x514
			 0x200ba983, /// 0x518
			 0x3b769694, /// 0x51c
			 0xc7b5c72d, /// 0x520
			 0xfb0858c0, /// 0x524
			 0x8d17c071, /// 0x528
			 0x90c5f8d7, /// 0x52c
			 0xb79cb99b, /// 0x530
			 0x2137ee7e, /// 0x534
			 0x1f3d9d33, /// 0x538
			 0x5cbd39e0, /// 0x53c
			 0x0fbeb43d, /// 0x540
			 0xdc719fcd, /// 0x544
			 0xc0d289b6, /// 0x548
			 0x93d88ebe, /// 0x54c
			 0x6c239cb4, /// 0x550
			 0xb564edbb, /// 0x554
			 0xf94267f5, /// 0x558
			 0x86615a58, /// 0x55c
			 0x4883e134, /// 0x560
			 0x3ca33d8f, /// 0x564
			 0x3ffc8ef2, /// 0x568
			 0xc5b3697d, /// 0x56c
			 0x1be52a6c, /// 0x570
			 0x88880d0f, /// 0x574
			 0x84877f0f, /// 0x578
			 0xbc3ff126, /// 0x57c
			 0x165872b2, /// 0x580
			 0x5b657575, /// 0x584
			 0xd7d4d0c5, /// 0x588
			 0xbf9ca377, /// 0x58c
			 0x192dd01d, /// 0x590
			 0x3fc9b691, /// 0x594
			 0x5d8b82fc, /// 0x598
			 0xd8aefee1, /// 0x59c
			 0x06bea332, /// 0x5a0
			 0xe7a96755, /// 0x5a4
			 0x7f91f328, /// 0x5a8
			 0x8d2c17f1, /// 0x5ac
			 0x00fef9ba, /// 0x5b0
			 0x8f4beb00, /// 0x5b4
			 0x2b168959, /// 0x5b8
			 0xb6552976, /// 0x5bc
			 0xe36b635c, /// 0x5c0
			 0x25aac5bd, /// 0x5c4
			 0x20f7a1c5, /// 0x5c8
			 0x3d0e70d1, /// 0x5cc
			 0x284e2d43, /// 0x5d0
			 0x1a4e3e58, /// 0x5d4
			 0xc2b895b3, /// 0x5d8
			 0x4eec3985, /// 0x5dc
			 0xf6a933a7, /// 0x5e0
			 0x95c5531d, /// 0x5e4
			 0x6a1958f0, /// 0x5e8
			 0x4b7dc77f, /// 0x5ec
			 0xb8bf829f, /// 0x5f0
			 0xc98bcc00, /// 0x5f4
			 0xb4040200, /// 0x5f8
			 0x8b7bd48f, /// 0x5fc
			 0x933261dd, /// 0x600
			 0xde3989c3, /// 0x604
			 0x34c263ef, /// 0x608
			 0xb7594bcc, /// 0x60c
			 0x58f6859c, /// 0x610
			 0xa96b561f, /// 0x614
			 0xf1b963b5, /// 0x618
			 0x8035acb3, /// 0x61c
			 0xf1965a05, /// 0x620
			 0xa7f22a04, /// 0x624
			 0xea87bc9e, /// 0x628
			 0x569cc9dd, /// 0x62c
			 0x2acd8ce9, /// 0x630
			 0xa7f5a53f, /// 0x634
			 0x9283c9a7, /// 0x638
			 0x7b03daec, /// 0x63c
			 0xa97082a7, /// 0x640
			 0x54d7c3e3, /// 0x644
			 0x910548e4, /// 0x648
			 0x87fc070f, /// 0x64c
			 0x4bf778fd, /// 0x650
			 0x5695a4ae, /// 0x654
			 0x34548b75, /// 0x658
			 0x086c3684, /// 0x65c
			 0x694d152a, /// 0x660
			 0x80183c02, /// 0x664
			 0x29bb96a2, /// 0x668
			 0x5bd42a46, /// 0x66c
			 0xa10071f2, /// 0x670
			 0x21422b83, /// 0x674
			 0xbd84268c, /// 0x678
			 0x2b69614b, /// 0x67c
			 0xb9a2b9da, /// 0x680
			 0x8f596738, /// 0x684
			 0xc66dddfe, /// 0x688
			 0x5b9f52a9, /// 0x68c
			 0xf00046ae, /// 0x690
			 0x2e2e67e0, /// 0x694
			 0x90906378, /// 0x698
			 0x62bba7a2, /// 0x69c
			 0xeac6b113, /// 0x6a0
			 0x63d571d3, /// 0x6a4
			 0x72d3fce3, /// 0x6a8
			 0x5a335970, /// 0x6ac
			 0xca411a6d, /// 0x6b0
			 0x4e562e9f, /// 0x6b4
			 0x02bb21a0, /// 0x6b8
			 0xa423ecb3, /// 0x6bc
			 0x022d45a7, /// 0x6c0
			 0xd8812cf5, /// 0x6c4
			 0x534559c0, /// 0x6c8
			 0xadb39f19, /// 0x6cc
			 0xd13826a5, /// 0x6d0
			 0x1b069f30, /// 0x6d4
			 0x156153b1, /// 0x6d8
			 0x5ce636f3, /// 0x6dc
			 0xec3fb8ad, /// 0x6e0
			 0x881dd84b, /// 0x6e4
			 0xf6df9b1e, /// 0x6e8
			 0xcad06aca, /// 0x6ec
			 0x061771dc, /// 0x6f0
			 0x40762b76, /// 0x6f4
			 0xdb721aa7, /// 0x6f8
			 0xcc8ad95b, /// 0x6fc
			 0x92bf9a99, /// 0x700
			 0x6cbb2c23, /// 0x704
			 0xae1c405d, /// 0x708
			 0xd3c7279e, /// 0x70c
			 0xbef16c4b, /// 0x710
			 0xf0330b6d, /// 0x714
			 0xd7174da1, /// 0x718
			 0x4187c5c2, /// 0x71c
			 0xb11ec1e1, /// 0x720
			 0xe81abcf9, /// 0x724
			 0x9c6f1ac6, /// 0x728
			 0xa3819342, /// 0x72c
			 0x77512bf1, /// 0x730
			 0x71810951, /// 0x734
			 0xffd66943, /// 0x738
			 0x3fba8e7f, /// 0x73c
			 0xb2053814, /// 0x740
			 0x3fcc825e, /// 0x744
			 0x157e84c4, /// 0x748
			 0xde6ab918, /// 0x74c
			 0xaecfe39c, /// 0x750
			 0xec4cf5d8, /// 0x754
			 0x4ba74288, /// 0x758
			 0xadfcb0ef, /// 0x75c
			 0xc3c9fdba, /// 0x760
			 0xc605021f, /// 0x764
			 0xe2905672, /// 0x768
			 0x76345276, /// 0x76c
			 0x1dabe170, /// 0x770
			 0x5282ac00, /// 0x774
			 0xd964738b, /// 0x778
			 0x9667cac5, /// 0x77c
			 0xbc7bc8d0, /// 0x780
			 0x63112261, /// 0x784
			 0x7c68f939, /// 0x788
			 0x70bfc1f5, /// 0x78c
			 0xf3f0ba2c, /// 0x790
			 0x8d9f712b, /// 0x794
			 0xe23505ec, /// 0x798
			 0xe71a3c72, /// 0x79c
			 0xaa194169, /// 0x7a0
			 0x65d6d0b8, /// 0x7a4
			 0x151eb4fa, /// 0x7a8
			 0x5e1822e6, /// 0x7ac
			 0xda583ead, /// 0x7b0
			 0x43555e8e, /// 0x7b4
			 0xfcf9075c, /// 0x7b8
			 0x29a86a12, /// 0x7bc
			 0xb1f71e7c, /// 0x7c0
			 0xdb1432ad, /// 0x7c4
			 0xc45315fe, /// 0x7c8
			 0x46c5411c, /// 0x7cc
			 0x4ba4b2df, /// 0x7d0
			 0x7f16f42f, /// 0x7d4
			 0x470580c3, /// 0x7d8
			 0xfd60c412, /// 0x7dc
			 0xe1ab2519, /// 0x7e0
			 0x0d857a6d, /// 0x7e4
			 0x3dd8849a, /// 0x7e8
			 0xa5473427, /// 0x7ec
			 0xc418c298, /// 0x7f0
			 0xd5b2de03, /// 0x7f4
			 0x38706f64, /// 0x7f8
			 0xccc4c216, /// 0x7fc
			 0x15caa700, /// 0x800
			 0x18463c0b, /// 0x804
			 0x8a1cf36d, /// 0x808
			 0xefe841d2, /// 0x80c
			 0xb21e86e1, /// 0x810
			 0x154801cd, /// 0x814
			 0x414231a5, /// 0x818
			 0xa2970208, /// 0x81c
			 0xc7d6fbdb, /// 0x820
			 0x10e710e1, /// 0x824
			 0x4340aa6f, /// 0x828
			 0x3267ba16, /// 0x82c
			 0x2171ce4a, /// 0x830
			 0x52608c6c, /// 0x834
			 0xd82fb9f2, /// 0x838
			 0x8c113868, /// 0x83c
			 0xcbb6bf4d, /// 0x840
			 0xb7e58bc2, /// 0x844
			 0x4385a3ba, /// 0x848
			 0xa75e2a4e, /// 0x84c
			 0xca63ba3e, /// 0x850
			 0x8f195529, /// 0x854
			 0x04a3bd0f, /// 0x858
			 0xeae4f330, /// 0x85c
			 0x46279f32, /// 0x860
			 0x51b3c5d8, /// 0x864
			 0xb3e11f77, /// 0x868
			 0x9c3bdeb1, /// 0x86c
			 0xf21ff087, /// 0x870
			 0xe8b9d7de, /// 0x874
			 0x7483c588, /// 0x878
			 0x9e85d78c, /// 0x87c
			 0x5f6c3139, /// 0x880
			 0xaf34bf5d, /// 0x884
			 0x08ba4d7d, /// 0x888
			 0x7edcd026, /// 0x88c
			 0x42240c3e, /// 0x890
			 0xc0fa698f, /// 0x894
			 0x32a730de, /// 0x898
			 0x28191b9a, /// 0x89c
			 0xc494bb76, /// 0x8a0
			 0x27f24d22, /// 0x8a4
			 0x199abc97, /// 0x8a8
			 0x6d0986b4, /// 0x8ac
			 0x08989d5e, /// 0x8b0
			 0x597783c0, /// 0x8b4
			 0x40509cff, /// 0x8b8
			 0x3584afe1, /// 0x8bc
			 0x721de9db, /// 0x8c0
			 0x42a9f100, /// 0x8c4
			 0xa9dce25f, /// 0x8c8
			 0x6fb6d557, /// 0x8cc
			 0xe2b2799b, /// 0x8d0
			 0x2f6b4bd2, /// 0x8d4
			 0x29871ab6, /// 0x8d8
			 0x6b5d9976, /// 0x8dc
			 0xde1e4464, /// 0x8e0
			 0xc027968e, /// 0x8e4
			 0x9d09ba9d, /// 0x8e8
			 0xeee81632, /// 0x8ec
			 0xe278695c, /// 0x8f0
			 0x7088d248, /// 0x8f4
			 0x38e0b041, /// 0x8f8
			 0xb92f3277, /// 0x8fc
			 0x4267a452, /// 0x900
			 0x2e1ef1bc, /// 0x904
			 0x4c5583cc, /// 0x908
			 0x4a0bb476, /// 0x90c
			 0x5078d66e, /// 0x910
			 0xbcaac695, /// 0x914
			 0x378fa111, /// 0x918
			 0x1c2c2c67, /// 0x91c
			 0xef2e5c0f, /// 0x920
			 0x3992a44a, /// 0x924
			 0x1c3a6559, /// 0x928
			 0x939bb2d1, /// 0x92c
			 0xaee93be3, /// 0x930
			 0xb710a489, /// 0x934
			 0xa86ae099, /// 0x938
			 0x8aa877fd, /// 0x93c
			 0x7bd3d375, /// 0x940
			 0x965308ef, /// 0x944
			 0x2ccac81f, /// 0x948
			 0xd67288cb, /// 0x94c
			 0x5d21a629, /// 0x950
			 0x99f310d1, /// 0x954
			 0x4998cafb, /// 0x958
			 0x7ecf4eb6, /// 0x95c
			 0x0b91dcf8, /// 0x960
			 0x9170963d, /// 0x964
			 0xa690d5fa, /// 0x968
			 0xc6c9b310, /// 0x96c
			 0xda6a497b, /// 0x970
			 0xe554dbde, /// 0x974
			 0x9f655cd6, /// 0x978
			 0x484af2bd, /// 0x97c
			 0xfa9306b2, /// 0x980
			 0x41e776d1, /// 0x984
			 0x32d9c978, /// 0x988
			 0x5143f353, /// 0x98c
			 0x9c645ff4, /// 0x990
			 0x27179ebe, /// 0x994
			 0x60b6f659, /// 0x998
			 0x84256e26, /// 0x99c
			 0x0c7c320c, /// 0x9a0
			 0x1355088d, /// 0x9a4
			 0xd06bed1f, /// 0x9a8
			 0xdd413f15, /// 0x9ac
			 0xdfb18fea, /// 0x9b0
			 0xc004b093, /// 0x9b4
			 0x8f061e01, /// 0x9b8
			 0x709099c6, /// 0x9bc
			 0xe2567db2, /// 0x9c0
			 0x496fa6dc, /// 0x9c4
			 0x8469559f, /// 0x9c8
			 0xd5c8d7dd, /// 0x9cc
			 0x52ea248d, /// 0x9d0
			 0x48b921c6, /// 0x9d4
			 0x844df2ff, /// 0x9d8
			 0xe4c09ffa, /// 0x9dc
			 0xe293efca, /// 0x9e0
			 0xd5e1345b, /// 0x9e4
			 0x16ac0cbf, /// 0x9e8
			 0xd713ef19, /// 0x9ec
			 0x6bbb1893, /// 0x9f0
			 0x601f3495, /// 0x9f4
			 0x7b388bd1, /// 0x9f8
			 0xab33032f, /// 0x9fc
			 0x35991967, /// 0xa00
			 0x02af16d4, /// 0xa04
			 0x9a5dc5be, /// 0xa08
			 0x7809b2e6, /// 0xa0c
			 0xd1809b4a, /// 0xa10
			 0x0ad9c744, /// 0xa14
			 0x023f735b, /// 0xa18
			 0x3d7fba73, /// 0xa1c
			 0x6ea7fd81, /// 0xa20
			 0x1d6f0d0e, /// 0xa24
			 0x0f802a74, /// 0xa28
			 0x044323db, /// 0xa2c
			 0x2fd7a52f, /// 0xa30
			 0xb7622419, /// 0xa34
			 0x7dc14695, /// 0xa38
			 0xa9597a50, /// 0xa3c
			 0x702b6951, /// 0xa40
			 0x08d567eb, /// 0xa44
			 0x756cd7df, /// 0xa48
			 0x26953554, /// 0xa4c
			 0xba3bde98, /// 0xa50
			 0xc322cd01, /// 0xa54
			 0x0c0a7f47, /// 0xa58
			 0x4801a16e, /// 0xa5c
			 0xe5b93efb, /// 0xa60
			 0x0e93c2ee, /// 0xa64
			 0x0d87d7c2, /// 0xa68
			 0x03031d2c, /// 0xa6c
			 0xacdbafba, /// 0xa70
			 0x745ab6f0, /// 0xa74
			 0x669e7cf5, /// 0xa78
			 0x8bfa1028, /// 0xa7c
			 0xe9621096, /// 0xa80
			 0x2053bb33, /// 0xa84
			 0x6533d747, /// 0xa88
			 0x03030d75, /// 0xa8c
			 0xea2da26f, /// 0xa90
			 0xef45c600, /// 0xa94
			 0x0953861c, /// 0xa98
			 0xea2b4fdd, /// 0xa9c
			 0x2716431c, /// 0xaa0
			 0x4538cbf5, /// 0xaa4
			 0xfe33df07, /// 0xaa8
			 0x941508a5, /// 0xaac
			 0x615aa975, /// 0xab0
			 0x0b6e141e, /// 0xab4
			 0x02f36679, /// 0xab8
			 0x495b260c, /// 0xabc
			 0x470381ca, /// 0xac0
			 0x95876fba, /// 0xac4
			 0xd7bf0b38, /// 0xac8
			 0x8f2b5c74, /// 0xacc
			 0xe9c609ef, /// 0xad0
			 0x5fe29e1c, /// 0xad4
			 0x2c6bab7f, /// 0xad8
			 0x35ddf2b4, /// 0xadc
			 0xc6c021b9, /// 0xae0
			 0xdb8b749c, /// 0xae4
			 0xf69a98d1, /// 0xae8
			 0xb77e8c6f, /// 0xaec
			 0x78a5b2a0, /// 0xaf0
			 0xb8e127b9, /// 0xaf4
			 0x5293e433, /// 0xaf8
			 0x770dbf0a, /// 0xafc
			 0xad4b9010, /// 0xb00
			 0x37a5968d, /// 0xb04
			 0xf62ebed2, /// 0xb08
			 0x3e9bf58c, /// 0xb0c
			 0xab27762d, /// 0xb10
			 0x5294fc60, /// 0xb14
			 0xf1460297, /// 0xb18
			 0xd127d9e9, /// 0xb1c
			 0x757ef6f0, /// 0xb20
			 0xb6f70a9d, /// 0xb24
			 0x9d68ae18, /// 0xb28
			 0xe371faa4, /// 0xb2c
			 0x99a53d13, /// 0xb30
			 0x4eeed7be, /// 0xb34
			 0xc4be233d, /// 0xb38
			 0x059a7e94, /// 0xb3c
			 0x29a56779, /// 0xb40
			 0x10faa174, /// 0xb44
			 0x8d1673ea, /// 0xb48
			 0x0342ce37, /// 0xb4c
			 0x494bfe95, /// 0xb50
			 0x6d643d3b, /// 0xb54
			 0xa5cc48ef, /// 0xb58
			 0xdaa4fbae, /// 0xb5c
			 0xa9e3da79, /// 0xb60
			 0x8f3c3e9e, /// 0xb64
			 0x9e2b2948, /// 0xb68
			 0xcf946c2a, /// 0xb6c
			 0xc8c86f3d, /// 0xb70
			 0xded9d33c, /// 0xb74
			 0xd3a683ba, /// 0xb78
			 0xfcd85e34, /// 0xb7c
			 0xe4edaa34, /// 0xb80
			 0x136ac7a4, /// 0xb84
			 0x061a3eaa, /// 0xb88
			 0x1076bad2, /// 0xb8c
			 0x2da26afe, /// 0xb90
			 0x10b801bb, /// 0xb94
			 0x67f5b8f4, /// 0xb98
			 0x71dfce70, /// 0xb9c
			 0x5d428b84, /// 0xba0
			 0xb33cf9fb, /// 0xba4
			 0x1c8c9b41, /// 0xba8
			 0x7a3795ee, /// 0xbac
			 0x21def9b8, /// 0xbb0
			 0xacce7b05, /// 0xbb4
			 0x6fc7b6fb, /// 0xbb8
			 0x05a4abb9, /// 0xbbc
			 0xb2872327, /// 0xbc0
			 0x061d864e, /// 0xbc4
			 0xa2626c09, /// 0xbc8
			 0x08fe6390, /// 0xbcc
			 0x2506ba28, /// 0xbd0
			 0x06ffdf49, /// 0xbd4
			 0x05b96735, /// 0xbd8
			 0xe2b77d82, /// 0xbdc
			 0xccf12460, /// 0xbe0
			 0x232fafb5, /// 0xbe4
			 0x0bc81e6b, /// 0xbe8
			 0x2cc5e49b, /// 0xbec
			 0xd5fa4c88, /// 0xbf0
			 0x476312e0, /// 0xbf4
			 0xc023f9ff, /// 0xbf8
			 0x47aef47e, /// 0xbfc
			 0x873cb27a, /// 0xc00
			 0xaf7990ea, /// 0xc04
			 0xa5768ed2, /// 0xc08
			 0x2748103a, /// 0xc0c
			 0xebc490c5, /// 0xc10
			 0xc6bfcce3, /// 0xc14
			 0xaa96369f, /// 0xc18
			 0x0cd01b81, /// 0xc1c
			 0x5af2f58d, /// 0xc20
			 0xb6c86dc4, /// 0xc24
			 0xc98af989, /// 0xc28
			 0x60c916f4, /// 0xc2c
			 0x2addd270, /// 0xc30
			 0xd3035b04, /// 0xc34
			 0xacce3ce6, /// 0xc38
			 0x698a315b, /// 0xc3c
			 0xed7369b9, /// 0xc40
			 0x6d7a12c8, /// 0xc44
			 0x61fc63e0, /// 0xc48
			 0x8b7c8d4f, /// 0xc4c
			 0xc80fed81, /// 0xc50
			 0xd2c13010, /// 0xc54
			 0xe1ee4ac6, /// 0xc58
			 0xf41768ca, /// 0xc5c
			 0x6ff77778, /// 0xc60
			 0x0acfe429, /// 0xc64
			 0xae592f9a, /// 0xc68
			 0xbf571818, /// 0xc6c
			 0x6aa981f5, /// 0xc70
			 0xd555e9b6, /// 0xc74
			 0xec8a6dd8, /// 0xc78
			 0xa0f36490, /// 0xc7c
			 0x73955073, /// 0xc80
			 0xb29469b2, /// 0xc84
			 0x9816f279, /// 0xc88
			 0xc89003fd, /// 0xc8c
			 0x4abbb5e9, /// 0xc90
			 0x09cf7380, /// 0xc94
			 0x28a90bda, /// 0xc98
			 0x6ffb97f3, /// 0xc9c
			 0x10e51d8c, /// 0xca0
			 0x030cfb42, /// 0xca4
			 0x43796931, /// 0xca8
			 0x5877b2a3, /// 0xcac
			 0xc09e1e4f, /// 0xcb0
			 0x425cebc6, /// 0xcb4
			 0x2f77f1cc, /// 0xcb8
			 0xc94b0768, /// 0xcbc
			 0xeb887de9, /// 0xcc0
			 0x54f78fcb, /// 0xcc4
			 0xfa2e7b35, /// 0xcc8
			 0xf594b444, /// 0xccc
			 0xf4fcb216, /// 0xcd0
			 0xaba8c7cb, /// 0xcd4
			 0x839c4265, /// 0xcd8
			 0x4bbbd00f, /// 0xcdc
			 0xc9f49cc3, /// 0xce0
			 0x116cdde3, /// 0xce4
			 0x07b1b29e, /// 0xce8
			 0xc9424f39, /// 0xcec
			 0x1c9df14a, /// 0xcf0
			 0x21dc8311, /// 0xcf4
			 0x0704e810, /// 0xcf8
			 0xd16f2a7f, /// 0xcfc
			 0x9277d256, /// 0xd00
			 0x39f3af1e, /// 0xd04
			 0xb95f07fe, /// 0xd08
			 0x56ead64b, /// 0xd0c
			 0x163e97b4, /// 0xd10
			 0x7de044c8, /// 0xd14
			 0xd8892149, /// 0xd18
			 0x7dbec939, /// 0xd1c
			 0xafe9418f, /// 0xd20
			 0x47813904, /// 0xd24
			 0x398831aa, /// 0xd28
			 0x5edde88c, /// 0xd2c
			 0x5c3d435e, /// 0xd30
			 0xb956df3b, /// 0xd34
			 0xa97a938e, /// 0xd38
			 0x2239c762, /// 0xd3c
			 0xc43b7cf3, /// 0xd40
			 0xce33dc2a, /// 0xd44
			 0x4b3c810d, /// 0xd48
			 0x071c6888, /// 0xd4c
			 0x07fced84, /// 0xd50
			 0xfd56b753, /// 0xd54
			 0xd6a212bf, /// 0xd58
			 0xfb24df7b, /// 0xd5c
			 0xf121b0f7, /// 0xd60
			 0x4d2c7cbc, /// 0xd64
			 0xda00d643, /// 0xd68
			 0xc5440524, /// 0xd6c
			 0xdef024b3, /// 0xd70
			 0x56c9720b, /// 0xd74
			 0xe9a7a264, /// 0xd78
			 0xd1fdee99, /// 0xd7c
			 0xc7bbebc9, /// 0xd80
			 0x2218ffa8, /// 0xd84
			 0x1031575f, /// 0xd88
			 0xa23692d9, /// 0xd8c
			 0x3418b58e, /// 0xd90
			 0xd166d864, /// 0xd94
			 0xb3d51a25, /// 0xd98
			 0xae3c59c9, /// 0xd9c
			 0xc952f4c1, /// 0xda0
			 0xd1a05cdb, /// 0xda4
			 0xabfa194f, /// 0xda8
			 0xac258cf4, /// 0xdac
			 0x76f42f13, /// 0xdb0
			 0x6ba0024c, /// 0xdb4
			 0xff0eb4e5, /// 0xdb8
			 0x5fb7ab15, /// 0xdbc
			 0xe4e6552e, /// 0xdc0
			 0xb6c012d2, /// 0xdc4
			 0xfdebab7d, /// 0xdc8
			 0x82660a5b, /// 0xdcc
			 0xc21ad99a, /// 0xdd0
			 0x8021782e, /// 0xdd4
			 0xa1d603f4, /// 0xdd8
			 0xa5edfc22, /// 0xddc
			 0x8dcfb94c, /// 0xde0
			 0xfc7c2e4b, /// 0xde4
			 0xcf0b6984, /// 0xde8
			 0xf5ab1b8e, /// 0xdec
			 0x709943e6, /// 0xdf0
			 0x881b788f, /// 0xdf4
			 0x98a7bf90, /// 0xdf8
			 0xc56a70b1, /// 0xdfc
			 0x07e53f72, /// 0xe00
			 0x4c3c67d0, /// 0xe04
			 0xa91fa981, /// 0xe08
			 0xb45c4f59, /// 0xe0c
			 0x89e4e61a, /// 0xe10
			 0x4c5b4328, /// 0xe14
			 0xa68f9023, /// 0xe18
			 0x6c489a3e, /// 0xe1c
			 0x0c69d32f, /// 0xe20
			 0x4fecd736, /// 0xe24
			 0x18d0faeb, /// 0xe28
			 0x5f5144b9, /// 0xe2c
			 0x08c3ebc1, /// 0xe30
			 0xf451ae0d, /// 0xe34
			 0xf46be5f5, /// 0xe38
			 0x807420fa, /// 0xe3c
			 0xa8b72fa0, /// 0xe40
			 0x7c07e74d, /// 0xe44
			 0xa7021b31, /// 0xe48
			 0xf5fb333a, /// 0xe4c
			 0x5e3175d8, /// 0xe50
			 0x394c2150, /// 0xe54
			 0xfbaa4a3b, /// 0xe58
			 0x9addeee0, /// 0xe5c
			 0x26a4519d, /// 0xe60
			 0x69d9ea43, /// 0xe64
			 0xdf42f522, /// 0xe68
			 0x2e08db8d, /// 0xe6c
			 0xf824d270, /// 0xe70
			 0x59dc0797, /// 0xe74
			 0x9cead97a, /// 0xe78
			 0x248cd455, /// 0xe7c
			 0xf57ce32c, /// 0xe80
			 0x4a4ec9d2, /// 0xe84
			 0xb2b7e1ad, /// 0xe88
			 0x146ebb47, /// 0xe8c
			 0x83740bd1, /// 0xe90
			 0xca188f6e, /// 0xe94
			 0xb38a8616, /// 0xe98
			 0xbc7dbdfa, /// 0xe9c
			 0x51b6e4e0, /// 0xea0
			 0xd615ef63, /// 0xea4
			 0xe7f38ffd, /// 0xea8
			 0xe03b873b, /// 0xeac
			 0xcef9cd2e, /// 0xeb0
			 0x446daead, /// 0xeb4
			 0x2a2d5fb1, /// 0xeb8
			 0xee91f7db, /// 0xebc
			 0x18a07857, /// 0xec0
			 0x8db73fda, /// 0xec4
			 0xa9525931, /// 0xec8
			 0x3a60983f, /// 0xecc
			 0x857898bc, /// 0xed0
			 0xfbd43ef5, /// 0xed4
			 0xb8a085ef, /// 0xed8
			 0x8e25f6a1, /// 0xedc
			 0x1eb4c2ac, /// 0xee0
			 0xc7bf2926, /// 0xee4
			 0x204972b9, /// 0xee8
			 0x1bdda2f4, /// 0xeec
			 0x1098d74b, /// 0xef0
			 0x06df3580, /// 0xef4
			 0xb345a037, /// 0xef8
			 0x88d56c7e, /// 0xefc
			 0x1ac9b5fc, /// 0xf00
			 0xc1af15a5, /// 0xf04
			 0x0efc27ac, /// 0xf08
			 0xe041dd20, /// 0xf0c
			 0x94d4ad45, /// 0xf10
			 0xd18c48b0, /// 0xf14
			 0x46d0dd22, /// 0xf18
			 0x82ba5d72, /// 0xf1c
			 0x004cec6b, /// 0xf20
			 0x2249bf98, /// 0xf24
			 0xff600c12, /// 0xf28
			 0xa72f9da7, /// 0xf2c
			 0x41e3e9d2, /// 0xf30
			 0x5998c98b, /// 0xf34
			 0xda5e0814, /// 0xf38
			 0x3533f060, /// 0xf3c
			 0x5195e31c, /// 0xf40
			 0x8754a95c, /// 0xf44
			 0xdccc2a03, /// 0xf48
			 0xdbdd5ab6, /// 0xf4c
			 0x6f977421, /// 0xf50
			 0x7c2b90cb, /// 0xf54
			 0xfc4f07c6, /// 0xf58
			 0xdfae834d, /// 0xf5c
			 0x5c69f4be, /// 0xf60
			 0xecf9a552, /// 0xf64
			 0x55168763, /// 0xf68
			 0x9eb60b7f, /// 0xf6c
			 0x9ee06856, /// 0xf70
			 0xdbd38c4e, /// 0xf74
			 0x590f17c4, /// 0xf78
			 0x3decc3dc, /// 0xf7c
			 0x84358254, /// 0xf80
			 0x3c011320, /// 0xf84
			 0x4f9c4f91, /// 0xf88
			 0x14d60fd2, /// 0xf8c
			 0x28cc8eda, /// 0xf90
			 0x8b60530b, /// 0xf94
			 0x5cd11281, /// 0xf98
			 0x86361c92, /// 0xf9c
			 0x037642f9, /// 0xfa0
			 0xf4df8d76, /// 0xfa4
			 0x115e6f61, /// 0xfa8
			 0x7bc70833, /// 0xfac
			 0xef146e12, /// 0xfb0
			 0xaa8c4bfd, /// 0xfb4
			 0x83d0f5a4, /// 0xfb8
			 0x3f21efde, /// 0xfbc
			 0x18127ea3, /// 0xfc0
			 0x605e3d7b, /// 0xfc4
			 0x0dce010a, /// 0xfc8
			 0xaf70a6fe, /// 0xfcc
			 0xd2b3d547, /// 0xfd0
			 0x9221bea7, /// 0xfd4
			 0xe785b047, /// 0xfd8
			 0xe2aea7bb, /// 0xfdc
			 0x12557113, /// 0xfe0
			 0x4581e510, /// 0xfe4
			 0xccfbac08, /// 0xfe8
			 0x924353e6, /// 0xfec
			 0x1b3ccba1, /// 0xff0
			 0xf15580c4, /// 0xff4
			 0x0e68b635, /// 0xff8
			 0xe5f69b6c /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xa461b093, /// 0x0
			 0xe3163ab2, /// 0x4
			 0xd059dd79, /// 0x8
			 0xe9282dd0, /// 0xc
			 0x80e618bd, /// 0x10
			 0x609a4074, /// 0x14
			 0x362e9f84, /// 0x18
			 0xe43a50ae, /// 0x1c
			 0xf3e382df, /// 0x20
			 0x495a1bee, /// 0x24
			 0xa1842a54, /// 0x28
			 0x0cd087ae, /// 0x2c
			 0x4604dba7, /// 0x30
			 0xccbae5f1, /// 0x34
			 0xd6cc91c2, /// 0x38
			 0x393bbbd2, /// 0x3c
			 0xb2b33b72, /// 0x40
			 0xdf7c4476, /// 0x44
			 0x51b92ad2, /// 0x48
			 0xcb0512be, /// 0x4c
			 0x54b1681c, /// 0x50
			 0x43198321, /// 0x54
			 0xce1dbd4f, /// 0x58
			 0x713d29c5, /// 0x5c
			 0x5d5bef88, /// 0x60
			 0x51da4bc0, /// 0x64
			 0x47e336ba, /// 0x68
			 0x7ecb24ec, /// 0x6c
			 0x7ca8db62, /// 0x70
			 0x99be6f0e, /// 0x74
			 0x85a218ba, /// 0x78
			 0x96ac766d, /// 0x7c
			 0x0bddf692, /// 0x80
			 0xf379199f, /// 0x84
			 0x5e9b421c, /// 0x88
			 0xcb414d7a, /// 0x8c
			 0x328cd3dd, /// 0x90
			 0xd7508bd3, /// 0x94
			 0x5c30b72c, /// 0x98
			 0x50186548, /// 0x9c
			 0x57313e46, /// 0xa0
			 0xfdcda7ab, /// 0xa4
			 0xd7b34a77, /// 0xa8
			 0x37e91a00, /// 0xac
			 0x2b7ac8af, /// 0xb0
			 0xe034f683, /// 0xb4
			 0x2b76e03a, /// 0xb8
			 0x087aed72, /// 0xbc
			 0x7e9859d4, /// 0xc0
			 0x53570b70, /// 0xc4
			 0x8faacd37, /// 0xc8
			 0xaa260dd0, /// 0xcc
			 0x417fb769, /// 0xd0
			 0x08e449d8, /// 0xd4
			 0x45f725c4, /// 0xd8
			 0xcaf5de8c, /// 0xdc
			 0x7092a338, /// 0xe0
			 0x910504aa, /// 0xe4
			 0x163653b7, /// 0xe8
			 0xc86d3451, /// 0xec
			 0x2e7f0a70, /// 0xf0
			 0xeee7735d, /// 0xf4
			 0x1e74d18f, /// 0xf8
			 0x5a0b116f, /// 0xfc
			 0x137b3bc3, /// 0x100
			 0xfc3ebba1, /// 0x104
			 0x195931a0, /// 0x108
			 0x20b799f7, /// 0x10c
			 0xcc703a07, /// 0x110
			 0x9269aa9e, /// 0x114
			 0xf387775e, /// 0x118
			 0xcec3934d, /// 0x11c
			 0x1d4c97bf, /// 0x120
			 0xf096a063, /// 0x124
			 0xdb29c19b, /// 0x128
			 0xcfe4343e, /// 0x12c
			 0xe55f8416, /// 0x130
			 0x45dc243f, /// 0x134
			 0x3a42eb46, /// 0x138
			 0x39775fb1, /// 0x13c
			 0x56b60b58, /// 0x140
			 0x9bd00214, /// 0x144
			 0x9265ec9a, /// 0x148
			 0x09be3cd8, /// 0x14c
			 0xfb12d48e, /// 0x150
			 0x9d260f54, /// 0x154
			 0xda6d723b, /// 0x158
			 0xe7001cea, /// 0x15c
			 0xe5e2b3bd, /// 0x160
			 0x90313dae, /// 0x164
			 0xfce95342, /// 0x168
			 0x1d6c4fa5, /// 0x16c
			 0x094bfe08, /// 0x170
			 0x05a82926, /// 0x174
			 0x061fee11, /// 0x178
			 0x6f2a91de, /// 0x17c
			 0xcffea5bc, /// 0x180
			 0x765a335d, /// 0x184
			 0x508bded4, /// 0x188
			 0x6b352f1f, /// 0x18c
			 0xedee0987, /// 0x190
			 0x73bbd938, /// 0x194
			 0x842f5418, /// 0x198
			 0xb52e7390, /// 0x19c
			 0x7f7a40c5, /// 0x1a0
			 0x486a8d66, /// 0x1a4
			 0x34262f68, /// 0x1a8
			 0x3032da24, /// 0x1ac
			 0x269f0e6c, /// 0x1b0
			 0x340d486e, /// 0x1b4
			 0x2d4d8dde, /// 0x1b8
			 0xb0075860, /// 0x1bc
			 0x276c3685, /// 0x1c0
			 0x77f8c2c6, /// 0x1c4
			 0x06e7a6ce, /// 0x1c8
			 0xed1463fe, /// 0x1cc
			 0xa432ad6f, /// 0x1d0
			 0x9f28bece, /// 0x1d4
			 0xbe12aa51, /// 0x1d8
			 0x460c1e59, /// 0x1dc
			 0xd31a4045, /// 0x1e0
			 0xb49beeab, /// 0x1e4
			 0xf29a1ab8, /// 0x1e8
			 0xf2da0f00, /// 0x1ec
			 0xa44aae34, /// 0x1f0
			 0x4e80cd18, /// 0x1f4
			 0x57a12f45, /// 0x1f8
			 0x622e85b5, /// 0x1fc
			 0x2e477ace, /// 0x200
			 0x2f6693f0, /// 0x204
			 0xd66a1468, /// 0x208
			 0x78b1f593, /// 0x20c
			 0x396819e1, /// 0x210
			 0x54a1d7b1, /// 0x214
			 0x6823c72c, /// 0x218
			 0x09b86ac9, /// 0x21c
			 0x048b8143, /// 0x220
			 0xb3860d87, /// 0x224
			 0x11b20602, /// 0x228
			 0x1d0322e1, /// 0x22c
			 0xb71b718c, /// 0x230
			 0x069dcb57, /// 0x234
			 0x6f5cbbef, /// 0x238
			 0x08d3c332, /// 0x23c
			 0xa087acef, /// 0x240
			 0x711ab3b9, /// 0x244
			 0x7ebe0a65, /// 0x248
			 0x49414cef, /// 0x24c
			 0xe0ca0af9, /// 0x250
			 0x8e7aea5d, /// 0x254
			 0x45295577, /// 0x258
			 0x83cadf2f, /// 0x25c
			 0x7cc45285, /// 0x260
			 0x2507f6f2, /// 0x264
			 0x66b76dad, /// 0x268
			 0x6522144b, /// 0x26c
			 0xa916f6c7, /// 0x270
			 0xd952ccc1, /// 0x274
			 0x2fdae114, /// 0x278
			 0x59a56400, /// 0x27c
			 0xa58013b2, /// 0x280
			 0x1aeaa8a0, /// 0x284
			 0xa788dd49, /// 0x288
			 0xf1c94dda, /// 0x28c
			 0x6565eadd, /// 0x290
			 0x0a2f31c6, /// 0x294
			 0x6a237068, /// 0x298
			 0xb789e816, /// 0x29c
			 0x2dd731a4, /// 0x2a0
			 0x0aebeca1, /// 0x2a4
			 0xf0f4b837, /// 0x2a8
			 0x01fa8d83, /// 0x2ac
			 0xb6c8b84b, /// 0x2b0
			 0x8b3d5004, /// 0x2b4
			 0x2e1ff23b, /// 0x2b8
			 0xf9c998e5, /// 0x2bc
			 0xc2cbae2f, /// 0x2c0
			 0x82983c07, /// 0x2c4
			 0x2a102d91, /// 0x2c8
			 0xc567e492, /// 0x2cc
			 0xf225779e, /// 0x2d0
			 0x86caa810, /// 0x2d4
			 0x1c910dd4, /// 0x2d8
			 0xbf2c1a7f, /// 0x2dc
			 0xe76e1bbc, /// 0x2e0
			 0xc2ad28e4, /// 0x2e4
			 0x8d7c900e, /// 0x2e8
			 0xf76ac67e, /// 0x2ec
			 0xfb9cded6, /// 0x2f0
			 0xbf0a2608, /// 0x2f4
			 0x32e1866e, /// 0x2f8
			 0x233decb4, /// 0x2fc
			 0x32e01e6c, /// 0x300
			 0x3e6323fb, /// 0x304
			 0x0d6d7ecc, /// 0x308
			 0x3209423f, /// 0x30c
			 0x5bf13b0e, /// 0x310
			 0xa553353c, /// 0x314
			 0xf6386317, /// 0x318
			 0x49a18afb, /// 0x31c
			 0x143aaca9, /// 0x320
			 0xdd22bd0a, /// 0x324
			 0xddf700b6, /// 0x328
			 0x09425674, /// 0x32c
			 0xfaee32be, /// 0x330
			 0xe0538546, /// 0x334
			 0xd3bfdcdb, /// 0x338
			 0x416f56ee, /// 0x33c
			 0x9c73d8a4, /// 0x340
			 0x429a1d63, /// 0x344
			 0x226ce200, /// 0x348
			 0x3ff91fd6, /// 0x34c
			 0x0b071f16, /// 0x350
			 0x2d5a8ddf, /// 0x354
			 0x5248e7bd, /// 0x358
			 0x61321fac, /// 0x35c
			 0xa7cac85a, /// 0x360
			 0x79a516a1, /// 0x364
			 0xe40f9974, /// 0x368
			 0xb5502515, /// 0x36c
			 0xbaff0de3, /// 0x370
			 0x37fee5bc, /// 0x374
			 0x0e291b8c, /// 0x378
			 0x05f89958, /// 0x37c
			 0x42ef4c2e, /// 0x380
			 0x008aee4b, /// 0x384
			 0x9c7dadc0, /// 0x388
			 0x3fa37716, /// 0x38c
			 0x4c5c7ee0, /// 0x390
			 0x6bd96ed0, /// 0x394
			 0x597ee53d, /// 0x398
			 0x82c6de64, /// 0x39c
			 0x95a14cb6, /// 0x3a0
			 0x8b0ce29c, /// 0x3a4
			 0x819c2cbe, /// 0x3a8
			 0x39195f0d, /// 0x3ac
			 0x2f46ee97, /// 0x3b0
			 0xe969a2f1, /// 0x3b4
			 0x715cac5b, /// 0x3b8
			 0xbf74b77f, /// 0x3bc
			 0xd806f42c, /// 0x3c0
			 0x153a286e, /// 0x3c4
			 0xb83090e7, /// 0x3c8
			 0x919d2ecf, /// 0x3cc
			 0x560d51d9, /// 0x3d0
			 0x9fba1260, /// 0x3d4
			 0x0d6a4548, /// 0x3d8
			 0x37c15072, /// 0x3dc
			 0xa09b576f, /// 0x3e0
			 0xdf1ad1f1, /// 0x3e4
			 0x2fc29f21, /// 0x3e8
			 0xb6f63f15, /// 0x3ec
			 0x3867082e, /// 0x3f0
			 0xe21b80a7, /// 0x3f4
			 0x5aea277b, /// 0x3f8
			 0x01f8c6ac, /// 0x3fc
			 0x07e7bf8a, /// 0x400
			 0x0fd6d330, /// 0x404
			 0x3abd68f3, /// 0x408
			 0x6e65ec4e, /// 0x40c
			 0xd6e1d3fb, /// 0x410
			 0x3f49faa2, /// 0x414
			 0x8283508c, /// 0x418
			 0x816a379b, /// 0x41c
			 0x7fad2f3f, /// 0x420
			 0xb9f775dd, /// 0x424
			 0x8bee90ff, /// 0x428
			 0x4c4b3c42, /// 0x42c
			 0xc40868ee, /// 0x430
			 0xb0b928b7, /// 0x434
			 0x703f2853, /// 0x438
			 0xa4dfb72c, /// 0x43c
			 0xd5039286, /// 0x440
			 0x5eb03b58, /// 0x444
			 0x5578d8a7, /// 0x448
			 0x0651c7a1, /// 0x44c
			 0x5f463b15, /// 0x450
			 0xad585095, /// 0x454
			 0x7ac9cf3d, /// 0x458
			 0xa71942f3, /// 0x45c
			 0xee34cb68, /// 0x460
			 0xe45f23dc, /// 0x464
			 0xb21433b2, /// 0x468
			 0x912c580b, /// 0x46c
			 0x25a68780, /// 0x470
			 0xa6f46bb3, /// 0x474
			 0xf62527e2, /// 0x478
			 0x9bb4ac46, /// 0x47c
			 0xeb056e8a, /// 0x480
			 0xaeee8415, /// 0x484
			 0x814e98d1, /// 0x488
			 0x778cd12f, /// 0x48c
			 0xcea4bdad, /// 0x490
			 0x0898a8d8, /// 0x494
			 0xacd896b2, /// 0x498
			 0x96e1e522, /// 0x49c
			 0x39aaff15, /// 0x4a0
			 0x0cc2c328, /// 0x4a4
			 0xc4411a91, /// 0x4a8
			 0x8c4c3a68, /// 0x4ac
			 0xf9391163, /// 0x4b0
			 0x5953494d, /// 0x4b4
			 0x4e7e8eef, /// 0x4b8
			 0x583119aa, /// 0x4bc
			 0xc0819b88, /// 0x4c0
			 0x5e59323a, /// 0x4c4
			 0xc276f511, /// 0x4c8
			 0xd22540f0, /// 0x4cc
			 0x5e5fa1df, /// 0x4d0
			 0xb6dbd591, /// 0x4d4
			 0xc65265de, /// 0x4d8
			 0x13c084a4, /// 0x4dc
			 0xecc8df6b, /// 0x4e0
			 0xa75ff1d4, /// 0x4e4
			 0x8a9422d1, /// 0x4e8
			 0x14da024e, /// 0x4ec
			 0xfde63b87, /// 0x4f0
			 0xc6b1964a, /// 0x4f4
			 0xb8447d47, /// 0x4f8
			 0xb9b7b3b4, /// 0x4fc
			 0x68a9e9f6, /// 0x500
			 0x05d54994, /// 0x504
			 0x78169120, /// 0x508
			 0xc525c052, /// 0x50c
			 0xbe4ead28, /// 0x510
			 0x14a9d90d, /// 0x514
			 0x0d8cc3eb, /// 0x518
			 0x1b7cb94a, /// 0x51c
			 0x8c95c019, /// 0x520
			 0x62a4070b, /// 0x524
			 0x4a8922f8, /// 0x528
			 0x3ef028a3, /// 0x52c
			 0x5b2db6e4, /// 0x530
			 0x213c84cb, /// 0x534
			 0x04202b4d, /// 0x538
			 0xaa061136, /// 0x53c
			 0x71f68318, /// 0x540
			 0x3db20a3e, /// 0x544
			 0x9aaefe29, /// 0x548
			 0x9cc2c142, /// 0x54c
			 0xded9319a, /// 0x550
			 0xe4513fbd, /// 0x554
			 0x7132e12e, /// 0x558
			 0xfaa59ce9, /// 0x55c
			 0x106ee615, /// 0x560
			 0x3f21a0be, /// 0x564
			 0x826edd2b, /// 0x568
			 0xca4be41a, /// 0x56c
			 0xd43972dc, /// 0x570
			 0x0a2372c7, /// 0x574
			 0x5d27a7d6, /// 0x578
			 0xfd98d246, /// 0x57c
			 0x0f6d0b9e, /// 0x580
			 0xced8753d, /// 0x584
			 0xb188cff3, /// 0x588
			 0xa98b5842, /// 0x58c
			 0x544f08a4, /// 0x590
			 0x43bbc0f3, /// 0x594
			 0x95fc39d9, /// 0x598
			 0xdbe0c936, /// 0x59c
			 0xd5e10d23, /// 0x5a0
			 0xddf09529, /// 0x5a4
			 0x18bd3466, /// 0x5a8
			 0x5d9f0298, /// 0x5ac
			 0xd9177568, /// 0x5b0
			 0x4dee5ad0, /// 0x5b4
			 0x1bd578b0, /// 0x5b8
			 0xb150fb6a, /// 0x5bc
			 0x076effba, /// 0x5c0
			 0xa569a806, /// 0x5c4
			 0x3e4caa2b, /// 0x5c8
			 0xf39d39dd, /// 0x5cc
			 0x2987ef9b, /// 0x5d0
			 0xf23b80bb, /// 0x5d4
			 0xc937d7eb, /// 0x5d8
			 0xadfc96b2, /// 0x5dc
			 0x4a139980, /// 0x5e0
			 0xe27c34f3, /// 0x5e4
			 0x96acb7c8, /// 0x5e8
			 0xc04eb7f5, /// 0x5ec
			 0x49b07c5e, /// 0x5f0
			 0x033d3155, /// 0x5f4
			 0x255a40e8, /// 0x5f8
			 0x0c06508f, /// 0x5fc
			 0xf9561aa6, /// 0x600
			 0x5c922b60, /// 0x604
			 0x0a1e23d5, /// 0x608
			 0xfbb187ff, /// 0x60c
			 0xf9ec7234, /// 0x610
			 0x35a0f4bb, /// 0x614
			 0x33f21ffa, /// 0x618
			 0xec3761c0, /// 0x61c
			 0xbb54bec6, /// 0x620
			 0x2d643e26, /// 0x624
			 0x9ca1a022, /// 0x628
			 0xec673b06, /// 0x62c
			 0xdb884dd5, /// 0x630
			 0xcb4b46e6, /// 0x634
			 0x21802a6d, /// 0x638
			 0x5feb0446, /// 0x63c
			 0x71f58976, /// 0x640
			 0xd44ba9be, /// 0x644
			 0x422713be, /// 0x648
			 0x2153c8f2, /// 0x64c
			 0x986bf210, /// 0x650
			 0x1799a118, /// 0x654
			 0xf4f31538, /// 0x658
			 0x727dd748, /// 0x65c
			 0x65f4101a, /// 0x660
			 0xddb0c571, /// 0x664
			 0x2196fca6, /// 0x668
			 0x729f28fe, /// 0x66c
			 0x55c02cab, /// 0x670
			 0xeca5ff09, /// 0x674
			 0xe9dceb13, /// 0x678
			 0x48960929, /// 0x67c
			 0x287b00ad, /// 0x680
			 0x95cdcf24, /// 0x684
			 0x9f05d9e7, /// 0x688
			 0x67eccf02, /// 0x68c
			 0xf7a3cb7b, /// 0x690
			 0x1bba2e57, /// 0x694
			 0x855e2fb4, /// 0x698
			 0xdcea4b91, /// 0x69c
			 0x68c048bc, /// 0x6a0
			 0x2892092c, /// 0x6a4
			 0x5d559b1b, /// 0x6a8
			 0x9138a38e, /// 0x6ac
			 0x3d79250d, /// 0x6b0
			 0x06f8cafe, /// 0x6b4
			 0xfcecfe20, /// 0x6b8
			 0x8761640d, /// 0x6bc
			 0x5cf340a4, /// 0x6c0
			 0xccf8a7ee, /// 0x6c4
			 0x52927027, /// 0x6c8
			 0x3df49d34, /// 0x6cc
			 0x9dbe7226, /// 0x6d0
			 0x311e9f6c, /// 0x6d4
			 0xaafe9839, /// 0x6d8
			 0xcb627d70, /// 0x6dc
			 0x73050f99, /// 0x6e0
			 0x41b04614, /// 0x6e4
			 0x6558513f, /// 0x6e8
			 0xdc31ee63, /// 0x6ec
			 0xf6f6ad48, /// 0x6f0
			 0x90bcde94, /// 0x6f4
			 0xa929d10b, /// 0x6f8
			 0x52e9f936, /// 0x6fc
			 0xce26a9ec, /// 0x700
			 0xa31f23fd, /// 0x704
			 0x8b7fd22d, /// 0x708
			 0xd0019689, /// 0x70c
			 0xa537c009, /// 0x710
			 0x659f931f, /// 0x714
			 0x8f410164, /// 0x718
			 0x5f483073, /// 0x71c
			 0xb7b89479, /// 0x720
			 0xe36e5816, /// 0x724
			 0x961314b2, /// 0x728
			 0x180a6bda, /// 0x72c
			 0x2acd7b2b, /// 0x730
			 0x03e47833, /// 0x734
			 0xce24e60d, /// 0x738
			 0x9d9b1a22, /// 0x73c
			 0x6066ecdd, /// 0x740
			 0x0a5497ae, /// 0x744
			 0x2d0236f2, /// 0x748
			 0xcc9d552d, /// 0x74c
			 0xc7d5aa6e, /// 0x750
			 0xaf7a63e7, /// 0x754
			 0x4887c83e, /// 0x758
			 0xfc7d8dcf, /// 0x75c
			 0x180eb212, /// 0x760
			 0x10126503, /// 0x764
			 0x599641a4, /// 0x768
			 0xf97ab8ee, /// 0x76c
			 0xca157d60, /// 0x770
			 0xb70d3d50, /// 0x774
			 0xd13edbfb, /// 0x778
			 0x176f0167, /// 0x77c
			 0x84ae7eab, /// 0x780
			 0x4825f9db, /// 0x784
			 0x8bb3697d, /// 0x788
			 0x4c1d1a0b, /// 0x78c
			 0x4d814a3b, /// 0x790
			 0x58f82900, /// 0x794
			 0x3d1954a4, /// 0x798
			 0x64e0f4a4, /// 0x79c
			 0x03a95337, /// 0x7a0
			 0xc1028f05, /// 0x7a4
			 0x4ef4299e, /// 0x7a8
			 0xd980f719, /// 0x7ac
			 0x461596be, /// 0x7b0
			 0xa4ec0cf9, /// 0x7b4
			 0xccc23c40, /// 0x7b8
			 0x60565173, /// 0x7bc
			 0x16b477d9, /// 0x7c0
			 0x04b4a150, /// 0x7c4
			 0x9f2fc5be, /// 0x7c8
			 0xdd618553, /// 0x7cc
			 0xd6aae0e4, /// 0x7d0
			 0xd132dba6, /// 0x7d4
			 0xfdb60d48, /// 0x7d8
			 0xbefe7bd0, /// 0x7dc
			 0xf704c233, /// 0x7e0
			 0xf527cfd7, /// 0x7e4
			 0x025ebb1f, /// 0x7e8
			 0x98797bd1, /// 0x7ec
			 0x9ae80848, /// 0x7f0
			 0x00ac95d3, /// 0x7f4
			 0xd1f30921, /// 0x7f8
			 0x1bab5ec7, /// 0x7fc
			 0x572164fb, /// 0x800
			 0x0756617b, /// 0x804
			 0xf65f0e06, /// 0x808
			 0x7e6c7dcd, /// 0x80c
			 0x9e453357, /// 0x810
			 0xb3375ace, /// 0x814
			 0xbbefeb9c, /// 0x818
			 0x19d7df6e, /// 0x81c
			 0x686458d6, /// 0x820
			 0x25a272c1, /// 0x824
			 0xac6a9945, /// 0x828
			 0x3f17a588, /// 0x82c
			 0x04d8d00d, /// 0x830
			 0x4bfc37ea, /// 0x834
			 0xf6d1f754, /// 0x838
			 0xe5fd029f, /// 0x83c
			 0x05ad5d61, /// 0x840
			 0x568e5066, /// 0x844
			 0x7b80c96a, /// 0x848
			 0x8ae7d703, /// 0x84c
			 0x3ce746f2, /// 0x850
			 0x1a99ff15, /// 0x854
			 0xcbf395ff, /// 0x858
			 0xa75e5dd4, /// 0x85c
			 0xd8d57b0e, /// 0x860
			 0xd1579dbe, /// 0x864
			 0x833b263d, /// 0x868
			 0xb8b94852, /// 0x86c
			 0x74f001d1, /// 0x870
			 0xbd6c5a23, /// 0x874
			 0xf9b855d7, /// 0x878
			 0x85612f49, /// 0x87c
			 0x9fad8b70, /// 0x880
			 0xd4e221ac, /// 0x884
			 0x8b10e97f, /// 0x888
			 0x4752a4f9, /// 0x88c
			 0x82dc51fb, /// 0x890
			 0x6e153886, /// 0x894
			 0x499573e3, /// 0x898
			 0xda9e57e6, /// 0x89c
			 0x418b7c93, /// 0x8a0
			 0x0395172c, /// 0x8a4
			 0x7ddd0cba, /// 0x8a8
			 0xde95883d, /// 0x8ac
			 0x35300f6c, /// 0x8b0
			 0x338e4991, /// 0x8b4
			 0x6c300d9a, /// 0x8b8
			 0x04854676, /// 0x8bc
			 0xd04e5b16, /// 0x8c0
			 0x3a58c4ee, /// 0x8c4
			 0xae4b0192, /// 0x8c8
			 0xe4736e65, /// 0x8cc
			 0xf6fa3e48, /// 0x8d0
			 0xa9aff074, /// 0x8d4
			 0x79e7f761, /// 0x8d8
			 0x8966f208, /// 0x8dc
			 0x683ec9b8, /// 0x8e0
			 0x3a5b0e4b, /// 0x8e4
			 0xed83b7ff, /// 0x8e8
			 0x63929398, /// 0x8ec
			 0x5810f3e5, /// 0x8f0
			 0x6d8ac403, /// 0x8f4
			 0x8d2fe4b3, /// 0x8f8
			 0xcd600bc3, /// 0x8fc
			 0x7abec760, /// 0x900
			 0x11ef24ec, /// 0x904
			 0x5b47520a, /// 0x908
			 0x93ca2f55, /// 0x90c
			 0x44087ee0, /// 0x910
			 0xa47b9833, /// 0x914
			 0x2fe1f6e4, /// 0x918
			 0xaf75e1a3, /// 0x91c
			 0xbfac599f, /// 0x920
			 0x282edf22, /// 0x924
			 0x24c97a98, /// 0x928
			 0x1a1962a6, /// 0x92c
			 0x49893a95, /// 0x930
			 0x7d5b8b2d, /// 0x934
			 0x5c99a6fa, /// 0x938
			 0x0a3826cd, /// 0x93c
			 0xde2b0a85, /// 0x940
			 0xb8cac5a4, /// 0x944
			 0x2e1e8de7, /// 0x948
			 0xe2d665c9, /// 0x94c
			 0x5c44ee3c, /// 0x950
			 0x4e1ebe32, /// 0x954
			 0x665f3c27, /// 0x958
			 0xcd7250e5, /// 0x95c
			 0xb0d8198b, /// 0x960
			 0x9a214c1f, /// 0x964
			 0x87d903e8, /// 0x968
			 0xa40eee65, /// 0x96c
			 0x6e01ee62, /// 0x970
			 0xcd08fc57, /// 0x974
			 0x6ddc544c, /// 0x978
			 0x654f7214, /// 0x97c
			 0xd550efcd, /// 0x980
			 0x42e8c11f, /// 0x984
			 0x35e403a5, /// 0x988
			 0xfc263b7a, /// 0x98c
			 0x5702c504, /// 0x990
			 0x9d945742, /// 0x994
			 0xf96627b5, /// 0x998
			 0x2ee25d35, /// 0x99c
			 0x3540f1b1, /// 0x9a0
			 0xa306a114, /// 0x9a4
			 0x43028df9, /// 0x9a8
			 0x36252202, /// 0x9ac
			 0x8278cf0d, /// 0x9b0
			 0x782ab3ff, /// 0x9b4
			 0x4ada0ed7, /// 0x9b8
			 0x21cb16d4, /// 0x9bc
			 0xc29aa7f8, /// 0x9c0
			 0xdb982eef, /// 0x9c4
			 0x0def46d0, /// 0x9c8
			 0x6d066891, /// 0x9cc
			 0x92aba8fa, /// 0x9d0
			 0x1acd40fa, /// 0x9d4
			 0x95163573, /// 0x9d8
			 0xebe103a4, /// 0x9dc
			 0x6e1c5b99, /// 0x9e0
			 0xaedd5633, /// 0x9e4
			 0xbafed6d7, /// 0x9e8
			 0xb6276dd5, /// 0x9ec
			 0xfa2b3a47, /// 0x9f0
			 0x9f73b569, /// 0x9f4
			 0x96d48cc7, /// 0x9f8
			 0xb8cf2134, /// 0x9fc
			 0xa7d77eba, /// 0xa00
			 0xe66cb430, /// 0xa04
			 0xdcf38f14, /// 0xa08
			 0x88309a62, /// 0xa0c
			 0x979054ed, /// 0xa10
			 0x447c1661, /// 0xa14
			 0x5e676ca5, /// 0xa18
			 0x01b52d60, /// 0xa1c
			 0xff67a208, /// 0xa20
			 0x1861432b, /// 0xa24
			 0xa3866518, /// 0xa28
			 0x36d14533, /// 0xa2c
			 0xbff06c99, /// 0xa30
			 0x0b63b19d, /// 0xa34
			 0xc81c6868, /// 0xa38
			 0x34f42f56, /// 0xa3c
			 0x6f751fd6, /// 0xa40
			 0x2fbbce77, /// 0xa44
			 0x7c339866, /// 0xa48
			 0x69512891, /// 0xa4c
			 0x626f93c6, /// 0xa50
			 0xe210ab6f, /// 0xa54
			 0xfb163d7a, /// 0xa58
			 0xcb87fbdf, /// 0xa5c
			 0x7ae439c0, /// 0xa60
			 0x5591da1e, /// 0xa64
			 0x4131dbfe, /// 0xa68
			 0x3c04f5df, /// 0xa6c
			 0x28fc0dd0, /// 0xa70
			 0x521a6249, /// 0xa74
			 0x01fdfb7c, /// 0xa78
			 0x593e7045, /// 0xa7c
			 0x49725fdb, /// 0xa80
			 0x5b5498be, /// 0xa84
			 0x6cf816b5, /// 0xa88
			 0xb13c76ba, /// 0xa8c
			 0xef531271, /// 0xa90
			 0x5ed50b08, /// 0xa94
			 0x9a75ccc7, /// 0xa98
			 0x49dad613, /// 0xa9c
			 0x94bd0c1e, /// 0xaa0
			 0x947dab5e, /// 0xaa4
			 0x7aa1b0f1, /// 0xaa8
			 0xf7c76f17, /// 0xaac
			 0x97bb8355, /// 0xab0
			 0x05a960ac, /// 0xab4
			 0x03b51d10, /// 0xab8
			 0xc69c37d1, /// 0xabc
			 0x9c130e74, /// 0xac0
			 0x836d8984, /// 0xac4
			 0xee887b00, /// 0xac8
			 0xe00a7c89, /// 0xacc
			 0x1ddee51e, /// 0xad0
			 0x43bae58e, /// 0xad4
			 0x9042c76d, /// 0xad8
			 0x8c4fab85, /// 0xadc
			 0x49a92407, /// 0xae0
			 0x95e7731e, /// 0xae4
			 0xbf18b695, /// 0xae8
			 0x60535e48, /// 0xaec
			 0x9847d736, /// 0xaf0
			 0xd4c5481e, /// 0xaf4
			 0x17a5c8b1, /// 0xaf8
			 0xb356407e, /// 0xafc
			 0x8b3120e5, /// 0xb00
			 0x26806f59, /// 0xb04
			 0x8098de41, /// 0xb08
			 0xb6f37853, /// 0xb0c
			 0x0093bccc, /// 0xb10
			 0xe545e3a0, /// 0xb14
			 0x183b8160, /// 0xb18
			 0xed6fb923, /// 0xb1c
			 0x4d57264e, /// 0xb20
			 0xdb95a28f, /// 0xb24
			 0x067438b4, /// 0xb28
			 0x8b487982, /// 0xb2c
			 0x3ee80ffc, /// 0xb30
			 0x8cbca147, /// 0xb34
			 0xf0db7993, /// 0xb38
			 0xae8ead0c, /// 0xb3c
			 0x1dcd6a2e, /// 0xb40
			 0x247a1935, /// 0xb44
			 0x371a9447, /// 0xb48
			 0xec91eb99, /// 0xb4c
			 0x92ef7356, /// 0xb50
			 0xdc74e801, /// 0xb54
			 0x0461962e, /// 0xb58
			 0xeaa23bf2, /// 0xb5c
			 0x68c46557, /// 0xb60
			 0x54ddd151, /// 0xb64
			 0x19b9ab2f, /// 0xb68
			 0x5e0bbc76, /// 0xb6c
			 0xfd7833f1, /// 0xb70
			 0xf570065a, /// 0xb74
			 0x8683e1c4, /// 0xb78
			 0x4c77a57a, /// 0xb7c
			 0x206a2827, /// 0xb80
			 0x7b104e57, /// 0xb84
			 0x8584798c, /// 0xb88
			 0x94bc2c6e, /// 0xb8c
			 0xc07c4852, /// 0xb90
			 0xd63f39d9, /// 0xb94
			 0xbdd38fa4, /// 0xb98
			 0x61ee43c0, /// 0xb9c
			 0x25517ff9, /// 0xba0
			 0x57f015d2, /// 0xba4
			 0x02359bb1, /// 0xba8
			 0xaa52a4f4, /// 0xbac
			 0xca94fd0a, /// 0xbb0
			 0xe4f68964, /// 0xbb4
			 0x8ec06d64, /// 0xbb8
			 0xa7e187d0, /// 0xbbc
			 0x886f91de, /// 0xbc0
			 0xc8d72776, /// 0xbc4
			 0xe429c285, /// 0xbc8
			 0x24aee97a, /// 0xbcc
			 0xa350c779, /// 0xbd0
			 0x87ab14ce, /// 0xbd4
			 0xbe2fa2ae, /// 0xbd8
			 0xbf0bc1fc, /// 0xbdc
			 0x1cd3d51b, /// 0xbe0
			 0x907074d4, /// 0xbe4
			 0xa986dbe8, /// 0xbe8
			 0x34b6cd78, /// 0xbec
			 0x8d06e6cc, /// 0xbf0
			 0xd9328e43, /// 0xbf4
			 0x825bee4f, /// 0xbf8
			 0x13dff228, /// 0xbfc
			 0x8f7e721d, /// 0xc00
			 0x3fcad912, /// 0xc04
			 0xa8b2cebb, /// 0xc08
			 0x07164841, /// 0xc0c
			 0xdd836acf, /// 0xc10
			 0x24ca80b3, /// 0xc14
			 0xcb0479e9, /// 0xc18
			 0x2c64fbd5, /// 0xc1c
			 0x0cbd53b8, /// 0xc20
			 0xf678a727, /// 0xc24
			 0x31a17c90, /// 0xc28
			 0x582639d7, /// 0xc2c
			 0x69d10d44, /// 0xc30
			 0x713fc05a, /// 0xc34
			 0xdcdf61ac, /// 0xc38
			 0x8d62bc03, /// 0xc3c
			 0x35080a40, /// 0xc40
			 0x10c31577, /// 0xc44
			 0x6370b35e, /// 0xc48
			 0x3bde9c82, /// 0xc4c
			 0x64fc7b1d, /// 0xc50
			 0x01c47d28, /// 0xc54
			 0x11705dae, /// 0xc58
			 0x5503dc5a, /// 0xc5c
			 0x50c9d178, /// 0xc60
			 0x885b1f80, /// 0xc64
			 0x32b77d92, /// 0xc68
			 0x440fc786, /// 0xc6c
			 0x3eb1416a, /// 0xc70
			 0xb422d768, /// 0xc74
			 0x9123e9b7, /// 0xc78
			 0x823c7b22, /// 0xc7c
			 0xa500d9e2, /// 0xc80
			 0xe62c92f2, /// 0xc84
			 0x29f2dcc4, /// 0xc88
			 0x704d8412, /// 0xc8c
			 0x8ba26f8e, /// 0xc90
			 0xe0de04c2, /// 0xc94
			 0x6c568c28, /// 0xc98
			 0x40e88e2b, /// 0xc9c
			 0x011ba858, /// 0xca0
			 0x6a357c66, /// 0xca4
			 0x95f60363, /// 0xca8
			 0xb7782dcd, /// 0xcac
			 0x64d386b9, /// 0xcb0
			 0x02860add, /// 0xcb4
			 0x03da5363, /// 0xcb8
			 0xe190e66b, /// 0xcbc
			 0x427dd69d, /// 0xcc0
			 0x4edcdcf3, /// 0xcc4
			 0xd0a289b1, /// 0xcc8
			 0x8c4d84e6, /// 0xccc
			 0x61cff3fc, /// 0xcd0
			 0xfe72decd, /// 0xcd4
			 0x8ed5baaa, /// 0xcd8
			 0x7ac0cc30, /// 0xcdc
			 0x3f03fe8a, /// 0xce0
			 0x7356c6d4, /// 0xce4
			 0x2fdf13cd, /// 0xce8
			 0xf083fef1, /// 0xcec
			 0x33d6fbee, /// 0xcf0
			 0xdb5fe5f1, /// 0xcf4
			 0x9bab15d4, /// 0xcf8
			 0x2cd6f825, /// 0xcfc
			 0x7c99b434, /// 0xd00
			 0x0f66a305, /// 0xd04
			 0x522d0de3, /// 0xd08
			 0xe7a5d6bb, /// 0xd0c
			 0x81b556e4, /// 0xd10
			 0xf250549a, /// 0xd14
			 0x431faf70, /// 0xd18
			 0x21214dfc, /// 0xd1c
			 0xae3633a3, /// 0xd20
			 0x407cf4ae, /// 0xd24
			 0x4d2dd28d, /// 0xd28
			 0x60809409, /// 0xd2c
			 0xcbdd4172, /// 0xd30
			 0x73b2c4d7, /// 0xd34
			 0x5c82b522, /// 0xd38
			 0x45b8bbe2, /// 0xd3c
			 0x14be1847, /// 0xd40
			 0x5149f1d3, /// 0xd44
			 0x3c0a33fc, /// 0xd48
			 0x98473699, /// 0xd4c
			 0x75fa7d3e, /// 0xd50
			 0xb2a21b6c, /// 0xd54
			 0x9c9800b2, /// 0xd58
			 0x8271cd3d, /// 0xd5c
			 0xd1233205, /// 0xd60
			 0x446895fb, /// 0xd64
			 0x60780fdd, /// 0xd68
			 0x0f289be5, /// 0xd6c
			 0x7303c36c, /// 0xd70
			 0x646dd868, /// 0xd74
			 0x9b2f3903, /// 0xd78
			 0x387f79ce, /// 0xd7c
			 0xe8c14b80, /// 0xd80
			 0x65bb7851, /// 0xd84
			 0xadb347d1, /// 0xd88
			 0x2e6d3ef0, /// 0xd8c
			 0x50717fe2, /// 0xd90
			 0x723c1ab0, /// 0xd94
			 0xff31c96a, /// 0xd98
			 0xbd6573e9, /// 0xd9c
			 0x6f89dc50, /// 0xda0
			 0xd2d4769e, /// 0xda4
			 0x7c74eee5, /// 0xda8
			 0x8f374cc4, /// 0xdac
			 0x09e2d577, /// 0xdb0
			 0xeb6bb512, /// 0xdb4
			 0x4ebdcc40, /// 0xdb8
			 0x66d1449b, /// 0xdbc
			 0xe1f9ff60, /// 0xdc0
			 0x6cc875e4, /// 0xdc4
			 0x95d6d7d9, /// 0xdc8
			 0x93574190, /// 0xdcc
			 0xca003959, /// 0xdd0
			 0x0c19b280, /// 0xdd4
			 0x6c2bed19, /// 0xdd8
			 0xe4af357d, /// 0xddc
			 0xcfde95a3, /// 0xde0
			 0x270acd57, /// 0xde4
			 0x5a2df95c, /// 0xde8
			 0x126f29e1, /// 0xdec
			 0x709a043e, /// 0xdf0
			 0xb8f0133f, /// 0xdf4
			 0x706fcdb8, /// 0xdf8
			 0x9a5163aa, /// 0xdfc
			 0xab5b4ff4, /// 0xe00
			 0xd2db523f, /// 0xe04
			 0xbe819fba, /// 0xe08
			 0x8c41ef42, /// 0xe0c
			 0xdf254735, /// 0xe10
			 0x0c7d7fb2, /// 0xe14
			 0xbe6b5c4e, /// 0xe18
			 0x6d361bce, /// 0xe1c
			 0x39ad8c13, /// 0xe20
			 0x1d9275f9, /// 0xe24
			 0x4f8cdbff, /// 0xe28
			 0x077c9d3b, /// 0xe2c
			 0x49e2c8ae, /// 0xe30
			 0x37d858c7, /// 0xe34
			 0xc7e03d0e, /// 0xe38
			 0xad4360ad, /// 0xe3c
			 0x43ba2503, /// 0xe40
			 0xee940c28, /// 0xe44
			 0x403e4097, /// 0xe48
			 0xd56c3c95, /// 0xe4c
			 0xe744b8ec, /// 0xe50
			 0x87ad460a, /// 0xe54
			 0x14c61d1a, /// 0xe58
			 0xfb004ed7, /// 0xe5c
			 0xd457afba, /// 0xe60
			 0x23d4e8fb, /// 0xe64
			 0x8310a66f, /// 0xe68
			 0x98608dff, /// 0xe6c
			 0x45985097, /// 0xe70
			 0xe9509e35, /// 0xe74
			 0x6a11af0a, /// 0xe78
			 0xd47d7a67, /// 0xe7c
			 0x21f15508, /// 0xe80
			 0x71115cd0, /// 0xe84
			 0x79f74ef6, /// 0xe88
			 0x5c03a25f, /// 0xe8c
			 0x8c02a39d, /// 0xe90
			 0x599f6055, /// 0xe94
			 0x5b2ec9b1, /// 0xe98
			 0xaeecd491, /// 0xe9c
			 0x7c6788a7, /// 0xea0
			 0x33540eeb, /// 0xea4
			 0xb7dca5bb, /// 0xea8
			 0x3cd913ad, /// 0xeac
			 0xca68da56, /// 0xeb0
			 0x0db3851a, /// 0xeb4
			 0xff9b0ca4, /// 0xeb8
			 0x60e02784, /// 0xebc
			 0x9bc57c2f, /// 0xec0
			 0x12618675, /// 0xec4
			 0xd2e5f729, /// 0xec8
			 0x7112655b, /// 0xecc
			 0xc4697944, /// 0xed0
			 0x6267c312, /// 0xed4
			 0x925d52b6, /// 0xed8
			 0xfadd6d6b, /// 0xedc
			 0x19b7a0ac, /// 0xee0
			 0xc5d6b2d7, /// 0xee4
			 0x3d89432d, /// 0xee8
			 0x9f8353ca, /// 0xeec
			 0xb9a8af55, /// 0xef0
			 0x39df0671, /// 0xef4
			 0x007d70d8, /// 0xef8
			 0xe298ac50, /// 0xefc
			 0x60f351a4, /// 0xf00
			 0x39012e00, /// 0xf04
			 0x9e0ff2e5, /// 0xf08
			 0x6d9ab78d, /// 0xf0c
			 0xb26347be, /// 0xf10
			 0x72896d85, /// 0xf14
			 0x451de88c, /// 0xf18
			 0x4976f543, /// 0xf1c
			 0x7e346918, /// 0xf20
			 0xf8e27a6f, /// 0xf24
			 0x45633c9d, /// 0xf28
			 0x029e7c6c, /// 0xf2c
			 0x2f452e23, /// 0xf30
			 0xb13e3382, /// 0xf34
			 0x03493e1d, /// 0xf38
			 0xa70d3df2, /// 0xf3c
			 0xe3ae9915, /// 0xf40
			 0x19f1411e, /// 0xf44
			 0x6031449a, /// 0xf48
			 0x48a6bf08, /// 0xf4c
			 0x5b9e7b8a, /// 0xf50
			 0x13a32d7d, /// 0xf54
			 0x2f026ecc, /// 0xf58
			 0xf64a3f3f, /// 0xf5c
			 0xd2469639, /// 0xf60
			 0x947a1c5b, /// 0xf64
			 0xd16d070c, /// 0xf68
			 0xd60c24ff, /// 0xf6c
			 0xd84f05e2, /// 0xf70
			 0xf3f61701, /// 0xf74
			 0x5530f8cb, /// 0xf78
			 0xdfb4ba57, /// 0xf7c
			 0x00981247, /// 0xf80
			 0x6aad12f6, /// 0xf84
			 0x93ffc1a3, /// 0xf88
			 0xfbf0db5d, /// 0xf8c
			 0x5464c35d, /// 0xf90
			 0x01ac1cc7, /// 0xf94
			 0x38469a87, /// 0xf98
			 0x86571b4c, /// 0xf9c
			 0xb6181d0a, /// 0xfa0
			 0x36936943, /// 0xfa4
			 0xa5225b37, /// 0xfa8
			 0xc1340686, /// 0xfac
			 0xb734829e, /// 0xfb0
			 0x81fb2daf, /// 0xfb4
			 0x77e25fbd, /// 0xfb8
			 0x2ac2d6ca, /// 0xfbc
			 0x5c032360, /// 0xfc0
			 0x50c712c0, /// 0xfc4
			 0x78a087c8, /// 0xfc8
			 0xf62903d6, /// 0xfcc
			 0xf3b3595c, /// 0xfd0
			 0xc5d57242, /// 0xfd4
			 0x1757654a, /// 0xfd8
			 0xc5e210d1, /// 0xfdc
			 0xd3f483a2, /// 0xfe0
			 0x8f49192f, /// 0xfe4
			 0x5f6dbb23, /// 0xfe8
			 0x57ebdb5f, /// 0xfec
			 0x1d5ea6bc, /// 0xff0
			 0x4caf59a5, /// 0xff4
			 0x15ac87d9, /// 0xff8
			 0xbe9195f8 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00000
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00004
			 0x0e000001,                                                  // Trailing 1s:                                /// 00008
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0000c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00010
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00014
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00018
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00020
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00028
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00030
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00034
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00038
			 0x55555555,                                                  // 4 random values                             /// 0003c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00058
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0005c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00060
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0006c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00070
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x4b000000,                                                  // 8388608.0                                   /// 00078
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0007c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00080
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00088
			 0x55555555,                                                  // 4 random values                             /// 0008c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00090
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00094
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0009c
			 0xffc00001,                                                  // -signaling NaN                              /// 000a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000c0
			 0xffc00001,                                                  // -signaling NaN                              /// 000c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x4b000000,                                                  // 8388608.0                                   /// 000d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00104
			 0x80011111,                                                  // -9.7958E-41                                 /// 00108
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00114
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00118
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00120
			 0xffc00001,                                                  // -signaling NaN                              /// 00124
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00128
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0012c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00138
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0013c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00140
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00144
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0014c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00150
			 0x0c400000,                                                  // Leading 1s:                                 /// 00154
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00158
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00160
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00164
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00168
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0017c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00180
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00184
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00188
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0018c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00190
			 0x7f800000,                                                  // inf                                         /// 00194
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00198
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00200
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0020c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00214
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0021c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00220
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00228
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0xffc00001,                                                  // -signaling NaN                              /// 00230
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00238
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0023c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00244
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00248
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00250
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0025c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00260
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00270
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00280
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00284
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00288
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c780000,                                                  // Leading 1s:                                 /// 00294
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 002b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002c0
			 0x33333333,                                                  // 4 random values                             /// 002c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c8
			 0x4b000000,                                                  // 8388608.0                                   /// 002cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00300
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00308
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00314
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00318
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0031c
			 0xcb000000,                                                  // -8388608.0                                  /// 00320
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00324
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0032c
			 0xff800000,                                                  // -inf                                        /// 00330
			 0x7f800000,                                                  // inf                                         /// 00334
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0033c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00344
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00348
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00350
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0c400000,                                                  // Leading 1s:                                 /// 0035c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00360
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00364
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00368
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00370
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00378
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0037c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00380
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00384
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00388
			 0xff800000,                                                  // -inf                                        /// 0038c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00390
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00394
			 0x0c700000,                                                  // Leading 1s:                                 /// 00398
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80000000,                                                  // -zero                                       /// 00404
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00408
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0040c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00410
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00414
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x55555555,                                                  // 4 random values                             /// 0041c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00428
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00430
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0043c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00440
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00444
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00450
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00458
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0045c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00460
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00464
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00468
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0046c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0047c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00484
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0048c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00490
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00498
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0049c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004ac
			 0x00000000,                                                  // zero                                        /// 004b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00504
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00508
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0050c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00011111,                                                  // 9.7958E-41                                  /// 00518
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0051c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00520
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00528
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0052c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00534
			 0x0e000001,                                                  // Trailing 1s:                                /// 00538
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0053c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00540
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00548
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0054c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00558
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00560
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00564
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00574
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0057c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00584
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00588
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0058c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00590
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00594
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0c780000,                                                  // Leading 1s:                                 /// 005a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 005f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00610
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00614
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0061c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0c400000,                                                  // Leading 1s:                                 /// 00628
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0062c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00634
			 0x0c700000,                                                  // Leading 1s:                                 /// 00638
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0063c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00640
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00644
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00654
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00658
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0065c
			 0xbf028f5c,                                                  // -0.51                                       /// 00660
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00664
			 0x80000000,                                                  // -zero                                       /// 00668
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00670
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00674
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00678
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0067c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xbf028f5c,                                                  // -0.51                                       /// 00684
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00688
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0068c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00690
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b8
			 0x3f028f5c,                                                  // 0.51                                        /// 006bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00700
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00708
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00710
			 0x4b000000,                                                  // 8388608.0                                   /// 00714
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00718
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00720
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00724
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00728
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0072c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00730
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00734
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0074c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00750
			 0x3f028f5c,                                                  // 0.51                                        /// 00754
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00758
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00760
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0076c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00770
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00774
			 0x0c700000,                                                  // Leading 1s:                                 /// 00778
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00780
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xcb000000,                                                  // -8388608.0                                  /// 00788
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0078c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00790
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00794
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00798
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 007f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00804
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0080c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00810
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00814
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00818
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0081c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00824
			 0x55555555,                                                  // 4 random values                             /// 00828
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0082c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00834
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00838
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00848
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0084c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00850
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00858
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00860
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00864
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00868
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0086c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00870
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x33333333,                                                  // 4 random values                             /// 00878
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0087c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00884
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0088c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00890
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x4b000000,                                                  // 8388608.0                                   /// 00898
			 0x0e000003,                                                  // Trailing 1s:                                /// 0089c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00900
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00904
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00908
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0090c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00914
			 0x4b000000,                                                  // 8388608.0                                   /// 00918
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00920
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00924
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00928
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00930
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00940
			 0x0e000003,                                                  // Trailing 1s:                                /// 00944
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00948
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00954
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0095c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00960
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00974
			 0x80000000,                                                  // -zero                                       /// 00978
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0097c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00980
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00984
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00988
			 0x4b000000,                                                  // 8388608.0                                   /// 0098c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00990
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00998
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a2c
			 0xff800000,                                                  // -inf                                        /// 00a30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a60
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa8
			 0xbf028f5c,                                                  // -0.51                                       /// 00aac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ac4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00adc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00afc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b80
			 0x80000000,                                                  // -zero                                       /// 00b84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b9c
			 0x00000000,                                                  // zero                                        /// 00ba0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ba8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bbc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bcc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bd4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00be4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bf0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x55555555,                                                  // 4 random values                             /// 00c00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c04
			 0x4b000000,                                                  // 8388608.0                                   /// 00c08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x3f028f5c,                                                  // 0.51                                        /// 00c24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xbf028f5c,                                                  // -0.51                                       /// 00c48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c60
			 0x3f028f5c,                                                  // 0.51                                        /// 00c64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c84
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ca8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cc8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd4
			 0xff800000,                                                  // -inf                                        /// 00cd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x55555555,                                                  // 4 random values                             /// 00ce0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00da4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00de0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dec
			 0x00000000,                                                  // zero                                        /// 00df0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e00
			 0x55555555,                                                  // 4 random values                             /// 00e04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e1c
			 0xff800000,                                                  // -inf                                        /// 00e20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e28
			 0x80000000,                                                  // -zero                                       /// 00e2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ebc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ec4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ec8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ecc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ed0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00edc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ef0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x3f028f5c,                                                  // 0.51                                        /// 00f74
			 0x4b000000,                                                  // 8388608.0                                   /// 00f78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc4
			 0x7f800000,                                                  // inf                                         /// 00fc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ff8
			 0x807ffffe // max subnorm - 1ulp                            // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00008
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0000c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00010
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00018
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00038
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00048
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0004c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00050
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00054
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00058
			 0x0c780000,                                                  // Leading 1s:                                 /// 0005c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00060
			 0x80011111,                                                  // -9.7958E-41                                 /// 00064
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00068
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0006c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00070
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00074
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00078
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0007c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00084
			 0x80000000,                                                  // -zero                                       /// 00088
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00098
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00104
			 0x3f028f5c,                                                  // 0.51                                        /// 00108
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0010c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00110
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00114
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xff800000,                                                  // -inf                                        /// 0011c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00120
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00130
			 0x0e000007,                                                  // Trailing 1s:                                /// 00134
			 0x4b000000,                                                  // 8388608.0                                   /// 00138
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0013c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00140
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00148
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0014c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e000007,                                                  // Trailing 1s:                                /// 00154
			 0x80011111,                                                  // -9.7958E-41                                 /// 00158
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0015c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00160
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00168
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0016c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00170
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00174
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0017c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00180
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0018c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00194
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00198
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0019c
			 0xff800000,                                                  // -inf                                        /// 001a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00210
			 0x0c700000,                                                  // Leading 1s:                                 /// 00214
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00218
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0021c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00220
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00224
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00228
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0022c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x7fc00001,                                                  // signaling NaN                               /// 0023c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00244
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00248
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0024c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00250
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00254
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00258
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0025c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00260
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00264
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00268
			 0x55555555,                                                  // 4 random values                             /// 0026c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00270
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00274
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c600000,                                                  // Leading 1s:                                 /// 0027c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00284
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x80000000,                                                  // -zero                                       /// 0028c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00290
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00294
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0029c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00308
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0030c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00318
			 0xff800000,                                                  // -inf                                        /// 0031c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x80000000,                                                  // -zero                                       /// 00328
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0032c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00330
			 0x0c780000,                                                  // Leading 1s:                                 /// 00334
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00338
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0033c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00348
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x7fc00001,                                                  // signaling NaN                               /// 00350
			 0x4b000000,                                                  // 8388608.0                                   /// 00354
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00368
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0036c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00370
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80011111,                                                  // -9.7958E-41                                 /// 0037c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00380
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00384
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00390
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000000,                                                  // zero                                        /// 00398
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0039c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0xffc00001,                                                  // -signaling NaN                              /// 003c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c4
			 0xbf028f5c,                                                  // -0.51                                       /// 003c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00400
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00408
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0040c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00410
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00414
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00418
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0041c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00420
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00424
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0042c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00438
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0043c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00440
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00448
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0044c
			 0x7fc00001,                                                  // signaling NaN                               /// 00450
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00454
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00460
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00464
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0046c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00470
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00474
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00478
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0047c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00480
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00484
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00488
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0048c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00494
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00498
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0049c
			 0x80000000,                                                  // -zero                                       /// 004a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a4
			 0x3f028f5c,                                                  // 0.51                                        /// 004a8
			 0xffc00001,                                                  // -signaling NaN                              /// 004ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 004b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 004d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00504
			 0x80000000,                                                  // -zero                                       /// 00508
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0050c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00514
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00518
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00520
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00524
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0052c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00534
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00538
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x7f800000,                                                  // inf                                         /// 00548
			 0x80000000,                                                  // -zero                                       /// 0054c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00550
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00554
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00558
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0055c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00564
			 0xbf028f5c,                                                  // -0.51                                       /// 00568
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0056c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00570
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0057c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00590
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00594
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00598
			 0x7fc00001,                                                  // signaling NaN                               /// 0059c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d4
			 0x3f028f5c,                                                  // 0.51                                        /// 005d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00600
			 0x00000000,                                                  // zero                                        /// 00604
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00608
			 0x7f800000,                                                  // inf                                         /// 0060c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80000000,                                                  // -zero                                       /// 0061c
			 0x00000000,                                                  // zero                                        /// 00620
			 0x80011111,                                                  // -9.7958E-41                                 /// 00624
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00628
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0062c
			 0xcb000000,                                                  // -8388608.0                                  /// 00630
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00634
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00640
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0064c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00654
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00658
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0065c
			 0x3f028f5c,                                                  // 0.51                                        /// 00660
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00664
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00668
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0066c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e000001,                                                  // Trailing 1s:                                /// 00674
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00684
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0068c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00690
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00694
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0069c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c0
			 0x00000000,                                                  // zero                                        /// 006c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00700
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00710
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0071c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0072c
			 0xcb000000,                                                  // -8388608.0                                  /// 00730
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00734
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00744
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00750
			 0xff800000,                                                  // -inf                                        /// 00754
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00760
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00768
			 0x55555555,                                                  // 4 random values                             /// 0076c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00778
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00784
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00798
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0079c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00800
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00804
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0e000007,                                                  // Trailing 1s:                                /// 0080c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00810
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00818
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0081c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00824
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00828
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0082c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00830
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00834
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00838
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0083c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00840
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00844
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00848
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00850
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00854
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00868
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00870
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00874
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00878
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00884
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00888
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x4b000000,                                                  // 8388608.0                                   /// 00890
			 0x7fc00001,                                                  // signaling NaN                               /// 00894
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00898
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0089c
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008ec
			 0x00000000,                                                  // zero                                        /// 008f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00900
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00904
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0090c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00910
			 0x4b000000,                                                  // 8388608.0                                   /// 00914
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00918
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00924
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00928
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0092c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00930
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00934
			 0x0c700000,                                                  // Leading 1s:                                 /// 00938
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0093c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00944
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00948
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00954
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00958
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00968
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00974
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00978
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00980
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00984
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00990
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0099c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a0
			 0x33333333,                                                  // 4 random values                             /// 009a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009ac
			 0x33333333,                                                  // 4 random values                             /// 009b0
			 0x80000000,                                                  // -zero                                       /// 009b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x00000000,                                                  // zero                                        /// 009ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x33333333,                                                  // 4 random values                             /// 00a60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a98
			 0x33333333,                                                  // 4 random values                             /// 00a9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xbf028f5c,                                                  // -0.51                                       /// 00aa8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ab0
			 0x7f800000,                                                  // inf                                         /// 00ab4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ab8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00abc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ac4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ac8
			 0x33333333,                                                  // 4 random values                             /// 00acc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ad4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00adc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ae4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0xffc00001,                                                  // -signaling NaN                              /// 00af4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00af8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00afc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b04
			 0x55555555,                                                  // 4 random values                             /// 00b08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xffc00001,                                                  // -signaling NaN                              /// 00b44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b64
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x33333333,                                                  // 4 random values                             /// 00bb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00be0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bf4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c10
			 0xff800000,                                                  // -inf                                        /// 00c14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xbf028f5c,                                                  // -0.51                                       /// 00c58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cb0
			 0x7f800000,                                                  // inf                                         /// 00cb4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ccc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cf0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cf4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0xff800000,                                                  // -inf                                        /// 00d14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00000000,                                                  // zero                                        /// 00d48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d5c
			 0x33333333,                                                  // 4 random values                             /// 00d60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da0
			 0x33333333,                                                  // 4 random values                             /// 00da4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ddc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00de8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df4
			 0x7fc00001,                                                  // signaling NaN                               /// 00df8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0xcb000000,                                                  // -8388608.0                                  /// 00e48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x55555555,                                                  // 4 random values                             /// 00ea0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ebc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ec8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ed4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00edc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ee4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ee8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ef4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ef8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0xff800000,                                                  // -inf                                        /// 00f40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7fc00001,                                                  // signaling NaN                               /// 00f54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f64
			 0x4b000000,                                                  // 8388608.0                                   /// 00f68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f74
			 0xff800000,                                                  // -inf                                        /// 00f78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fbc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ff4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0xffffffff // QNan                                          // SP - ve numbers                             /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000000,                                                  // zero                                        /// 00000
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00004
			 0xcb000000,                                                  // -8388608.0                                  /// 00008
			 0xff800000,                                                  // -inf                                        /// 0000c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00010
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00014
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00018
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0001c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00020
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00028
			 0x0c400000,                                                  // Leading 1s:                                 /// 0002c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00030
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00034
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00038
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0003c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00044
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0004c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00050
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0005c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00060
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00068
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00074
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00078
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0007c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00080
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00088
			 0x55555555,                                                  // 4 random values                             /// 0008c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00094
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00098
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0009c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a4
			 0x3f028f5c,                                                  // 0.51                                        /// 000a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00104
			 0x00000000,                                                  // zero                                        /// 00108
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0010c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00110
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00114
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00118
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0011c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00124
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0012c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00130
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00138
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0013c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0014c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00150
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00154
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0015c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0016c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00170
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00174
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0017c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00180
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0e000001,                                                  // Trailing 1s:                                /// 00188
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00190
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00198
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0019c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ac
			 0xff800000,                                                  // -inf                                        /// 001b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0xcb000000,                                                  // -8388608.0                                  /// 001f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00204
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00208
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0020c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00210
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0xff800000,                                                  // -inf                                        /// 00218
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00220
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00224
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0022c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00230
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00234
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00240
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00244
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00248
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0c780000,                                                  // Leading 1s:                                 /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00260
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00264
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0026c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00270
			 0x0c600000,                                                  // Leading 1s:                                 /// 00274
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00280
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00284
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00288
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0029c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x33333333,                                                  // 4 random values                             /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0030c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00310
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00320
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00324
			 0x4b000000,                                                  // 8388608.0                                   /// 00328
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0032c
			 0xcb000000,                                                  // -8388608.0                                  /// 00330
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00334
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00338
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0033c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00344
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00348
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00350
			 0x33333333,                                                  // 4 random values                             /// 00354
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00358
			 0x33333333,                                                  // 4 random values                             /// 0035c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00360
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00368
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00370
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00374
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0037c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0c600000,                                                  // Leading 1s:                                 /// 00384
			 0x33333333,                                                  // 4 random values                             /// 00388
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0xffc00001,                                                  // -signaling NaN                              /// 00398
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0039c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x7f800000,                                                  // inf                                         /// 003ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x7fc00001,                                                  // signaling NaN                               /// 00404
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00408
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x55555555,                                                  // 4 random values                             /// 00418
			 0xffc00001,                                                  // -signaling NaN                              /// 0041c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00420
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000000,                                                  // -zero                                       /// 0042c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00430
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00434
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00444
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00448
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0044c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00450
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00454
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00458
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0045c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00460
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0046c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00470
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00474
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0047c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00480
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00484
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00488
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x33333333,                                                  // 4 random values                             /// 00490
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00494
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a8
			 0x80000000,                                                  // -zero                                       /// 004ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00500
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00508
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0050c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00510
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00518
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0051c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00520
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xbf028f5c,                                                  // -0.51                                       /// 00530
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00534
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00544
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00548
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0054c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00550
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00554
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00564
			 0x00000000,                                                  // zero                                        /// 00568
			 0x0c600000,                                                  // Leading 1s:                                 /// 0056c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00570
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x3f028f5c,                                                  // 0.51                                        /// 0057c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00580
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00588
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0058c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00594
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005ac
			 0x3f028f5c,                                                  // 0.51                                        /// 005b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x3f028f5c,                                                  // 0.51                                        /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005bc
			 0x00000000,                                                  // zero                                        /// 005c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005cc
			 0xcb000000,                                                  // -8388608.0                                  /// 005d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00604
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00608
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00610
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00618
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x7fc00001,                                                  // signaling NaN                               /// 00628
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00638
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00644
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00648
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00650
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00654
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00658
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0065c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00664
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0066c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00674
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0068c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x3f028f5c,                                                  // 0.51                                        /// 00704
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00708
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0070c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00710
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00714
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00718
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0071c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00728
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0073c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00748
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00750
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e000003,                                                  // Trailing 1s:                                /// 00758
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0075c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00768
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00770
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0077c
			 0x7f800000,                                                  // inf                                         /// 00780
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0078c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00798
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0079c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f8
			 0xbf028f5c,                                                  // -0.51                                       /// 007fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00800
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00808
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0080c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00814
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00818
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0081c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00820
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00824
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0xffc00001,                                                  // -signaling NaN                              /// 0082c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00830
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00838
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00840
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00848
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00850
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00854
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00858
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0085c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00860
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00864
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00868
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00874
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00878
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00880
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00884
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0088c
			 0x55555555,                                                  // 4 random values                             /// 00890
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00894
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0089c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00900
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00904
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00908
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0090c
			 0x00000000,                                                  // zero                                        /// 00910
			 0x0e000007,                                                  // Trailing 1s:                                /// 00914
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00918
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00924
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00930
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00934
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0093c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00940
			 0x80000000,                                                  // -zero                                       /// 00944
			 0xffc00001,                                                  // -signaling NaN                              /// 00948
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0094c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00958
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0095c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00960
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00964
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00968
			 0x55555555,                                                  // 4 random values                             /// 0096c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00970
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x55555555,                                                  // 4 random values                             /// 00978
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0097c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00988
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00990
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00994
			 0xff800000,                                                  // -inf                                        /// 00998
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0099c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a8
			 0x33333333,                                                  // 4 random values                             /// 009ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a20
			 0xcb000000,                                                  // -8388608.0                                  /// 00a24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00aa8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00abc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ac4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00acc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad0
			 0x00000000,                                                  // zero                                        /// 00ad4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00afc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b30
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b34
			 0x33333333,                                                  // 4 random values                             /// 00b38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b5c
			 0xff800000,                                                  // -inf                                        /// 00b60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ba0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c60
			 0x33333333,                                                  // 4 random values                             /// 00c64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x7f800000,                                                  // inf                                         /// 00c74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000000,                                                  // zero                                        /// 00c94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ca4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ca8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ccc
			 0xff800000,                                                  // -inf                                        /// 00cd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cd4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ce4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cec
			 0xff800000,                                                  // -inf                                        /// 00cf0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cfc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d0c
			 0x7f800000,                                                  // inf                                         /// 00d10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d14
			 0xff800000,                                                  // -inf                                        /// 00d18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f800000,                                                  // inf                                         /// 00d20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d44
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d70
			 0x33333333,                                                  // 4 random values                             /// 00d74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dcc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ddc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7fc00001,                                                  // signaling NaN                               /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e28
			 0x7f800000,                                                  // inf                                         /// 00e2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x33333333,                                                  // 4 random values                             /// 00e40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0xffc00001,                                                  // -signaling NaN                              /// 00e4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e54
			 0xcb000000,                                                  // -8388608.0                                  /// 00e58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e78
			 0x7f800000,                                                  // inf                                         /// 00e7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e8c
			 0x7f800000,                                                  // inf                                         /// 00e90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ea0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000000,                                                  // -zero                                       /// 00ec0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ec8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ecc
			 0x7f800000,                                                  // inf                                         /// 00ed0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ef4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ef8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00efc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff8
			 0xAAAAAAAA                                                  // 4 random values                             /// last
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
			 0x00000558,
			 0x00000510,
			 0x000005bc,
			 0x00000640,
			 0x00000614,
			 0x00000584,
			 0x00000698,
			 0x000002f0,

			 /// vpu register f2
			 0x41980000,
			 0x41100000,
			 0x41400000,
			 0x41700000,
			 0x41a80000,
			 0x41880000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f3
			 0x40e00000,
			 0x41200000,
			 0x41f00000,
			 0x40000000,
			 0x42000000,
			 0x40000000,
			 0x40800000,
			 0x41a00000,

			 /// vpu register f4
			 0x41c00000,
			 0x41f00000,
			 0x41300000,
			 0x41e00000,
			 0x42000000,
			 0x40c00000,
			 0x40e00000,
			 0x41900000,

			 /// vpu register f5
			 0x42000000,
			 0x41e80000,
			 0x41d00000,
			 0x41880000,
			 0x41f00000,
			 0x42000000,
			 0x41500000,
			 0x40400000,

			 /// vpu register f6
			 0x40c00000,
			 0x40e00000,
			 0x41b00000,
			 0x41f80000,
			 0x41c80000,
			 0x41900000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f7
			 0x41d80000,
			 0x41e00000,
			 0x42000000,
			 0x41200000,
			 0x41d80000,
			 0x41500000,
			 0x42000000,
			 0x41d80000,

			 /// vpu register f8
			 0x40e00000,
			 0x40e00000,
			 0x40800000,
			 0x3f800000,
			 0x41b00000,
			 0x41d00000,
			 0x3f800000,
			 0x40e00000,

			 /// vpu register f9
			 0x41600000,
			 0x40e00000,
			 0x41880000,
			 0x41c80000,
			 0x41b00000,
			 0x41a00000,
			 0x41880000,
			 0x40400000,

			 /// vpu register f10
			 0x41f00000,
			 0x41400000,
			 0x41800000,
			 0x41c00000,
			 0x3f800000,
			 0x40000000,
			 0x40400000,
			 0x41c80000,

			 /// vpu register f11
			 0x41e80000,
			 0x3f800000,
			 0x41b00000,
			 0x41980000,
			 0x41880000,
			 0x40400000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f12
			 0x3f800000,
			 0x40800000,
			 0x41800000,
			 0x41400000,
			 0x41880000,
			 0x41900000,
			 0x40000000,
			 0x40000000,

			 /// vpu register f13
			 0x40400000,
			 0x41300000,
			 0x41300000,
			 0x41f00000,
			 0x41300000,
			 0x41200000,
			 0x41500000,
			 0x40800000,

			 /// vpu register f14
			 0x41800000,
			 0x41a80000,
			 0x41900000,
			 0x42000000,
			 0x41a00000,
			 0x41e00000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f15
			 0x41b80000,
			 0x41400000,
			 0x41e80000,
			 0x41e00000,
			 0x41000000,
			 0x41900000,
			 0x41800000,
			 0x41e80000,

			 /// vpu register f16
			 0x41d00000,
			 0x41e00000,
			 0x40400000,
			 0x41e80000,
			 0x41e80000,
			 0x41300000,
			 0x41980000,
			 0x41d80000,

			 /// vpu register f17
			 0x41b00000,
			 0x41d00000,
			 0x41c80000,
			 0x41b80000,
			 0x40c00000,
			 0x40e00000,
			 0x41200000,
			 0x41700000,

			 /// vpu register f18
			 0x41400000,
			 0x41a00000,
			 0x40000000,
			 0x41b00000,
			 0x41000000,
			 0x41100000,
			 0x41300000,
			 0x41900000,

			 /// vpu register f19
			 0x41c80000,
			 0x41900000,
			 0x40e00000,
			 0x41000000,
			 0x41f80000,
			 0x41a80000,
			 0x40800000,
			 0x41f80000,

			 /// vpu register f20
			 0x41980000,
			 0x40c00000,
			 0x41000000,
			 0x41000000,
			 0x41700000,
			 0x41e00000,
			 0x3f800000,
			 0x40a00000,

			 /// vpu register f21
			 0x41880000,
			 0x41600000,
			 0x40e00000,
			 0x41980000,
			 0x41900000,
			 0x41600000,
			 0x41f80000,
			 0x41400000,

			 /// vpu register f22
			 0x41000000,
			 0x40400000,
			 0x42000000,
			 0x41980000,
			 0x41b00000,
			 0x41000000,
			 0x41700000,
			 0x41100000,

			 /// vpu register f23
			 0x40400000,
			 0x41000000,
			 0x41c00000,
			 0x41700000,
			 0x41980000,
			 0x41000000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f24
			 0x41900000,
			 0x41400000,
			 0x41a00000,
			 0x41a00000,
			 0x41a00000,
			 0x41a80000,
			 0x41600000,
			 0x3f800000,

			 /// vpu register f25
			 0x41f80000,
			 0x3f800000,
			 0x41a80000,
			 0x41000000,
			 0x40a00000,
			 0x41c00000,
			 0x41a80000,
			 0x41300000,

			 /// vpu register f26
			 0x41e00000,
			 0x41900000,
			 0x41b00000,
			 0x41d00000,
			 0x3f800000,
			 0x40800000,
			 0x40e00000,
			 0x41000000,

			 /// vpu register f27
			 0x41a00000,
			 0x41a80000,
			 0x41400000,
			 0x41d80000,
			 0x41900000,
			 0x41c00000,
			 0x41700000,
			 0x41100000,

			 /// vpu register f28
			 0x41a00000,
			 0x40000000,
			 0x41a80000,
			 0x41d00000,
			 0x41f00000,
			 0x40e00000,
			 0x41a80000,
			 0x41700000,

			 /// vpu register f29
			 0x41e00000,
			 0x41d80000,
			 0x40e00000,
			 0x41d80000,
			 0x41980000,
			 0x41100000,
			 0x41300000,
			 0x41400000,

			 /// vpu register f30
			 0x41900000,
			 0x41200000,
			 0x41800000,
			 0x41b00000,
			 0x41c80000,
			 0x41a00000,
			 0x41a80000,
			 0x3f800000,

			 /// vpu register f31
			 0x40800000,
			 0x41e00000,
			 0x40400000,
			 0x41f00000,
			 0x41e00000,
			 0x41500000,
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
		"nop\n"
		"nop\n"
		"famomaxg.ps f27, f1 (x12)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f21, f1 (x13)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f29, f1 (x12)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f24, f1 (x11)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f2, f1 (x15)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f22, f1 (x11)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f12, f1 (x14)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f22, f1 (x15)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f12, f1 (x14)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f28, f1 (x15)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f20, f1 (x15)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f24, f1 (x12)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f16, f1 (x13)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f28, f1 (x15)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f25, f1 (x11)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f29, f1 (x11)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f21, f1 (x13)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x14)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f6, f1 (x12)\n"                          ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f19, f1 (x13)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f26, f1 (x14)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f8, f1 (x11)\n"                          ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f27, f1 (x14)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f26, f1 (x15)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f16, f1 (x13)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f29, f1 (x15)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x11)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f25, f1 (x12)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f30, f1 (x13)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f3, f1 (x12)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f22, f1 (x13)\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f4, f1 (x13)\n"                          ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f25, f1 (x15)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f27, f1 (x11)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f16, f1 (x11)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f8, f1 (x14)\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f5, f1 (x13)\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f26, f1 (x13)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f4, f1 (x13)\n"                          ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f13, f1 (x12)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f8, f1 (x12)\n"                          ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f8, f1 (x14)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f30, f1 (x13)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f16, f1 (x12)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f5, f1 (x12)\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f22, f1 (x11)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f22, f1 (x12)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f10, f1 (x12)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x15)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f2, f1 (x11)\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f4, f1 (x15)\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f13, f1 (x11)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f3, f1 (x14)\n"                          ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f21, f1 (x14)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f30, f1 (x11)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f25, f1 (x13)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f19, f1 (x12)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f6, f1 (x11)\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f20, f1 (x11)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f20, f1 (x11)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f9, f1 (x12)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x13)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f12, f1 (x11)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f7, f1 (x13)\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f6, f1 (x14)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f30, f1 (x13)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f26, f1 (x15)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x12)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f13, f1 (x14)\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f29, f1 (x12)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f12, f1 (x14)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f8, f1 (x15)\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f20, f1 (x12)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f14, f1 (x15)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f8, f1 (x12)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f13, f1 (x13)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f12, f1 (x14)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f25, f1 (x15)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f28, f1 (x12)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f29, f1 (x11)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f21, f1 (x13)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f17, f1 (x12)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x12)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f3, f1 (x12)\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f14, f1 (x15)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f9, f1 (x14)\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f2, f1 (x11)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x14)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f7, f1 (x12)\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f30, f1 (x12)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f22, f1 (x12)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f15, f1 (x11)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f15, f1 (x12)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f18, f1 (x14)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f12, f1 (x12)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f21, f1 (x13)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f7, f1 (x11)\n"                          ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f28, f1 (x11)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.ps f30, f1 (x12)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
