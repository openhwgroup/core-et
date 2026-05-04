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

#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {
        *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

	uint64_t mid, tid;
	
	mid = get_minion_id();
	tid = get_thread_id();
	if (mid != 0 && tid != 0) { C_TEST_FAIL; return -1;};
	volatile uint32_t m_11[1024] __attribute__ ((aligned (4096))) = {
			 0xb8fb371b, /// 0x0
			 0x4a8a44cd, /// 0x4
			 0xfccbf8f4, /// 0x8
			 0x11243da3, /// 0xc
			 0x9bce85ee, /// 0x10
			 0x8dda82fe, /// 0x14
			 0x75d1cecf, /// 0x18
			 0xc05c3443, /// 0x1c
			 0x40c9c2db, /// 0x20
			 0x0d9e0aa0, /// 0x24
			 0xe2bc84c6, /// 0x28
			 0x8a606d0b, /// 0x2c
			 0x78c48d2b, /// 0x30
			 0x7e6f9e36, /// 0x34
			 0x94335099, /// 0x38
			 0x4bfab8cd, /// 0x3c
			 0xc4a4fdb0, /// 0x40
			 0xe3d6a8ec, /// 0x44
			 0xab25f669, /// 0x48
			 0xe257537a, /// 0x4c
			 0x083dbba1, /// 0x50
			 0x9544344d, /// 0x54
			 0x3ce90bbf, /// 0x58
			 0xe3d4a87c, /// 0x5c
			 0x89d8d701, /// 0x60
			 0x55516fe3, /// 0x64
			 0xc28fa27e, /// 0x68
			 0xd5f384e4, /// 0x6c
			 0x5c7a6344, /// 0x70
			 0xb61b678c, /// 0x74
			 0xc5c01a62, /// 0x78
			 0x0aee1c31, /// 0x7c
			 0xf64d02a4, /// 0x80
			 0x1d475126, /// 0x84
			 0x6b734237, /// 0x88
			 0x5fedb50d, /// 0x8c
			 0xd3af3c5e, /// 0x90
			 0xd24890aa, /// 0x94
			 0xeedb59d8, /// 0x98
			 0x92a5bb6b, /// 0x9c
			 0x032f6172, /// 0xa0
			 0x16615890, /// 0xa4
			 0xbafd8d23, /// 0xa8
			 0x1db5fa52, /// 0xac
			 0x259ccf63, /// 0xb0
			 0xb64532ed, /// 0xb4
			 0x9cecf73b, /// 0xb8
			 0xb27267ee, /// 0xbc
			 0x111ba7aa, /// 0xc0
			 0xef0fc28e, /// 0xc4
			 0x48e42ddd, /// 0xc8
			 0xbf665f65, /// 0xcc
			 0x16940aa1, /// 0xd0
			 0x8da2ded4, /// 0xd4
			 0x9f24503d, /// 0xd8
			 0xeaa383e8, /// 0xdc
			 0xf9ad2152, /// 0xe0
			 0x69586ae2, /// 0xe4
			 0x35fbe319, /// 0xe8
			 0xad37a0f4, /// 0xec
			 0x2b3e9fa1, /// 0xf0
			 0xd524a84c, /// 0xf4
			 0x474dd43b, /// 0xf8
			 0x54d179c8, /// 0xfc
			 0x8d861bef, /// 0x100
			 0x485b0939, /// 0x104
			 0x2a4cd915, /// 0x108
			 0xfb0dc7fa, /// 0x10c
			 0x8550a4b3, /// 0x110
			 0xdbb6b09f, /// 0x114
			 0x378ebb6f, /// 0x118
			 0xa9ac9122, /// 0x11c
			 0x02eabaab, /// 0x120
			 0xc7232971, /// 0x124
			 0x810bd4b9, /// 0x128
			 0x150ecdda, /// 0x12c
			 0xc4995ce1, /// 0x130
			 0xb7a4bec5, /// 0x134
			 0xe30315b9, /// 0x138
			 0xd0787f5b, /// 0x13c
			 0x875bf795, /// 0x140
			 0x763377cb, /// 0x144
			 0x3db01e7c, /// 0x148
			 0x36dd6879, /// 0x14c
			 0x46a009e1, /// 0x150
			 0x1956e55e, /// 0x154
			 0x2dfa5a48, /// 0x158
			 0x1a4b45cf, /// 0x15c
			 0x13193dd6, /// 0x160
			 0x26a58868, /// 0x164
			 0xabe8d071, /// 0x168
			 0x3403ab1d, /// 0x16c
			 0x2143d3f9, /// 0x170
			 0xd80c9b1f, /// 0x174
			 0xd97e4022, /// 0x178
			 0x0735574a, /// 0x17c
			 0x726a9956, /// 0x180
			 0x001df271, /// 0x184
			 0x34eb304f, /// 0x188
			 0xe256eb3a, /// 0x18c
			 0x09573a3f, /// 0x190
			 0x8a1758a5, /// 0x194
			 0x08a8bca5, /// 0x198
			 0x451f0d88, /// 0x19c
			 0x9d4d9ce7, /// 0x1a0
			 0xa99cb13c, /// 0x1a4
			 0x3ff725f0, /// 0x1a8
			 0xa634d2d5, /// 0x1ac
			 0x47982141, /// 0x1b0
			 0x03c55f83, /// 0x1b4
			 0xd54c068d, /// 0x1b8
			 0x23c24e11, /// 0x1bc
			 0xb34e32b8, /// 0x1c0
			 0x9433b631, /// 0x1c4
			 0xed83d5d6, /// 0x1c8
			 0xc29ed3a7, /// 0x1cc
			 0x172efdbf, /// 0x1d0
			 0xb643a97b, /// 0x1d4
			 0xe22532db, /// 0x1d8
			 0x24194a1d, /// 0x1dc
			 0x9de64134, /// 0x1e0
			 0x48feca04, /// 0x1e4
			 0x0e5dfaf0, /// 0x1e8
			 0xc40a69c0, /// 0x1ec
			 0x5fcc24a7, /// 0x1f0
			 0xcb125483, /// 0x1f4
			 0x90c97945, /// 0x1f8
			 0x55daca2d, /// 0x1fc
			 0x89cd7f13, /// 0x200
			 0xf004df5b, /// 0x204
			 0x8bb7a1d4, /// 0x208
			 0xc6dca1da, /// 0x20c
			 0x20d0d06b, /// 0x210
			 0x0a5c12f9, /// 0x214
			 0xc77f91ef, /// 0x218
			 0x315937a6, /// 0x21c
			 0x80480d0d, /// 0x220
			 0x86f79a42, /// 0x224
			 0xb8b7d539, /// 0x228
			 0x9997e250, /// 0x22c
			 0xd40021ba, /// 0x230
			 0x5c23b902, /// 0x234
			 0xae16c558, /// 0x238
			 0xa7739840, /// 0x23c
			 0x165e20d1, /// 0x240
			 0xb21ca3e0, /// 0x244
			 0x4a034b3d, /// 0x248
			 0x742ee4c7, /// 0x24c
			 0xef746452, /// 0x250
			 0x76262347, /// 0x254
			 0xa42c7e5d, /// 0x258
			 0xb7b84964, /// 0x25c
			 0xcec3dac7, /// 0x260
			 0x5120d035, /// 0x264
			 0x88629031, /// 0x268
			 0xe259ace3, /// 0x26c
			 0xe3bb9da4, /// 0x270
			 0xa0d9574e, /// 0x274
			 0x8dfb82fe, /// 0x278
			 0x019fd581, /// 0x27c
			 0x225442f1, /// 0x280
			 0x4b200fc8, /// 0x284
			 0xdccc46f8, /// 0x288
			 0x3a9f5206, /// 0x28c
			 0x28080368, /// 0x290
			 0xa9393b65, /// 0x294
			 0xe23bcbdb, /// 0x298
			 0x8feded5a, /// 0x29c
			 0x8d03008e, /// 0x2a0
			 0x23639c15, /// 0x2a4
			 0xd916081b, /// 0x2a8
			 0x00d2aa10, /// 0x2ac
			 0xd4fcb174, /// 0x2b0
			 0x7b242376, /// 0x2b4
			 0x927cf9ef, /// 0x2b8
			 0xfeef653d, /// 0x2bc
			 0x729c035b, /// 0x2c0
			 0xfbb02ba6, /// 0x2c4
			 0x8197fe48, /// 0x2c8
			 0xb044f095, /// 0x2cc
			 0xfe65a4bc, /// 0x2d0
			 0xf0ff00d6, /// 0x2d4
			 0x8c9a9f27, /// 0x2d8
			 0x0ab4ef91, /// 0x2dc
			 0x43455d86, /// 0x2e0
			 0x437ef511, /// 0x2e4
			 0xc4c74ddb, /// 0x2e8
			 0x9eaeb1fc, /// 0x2ec
			 0x0aeb8d30, /// 0x2f0
			 0x93c5dd59, /// 0x2f4
			 0x29530678, /// 0x2f8
			 0xc24e554f, /// 0x2fc
			 0x58dc7bf2, /// 0x300
			 0x3f03a1ce, /// 0x304
			 0x62428a32, /// 0x308
			 0xf899cedb, /// 0x30c
			 0x44c6f631, /// 0x310
			 0x52659821, /// 0x314
			 0xdb8a7d24, /// 0x318
			 0xae84ef19, /// 0x31c
			 0x5b1c7721, /// 0x320
			 0x5f1d68d2, /// 0x324
			 0x01f4c5fe, /// 0x328
			 0x7982dbe6, /// 0x32c
			 0x7918e219, /// 0x330
			 0xab41165f, /// 0x334
			 0xb44f934d, /// 0x338
			 0x870743c2, /// 0x33c
			 0xa758183d, /// 0x340
			 0x4face33d, /// 0x344
			 0x88896ff9, /// 0x348
			 0xa8931efd, /// 0x34c
			 0x0d25387a, /// 0x350
			 0x9dd662bd, /// 0x354
			 0x9963b206, /// 0x358
			 0x61a56a6f, /// 0x35c
			 0xa5ef6aa3, /// 0x360
			 0xce63152c, /// 0x364
			 0x3026add3, /// 0x368
			 0x70adf6d0, /// 0x36c
			 0x90ca964e, /// 0x370
			 0x6210ff87, /// 0x374
			 0xfa597394, /// 0x378
			 0x7d0746ed, /// 0x37c
			 0xa87d7436, /// 0x380
			 0xb580934d, /// 0x384
			 0xcbcad5e2, /// 0x388
			 0x0207bfcf, /// 0x38c
			 0xe1e9a816, /// 0x390
			 0x35abe8c2, /// 0x394
			 0x800c6a17, /// 0x398
			 0x26e06517, /// 0x39c
			 0xdc2f11d5, /// 0x3a0
			 0x0e34d818, /// 0x3a4
			 0x66231df6, /// 0x3a8
			 0x68d5ec00, /// 0x3ac
			 0x6f61c03d, /// 0x3b0
			 0xbb8f0080, /// 0x3b4
			 0x828a1412, /// 0x3b8
			 0xde6e2867, /// 0x3bc
			 0x5f0f870e, /// 0x3c0
			 0x6577fe9e, /// 0x3c4
			 0x763943f6, /// 0x3c8
			 0xef314094, /// 0x3cc
			 0x8ab95c02, /// 0x3d0
			 0xd1af70d5, /// 0x3d4
			 0xdd3081a0, /// 0x3d8
			 0x981036de, /// 0x3dc
			 0x1ada18d7, /// 0x3e0
			 0x74f5bb08, /// 0x3e4
			 0x6c84cc21, /// 0x3e8
			 0x7818faac, /// 0x3ec
			 0xfdd066e6, /// 0x3f0
			 0x9499251d, /// 0x3f4
			 0x523cb8c7, /// 0x3f8
			 0x367ccbc9, /// 0x3fc
			 0x3109dc86, /// 0x400
			 0x7e882d1d, /// 0x404
			 0x3fefb657, /// 0x408
			 0x2685dcef, /// 0x40c
			 0x8b434617, /// 0x410
			 0x6554efd2, /// 0x414
			 0x9d8a3193, /// 0x418
			 0xf7e2b286, /// 0x41c
			 0xafedc324, /// 0x420
			 0xa014581f, /// 0x424
			 0x0ed66e02, /// 0x428
			 0x679dfe96, /// 0x42c
			 0x7a99ca76, /// 0x430
			 0x8f2d686d, /// 0x434
			 0xeae7ded3, /// 0x438
			 0xddcdcf12, /// 0x43c
			 0xd33214a4, /// 0x440
			 0x5db8f17c, /// 0x444
			 0x1b9df43d, /// 0x448
			 0x8d9323d7, /// 0x44c
			 0x31f87746, /// 0x450
			 0x75bdd6e8, /// 0x454
			 0xd5f796c5, /// 0x458
			 0x27d9aa6a, /// 0x45c
			 0xcdc202a8, /// 0x460
			 0x696d3e63, /// 0x464
			 0xe59ee51f, /// 0x468
			 0xd1bc63df, /// 0x46c
			 0x3e0c6ee2, /// 0x470
			 0x4464156d, /// 0x474
			 0xe5f2df4d, /// 0x478
			 0x80e5d39c, /// 0x47c
			 0xcd5c69eb, /// 0x480
			 0xadd6a955, /// 0x484
			 0xf1b3ff6d, /// 0x488
			 0xb0fc1d4a, /// 0x48c
			 0x95e4c640, /// 0x490
			 0x9dfaad0c, /// 0x494
			 0xa3d51531, /// 0x498
			 0x93787f1f, /// 0x49c
			 0xeaca468d, /// 0x4a0
			 0xb60a6349, /// 0x4a4
			 0xfc30535a, /// 0x4a8
			 0xe1639b92, /// 0x4ac
			 0xe38200d5, /// 0x4b0
			 0x3b4c2a6b, /// 0x4b4
			 0x4cbf101b, /// 0x4b8
			 0xe27884df, /// 0x4bc
			 0x70df8129, /// 0x4c0
			 0x0fbb3171, /// 0x4c4
			 0x161f8c42, /// 0x4c8
			 0x192ddddb, /// 0x4cc
			 0x8ca1b176, /// 0x4d0
			 0x6e106f50, /// 0x4d4
			 0xdc64ffcd, /// 0x4d8
			 0x011809bc, /// 0x4dc
			 0x1e24b40e, /// 0x4e0
			 0xb5e829e6, /// 0x4e4
			 0x4371dc4d, /// 0x4e8
			 0x0dd4fb6f, /// 0x4ec
			 0x00b4e53a, /// 0x4f0
			 0x8e2db8b0, /// 0x4f4
			 0x65ac4945, /// 0x4f8
			 0xb6cae1e5, /// 0x4fc
			 0x22db762d, /// 0x500
			 0x8911ecc7, /// 0x504
			 0x4d2515bd, /// 0x508
			 0x69a244b4, /// 0x50c
			 0xe285ba8a, /// 0x510
			 0xfa49cf12, /// 0x514
			 0xae3fc1ae, /// 0x518
			 0xca4f5429, /// 0x51c
			 0x19f16454, /// 0x520
			 0x869849e5, /// 0x524
			 0x315cb7ba, /// 0x528
			 0x0fe7e146, /// 0x52c
			 0x0c1ff1d9, /// 0x530
			 0xcbd34536, /// 0x534
			 0xe3803bfc, /// 0x538
			 0xe4456230, /// 0x53c
			 0x28ed6524, /// 0x540
			 0x17bf0c09, /// 0x544
			 0x3ceb1983, /// 0x548
			 0x7922e67a, /// 0x54c
			 0x530413ab, /// 0x550
			 0xee7158ca, /// 0x554
			 0x1b863276, /// 0x558
			 0x42515a2c, /// 0x55c
			 0x151978a5, /// 0x560
			 0x0baf6903, /// 0x564
			 0x19dc9fbd, /// 0x568
			 0xadf034e2, /// 0x56c
			 0x45b4211c, /// 0x570
			 0x92c7ab04, /// 0x574
			 0x30fa03d4, /// 0x578
			 0xee92a8d5, /// 0x57c
			 0x3d2c2f94, /// 0x580
			 0x2bdf4066, /// 0x584
			 0x3a6534ed, /// 0x588
			 0xf90ddb50, /// 0x58c
			 0x8d4b440d, /// 0x590
			 0x2bf74d58, /// 0x594
			 0x582cc3a3, /// 0x598
			 0xfda4126e, /// 0x59c
			 0xdd02a528, /// 0x5a0
			 0xc8eca544, /// 0x5a4
			 0x75b28b97, /// 0x5a8
			 0x21ce780c, /// 0x5ac
			 0x77cd0f6d, /// 0x5b0
			 0x8c6210ef, /// 0x5b4
			 0x7e7ce8cf, /// 0x5b8
			 0x6c686d46, /// 0x5bc
			 0xab7949c4, /// 0x5c0
			 0xa77ddead, /// 0x5c4
			 0x06f897b5, /// 0x5c8
			 0xfaf3ea3f, /// 0x5cc
			 0x40a6059d, /// 0x5d0
			 0x47ce2364, /// 0x5d4
			 0x88025566, /// 0x5d8
			 0xc227cd05, /// 0x5dc
			 0x2065bfea, /// 0x5e0
			 0x121fab6b, /// 0x5e4
			 0x64a16797, /// 0x5e8
			 0x876a66b2, /// 0x5ec
			 0x8c0d9d85, /// 0x5f0
			 0x5e4bbef9, /// 0x5f4
			 0xf9511d39, /// 0x5f8
			 0x99375432, /// 0x5fc
			 0x316561a0, /// 0x600
			 0x4bdd01f5, /// 0x604
			 0xc585ff22, /// 0x608
			 0x580f50dd, /// 0x60c
			 0xef22eedc, /// 0x610
			 0x6b51af39, /// 0x614
			 0x697e7b34, /// 0x618
			 0xc989ff61, /// 0x61c
			 0x8fbde12a, /// 0x620
			 0xfa093237, /// 0x624
			 0x9f6d9158, /// 0x628
			 0x948cd483, /// 0x62c
			 0x4dd84b9b, /// 0x630
			 0x4c4e3cb9, /// 0x634
			 0x8e0ea54f, /// 0x638
			 0x9fff5d51, /// 0x63c
			 0x2c60801c, /// 0x640
			 0x05088db7, /// 0x644
			 0x2a20500b, /// 0x648
			 0x43461f19, /// 0x64c
			 0x0172285d, /// 0x650
			 0xc9414e9c, /// 0x654
			 0xd1a84b08, /// 0x658
			 0xf8a07549, /// 0x65c
			 0x520d6b93, /// 0x660
			 0x1a9546bc, /// 0x664
			 0x4c0d4771, /// 0x668
			 0x94a4ae1b, /// 0x66c
			 0xe9c1ffa5, /// 0x670
			 0x8b498b45, /// 0x674
			 0xb3033e5d, /// 0x678
			 0x29623518, /// 0x67c
			 0x41719eaf, /// 0x680
			 0x5ed78599, /// 0x684
			 0xcee6d2e1, /// 0x688
			 0x0ac9a6b8, /// 0x68c
			 0x4b7d82e5, /// 0x690
			 0x5a4c8ede, /// 0x694
			 0x836f8956, /// 0x698
			 0xe1bd5670, /// 0x69c
			 0xb59e4065, /// 0x6a0
			 0x21ce56c7, /// 0x6a4
			 0x8572b7bc, /// 0x6a8
			 0xeef0970a, /// 0x6ac
			 0x980cd4bf, /// 0x6b0
			 0xe368b487, /// 0x6b4
			 0xacbf9568, /// 0x6b8
			 0xf3bdbc4a, /// 0x6bc
			 0x8a93435a, /// 0x6c0
			 0x1602b61b, /// 0x6c4
			 0xf8eec314, /// 0x6c8
			 0x5d7f1c5a, /// 0x6cc
			 0x80848184, /// 0x6d0
			 0x15bce80d, /// 0x6d4
			 0x8828c458, /// 0x6d8
			 0xeda90c63, /// 0x6dc
			 0x99dfc931, /// 0x6e0
			 0x31575b5b, /// 0x6e4
			 0xbb5fb998, /// 0x6e8
			 0xa9b4263b, /// 0x6ec
			 0xe4929476, /// 0x6f0
			 0x232dac42, /// 0x6f4
			 0xa36a85b8, /// 0x6f8
			 0x20d45ac8, /// 0x6fc
			 0x78dc561f, /// 0x700
			 0xd79be0f8, /// 0x704
			 0xa1f747d9, /// 0x708
			 0x1ffc9334, /// 0x70c
			 0x9195a722, /// 0x710
			 0x00d759c2, /// 0x714
			 0x7f460450, /// 0x718
			 0x46db9b63, /// 0x71c
			 0x6c954397, /// 0x720
			 0x4e4a6aab, /// 0x724
			 0x74d0b807, /// 0x728
			 0xc88d0af6, /// 0x72c
			 0xfc44d9de, /// 0x730
			 0x18bd5ef5, /// 0x734
			 0xb549b64d, /// 0x738
			 0xa236572b, /// 0x73c
			 0xe9eff09d, /// 0x740
			 0xa49ab728, /// 0x744
			 0x0d28fdee, /// 0x748
			 0x80cded82, /// 0x74c
			 0x4c094329, /// 0x750
			 0xfacd39e9, /// 0x754
			 0xa40d462e, /// 0x758
			 0xdfd1f51f, /// 0x75c
			 0x04d8a580, /// 0x760
			 0x8f214204, /// 0x764
			 0x18364836, /// 0x768
			 0xafbb88a5, /// 0x76c
			 0x70722e28, /// 0x770
			 0x2ada0aac, /// 0x774
			 0x58f0b2b1, /// 0x778
			 0x77f8a2b3, /// 0x77c
			 0xa374cfef, /// 0x780
			 0x7c802558, /// 0x784
			 0xe6400775, /// 0x788
			 0x02ed8e5f, /// 0x78c
			 0x89308983, /// 0x790
			 0x1b9654c2, /// 0x794
			 0xe6428ba7, /// 0x798
			 0x4b486b9b, /// 0x79c
			 0x659a10e3, /// 0x7a0
			 0x285df9f5, /// 0x7a4
			 0x8588b0f5, /// 0x7a8
			 0x64c5ad23, /// 0x7ac
			 0xda69ea49, /// 0x7b0
			 0x5b3d3dd7, /// 0x7b4
			 0x7f57b9f5, /// 0x7b8
			 0xf2a4f71f, /// 0x7bc
			 0x8b325581, /// 0x7c0
			 0x6c6021dc, /// 0x7c4
			 0xac3bf197, /// 0x7c8
			 0xed34abd0, /// 0x7cc
			 0x2b342c67, /// 0x7d0
			 0x2f0f9dd4, /// 0x7d4
			 0x122ac906, /// 0x7d8
			 0x35eb5d95, /// 0x7dc
			 0xb3aea67a, /// 0x7e0
			 0xac7b5203, /// 0x7e4
			 0xbc617574, /// 0x7e8
			 0xe7b78f5b, /// 0x7ec
			 0xde194f6e, /// 0x7f0
			 0xad527ff2, /// 0x7f4
			 0x33cd2a55, /// 0x7f8
			 0x447ab61d, /// 0x7fc
			 0xd56bb95f, /// 0x800
			 0x7c5fd21a, /// 0x804
			 0x827ddc38, /// 0x808
			 0xa11fdc24, /// 0x80c
			 0x671f6f3c, /// 0x810
			 0x983d1803, /// 0x814
			 0x0f4a6b6f, /// 0x818
			 0xcfe5faa0, /// 0x81c
			 0x36e52503, /// 0x820
			 0x93d1faec, /// 0x824
			 0x5b9385e0, /// 0x828
			 0x3470951b, /// 0x82c
			 0x2bca4856, /// 0x830
			 0x566ea1de, /// 0x834
			 0x59954da6, /// 0x838
			 0xa1cc9e39, /// 0x83c
			 0x9fb13843, /// 0x840
			 0xe119672f, /// 0x844
			 0xcacf31d5, /// 0x848
			 0x22eebd95, /// 0x84c
			 0x70739281, /// 0x850
			 0x8e523525, /// 0x854
			 0x9914f9fa, /// 0x858
			 0x885bbab2, /// 0x85c
			 0x518863fd, /// 0x860
			 0x82cf81be, /// 0x864
			 0xb2114e92, /// 0x868
			 0x5159e563, /// 0x86c
			 0xc0d71ea3, /// 0x870
			 0x95230187, /// 0x874
			 0xa592b197, /// 0x878
			 0xdb3be063, /// 0x87c
			 0x0b8374d1, /// 0x880
			 0xed8b86a4, /// 0x884
			 0xadf6d140, /// 0x888
			 0x5c458d92, /// 0x88c
			 0xca977739, /// 0x890
			 0x94c99d78, /// 0x894
			 0x3fabbcd4, /// 0x898
			 0xaa4dbfd6, /// 0x89c
			 0x69bdcfe1, /// 0x8a0
			 0x84297d88, /// 0x8a4
			 0x79890954, /// 0x8a8
			 0xbfcab52b, /// 0x8ac
			 0x462d9757, /// 0x8b0
			 0x3ad03ceb, /// 0x8b4
			 0xbe36918f, /// 0x8b8
			 0x56be82f4, /// 0x8bc
			 0x7c6baaa7, /// 0x8c0
			 0x3d7c50bd, /// 0x8c4
			 0xc669d0eb, /// 0x8c8
			 0xb0e2d5a7, /// 0x8cc
			 0xf645bf60, /// 0x8d0
			 0xdb259798, /// 0x8d4
			 0xfb06bcd8, /// 0x8d8
			 0x4393ec39, /// 0x8dc
			 0x58687b00, /// 0x8e0
			 0xaa0a2cc7, /// 0x8e4
			 0x3c6385d7, /// 0x8e8
			 0x2df90fc7, /// 0x8ec
			 0xdfcffb48, /// 0x8f0
			 0x75d0d19b, /// 0x8f4
			 0x7ae88a87, /// 0x8f8
			 0xca55a0eb, /// 0x8fc
			 0x0b10d917, /// 0x900
			 0x2f525180, /// 0x904
			 0x30a7952f, /// 0x908
			 0x5fb70728, /// 0x90c
			 0x0c1536a7, /// 0x910
			 0x846752ae, /// 0x914
			 0x0e36c4ef, /// 0x918
			 0x2159e15e, /// 0x91c
			 0xb47eef02, /// 0x920
			 0xd72f7d68, /// 0x924
			 0x713f3ffa, /// 0x928
			 0x6a17f22f, /// 0x92c
			 0xb36782a3, /// 0x930
			 0x70027454, /// 0x934
			 0x695e2b15, /// 0x938
			 0x89663ecd, /// 0x93c
			 0x5e37ce11, /// 0x940
			 0xef9fb914, /// 0x944
			 0x97619250, /// 0x948
			 0x12625862, /// 0x94c
			 0x7dad0d19, /// 0x950
			 0x20697d1c, /// 0x954
			 0x4cee4fcf, /// 0x958
			 0x8e78dc85, /// 0x95c
			 0x5ef03b39, /// 0x960
			 0x3e4b95e5, /// 0x964
			 0x5ab2591b, /// 0x968
			 0xc7e36198, /// 0x96c
			 0xd8f36dde, /// 0x970
			 0xa1b0ce32, /// 0x974
			 0x68ea1259, /// 0x978
			 0x50d1a719, /// 0x97c
			 0x5bf4973f, /// 0x980
			 0x02342030, /// 0x984
			 0xab05788f, /// 0x988
			 0xcb064b72, /// 0x98c
			 0xdb404afd, /// 0x990
			 0x8daafa4d, /// 0x994
			 0xf2714967, /// 0x998
			 0x01f268d7, /// 0x99c
			 0x6d59ea55, /// 0x9a0
			 0xc5110eaa, /// 0x9a4
			 0x549699d7, /// 0x9a8
			 0xa9b380d3, /// 0x9ac
			 0xba1ac930, /// 0x9b0
			 0x899c4343, /// 0x9b4
			 0x6c36ec49, /// 0x9b8
			 0xba5a579e, /// 0x9bc
			 0x5198a868, /// 0x9c0
			 0x425f8eca, /// 0x9c4
			 0x415996bc, /// 0x9c8
			 0x3ecb54f8, /// 0x9cc
			 0x0cd8af7d, /// 0x9d0
			 0xac8e4e1f, /// 0x9d4
			 0x4a5640f5, /// 0x9d8
			 0xb76f8c58, /// 0x9dc
			 0xeefc19f0, /// 0x9e0
			 0xf6490704, /// 0x9e4
			 0x8559131e, /// 0x9e8
			 0x3ae6ecea, /// 0x9ec
			 0x25fc93ed, /// 0x9f0
			 0x75ab1591, /// 0x9f4
			 0x5a7bd601, /// 0x9f8
			 0xc35f74a3, /// 0x9fc
			 0xceeaa4d7, /// 0xa00
			 0xa21b0684, /// 0xa04
			 0xdc041847, /// 0xa08
			 0x2e64615a, /// 0xa0c
			 0xec87babe, /// 0xa10
			 0x903c1261, /// 0xa14
			 0x969d7864, /// 0xa18
			 0x5b53d7c4, /// 0xa1c
			 0x8e3e5fc0, /// 0xa20
			 0x2a5b2e50, /// 0xa24
			 0x395a7068, /// 0xa28
			 0xbccd008f, /// 0xa2c
			 0x482a672c, /// 0xa30
			 0x522def2d, /// 0xa34
			 0xf801a176, /// 0xa38
			 0xc6fe06d0, /// 0xa3c
			 0xd7059c03, /// 0xa40
			 0xe51d6961, /// 0xa44
			 0xdbab6e41, /// 0xa48
			 0x90b43d8f, /// 0xa4c
			 0x26980ef8, /// 0xa50
			 0xb1afe818, /// 0xa54
			 0x887811dc, /// 0xa58
			 0x267ecd7c, /// 0xa5c
			 0xd2d88b1d, /// 0xa60
			 0x3c5c8078, /// 0xa64
			 0x29748429, /// 0xa68
			 0x14813dcd, /// 0xa6c
			 0x53ad1c2b, /// 0xa70
			 0x6111c611, /// 0xa74
			 0xe3017f26, /// 0xa78
			 0x924ef17d, /// 0xa7c
			 0xb7c13463, /// 0xa80
			 0x35978f22, /// 0xa84
			 0x8038f9d3, /// 0xa88
			 0x3185c4a3, /// 0xa8c
			 0x47921af8, /// 0xa90
			 0x301f3eae, /// 0xa94
			 0xbe7c6a70, /// 0xa98
			 0x126d35b7, /// 0xa9c
			 0xd7a368b5, /// 0xaa0
			 0xdb50a1ff, /// 0xaa4
			 0x4b6a10c7, /// 0xaa8
			 0xbb062e3b, /// 0xaac
			 0xf7d80548, /// 0xab0
			 0xa40a0793, /// 0xab4
			 0x0744b08c, /// 0xab8
			 0x86362aa5, /// 0xabc
			 0x062e9b3f, /// 0xac0
			 0x695c26d9, /// 0xac4
			 0x3304ef54, /// 0xac8
			 0xe525e03e, /// 0xacc
			 0x25f7769c, /// 0xad0
			 0x2c00efa1, /// 0xad4
			 0xc517ca8d, /// 0xad8
			 0x90df8545, /// 0xadc
			 0x13895172, /// 0xae0
			 0x23614f7e, /// 0xae4
			 0x315c47eb, /// 0xae8
			 0xbd2572dd, /// 0xaec
			 0xa6522390, /// 0xaf0
			 0x1e753f92, /// 0xaf4
			 0x147328d0, /// 0xaf8
			 0x7220a4b0, /// 0xafc
			 0x5194185a, /// 0xb00
			 0xcf8966fb, /// 0xb04
			 0x73ba6765, /// 0xb08
			 0xaa9d4e14, /// 0xb0c
			 0xcff4d23e, /// 0xb10
			 0x3b17a7a3, /// 0xb14
			 0xff08794a, /// 0xb18
			 0x31d9680d, /// 0xb1c
			 0x50da465e, /// 0xb20
			 0xe775cdb1, /// 0xb24
			 0x692f7af8, /// 0xb28
			 0x0b50ec72, /// 0xb2c
			 0xfc0c4b6d, /// 0xb30
			 0xca231947, /// 0xb34
			 0xdb2e8641, /// 0xb38
			 0x68e1c29b, /// 0xb3c
			 0x9550b084, /// 0xb40
			 0xd5b4c1ae, /// 0xb44
			 0x4281a621, /// 0xb48
			 0x1c540ca8, /// 0xb4c
			 0xde068f60, /// 0xb50
			 0x0e7de008, /// 0xb54
			 0x954408ba, /// 0xb58
			 0x3f3dc80f, /// 0xb5c
			 0x04eed4d2, /// 0xb60
			 0x6649f55b, /// 0xb64
			 0x63fb53e3, /// 0xb68
			 0xfc992288, /// 0xb6c
			 0x1caa7896, /// 0xb70
			 0xba3e1542, /// 0xb74
			 0xca6489ac, /// 0xb78
			 0xc902ce78, /// 0xb7c
			 0xb7256f1b, /// 0xb80
			 0x39de19b3, /// 0xb84
			 0x35947030, /// 0xb88
			 0x94800e97, /// 0xb8c
			 0x52367218, /// 0xb90
			 0x5f9e47d8, /// 0xb94
			 0x9a43b5e1, /// 0xb98
			 0x86d59e2b, /// 0xb9c
			 0xdacde39e, /// 0xba0
			 0x2a8f1834, /// 0xba4
			 0xed35767d, /// 0xba8
			 0x9e9ab9b7, /// 0xbac
			 0xa8c529bc, /// 0xbb0
			 0x6912c62d, /// 0xbb4
			 0xf9a43fdb, /// 0xbb8
			 0xc2498651, /// 0xbbc
			 0x6a4ed123, /// 0xbc0
			 0xc0ca0c11, /// 0xbc4
			 0x9776d827, /// 0xbc8
			 0x8510b65f, /// 0xbcc
			 0x8a4938ce, /// 0xbd0
			 0xb0df8804, /// 0xbd4
			 0x53b596c6, /// 0xbd8
			 0xd67f9ef2, /// 0xbdc
			 0xe1d61452, /// 0xbe0
			 0x46355c6d, /// 0xbe4
			 0x415e3f35, /// 0xbe8
			 0x7f636cd1, /// 0xbec
			 0xdbb276af, /// 0xbf0
			 0xd268249d, /// 0xbf4
			 0x6007c879, /// 0xbf8
			 0x2dfa0f86, /// 0xbfc
			 0x36542f22, /// 0xc00
			 0xf5111514, /// 0xc04
			 0x8eb35e6a, /// 0xc08
			 0xec665141, /// 0xc0c
			 0xe4498885, /// 0xc10
			 0xe82a70d5, /// 0xc14
			 0x2a5e5f73, /// 0xc18
			 0x78093a71, /// 0xc1c
			 0xcfb3b285, /// 0xc20
			 0x8913a685, /// 0xc24
			 0x09df6c3f, /// 0xc28
			 0xbe7f6e4f, /// 0xc2c
			 0x6886e6fa, /// 0xc30
			 0xedcd4491, /// 0xc34
			 0xf5e889a0, /// 0xc38
			 0xa465d7be, /// 0xc3c
			 0xad29a94e, /// 0xc40
			 0x805afb08, /// 0xc44
			 0xb1c5b09d, /// 0xc48
			 0x0dd23bf8, /// 0xc4c
			 0x5da786cb, /// 0xc50
			 0x288c5517, /// 0xc54
			 0x35bf07b9, /// 0xc58
			 0x3f7e07ae, /// 0xc5c
			 0xf7a7b6b6, /// 0xc60
			 0xbcd98815, /// 0xc64
			 0x8d81fc13, /// 0xc68
			 0xbf1c0167, /// 0xc6c
			 0xa70f87ae, /// 0xc70
			 0x4b26d862, /// 0xc74
			 0x4af45359, /// 0xc78
			 0xf8e8b573, /// 0xc7c
			 0x68e53830, /// 0xc80
			 0xdb054b98, /// 0xc84
			 0xec3a4c48, /// 0xc88
			 0xbe832b88, /// 0xc8c
			 0x7b1e6049, /// 0xc90
			 0xdd882bc3, /// 0xc94
			 0x15bd623a, /// 0xc98
			 0xe046d8e1, /// 0xc9c
			 0x1f76676c, /// 0xca0
			 0xca2ea657, /// 0xca4
			 0x17c02ee2, /// 0xca8
			 0x5938787c, /// 0xcac
			 0x00e5f679, /// 0xcb0
			 0x41ea1c2d, /// 0xcb4
			 0x9098bfd0, /// 0xcb8
			 0x8ef3a6c3, /// 0xcbc
			 0x12d0e4d1, /// 0xcc0
			 0x60c179c0, /// 0xcc4
			 0x459296cc, /// 0xcc8
			 0x78102a4f, /// 0xccc
			 0xc4832710, /// 0xcd0
			 0xeff121dd, /// 0xcd4
			 0xf9e416fb, /// 0xcd8
			 0xc60f84b6, /// 0xcdc
			 0xcde15140, /// 0xce0
			 0xe0b107b5, /// 0xce4
			 0x19765f04, /// 0xce8
			 0xed09f15a, /// 0xcec
			 0xeb9673d6, /// 0xcf0
			 0x89ebe9c8, /// 0xcf4
			 0x87cabd97, /// 0xcf8
			 0xb3cdd6ac, /// 0xcfc
			 0x32752c39, /// 0xd00
			 0x65c8ca25, /// 0xd04
			 0x6f78c403, /// 0xd08
			 0x5487a63a, /// 0xd0c
			 0x0cd6536f, /// 0xd10
			 0xf32b2eec, /// 0xd14
			 0x9fc018c3, /// 0xd18
			 0x512d5175, /// 0xd1c
			 0x9d89b017, /// 0xd20
			 0xef0b65ec, /// 0xd24
			 0x514e3873, /// 0xd28
			 0x09317a56, /// 0xd2c
			 0x4879e626, /// 0xd30
			 0x6b885660, /// 0xd34
			 0x7776ad0c, /// 0xd38
			 0xef671264, /// 0xd3c
			 0xf93ec666, /// 0xd40
			 0x8c790a8e, /// 0xd44
			 0xae549699, /// 0xd48
			 0x4c374a38, /// 0xd4c
			 0x15e7d850, /// 0xd50
			 0x8a5d071a, /// 0xd54
			 0xf57d975d, /// 0xd58
			 0xeaab4dd5, /// 0xd5c
			 0xbdf20868, /// 0xd60
			 0xea7729b5, /// 0xd64
			 0x0e2c81e3, /// 0xd68
			 0xd1793d47, /// 0xd6c
			 0xcd8170ef, /// 0xd70
			 0x426ef426, /// 0xd74
			 0x6b72eee9, /// 0xd78
			 0x9cd9d963, /// 0xd7c
			 0x9425fa32, /// 0xd80
			 0x8eb95703, /// 0xd84
			 0xf24207e1, /// 0xd88
			 0xf66214c1, /// 0xd8c
			 0xe4b7f56a, /// 0xd90
			 0xe314cecb, /// 0xd94
			 0x5dfb79b4, /// 0xd98
			 0xeb22e47b, /// 0xd9c
			 0xc29f0cf9, /// 0xda0
			 0x0560d1bd, /// 0xda4
			 0x758ce623, /// 0xda8
			 0x6920aae5, /// 0xdac
			 0xbed64ba1, /// 0xdb0
			 0xeb30238b, /// 0xdb4
			 0x692a5ef6, /// 0xdb8
			 0xab72072f, /// 0xdbc
			 0xc55a2175, /// 0xdc0
			 0xe17ac808, /// 0xdc4
			 0xe7986d39, /// 0xdc8
			 0x9aa6a063, /// 0xdcc
			 0xe1d955d4, /// 0xdd0
			 0xe85081c8, /// 0xdd4
			 0xc65c0423, /// 0xdd8
			 0xa3300003, /// 0xddc
			 0x89a2477c, /// 0xde0
			 0x9cfaa304, /// 0xde4
			 0x536a02f0, /// 0xde8
			 0x851ee22f, /// 0xdec
			 0xe83166c0, /// 0xdf0
			 0x9fd2c22b, /// 0xdf4
			 0xd2b9afae, /// 0xdf8
			 0x299ba408, /// 0xdfc
			 0xe0df425f, /// 0xe00
			 0xdc23b2a4, /// 0xe04
			 0x8e91371b, /// 0xe08
			 0xa36b6631, /// 0xe0c
			 0x525fbec0, /// 0xe10
			 0xbaa45481, /// 0xe14
			 0x35e95dc5, /// 0xe18
			 0xd0665a6c, /// 0xe1c
			 0xc9ba7525, /// 0xe20
			 0x30679287, /// 0xe24
			 0xbd615945, /// 0xe28
			 0xff535484, /// 0xe2c
			 0x9220140a, /// 0xe30
			 0xb30b03d6, /// 0xe34
			 0x9c7ae9cc, /// 0xe38
			 0x809a6e12, /// 0xe3c
			 0xd85b97fb, /// 0xe40
			 0x562f00a8, /// 0xe44
			 0x92262194, /// 0xe48
			 0x775b121c, /// 0xe4c
			 0x08e2bd8c, /// 0xe50
			 0x47c6595e, /// 0xe54
			 0xc76b2cb4, /// 0xe58
			 0x1922f9b0, /// 0xe5c
			 0xfb7d0de8, /// 0xe60
			 0x02fe186c, /// 0xe64
			 0x096ddd50, /// 0xe68
			 0x868998d5, /// 0xe6c
			 0x1e92a463, /// 0xe70
			 0x247341a1, /// 0xe74
			 0x27b3efa8, /// 0xe78
			 0x0b44b5bb, /// 0xe7c
			 0x7b0dfa1e, /// 0xe80
			 0xfe1fd4e6, /// 0xe84
			 0xf6b2ee3a, /// 0xe88
			 0x40fb1d38, /// 0xe8c
			 0xe3768ef1, /// 0xe90
			 0x4020a469, /// 0xe94
			 0x08eeb651, /// 0xe98
			 0x47382249, /// 0xe9c
			 0x1576db0a, /// 0xea0
			 0x1c272066, /// 0xea4
			 0x97f11d3f, /// 0xea8
			 0x609b8345, /// 0xeac
			 0x9cb577fc, /// 0xeb0
			 0x697fac17, /// 0xeb4
			 0xcd80413c, /// 0xeb8
			 0x22183b40, /// 0xebc
			 0xedad1e45, /// 0xec0
			 0x3c0ff02c, /// 0xec4
			 0xbfa5ce22, /// 0xec8
			 0x4b8a3a60, /// 0xecc
			 0x75bbdc33, /// 0xed0
			 0xd6c3b8d5, /// 0xed4
			 0x9920f333, /// 0xed8
			 0x51060a49, /// 0xedc
			 0x37743ccf, /// 0xee0
			 0x86088453, /// 0xee4
			 0xaaa4b36f, /// 0xee8
			 0xc1ea0e37, /// 0xeec
			 0x17198d7d, /// 0xef0
			 0xf45995f6, /// 0xef4
			 0xa6982f06, /// 0xef8
			 0x665459d6, /// 0xefc
			 0x1c98ee29, /// 0xf00
			 0x3c838397, /// 0xf04
			 0x8cfb5d7c, /// 0xf08
			 0xf7f410d4, /// 0xf0c
			 0xed0130ba, /// 0xf10
			 0x2baa2052, /// 0xf14
			 0x1883247b, /// 0xf18
			 0xe009e315, /// 0xf1c
			 0xda8b2948, /// 0xf20
			 0xa4a11962, /// 0xf24
			 0x993d32d5, /// 0xf28
			 0x6fc71337, /// 0xf2c
			 0x66feccb5, /// 0xf30
			 0x7f2a6637, /// 0xf34
			 0xa9923762, /// 0xf38
			 0x9dd2fb05, /// 0xf3c
			 0x319724cb, /// 0xf40
			 0x6e6d1b39, /// 0xf44
			 0x8ce21240, /// 0xf48
			 0xc52c2923, /// 0xf4c
			 0x594373a7, /// 0xf50
			 0x3299f8a0, /// 0xf54
			 0xb0aeedd5, /// 0xf58
			 0x27635531, /// 0xf5c
			 0xa08726ad, /// 0xf60
			 0xdf14c8eb, /// 0xf64
			 0x5eaeb3d8, /// 0xf68
			 0x5d100deb, /// 0xf6c
			 0xa5514f61, /// 0xf70
			 0x8c7b8e57, /// 0xf74
			 0xf4898d90, /// 0xf78
			 0x73702258, /// 0xf7c
			 0xf6e58473, /// 0xf80
			 0xaaec206a, /// 0xf84
			 0x178ccea4, /// 0xf88
			 0x0bb42499, /// 0xf8c
			 0xb8905f32, /// 0xf90
			 0xbb6e5613, /// 0xf94
			 0xb5e50db0, /// 0xf98
			 0x6286e398, /// 0xf9c
			 0x1ddf1d36, /// 0xfa0
			 0x0527f5c7, /// 0xfa4
			 0x5548580b, /// 0xfa8
			 0x9130f31e, /// 0xfac
			 0xfc6340f6, /// 0xfb0
			 0x66f2b926, /// 0xfb4
			 0x051eb075, /// 0xfb8
			 0x71fd0254, /// 0xfbc
			 0xf7286a88, /// 0xfc0
			 0x403a6a46, /// 0xfc4
			 0xa1d3d5f3, /// 0xfc8
			 0xcf8eb084, /// 0xfcc
			 0x9a88cf7b, /// 0xfd0
			 0x4636090b, /// 0xfd4
			 0x7c0f7da6, /// 0xfd8
			 0x12680a69, /// 0xfdc
			 0x4fc4c765, /// 0xfe0
			 0xf8e7f8ee, /// 0xfe4
			 0xdef0bc1e, /// 0xfe8
			 0xcb45e8c5, /// 0xfec
			 0xec0b8188, /// 0xff0
			 0x9bc7cae4, /// 0xff4
			 0x73f055fa, /// 0xff8
			 0x428eddde /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00000
			 0x80011111,                                                  // -9.7958E-41                                 /// 00004
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x55555555,                                                  // 4 random values                             /// 0000c
			 0x33333333,                                                  // 4 random values                             /// 00010
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00014
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00018
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00028
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0002c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00030
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00034
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00038
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0003c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00040
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00044
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00048
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00050
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00054
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00058
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x55555555,                                                  // 4 random values                             /// 00068
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0006c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x00000000,                                                  // zero                                        /// 00074
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00078
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00084
			 0x80000000,                                                  // -zero                                       /// 00088
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0008c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00090
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00094
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00098
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0009c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000f4
			 0xbf028f5c,                                                  // -0.51                                       /// 000f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00104
			 0x80011111,                                                  // -9.7958E-41                                 /// 00108
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0010c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00114
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00118
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0011c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0c600000,                                                  // Leading 1s:                                 /// 00124
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00128
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0012c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00130
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00134
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0013c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00140
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00148
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0014c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00150
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00154
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00158
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00160
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00168
			 0xffc00001,                                                  // -signaling NaN                              /// 0016c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0017c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00180
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00188
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0018c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00190
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00194
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00198
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x55555555,                                                  // 4 random values                             /// 001f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00200
			 0x4b000000,                                                  // 8388608.0                                   /// 00204
			 0x4b000000,                                                  // 8388608.0                                   /// 00208
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00210
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00214
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00218
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00220
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00228
			 0x00011111,                                                  // 9.7958E-41                                  /// 0022c
			 0x7fc00001,                                                  // signaling NaN                               /// 00230
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00234
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0xbf028f5c,                                                  // -0.51                                       /// 0023c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00240
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00244
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00248
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0024c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00258
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0025c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00260
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00274
			 0xcb000000,                                                  // -8388608.0                                  /// 00278
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0027c
			 0x33333333,                                                  // 4 random values                             /// 00280
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00288
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00290
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00294
			 0x0e000007,                                                  // Trailing 1s:                                /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x33333333,                                                  // 4 random values                             /// 002b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00304
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00308
			 0x0e000007,                                                  // Trailing 1s:                                /// 0030c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00314
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00318
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0031c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x3f028f5c,                                                  // 0.51                                        /// 00324
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0032c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00330
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00334
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0033c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00340
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0034c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00350
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00354
			 0x7f800000,                                                  // inf                                         /// 00358
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0035c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00360
			 0xffc00001,                                                  // -signaling NaN                              /// 00364
			 0x4b000000,                                                  // 8388608.0                                   /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00370
			 0x0e000001,                                                  // Trailing 1s:                                /// 00374
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00378
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00380
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00384
			 0x7f800000,                                                  // inf                                         /// 00388
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x0c780000,                                                  // Leading 1s:                                 /// 00394
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xbf028f5c,                                                  // -0.51                                       /// 003d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00404
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00408
			 0x3f028f5c,                                                  // 0.51                                        /// 0040c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00414
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0041c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00428
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0042c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00430
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00434
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00438
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0043c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00444
			 0x80000000,                                                  // -zero                                       /// 00448
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0044c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00454
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00458
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xff800000,                                                  // -inf                                        /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00470
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00474
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00484
			 0xffc00001,                                                  // -signaling NaN                              /// 00488
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0048c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00490
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00494
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00498
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0049c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004cc
			 0x33333333,                                                  // 4 random values                             /// 004d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x7fc00001,                                                  // signaling NaN                               /// 004fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0xffc00001,                                                  // -signaling NaN                              /// 00508
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00510
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0051c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00520
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00528
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0052c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00534
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00540
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00544
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00550
			 0x7f800000,                                                  // inf                                         /// 00554
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0055c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00564
			 0x80011111,                                                  // -9.7958E-41                                 /// 00568
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0056c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00000000,                                                  // zero                                        /// 00574
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00584
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00588
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00590
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00598
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x3f028f5c,                                                  // 0.51                                        /// 005a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005ac
			 0xff800000,                                                  // -inf                                        /// 005b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x7fc00001,                                                  // signaling NaN                               /// 005c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x55555555,                                                  // 4 random values                             /// 005d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00600
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00604
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0060c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00610
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00614
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0061c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00620
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00624
			 0x33333333,                                                  // 4 random values                             /// 00628
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0062c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00630
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00634
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00638
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0063c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00640
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00644
			 0xff800000,                                                  // -inf                                        /// 00648
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0064c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00650
			 0x0e000007,                                                  // Trailing 1s:                                /// 00654
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00658
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0065c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00660
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00664
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00668
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0066c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00674
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00678
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0068c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006dc
			 0x7f800000,                                                  // inf                                         /// 006e0
			 0xffc00001,                                                  // -signaling NaN                              /// 006e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00700
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00704
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00708
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00710
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00714
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00718
			 0x7f800000,                                                  // inf                                         /// 0071c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0072c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00734
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00738
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0073c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00740
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00744
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00748
			 0x00000000,                                                  // zero                                        /// 0074c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00750
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00758
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00764
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0076c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00770
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00774
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00778
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00780
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80011111,                                                  // -9.7958E-41                                 /// 00788
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000000,                                                  // zero                                        /// 007b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c0
			 0xff800000,                                                  // -inf                                        /// 007c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007cc
			 0xffc00001,                                                  // -signaling NaN                              /// 007d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f0
			 0x7f800000,                                                  // inf                                         /// 007f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00804
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00808
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00810
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00814
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00818
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0081c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00824
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00828
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00011111,                                                  // 9.7958E-41                                  /// 00834
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00838
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00844
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00848
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00850
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00854
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00858
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0085c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00860
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00868
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0087c
			 0x7fc00001,                                                  // signaling NaN                               /// 00880
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x7f800000,                                                  // inf                                         /// 00888
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0088c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00890
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0089c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008ec
			 0x00000000,                                                  // zero                                        /// 008f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xffc00001,                                                  // -signaling NaN                              /// 008f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00900
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00904
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00908
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0090c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00914
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0091c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00928
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0092c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00934
			 0x00011111,                                                  // 9.7958E-41                                  /// 00938
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0093c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00940
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00948
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0094c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00950
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00954
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00958
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0095c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00960
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x0c600000,                                                  // Leading 1s:                                 /// 0096c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00974
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00978
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0097c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00980
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0xbf028f5c,                                                  // -0.51                                       /// 00994
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e000003,                                                  // Trailing 1s:                                /// 0099c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c4
			 0x55555555,                                                  // 4 random values                             /// 009c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a00
			 0x4b000000,                                                  // 8388608.0                                   /// 00a04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a18
			 0x7fc00001,                                                  // signaling NaN                               /// 00a1c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aa8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ab8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00abc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ac0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00acc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00af0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000000,                                                  // -zero                                       /// 00b54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b68
			 0xffc00001,                                                  // -signaling NaN                              /// 00b6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b74
			 0xffc00001,                                                  // -signaling NaN                              /// 00b78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ba0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x33333333,                                                  // 4 random values                             /// 00bc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bd4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bd8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bdc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bf0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0xbf028f5c,                                                  // -0.51                                       /// 00c38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c44
			 0xffc00001,                                                  // -signaling NaN                              /// 00c48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c58
			 0x00000000,                                                  // zero                                        /// 00c5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0xff800000,                                                  // -inf                                        /// 00c68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c7c
			 0xff800000,                                                  // -inf                                        /// 00c80
			 0x55555555,                                                  // 4 random values                             /// 00c84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ca0
			 0x80000000,                                                  // -zero                                       /// 00ca4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ce0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ce4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d04
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d14
			 0x7f800000,                                                  // inf                                         /// 00d18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xff800000,                                                  // -inf                                        /// 00d24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xff800000,                                                  // -inf                                        /// 00d64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00db8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00de4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00de8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00df8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0xff800000,                                                  // -inf                                        /// 00e40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x55555555,                                                  // 4 random values                             /// 00e9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00eb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ec4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ec8
			 0x55555555,                                                  // 4 random values                             /// 00ecc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ed4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ed8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00efc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fac
			 0x7fc00001,                                                  // signaling NaN                               /// 00fb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff8
			 0x0c7ff000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000000,                                                  // zero                                        /// 00000
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00004
			 0x3f028f5c,                                                  // 0.51                                        /// 00008
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0000c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00018
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0001c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00020
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00000000,                                                  // zero                                        /// 00028
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00030
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00034
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00038
			 0x55555555,                                                  // 4 random values                             /// 0003c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00044
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0004c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00050
			 0x3f028f5c,                                                  // 0.51                                        /// 00054
			 0x0c400000,                                                  // Leading 1s:                                 /// 00058
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0005c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00064
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00068
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00070
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00074
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00084
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00088
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0008c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00090
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b4
			 0x55555555,                                                  // 4 random values                             /// 000b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e4
			 0xcb000000,                                                  // -8388608.0                                  /// 000e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00104
			 0x7fc00001,                                                  // signaling NaN                               /// 00108
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00114
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00118
			 0x7f800000,                                                  // inf                                         /// 0011c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00128
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0012c
			 0xcb000000,                                                  // -8388608.0                                  /// 00130
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00140
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00144
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00148
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0014c
			 0x3f028f5c,                                                  // 0.51                                        /// 00150
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00154
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00158
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00168
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0016c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00170
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00174
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00178
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0017c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00180
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00184
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00188
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0018c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00190
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0019c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x3f028f5c,                                                  // 0.51                                        /// 001cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00208
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00214
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00218
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0021c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00224
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0022c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00230
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00234
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00238
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00240
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00244
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00248
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00254
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00258
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0025c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00270
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00274
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00278
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0027c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00284
			 0x55555555,                                                  // 4 random values                             /// 00288
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0028c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00290
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00298
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0xff800000,                                                  // -inf                                        /// 002a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002b0
			 0x4b000000,                                                  // 8388608.0                                   /// 002b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xcb000000,                                                  // -8388608.0                                  /// 002c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002ec
			 0x00000000,                                                  // zero                                        /// 002f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00300
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00304
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00308
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0030c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x4b000000,                                                  // 8388608.0                                   /// 00320
			 0xcb000000,                                                  // -8388608.0                                  /// 00324
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00328
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0032c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00330
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00338
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0033c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00344
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00348
			 0x00011111,                                                  // 9.7958E-41                                  /// 0034c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00354
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00358
			 0x4b000000,                                                  // 8388608.0                                   /// 0035c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00364
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0036c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00370
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00378
			 0xbf028f5c,                                                  // -0.51                                       /// 0037c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00384
			 0xff800000,                                                  // -inf                                        /// 00388
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0038c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00394
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00398
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b4
			 0x4b000000,                                                  // 8388608.0                                   /// 003b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00400
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00408
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0040c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00410
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00414
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0041c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00424
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00430
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00434
			 0x33333333,                                                  // 4 random values                             /// 00438
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0043c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00444
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00448
			 0x00000000,                                                  // zero                                        /// 0044c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00450
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80000000,                                                  // -zero                                       /// 00458
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0046c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00470
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00474
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00478
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00480
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00484
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00488
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0048c
			 0x00000000,                                                  // zero                                        /// 00490
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00494
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00498
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xff800000,                                                  // -inf                                        /// 004cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00500
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00504
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0051c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00520
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00524
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00528
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0052c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00530
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00534
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00538
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0053c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00011111,                                                  // 9.7958E-41                                  /// 00544
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00548
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00558
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00560
			 0xffc00001,                                                  // -signaling NaN                              /// 00564
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00574
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xffc00001,                                                  // -signaling NaN                              /// 0057c
			 0xcb000000,                                                  // -8388608.0                                  /// 00580
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00584
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0058c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00590
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f8
			 0x80000000,                                                  // -zero                                       /// 005fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00600
			 0x0c780000,                                                  // Leading 1s:                                 /// 00604
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00608
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0060c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00620
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00634
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00638
			 0x0e000001,                                                  // Trailing 1s:                                /// 0063c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00640
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00644
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00648
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0064c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00650
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00654
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00660
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00664
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00668
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00674
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00678
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0067c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00680
			 0x7fc00001,                                                  // signaling NaN                               /// 00684
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00688
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0069c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006dc
			 0xcb000000,                                                  // -8388608.0                                  /// 006e0
			 0x7f800000,                                                  // inf                                         /// 006e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006e8
			 0xffc00001,                                                  // -signaling NaN                              /// 006ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xbf028f5c,                                                  // -0.51                                       /// 006f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00700
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00704
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00708
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0070c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00710
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00714
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00718
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0071c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00720
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00724
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00728
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00734
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00740
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00750
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00758
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00760
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00764
			 0x33333333,                                                  // 4 random values                             /// 00768
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00774
			 0x0c400000,                                                  // Leading 1s:                                 /// 00778
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00780
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00788
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0078c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00790
			 0x4b000000,                                                  // 8388608.0                                   /// 00794
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00798
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0079c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a4
			 0x7fc00001,                                                  // signaling NaN                               /// 007a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x55555555,                                                  // 4 random values                             /// 007f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00800
			 0xffc00001,                                                  // -signaling NaN                              /// 00804
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0080c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0c400000,                                                  // Leading 1s:                                 /// 0081c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00820
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00824
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00828
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0082c
			 0x4b000000,                                                  // 8388608.0                                   /// 00830
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00834
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00838
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00848
			 0x80000000,                                                  // -zero                                       /// 0084c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00850
			 0xbf028f5c,                                                  // -0.51                                       /// 00854
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00858
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0085c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00860
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00868
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00870
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00878
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0087c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00884
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0088c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00890
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00894
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00898
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0089c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ec
			 0xcb000000,                                                  // -8388608.0                                  /// 008f0
			 0x7fc00001,                                                  // signaling NaN                               /// 008f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x33333333,                                                  // 4 random values                             /// 00904
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00910
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0091c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00928
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0092c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00930
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0xcb000000,                                                  // -8388608.0                                  /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00940
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0094c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00954
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00958
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00968
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00974
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0097c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00984
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00988
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00990
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00998
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0099c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009a4
			 0x00000000,                                                  // zero                                        /// 009a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a08
			 0x33333333,                                                  // 4 random values                             /// 00a0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a24
			 0xffc00001,                                                  // -signaling NaN                              /// 00a28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x33333333,                                                  // 4 random values                             /// 00a38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0xff800000,                                                  // -inf                                        /// 00a44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a98
			 0x00000000,                                                  // zero                                        /// 00a9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ab0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00abc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ac4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00acc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00adc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ae0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x55555555,                                                  // 4 random values                             /// 00aec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b88
			 0xbf028f5c,                                                  // -0.51                                       /// 00b8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ba4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x33333333,                                                  // 4 random values                             /// 00bbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bc8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00be4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bfc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c58
			 0xcb000000,                                                  // -8388608.0                                  /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000000,                                                  // -zero                                       /// 00c74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ce8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cf4
			 0x00000000,                                                  // zero                                        /// 00cf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d20
			 0x00000000,                                                  // zero                                        /// 00d24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00000000,                                                  // zero                                        /// 00da8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dbc
			 0x00000000,                                                  // zero                                        /// 00dc0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x7f800000,                                                  // inf                                         /// 00dcc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x4b000000,                                                  // 8388608.0                                   /// 00df0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00df4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dfc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0xffc00001,                                                  // -signaling NaN                              /// 00e9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ecc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ed0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ed8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x55555555,                                                  // 4 random values                             /// 00f44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x3f028f5c,                                                  // 0.51                                        /// 00f74
			 0x55555555,                                                  // 4 random values                             /// 00f78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x4b000000,                                                  // 8388608.0                                   /// 00f94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x7f800000,                                                  // inf                                         /// 00fac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fe8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x0d00fff0                                                  // Set of 1s                                   /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x2fafd559, /// 0x0
			 0xb42d2139, /// 0x4
			 0x423e7b41, /// 0x8
			 0x561e39f1, /// 0xc
			 0xc8a0225c, /// 0x10
			 0x67ab55e9, /// 0x14
			 0x3dc58bc2, /// 0x18
			 0xa583c74a, /// 0x1c
			 0xe691e380, /// 0x20
			 0xab0f11f9, /// 0x24
			 0x80e1f8d8, /// 0x28
			 0xf155061a, /// 0x2c
			 0xfebfb791, /// 0x30
			 0xb0752a58, /// 0x34
			 0xdb037aed, /// 0x38
			 0x6d4cb3aa, /// 0x3c
			 0x9f12dc31, /// 0x40
			 0x0f6a1e50, /// 0x44
			 0x2af18ea8, /// 0x48
			 0x824add73, /// 0x4c
			 0xe458b373, /// 0x50
			 0x5079b133, /// 0x54
			 0x476d5a4c, /// 0x58
			 0xe1f33fa9, /// 0x5c
			 0x9f2336c7, /// 0x60
			 0x3ae93d99, /// 0x64
			 0xd0fb8a97, /// 0x68
			 0xe7a3380f, /// 0x6c
			 0xddb48d32, /// 0x70
			 0x5d32c368, /// 0x74
			 0xce75abe2, /// 0x78
			 0x7865388c, /// 0x7c
			 0xea05e684, /// 0x80
			 0xdca6a2ed, /// 0x84
			 0x626c89d3, /// 0x88
			 0xc5eb43fc, /// 0x8c
			 0x28718237, /// 0x90
			 0x76c3a9f4, /// 0x94
			 0x15baabfc, /// 0x98
			 0xc46d140d, /// 0x9c
			 0x733f77a5, /// 0xa0
			 0x9f91b961, /// 0xa4
			 0xe0d38608, /// 0xa8
			 0x32967fb7, /// 0xac
			 0x683bbaa7, /// 0xb0
			 0xfb801a1d, /// 0xb4
			 0x9bdaa49b, /// 0xb8
			 0x66a96293, /// 0xbc
			 0x3ace8dde, /// 0xc0
			 0x49979582, /// 0xc4
			 0x107c9f43, /// 0xc8
			 0x82f44a7b, /// 0xcc
			 0x46ca0aaf, /// 0xd0
			 0x872827f7, /// 0xd4
			 0xa747dd6b, /// 0xd8
			 0x7bd929e6, /// 0xdc
			 0xa794a79d, /// 0xe0
			 0x6f1b04ef, /// 0xe4
			 0x0a76363b, /// 0xe8
			 0xe305ca14, /// 0xec
			 0x1730f253, /// 0xf0
			 0xdf55efe6, /// 0xf4
			 0xa8537967, /// 0xf8
			 0x651f0322, /// 0xfc
			 0xf2d93153, /// 0x100
			 0xb0cd5131, /// 0x104
			 0x2a79e7af, /// 0x108
			 0x13b3b18a, /// 0x10c
			 0x6a46e526, /// 0x110
			 0xde70853a, /// 0x114
			 0x4d3bee7a, /// 0x118
			 0x60041272, /// 0x11c
			 0xcb498d15, /// 0x120
			 0x94aeff8b, /// 0x124
			 0x4823db54, /// 0x128
			 0xff990018, /// 0x12c
			 0x18f6f5db, /// 0x130
			 0x026fe317, /// 0x134
			 0x448e6aef, /// 0x138
			 0x52e92364, /// 0x13c
			 0x14d3ebe2, /// 0x140
			 0x6d271e88, /// 0x144
			 0x31f5ce71, /// 0x148
			 0xe1f00319, /// 0x14c
			 0x640452c2, /// 0x150
			 0x2ca80a7b, /// 0x154
			 0x5b68844d, /// 0x158
			 0x175898de, /// 0x15c
			 0xf2dfd02f, /// 0x160
			 0x2f3ee2ca, /// 0x164
			 0x115f6d3f, /// 0x168
			 0xe5489f42, /// 0x16c
			 0x59828a3e, /// 0x170
			 0x4b6de3af, /// 0x174
			 0x9cd2d857, /// 0x178
			 0x8005960d, /// 0x17c
			 0x9bde0c76, /// 0x180
			 0x1c271ca4, /// 0x184
			 0xc7fff027, /// 0x188
			 0xfecacde0, /// 0x18c
			 0xa6590486, /// 0x190
			 0x26b847ca, /// 0x194
			 0x75ba68f7, /// 0x198
			 0x127f080d, /// 0x19c
			 0x61e57629, /// 0x1a0
			 0x855a4614, /// 0x1a4
			 0x3bce49f5, /// 0x1a8
			 0x5dffaa95, /// 0x1ac
			 0x0091f66d, /// 0x1b0
			 0xc76b164a, /// 0x1b4
			 0x8740e50a, /// 0x1b8
			 0x74ec3af9, /// 0x1bc
			 0x9b0095fd, /// 0x1c0
			 0x2f1c3bed, /// 0x1c4
			 0x75cefdb6, /// 0x1c8
			 0x7a1b515a, /// 0x1cc
			 0x16c08a28, /// 0x1d0
			 0x5f9ab431, /// 0x1d4
			 0xc5f3b931, /// 0x1d8
			 0x7e513c82, /// 0x1dc
			 0xb373d0a3, /// 0x1e0
			 0xacdedbd2, /// 0x1e4
			 0xe2783909, /// 0x1e8
			 0x992dad70, /// 0x1ec
			 0x1b37c300, /// 0x1f0
			 0x3415c0e8, /// 0x1f4
			 0x5ada7ced, /// 0x1f8
			 0xf2128d98, /// 0x1fc
			 0xc492fbf9, /// 0x200
			 0x8d0f61a0, /// 0x204
			 0x7cddc939, /// 0x208
			 0x579c243b, /// 0x20c
			 0xec79b6bb, /// 0x210
			 0x6f4ac3af, /// 0x214
			 0x09db1818, /// 0x218
			 0x04ec67c3, /// 0x21c
			 0x734228e8, /// 0x220
			 0xd55e82ea, /// 0x224
			 0x7ce5a593, /// 0x228
			 0x72e52a06, /// 0x22c
			 0x700f1e8f, /// 0x230
			 0xaaf0e3d5, /// 0x234
			 0x2435afe1, /// 0x238
			 0xcb17ba28, /// 0x23c
			 0xbbb79784, /// 0x240
			 0xfff3157c, /// 0x244
			 0x5aa22a73, /// 0x248
			 0x084c00a0, /// 0x24c
			 0x33fbeb45, /// 0x250
			 0xe5277396, /// 0x254
			 0x6b78c22a, /// 0x258
			 0x6ffb389e, /// 0x25c
			 0x67f29e14, /// 0x260
			 0x4b1f0b50, /// 0x264
			 0xbd11d44d, /// 0x268
			 0x27ba6a7e, /// 0x26c
			 0xc4ce9de0, /// 0x270
			 0x36e1eb01, /// 0x274
			 0x15502bbd, /// 0x278
			 0x8a7c84cb, /// 0x27c
			 0xe0a94469, /// 0x280
			 0xe9705258, /// 0x284
			 0x9fbd24f9, /// 0x288
			 0x018e7b0c, /// 0x28c
			 0x00f3d4ff, /// 0x290
			 0xa0ac59af, /// 0x294
			 0xe304a15c, /// 0x298
			 0x30adefcf, /// 0x29c
			 0xa129f1ad, /// 0x2a0
			 0x6a5814c3, /// 0x2a4
			 0x8d76cb2b, /// 0x2a8
			 0x40084c56, /// 0x2ac
			 0x09d59d64, /// 0x2b0
			 0x700d4b01, /// 0x2b4
			 0x7708263f, /// 0x2b8
			 0x310a9856, /// 0x2bc
			 0x920160a6, /// 0x2c0
			 0x391fa8de, /// 0x2c4
			 0x0c3090b7, /// 0x2c8
			 0xa9ae2e86, /// 0x2cc
			 0x9d3a83b3, /// 0x2d0
			 0x2a29ed37, /// 0x2d4
			 0x5fe35116, /// 0x2d8
			 0x52ea4c13, /// 0x2dc
			 0x717eb94f, /// 0x2e0
			 0x86801083, /// 0x2e4
			 0x127210f1, /// 0x2e8
			 0xdf553c3a, /// 0x2ec
			 0xcc3eae50, /// 0x2f0
			 0x254e54b9, /// 0x2f4
			 0x30ffedca, /// 0x2f8
			 0x67a70db6, /// 0x2fc
			 0xd720eef3, /// 0x300
			 0x002e0367, /// 0x304
			 0x2203fa7d, /// 0x308
			 0x8bd28da4, /// 0x30c
			 0xbfad5007, /// 0x310
			 0x22c291b8, /// 0x314
			 0x06b712e3, /// 0x318
			 0xea63755d, /// 0x31c
			 0xaa8fd511, /// 0x320
			 0x5e4d7e66, /// 0x324
			 0xd6bd2b92, /// 0x328
			 0x6141b36b, /// 0x32c
			 0xb71a767b, /// 0x330
			 0x72c61004, /// 0x334
			 0x63d6f920, /// 0x338
			 0xdf74e491, /// 0x33c
			 0x40db7c42, /// 0x340
			 0x96e75a2c, /// 0x344
			 0xb8133f5e, /// 0x348
			 0xebfea144, /// 0x34c
			 0x99e91e99, /// 0x350
			 0x4757b1bf, /// 0x354
			 0x90cc1088, /// 0x358
			 0x9948da5c, /// 0x35c
			 0xcaa4ff4e, /// 0x360
			 0x70e23d76, /// 0x364
			 0x08270ab5, /// 0x368
			 0x16d628cb, /// 0x36c
			 0xe412532d, /// 0x370
			 0x6a960b4d, /// 0x374
			 0xc3ad2bde, /// 0x378
			 0xec2ef433, /// 0x37c
			 0x7dba2b56, /// 0x380
			 0x4cd65d04, /// 0x384
			 0xa2fcabdc, /// 0x388
			 0x451430db, /// 0x38c
			 0xb5eb13c0, /// 0x390
			 0xb41db287, /// 0x394
			 0x76602d28, /// 0x398
			 0xcfb8784a, /// 0x39c
			 0xcd09cd16, /// 0x3a0
			 0xc92e0873, /// 0x3a4
			 0xbbe919fe, /// 0x3a8
			 0x05d82848, /// 0x3ac
			 0x25f64d4f, /// 0x3b0
			 0xd1dd6bcf, /// 0x3b4
			 0x8c61a959, /// 0x3b8
			 0xad387818, /// 0x3bc
			 0x643bd6ba, /// 0x3c0
			 0x8b105c1f, /// 0x3c4
			 0x7ead1d41, /// 0x3c8
			 0xb0685aef, /// 0x3cc
			 0xbe97490f, /// 0x3d0
			 0xc2d3c9df, /// 0x3d4
			 0x8d72e077, /// 0x3d8
			 0x5c54562a, /// 0x3dc
			 0x7f0ede61, /// 0x3e0
			 0xd1fec75b, /// 0x3e4
			 0x0dea86bf, /// 0x3e8
			 0x23510e96, /// 0x3ec
			 0x007d856e, /// 0x3f0
			 0x4f596ed4, /// 0x3f4
			 0x842d127c, /// 0x3f8
			 0xff7a6919, /// 0x3fc
			 0x8247c42c, /// 0x400
			 0x0ab1ca9b, /// 0x404
			 0xf5cae48e, /// 0x408
			 0x070b6a4b, /// 0x40c
			 0x20d26513, /// 0x410
			 0xe6f79ac1, /// 0x414
			 0x72556bd6, /// 0x418
			 0x13252cd6, /// 0x41c
			 0xf91871b4, /// 0x420
			 0xe2aadb44, /// 0x424
			 0x816b6c9b, /// 0x428
			 0xeaeba0d8, /// 0x42c
			 0xc45f498a, /// 0x430
			 0x22860134, /// 0x434
			 0x50cc5d4e, /// 0x438
			 0xbc118a01, /// 0x43c
			 0x51bb0045, /// 0x440
			 0x14b70c66, /// 0x444
			 0xec2bfa33, /// 0x448
			 0x919bc5de, /// 0x44c
			 0x02d81314, /// 0x450
			 0xc6ffc4fc, /// 0x454
			 0xc9c12451, /// 0x458
			 0x63765067, /// 0x45c
			 0x44602281, /// 0x460
			 0xb26ef0dc, /// 0x464
			 0x27d09b4a, /// 0x468
			 0xf0d3814f, /// 0x46c
			 0x7c162a57, /// 0x470
			 0x00d11a3e, /// 0x474
			 0x99e35e42, /// 0x478
			 0xffa5697c, /// 0x47c
			 0x08d657ed, /// 0x480
			 0xa899c3ce, /// 0x484
			 0xb05ffc38, /// 0x488
			 0xe72b6f2a, /// 0x48c
			 0xee625945, /// 0x490
			 0x52d796f9, /// 0x494
			 0x1db88024, /// 0x498
			 0xf5fc6f52, /// 0x49c
			 0x54c75e4d, /// 0x4a0
			 0x6676c071, /// 0x4a4
			 0x71d85f54, /// 0x4a8
			 0x607ecb61, /// 0x4ac
			 0x99f98cff, /// 0x4b0
			 0xb628be72, /// 0x4b4
			 0x4cef9833, /// 0x4b8
			 0x48f05387, /// 0x4bc
			 0x65b6bffc, /// 0x4c0
			 0x840f3210, /// 0x4c4
			 0xaf185fbb, /// 0x4c8
			 0x42c47907, /// 0x4cc
			 0xc270470f, /// 0x4d0
			 0x07d85a1b, /// 0x4d4
			 0xd7c6d55e, /// 0x4d8
			 0xbe940334, /// 0x4dc
			 0xc24c0623, /// 0x4e0
			 0x7b733e34, /// 0x4e4
			 0xb8ef8ee9, /// 0x4e8
			 0x68072e62, /// 0x4ec
			 0xcd51b77f, /// 0x4f0
			 0xe783912e, /// 0x4f4
			 0x844897a4, /// 0x4f8
			 0x62116512, /// 0x4fc
			 0x265f94f6, /// 0x500
			 0xb41f17bd, /// 0x504
			 0x195bb37e, /// 0x508
			 0xea6a5960, /// 0x50c
			 0xf71bcb54, /// 0x510
			 0x0f1c9e0b, /// 0x514
			 0xab76ab19, /// 0x518
			 0x1693bd91, /// 0x51c
			 0x88326746, /// 0x520
			 0xfc4dbc73, /// 0x524
			 0x45208ad5, /// 0x528
			 0xc61d9b81, /// 0x52c
			 0x55ab546e, /// 0x530
			 0xad8e2c55, /// 0x534
			 0xc6e2d38d, /// 0x538
			 0x82ca480f, /// 0x53c
			 0x106d9dc5, /// 0x540
			 0x1d10ca65, /// 0x544
			 0xc1c7f34c, /// 0x548
			 0x85be10af, /// 0x54c
			 0x3ccb67cb, /// 0x550
			 0xd9031910, /// 0x554
			 0x5355be49, /// 0x558
			 0x21f4f433, /// 0x55c
			 0xa50354c1, /// 0x560
			 0x305a9fe2, /// 0x564
			 0xf91211f7, /// 0x568
			 0x5d53addd, /// 0x56c
			 0x986f6ccf, /// 0x570
			 0x9154c6fd, /// 0x574
			 0xf082b1be, /// 0x578
			 0x1ae3c108, /// 0x57c
			 0xaa8cf91a, /// 0x580
			 0xcd84fc48, /// 0x584
			 0x07768b21, /// 0x588
			 0x61608fad, /// 0x58c
			 0x0c1c53c4, /// 0x590
			 0xe78b4c27, /// 0x594
			 0xc0db9618, /// 0x598
			 0xb80eaad7, /// 0x59c
			 0xc20bfa51, /// 0x5a0
			 0xf63b28e5, /// 0x5a4
			 0xcbe97aa1, /// 0x5a8
			 0x3b3a57aa, /// 0x5ac
			 0x463a208d, /// 0x5b0
			 0x581cbd46, /// 0x5b4
			 0x54eef118, /// 0x5b8
			 0x853a6072, /// 0x5bc
			 0x442c1948, /// 0x5c0
			 0x91ec053a, /// 0x5c4
			 0xd1ad02f1, /// 0x5c8
			 0x5b07e7a1, /// 0x5cc
			 0x210e93a8, /// 0x5d0
			 0x1ed84569, /// 0x5d4
			 0x725b1dde, /// 0x5d8
			 0x84909cc5, /// 0x5dc
			 0x27372f45, /// 0x5e0
			 0xf9ffcf0c, /// 0x5e4
			 0x1126f233, /// 0x5e8
			 0x468b12e3, /// 0x5ec
			 0xd888ee6f, /// 0x5f0
			 0xb42513d6, /// 0x5f4
			 0x919c7888, /// 0x5f8
			 0x4537bbc0, /// 0x5fc
			 0x6ef31df3, /// 0x600
			 0xb8eed14b, /// 0x604
			 0xdcf6d64d, /// 0x608
			 0x4f552616, /// 0x60c
			 0x3b48d85a, /// 0x610
			 0x44c08430, /// 0x614
			 0x858e3b1d, /// 0x618
			 0x228ba2c7, /// 0x61c
			 0x72d27fe1, /// 0x620
			 0x657714c4, /// 0x624
			 0x10aef8d2, /// 0x628
			 0x07c38ab4, /// 0x62c
			 0x224448dc, /// 0x630
			 0x97d56fa1, /// 0x634
			 0x81caa11f, /// 0x638
			 0xe30d86a8, /// 0x63c
			 0x63d543fb, /// 0x640
			 0x733c2bce, /// 0x644
			 0xee361bd7, /// 0x648
			 0x5f24d615, /// 0x64c
			 0x684eb425, /// 0x650
			 0x96999e67, /// 0x654
			 0x806a0ad0, /// 0x658
			 0x4c5caf8b, /// 0x65c
			 0x6bb37376, /// 0x660
			 0x97a9d96c, /// 0x664
			 0x38a5e1fb, /// 0x668
			 0x26ce5dbf, /// 0x66c
			 0x677129d3, /// 0x670
			 0x5c154ec3, /// 0x674
			 0x252fb98c, /// 0x678
			 0xefad2f06, /// 0x67c
			 0x4c3c503b, /// 0x680
			 0xffc17a48, /// 0x684
			 0xb4ab7113, /// 0x688
			 0x92604d4e, /// 0x68c
			 0xc360eb8a, /// 0x690
			 0xc3ab7597, /// 0x694
			 0xe6e995c1, /// 0x698
			 0x093d2a77, /// 0x69c
			 0x24ca243f, /// 0x6a0
			 0xd5852a78, /// 0x6a4
			 0xd87f4fc2, /// 0x6a8
			 0xe6c3fed6, /// 0x6ac
			 0x80e7606c, /// 0x6b0
			 0x36e6b845, /// 0x6b4
			 0xa2f20089, /// 0x6b8
			 0xd1b41917, /// 0x6bc
			 0x6bb9d126, /// 0x6c0
			 0xb64c9590, /// 0x6c4
			 0xdd5ec0bc, /// 0x6c8
			 0x1159d0c1, /// 0x6cc
			 0xcd15f847, /// 0x6d0
			 0xe8c11090, /// 0x6d4
			 0x0ea85dfc, /// 0x6d8
			 0xd7e52be0, /// 0x6dc
			 0xd09ce818, /// 0x6e0
			 0x786faae3, /// 0x6e4
			 0x660b2561, /// 0x6e8
			 0xe5c24b32, /// 0x6ec
			 0x24cad79e, /// 0x6f0
			 0xc30f08cb, /// 0x6f4
			 0x00bf04a0, /// 0x6f8
			 0x64f85e3a, /// 0x6fc
			 0xd550c693, /// 0x700
			 0xf76668e3, /// 0x704
			 0xc4d70903, /// 0x708
			 0x4fc97f58, /// 0x70c
			 0xcef8ccc7, /// 0x710
			 0x34cb024c, /// 0x714
			 0x19169cef, /// 0x718
			 0xfff7a63e, /// 0x71c
			 0x5bc6bbd4, /// 0x720
			 0xbabc9b4c, /// 0x724
			 0xae64e1c4, /// 0x728
			 0xf51e89fd, /// 0x72c
			 0x8be92ee1, /// 0x730
			 0x9ec087b8, /// 0x734
			 0xe9b13ac9, /// 0x738
			 0xa9649ddf, /// 0x73c
			 0x6cadde34, /// 0x740
			 0xf987bbe2, /// 0x744
			 0x0cb023a4, /// 0x748
			 0x41b98d8f, /// 0x74c
			 0xc3ee5366, /// 0x750
			 0x6a59af7a, /// 0x754
			 0xabea625a, /// 0x758
			 0x87b9f8c1, /// 0x75c
			 0xc320bb18, /// 0x760
			 0xe3e4409f, /// 0x764
			 0xe074e183, /// 0x768
			 0xe74171e6, /// 0x76c
			 0xf7abe55f, /// 0x770
			 0x4269e2b4, /// 0x774
			 0x74405c7a, /// 0x778
			 0x1d820e40, /// 0x77c
			 0xf0bf8ddf, /// 0x780
			 0xa1204223, /// 0x784
			 0xc1ac08ed, /// 0x788
			 0xb65fa7d5, /// 0x78c
			 0x8cd53e6f, /// 0x790
			 0x33a63d40, /// 0x794
			 0xb90e8c77, /// 0x798
			 0x8f9cc378, /// 0x79c
			 0x60d34b1b, /// 0x7a0
			 0x8323b09c, /// 0x7a4
			 0x1bf68332, /// 0x7a8
			 0xf1f2a3f9, /// 0x7ac
			 0x486a19f0, /// 0x7b0
			 0x42625f3c, /// 0x7b4
			 0x53c4cb8e, /// 0x7b8
			 0x27d58d80, /// 0x7bc
			 0xa102f78b, /// 0x7c0
			 0x2d61b465, /// 0x7c4
			 0x1a97856e, /// 0x7c8
			 0xd0bacdde, /// 0x7cc
			 0xcafbfd1a, /// 0x7d0
			 0xa45402c1, /// 0x7d4
			 0xda14c512, /// 0x7d8
			 0x8a27da5e, /// 0x7dc
			 0xcaa8dd6a, /// 0x7e0
			 0x3a3d89c6, /// 0x7e4
			 0xf9d479b5, /// 0x7e8
			 0x5ef0c8d5, /// 0x7ec
			 0x08d2b9c2, /// 0x7f0
			 0x69088d99, /// 0x7f4
			 0xb419c668, /// 0x7f8
			 0x9035f7ab, /// 0x7fc
			 0x3cba6a23, /// 0x800
			 0x1f7fa640, /// 0x804
			 0xec434122, /// 0x808
			 0xfd70f5ad, /// 0x80c
			 0xfe69e01c, /// 0x810
			 0xd44817d3, /// 0x814
			 0x138b27e0, /// 0x818
			 0x61e79f96, /// 0x81c
			 0x73a5622c, /// 0x820
			 0xaaa15157, /// 0x824
			 0x6a847e6b, /// 0x828
			 0x38d345f0, /// 0x82c
			 0x3d8a5893, /// 0x830
			 0xea8eabed, /// 0x834
			 0x40518b72, /// 0x838
			 0xd916203d, /// 0x83c
			 0x4467ccf1, /// 0x840
			 0x46f1353f, /// 0x844
			 0x89b376ea, /// 0x848
			 0x4877163f, /// 0x84c
			 0x2b2dfc94, /// 0x850
			 0x2275c9af, /// 0x854
			 0x37469baa, /// 0x858
			 0xba5e9b0b, /// 0x85c
			 0xbfd526ba, /// 0x860
			 0xb6d8af16, /// 0x864
			 0x1e950db5, /// 0x868
			 0x1642df7c, /// 0x86c
			 0x65c01fd2, /// 0x870
			 0x0657c38b, /// 0x874
			 0x7e8cc8b2, /// 0x878
			 0x20182753, /// 0x87c
			 0xfb157b73, /// 0x880
			 0x3bff7490, /// 0x884
			 0xf036b25b, /// 0x888
			 0x7ee1d355, /// 0x88c
			 0xc454f779, /// 0x890
			 0xfd822698, /// 0x894
			 0xd0e528e9, /// 0x898
			 0xb1b3862f, /// 0x89c
			 0x77281d17, /// 0x8a0
			 0xf5d10e17, /// 0x8a4
			 0xc6407af0, /// 0x8a8
			 0x82a19c14, /// 0x8ac
			 0xfc8389f4, /// 0x8b0
			 0xfa1691a0, /// 0x8b4
			 0x4ef9160d, /// 0x8b8
			 0x7aa3a540, /// 0x8bc
			 0x7703ea18, /// 0x8c0
			 0x3bd4452d, /// 0x8c4
			 0x505ba689, /// 0x8c8
			 0x72f185eb, /// 0x8cc
			 0xfe7f7097, /// 0x8d0
			 0xab9ee9af, /// 0x8d4
			 0x034bbf0b, /// 0x8d8
			 0x1f785bfc, /// 0x8dc
			 0x4c368f08, /// 0x8e0
			 0x9ca8d2cc, /// 0x8e4
			 0x3fa8e483, /// 0x8e8
			 0x428b0763, /// 0x8ec
			 0xfeb84cb0, /// 0x8f0
			 0xde03f8c9, /// 0x8f4
			 0x9fbc2bc2, /// 0x8f8
			 0x82bf897f, /// 0x8fc
			 0x8508a1ce, /// 0x900
			 0x5ccd9052, /// 0x904
			 0x771f0aaf, /// 0x908
			 0xca9a48f2, /// 0x90c
			 0x576f9e89, /// 0x910
			 0x76ec610d, /// 0x914
			 0x9bb64d4f, /// 0x918
			 0x631b1fc4, /// 0x91c
			 0x66621f0d, /// 0x920
			 0xb8fa1aa1, /// 0x924
			 0xd7c3a278, /// 0x928
			 0x532da414, /// 0x92c
			 0xb6c96c35, /// 0x930
			 0xcff80a39, /// 0x934
			 0x1c5a5387, /// 0x938
			 0x705f19c2, /// 0x93c
			 0xe328f249, /// 0x940
			 0x45a5dc51, /// 0x944
			 0x34dff6d1, /// 0x948
			 0xb5b7b46d, /// 0x94c
			 0x9455d5de, /// 0x950
			 0xaa2fa785, /// 0x954
			 0x93a80003, /// 0x958
			 0x293b7868, /// 0x95c
			 0x33ffca18, /// 0x960
			 0x78ec525b, /// 0x964
			 0x2fc02fb4, /// 0x968
			 0xa29deb5c, /// 0x96c
			 0x21d0036e, /// 0x970
			 0x6fa8f7e1, /// 0x974
			 0xc745c67f, /// 0x978
			 0xe6c026bb, /// 0x97c
			 0xe0b31b03, /// 0x980
			 0x06c27aec, /// 0x984
			 0x1554a4c1, /// 0x988
			 0x58ef9be2, /// 0x98c
			 0xae37b5f4, /// 0x990
			 0xbe46e561, /// 0x994
			 0xe6daa8c7, /// 0x998
			 0x610b8b17, /// 0x99c
			 0xb8cf42be, /// 0x9a0
			 0x60f8687a, /// 0x9a4
			 0x013058bd, /// 0x9a8
			 0x8afd99c2, /// 0x9ac
			 0xccdb15f6, /// 0x9b0
			 0x9161a785, /// 0x9b4
			 0x9e48cc36, /// 0x9b8
			 0x89d4e93c, /// 0x9bc
			 0x58f8e402, /// 0x9c0
			 0xd1bf2cdd, /// 0x9c4
			 0x327472ad, /// 0x9c8
			 0x35dac2c9, /// 0x9cc
			 0x25e7d434, /// 0x9d0
			 0x9386ceb2, /// 0x9d4
			 0xccb4e12d, /// 0x9d8
			 0xbcca5369, /// 0x9dc
			 0xe8a8678c, /// 0x9e0
			 0xa910d73d, /// 0x9e4
			 0x39e0ae6e, /// 0x9e8
			 0x2ff3766c, /// 0x9ec
			 0x4c86f729, /// 0x9f0
			 0x802de814, /// 0x9f4
			 0x9689715b, /// 0x9f8
			 0xcaa6bc77, /// 0x9fc
			 0x1f2e7e6c, /// 0xa00
			 0xb4136682, /// 0xa04
			 0xcc03b680, /// 0xa08
			 0xe53e7b7e, /// 0xa0c
			 0x3f37b6c0, /// 0xa10
			 0x3d932227, /// 0xa14
			 0x53cc4934, /// 0xa18
			 0x1eb37c8b, /// 0xa1c
			 0x67d13d32, /// 0xa20
			 0x97ee7759, /// 0xa24
			 0x43020079, /// 0xa28
			 0x6d3a7789, /// 0xa2c
			 0xda1c5a12, /// 0xa30
			 0x2096631f, /// 0xa34
			 0xa6e178d6, /// 0xa38
			 0x8a09aa71, /// 0xa3c
			 0x1d21ea95, /// 0xa40
			 0xf2b694dc, /// 0xa44
			 0xba4aad22, /// 0xa48
			 0xfcc3e741, /// 0xa4c
			 0xa8cffd00, /// 0xa50
			 0x6560dafa, /// 0xa54
			 0x7c46d936, /// 0xa58
			 0xc522605b, /// 0xa5c
			 0x53d78c36, /// 0xa60
			 0xe9821cf1, /// 0xa64
			 0x5b100da4, /// 0xa68
			 0x2df1b2ae, /// 0xa6c
			 0xb0abb609, /// 0xa70
			 0x5db1898f, /// 0xa74
			 0x4d1d1918, /// 0xa78
			 0xd11107c7, /// 0xa7c
			 0xadbd575c, /// 0xa80
			 0x9bdb7bb0, /// 0xa84
			 0x8135da1d, /// 0xa88
			 0x13d4fb70, /// 0xa8c
			 0x25f3ea80, /// 0xa90
			 0xfdbb654c, /// 0xa94
			 0x742c2f6a, /// 0xa98
			 0x4560e4e5, /// 0xa9c
			 0xff332813, /// 0xaa0
			 0x7c7f0cb1, /// 0xaa4
			 0xed98729f, /// 0xaa8
			 0xd81f4ca3, /// 0xaac
			 0x0e5fb09b, /// 0xab0
			 0x197a1144, /// 0xab4
			 0x6f755009, /// 0xab8
			 0x940b9234, /// 0xabc
			 0xd18dac45, /// 0xac0
			 0xbea0c8e1, /// 0xac4
			 0xe0a7e9ba, /// 0xac8
			 0xeae24e30, /// 0xacc
			 0x53df5c02, /// 0xad0
			 0x9f31f551, /// 0xad4
			 0x5bf8274b, /// 0xad8
			 0xd0afec4b, /// 0xadc
			 0x408248fb, /// 0xae0
			 0x82f8395d, /// 0xae4
			 0xe5a04500, /// 0xae8
			 0x0bf8f947, /// 0xaec
			 0xf27b6e02, /// 0xaf0
			 0xa9bef0d9, /// 0xaf4
			 0x661e66f1, /// 0xaf8
			 0x89b4b7b1, /// 0xafc
			 0x999a5cd7, /// 0xb00
			 0xa81e5344, /// 0xb04
			 0xefd7fe20, /// 0xb08
			 0xd7991fe5, /// 0xb0c
			 0x586970bf, /// 0xb10
			 0xe2eb416d, /// 0xb14
			 0xe89d93ac, /// 0xb18
			 0xf9c05991, /// 0xb1c
			 0x05a3ee18, /// 0xb20
			 0xc567c666, /// 0xb24
			 0xe9fa2749, /// 0xb28
			 0x8acceacf, /// 0xb2c
			 0x7922f030, /// 0xb30
			 0x1ec82a54, /// 0xb34
			 0xa0747103, /// 0xb38
			 0x005f435c, /// 0xb3c
			 0xbad27198, /// 0xb40
			 0x90367a55, /// 0xb44
			 0xcb46210a, /// 0xb48
			 0xa5b0b44e, /// 0xb4c
			 0x1f48e44e, /// 0xb50
			 0x110a46ad, /// 0xb54
			 0x19b294ec, /// 0xb58
			 0x33183c05, /// 0xb5c
			 0xb711a9ef, /// 0xb60
			 0xeffd7cfe, /// 0xb64
			 0xa15a504c, /// 0xb68
			 0x0d04c3f8, /// 0xb6c
			 0xa4fcfc4b, /// 0xb70
			 0x27168caa, /// 0xb74
			 0x26774a96, /// 0xb78
			 0x1565a8ed, /// 0xb7c
			 0x5fe2d175, /// 0xb80
			 0xa8bc61ae, /// 0xb84
			 0xe2b5bf1e, /// 0xb88
			 0xa0aba26f, /// 0xb8c
			 0xc37697e2, /// 0xb90
			 0x79e87599, /// 0xb94
			 0xfec3e5aa, /// 0xb98
			 0x50329d45, /// 0xb9c
			 0xbef61a8e, /// 0xba0
			 0x91842962, /// 0xba4
			 0x9eb57b23, /// 0xba8
			 0xc44996f1, /// 0xbac
			 0xe287214c, /// 0xbb0
			 0x4b8f235c, /// 0xbb4
			 0xf6df6c1a, /// 0xbb8
			 0x166ae074, /// 0xbbc
			 0x1f28ace0, /// 0xbc0
			 0x053dc86a, /// 0xbc4
			 0x3da98b70, /// 0xbc8
			 0x8d4072a6, /// 0xbcc
			 0x3615ac17, /// 0xbd0
			 0x068ee403, /// 0xbd4
			 0xb9361bb4, /// 0xbd8
			 0x57e115d9, /// 0xbdc
			 0x025a660a, /// 0xbe0
			 0x67a94933, /// 0xbe4
			 0x41ab598c, /// 0xbe8
			 0x76749f3d, /// 0xbec
			 0x48b6d5cd, /// 0xbf0
			 0xc3078056, /// 0xbf4
			 0xa89a563c, /// 0xbf8
			 0xbb3b794b, /// 0xbfc
			 0xbce3092b, /// 0xc00
			 0xef80d00d, /// 0xc04
			 0xee9c082e, /// 0xc08
			 0xb9ccb1c9, /// 0xc0c
			 0x0f48aea9, /// 0xc10
			 0xf201eddf, /// 0xc14
			 0x3d4a09da, /// 0xc18
			 0x255bee13, /// 0xc1c
			 0x349ac755, /// 0xc20
			 0x64955572, /// 0xc24
			 0xb1afae9d, /// 0xc28
			 0xb692dfff, /// 0xc2c
			 0x8162d265, /// 0xc30
			 0xd13759ea, /// 0xc34
			 0x7706d764, /// 0xc38
			 0x6266ce5b, /// 0xc3c
			 0x487668a2, /// 0xc40
			 0x0a775c07, /// 0xc44
			 0x0bbe81b4, /// 0xc48
			 0x35b88767, /// 0xc4c
			 0xbe4b1015, /// 0xc50
			 0x27c540f6, /// 0xc54
			 0x011535d8, /// 0xc58
			 0xa78df9b0, /// 0xc5c
			 0x97102391, /// 0xc60
			 0x1a47f800, /// 0xc64
			 0x90b2d757, /// 0xc68
			 0x1760595c, /// 0xc6c
			 0x61301bbe, /// 0xc70
			 0x1d3be4ad, /// 0xc74
			 0xdaaf528b, /// 0xc78
			 0xc1661d18, /// 0xc7c
			 0x8fed27eb, /// 0xc80
			 0x2ead1c08, /// 0xc84
			 0x89e85b76, /// 0xc88
			 0xa99c5f6e, /// 0xc8c
			 0xc215eeea, /// 0xc90
			 0xe71fd206, /// 0xc94
			 0x957b4c16, /// 0xc98
			 0x2e50a542, /// 0xc9c
			 0x0476663e, /// 0xca0
			 0x4d312297, /// 0xca4
			 0x69146e4f, /// 0xca8
			 0x43c6a4f6, /// 0xcac
			 0xb754d117, /// 0xcb0
			 0xda72f4b6, /// 0xcb4
			 0x59ae7899, /// 0xcb8
			 0x3954b9f2, /// 0xcbc
			 0x3284ae4a, /// 0xcc0
			 0x6a2b784d, /// 0xcc4
			 0xbdb9928a, /// 0xcc8
			 0xec44ba7e, /// 0xccc
			 0xa8acd0e5, /// 0xcd0
			 0x6345c645, /// 0xcd4
			 0xf5106dea, /// 0xcd8
			 0xb331e327, /// 0xcdc
			 0xd3a881d8, /// 0xce0
			 0x280b00c9, /// 0xce4
			 0x857f5ccb, /// 0xce8
			 0xe4237863, /// 0xcec
			 0x150f3658, /// 0xcf0
			 0xa4fd3854, /// 0xcf4
			 0xe878f05f, /// 0xcf8
			 0x04a838a3, /// 0xcfc
			 0xf19538a3, /// 0xd00
			 0x8fbec1d7, /// 0xd04
			 0xaf30ffea, /// 0xd08
			 0x5daea680, /// 0xd0c
			 0x9edf947b, /// 0xd10
			 0x20fedb59, /// 0xd14
			 0x2161bcba, /// 0xd18
			 0x0d19bdc1, /// 0xd1c
			 0x8e0457cc, /// 0xd20
			 0x02bc1614, /// 0xd24
			 0x89103d43, /// 0xd28
			 0xb6079ea0, /// 0xd2c
			 0x02058c39, /// 0xd30
			 0xb49537c2, /// 0xd34
			 0xfc458ecd, /// 0xd38
			 0x08f8ca91, /// 0xd3c
			 0xa6db1ca6, /// 0xd40
			 0x11addedc, /// 0xd44
			 0x9e694d34, /// 0xd48
			 0x208fd666, /// 0xd4c
			 0xe80438bf, /// 0xd50
			 0xb80c0c05, /// 0xd54
			 0xccf5540c, /// 0xd58
			 0x1ee8b458, /// 0xd5c
			 0xb11f255e, /// 0xd60
			 0x432f0895, /// 0xd64
			 0x82161848, /// 0xd68
			 0x73a798f9, /// 0xd6c
			 0x9dadad4b, /// 0xd70
			 0xdd33f0cc, /// 0xd74
			 0x987c906d, /// 0xd78
			 0x634b9088, /// 0xd7c
			 0x1b56b7c6, /// 0xd80
			 0x625c483f, /// 0xd84
			 0x2c93551f, /// 0xd88
			 0xfbdefe9f, /// 0xd8c
			 0x7c0b1347, /// 0xd90
			 0x40af9973, /// 0xd94
			 0x52b9dd3a, /// 0xd98
			 0x4f86381c, /// 0xd9c
			 0xb3b473c6, /// 0xda0
			 0xd136e776, /// 0xda4
			 0xe1ac4d22, /// 0xda8
			 0x8c6c8b99, /// 0xdac
			 0x5cef19c0, /// 0xdb0
			 0x4d417a27, /// 0xdb4
			 0x05020907, /// 0xdb8
			 0x3c04a5ce, /// 0xdbc
			 0xfb9d7eef, /// 0xdc0
			 0x4637a503, /// 0xdc4
			 0x9e5bd9f6, /// 0xdc8
			 0x5a256af8, /// 0xdcc
			 0xc2fc1b6a, /// 0xdd0
			 0x3d5da271, /// 0xdd4
			 0xd59d27ab, /// 0xdd8
			 0x3a4db7ad, /// 0xddc
			 0xb7ae6fe9, /// 0xde0
			 0x487a588a, /// 0xde4
			 0x60f11f69, /// 0xde8
			 0x3efbad81, /// 0xdec
			 0x3dc47b97, /// 0xdf0
			 0x72c6644e, /// 0xdf4
			 0x033c3b16, /// 0xdf8
			 0x703ff1d8, /// 0xdfc
			 0x4eb913cf, /// 0xe00
			 0xf26f7022, /// 0xe04
			 0x575e2324, /// 0xe08
			 0x60ef6388, /// 0xe0c
			 0x16e55b48, /// 0xe10
			 0xa3b2c94a, /// 0xe14
			 0x94982c1f, /// 0xe18
			 0xb7ef01ae, /// 0xe1c
			 0x529777c7, /// 0xe20
			 0xac7f8322, /// 0xe24
			 0x2108c3b8, /// 0xe28
			 0x33fe601c, /// 0xe2c
			 0xa0c3b691, /// 0xe30
			 0x9bf8218e, /// 0xe34
			 0xf525eb6c, /// 0xe38
			 0x574b634c, /// 0xe3c
			 0x198c46d3, /// 0xe40
			 0x36eb6de5, /// 0xe44
			 0x05ca778b, /// 0xe48
			 0x26b9543d, /// 0xe4c
			 0x7ffd05b0, /// 0xe50
			 0x67577e96, /// 0xe54
			 0x26d1120f, /// 0xe58
			 0x181bd464, /// 0xe5c
			 0x1f1334d3, /// 0xe60
			 0x6daa906d, /// 0xe64
			 0x08bf1e33, /// 0xe68
			 0x90df8b3e, /// 0xe6c
			 0x8d458f86, /// 0xe70
			 0xa57b8ffc, /// 0xe74
			 0xc26bce51, /// 0xe78
			 0x23ebd35d, /// 0xe7c
			 0xd1c6ffb6, /// 0xe80
			 0x1f3eb061, /// 0xe84
			 0x37235841, /// 0xe88
			 0xd15f37aa, /// 0xe8c
			 0xac440448, /// 0xe90
			 0xd5126462, /// 0xe94
			 0xe70cb2ea, /// 0xe98
			 0xb49df1e9, /// 0xe9c
			 0x0a1f773c, /// 0xea0
			 0x66ee51e6, /// 0xea4
			 0xf610a392, /// 0xea8
			 0xfeb544f3, /// 0xeac
			 0x13bbd8c2, /// 0xeb0
			 0xa6b43aea, /// 0xeb4
			 0x1189df1f, /// 0xeb8
			 0xed5230e1, /// 0xebc
			 0xc23f0edd, /// 0xec0
			 0xee2f1a7f, /// 0xec4
			 0x3b95a4f4, /// 0xec8
			 0x3b404186, /// 0xecc
			 0x1e71e497, /// 0xed0
			 0xfcbd8c19, /// 0xed4
			 0xacc4a0f1, /// 0xed8
			 0x01350ec7, /// 0xedc
			 0x1d5a3253, /// 0xee0
			 0x6ad8bddb, /// 0xee4
			 0x6109f0e1, /// 0xee8
			 0x4ca97220, /// 0xeec
			 0xce26e862, /// 0xef0
			 0x89b4cf56, /// 0xef4
			 0xeee1bf59, /// 0xef8
			 0xad083d46, /// 0xefc
			 0xd1abadba, /// 0xf00
			 0x8bd002a0, /// 0xf04
			 0x9593fe64, /// 0xf08
			 0x0f190ca7, /// 0xf0c
			 0x74366aed, /// 0xf10
			 0x00148b94, /// 0xf14
			 0xe0619ad0, /// 0xf18
			 0xec5150b9, /// 0xf1c
			 0x684fdb20, /// 0xf20
			 0x6e393034, /// 0xf24
			 0x3d94359c, /// 0xf28
			 0x281590fc, /// 0xf2c
			 0x6fd88eda, /// 0xf30
			 0xa514b70b, /// 0xf34
			 0xe90ab304, /// 0xf38
			 0x2d504a2c, /// 0xf3c
			 0xd83c8b87, /// 0xf40
			 0xd243daa8, /// 0xf44
			 0x71bc8409, /// 0xf48
			 0x9ba4cfe8, /// 0xf4c
			 0x191a2ca6, /// 0xf50
			 0x324c5713, /// 0xf54
			 0x38b8b414, /// 0xf58
			 0x9e9f4eab, /// 0xf5c
			 0xfe0bd3dd, /// 0xf60
			 0x5499a56d, /// 0xf64
			 0xf0bbab55, /// 0xf68
			 0xa6305c1a, /// 0xf6c
			 0x43168986, /// 0xf70
			 0x903d72b4, /// 0xf74
			 0x3bc39ad3, /// 0xf78
			 0xc3b5611a, /// 0xf7c
			 0x5959517f, /// 0xf80
			 0xf94ec635, /// 0xf84
			 0xc7539b85, /// 0xf88
			 0xdce4c740, /// 0xf8c
			 0xc40fd250, /// 0xf90
			 0x8257531d, /// 0xf94
			 0x9f8669c1, /// 0xf98
			 0x7eab1a1f, /// 0xf9c
			 0x20e8e024, /// 0xfa0
			 0x67e95d8b, /// 0xfa4
			 0x3405ccb8, /// 0xfa8
			 0xdccc286e, /// 0xfac
			 0x9a7a3f48, /// 0xfb0
			 0x39715a94, /// 0xfb4
			 0x23c74838, /// 0xfb8
			 0x4af0b610, /// 0xfbc
			 0x28e2da09, /// 0xfc0
			 0x9f40474e, /// 0xfc4
			 0xbe9f7e92, /// 0xfc8
			 0xdbf883dc, /// 0xfcc
			 0x81503d3a, /// 0xfd0
			 0xa0782572, /// 0xfd4
			 0xbc044ca1, /// 0xfd8
			 0x1f91f8be, /// 0xfdc
			 0x48683590, /// 0xfe0
			 0xf50547a6, /// 0xfe4
			 0x7ea814a0, /// 0xfe8
			 0xa24522cb, /// 0xfec
			 0x32077dec, /// 0xff0
			 0x30b09e8c, /// 0xff4
			 0xaff951e5, /// 0xff8
			 0x52cdb657 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0c400000,                                                  // Leading 1s:                                 /// 00004
			 0x0c780000,                                                  // Leading 1s:                                 /// 00008
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00010
			 0x0c600000,                                                  // Leading 1s:                                 /// 00014
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0001c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00020
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00024
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00028
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0002c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00030
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00034
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00038
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0003c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00040
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00044
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00050
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00054
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00058
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00060
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0006c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00074
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0007c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00080
			 0x4b000000,                                                  // 8388608.0                                   /// 00084
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0008c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00094
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0009c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000e4
			 0x80000000,                                                  // -zero                                       /// 000e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00100
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00108
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00128
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0012c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00130
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00134
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00138
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00140
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00144
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0014c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00150
			 0x4b000000,                                                  // 8388608.0                                   /// 00154
			 0x7fc00001,                                                  // signaling NaN                               /// 00158
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0016c
			 0x7f800000,                                                  // inf                                         /// 00170
			 0x4b000000,                                                  // 8388608.0                                   /// 00174
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00178
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0017c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00184
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0018c
			 0xcb000000,                                                  // -8388608.0                                  /// 00190
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00194
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0019c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00200
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00204
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00210
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00214
			 0x0c600000,                                                  // Leading 1s:                                 /// 00218
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00220
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00224
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0022c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00230
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00234
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00240
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00244
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00248
			 0xcb000000,                                                  // -8388608.0                                  /// 0024c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e000007,                                                  // Trailing 1s:                                /// 00254
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0025c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00260
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00264
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00270
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00274
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00278
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0027c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00284
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0028c
			 0xbf028f5c,                                                  // -0.51                                       /// 00290
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00294
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f0
			 0xcb000000,                                                  // -8388608.0                                  /// 002f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00304
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00308
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00310
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00318
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0031c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00330
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00334
			 0x0c400000,                                                  // Leading 1s:                                 /// 00338
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00344
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00350
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00354
			 0x0e000007,                                                  // Trailing 1s:                                /// 00358
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0035c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00360
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00368
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00374
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00380
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00384
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00388
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00390
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0039c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x3f028f5c,                                                  // 0.51                                        /// 003ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00400
			 0x80000000,                                                  // -zero                                       /// 00404
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00408
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00410
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00414
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00420
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00424
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00428
			 0x0e000007,                                                  // Trailing 1s:                                /// 0042c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00430
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00440
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00444
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00448
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00450
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0045c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00468
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0046c
			 0x7f800000,                                                  // inf                                         /// 00470
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00474
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00478
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0047c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00484
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00488
			 0xffc00001,                                                  // -signaling NaN                              /// 0048c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00490
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00494
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00498
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0049c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80000000,                                                  // -zero                                       /// 004ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x7f800000,                                                  // inf                                         /// 004d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 004e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00500
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00504
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00508
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00514
			 0x0e000001,                                                  // Trailing 1s:                                /// 00518
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0xbf028f5c,                                                  // -0.51                                       /// 00524
			 0x0c600000,                                                  // Leading 1s:                                 /// 00528
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0052c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00534
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00540
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00548
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00550
			 0x0c700000,                                                  // Leading 1s:                                 /// 00554
			 0x0e000003,                                                  // Trailing 1s:                                /// 00558
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0055c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00564
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00568
			 0x00011111,                                                  // 9.7958E-41                                  /// 0056c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00570
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00574
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0057c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00580
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00584
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00588
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0058c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00594
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00598
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c400000,                                                  // Leading 1s:                                 /// 0060c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00610
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00614
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00618
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0061c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00620
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00634
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00638
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0063c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00648
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00654
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00658
			 0x7fc00001,                                                  // signaling NaN                               /// 0065c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00660
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00670
			 0x0e000003,                                                  // Trailing 1s:                                /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0067c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00680
			 0xbf028f5c,                                                  // -0.51                                       /// 00684
			 0xffc00001,                                                  // -signaling NaN                              /// 00688
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00690
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00694
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0069c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x7fc00001,                                                  // signaling NaN                               /// 006fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00704
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00708
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00714
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0071c
			 0xffc00001,                                                  // -signaling NaN                              /// 00720
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7f800000,                                                  // inf                                         /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80000000,                                                  // -zero                                       /// 0073c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00740
			 0x0e000007,                                                  // Trailing 1s:                                /// 00744
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00748
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0074c
			 0x80000000,                                                  // -zero                                       /// 00750
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00758
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00760
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00764
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00768
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00770
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x55555555,                                                  // 4 random values                             /// 00784
			 0x3f028f5c,                                                  // 0.51                                        /// 00788
			 0x4b000000,                                                  // 8388608.0                                   /// 0078c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x0c700000,                                                  // Leading 1s:                                 /// 00794
			 0x0c700000,                                                  // Leading 1s:                                 /// 00798
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0079c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00800
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00804
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00808
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0080c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00810
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00818
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0081c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00828
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0082c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00830
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00834
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0083c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00840
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00844
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00848
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0084c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00854
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00858
			 0x00000000,                                                  // zero                                        /// 0085c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00860
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00864
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00868
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0086c
			 0x3f028f5c,                                                  // 0.51                                        /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00880
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00884
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0088c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00890
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00894
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x7fc00001,                                                  // signaling NaN                               /// 008b4
			 0x7f800000,                                                  // inf                                         /// 008b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00904
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00918
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00924
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00928
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0092c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00938
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0093c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00940
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0094c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00950
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00954
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00958
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80000000,                                                  // -zero                                       /// 0096c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00978
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0097c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00980
			 0x4b000000,                                                  // 8388608.0                                   /// 00984
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0098c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00990
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00998
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0099c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x55555555,                                                  // 4 random values                             /// 00a50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a64
			 0x33333333,                                                  // 4 random values                             /// 00a68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa4
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00abc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00acc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0xcb000000,                                                  // -8388608.0                                  /// 00adc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ae0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae8
			 0x55555555,                                                  // 4 random values                             /// 00aec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00af4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00afc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b50
			 0x7f800000,                                                  // inf                                         /// 00b54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b98
			 0x80000000,                                                  // -zero                                       /// 00b9c
			 0x7f800000,                                                  // inf                                         /// 00ba0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ba8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000000,                                                  // -zero                                       /// 00bb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bcc
			 0xffc00001,                                                  // -signaling NaN                              /// 00bd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bdc
			 0x7f800000,                                                  // inf                                         /// 00be0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c68
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c7c
			 0x33333333,                                                  // 4 random values                             /// 00c80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ca8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d3c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d78
			 0x7fc00001,                                                  // signaling NaN                               /// 00d7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00db4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dcc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00de4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00df0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00df4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xbf028f5c,                                                  // -0.51                                       /// 00e78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ebc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ec4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ed4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00edc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ee8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x55555555,                                                  // 4 random values                             /// 00f00
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f34
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f3c
			 0x33333333,                                                  // 4 random values                             /// 00f40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x4b000000,                                                  // 8388608.0                                   /// 00f98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x7f800000,                                                  // inf                                         /// 00fa4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fe4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ff4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x7fbfffff // SNaN                                          // SP +ve numbers                              /// last
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
			 0x000006f0,
			 0x000003e8,
			 0x00000278,
			 0x00000778,
			 0x00000368,
			 0x000004d4,
			 0x00000624,
			 0x000006e4,

			 /// vpu register f2
			 0x41c80000,
			 0x3f800000,
			 0x41d00000,
			 0x41d00000,
			 0x41500000,
			 0x41500000,
			 0x40a00000,
			 0x41e00000,

			 /// vpu register f3
			 0x41d80000,
			 0x41980000,
			 0x41880000,
			 0x41f00000,
			 0x40e00000,
			 0x41e00000,
			 0x41200000,
			 0x41000000,

			 /// vpu register f4
			 0x40400000,
			 0x41e00000,
			 0x40000000,
			 0x41000000,
			 0x42000000,
			 0x41c00000,
			 0x41400000,
			 0x40400000,

			 /// vpu register f5
			 0x40000000,
			 0x40800000,
			 0x41f00000,
			 0x41400000,
			 0x41700000,
			 0x41c00000,
			 0x41200000,
			 0x41100000,

			 /// vpu register f6
			 0x41b00000,
			 0x41f00000,
			 0x41d80000,
			 0x41a00000,
			 0x41d80000,
			 0x41600000,
			 0x40a00000,
			 0x41b80000,

			 /// vpu register f7
			 0x40800000,
			 0x42000000,
			 0x41700000,
			 0x41900000,
			 0x41500000,
			 0x41200000,
			 0x3f800000,
			 0x41d80000,

			 /// vpu register f8
			 0x41900000,
			 0x41300000,
			 0x41e80000,
			 0x42000000,
			 0x41700000,
			 0x41c80000,
			 0x41c80000,
			 0x41700000,

			 /// vpu register f9
			 0x41c80000,
			 0x41900000,
			 0x40e00000,
			 0x41f80000,
			 0x41c00000,
			 0x41e00000,
			 0x40a00000,
			 0x40000000,

			 /// vpu register f10
			 0x41c00000,
			 0x41880000,
			 0x41b00000,
			 0x40000000,
			 0x41100000,
			 0x3f800000,
			 0x41e00000,
			 0x41c00000,

			 /// vpu register f11
			 0x40000000,
			 0x41900000,
			 0x41880000,
			 0x41400000,
			 0x41b00000,
			 0x41e00000,
			 0x3f800000,
			 0x41a00000,

			 /// vpu register f12
			 0x41b00000,
			 0x41880000,
			 0x3f800000,
			 0x41500000,
			 0x41000000,
			 0x40400000,
			 0x41100000,
			 0x41500000,

			 /// vpu register f13
			 0x41900000,
			 0x40a00000,
			 0x40c00000,
			 0x41f80000,
			 0x40400000,
			 0x41f80000,
			 0x41100000,
			 0x40800000,

			 /// vpu register f14
			 0x41900000,
			 0x40c00000,
			 0x40a00000,
			 0x41200000,
			 0x41200000,
			 0x41a00000,
			 0x40c00000,
			 0x41800000,

			 /// vpu register f15
			 0x40000000,
			 0x40800000,
			 0x3f800000,
			 0x40c00000,
			 0x41a80000,
			 0x41200000,
			 0x40a00000,
			 0x41800000,

			 /// vpu register f16
			 0x41880000,
			 0x41d80000,
			 0x41d80000,
			 0x40c00000,
			 0x40400000,
			 0x41980000,
			 0x41d00000,
			 0x41a00000,

			 /// vpu register f17
			 0x41f80000,
			 0x41f80000,
			 0x40a00000,
			 0x41600000,
			 0x42000000,
			 0x41e80000,
			 0x41d00000,
			 0x41900000,

			 /// vpu register f18
			 0x41400000,
			 0x40800000,
			 0x41b80000,
			 0x3f800000,
			 0x41980000,
			 0x41900000,
			 0x41b80000,
			 0x40a00000,

			 /// vpu register f19
			 0x41400000,
			 0x41b80000,
			 0x41d80000,
			 0x41100000,
			 0x41500000,
			 0x42000000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f20
			 0x41e80000,
			 0x41f00000,
			 0x41a00000,
			 0x3f800000,
			 0x41a00000,
			 0x41c00000,
			 0x41880000,
			 0x41100000,

			 /// vpu register f21
			 0x41800000,
			 0x41c00000,
			 0x41100000,
			 0x40a00000,
			 0x3f800000,
			 0x41880000,
			 0x41980000,
			 0x41b00000,

			 /// vpu register f22
			 0x41f80000,
			 0x41f00000,
			 0x41400000,
			 0x41b00000,
			 0x40a00000,
			 0x41e80000,
			 0x41100000,
			 0x41b00000,

			 /// vpu register f23
			 0x41b80000,
			 0x41d00000,
			 0x41c80000,
			 0x41100000,
			 0x41300000,
			 0x41a80000,
			 0x41600000,
			 0x41900000,

			 /// vpu register f24
			 0x41000000,
			 0x41700000,
			 0x42000000,
			 0x41200000,
			 0x41700000,
			 0x41500000,
			 0x40e00000,
			 0x40000000,

			 /// vpu register f25
			 0x40800000,
			 0x40000000,
			 0x41200000,
			 0x41400000,
			 0x40e00000,
			 0x41b80000,
			 0x41f00000,
			 0x41700000,

			 /// vpu register f26
			 0x41300000,
			 0x41300000,
			 0x3f800000,
			 0x41400000,
			 0x41d00000,
			 0x40e00000,
			 0x41200000,
			 0x3f800000,

			 /// vpu register f27
			 0x41e00000,
			 0x41600000,
			 0x41500000,
			 0x41a80000,
			 0x41980000,
			 0x41880000,
			 0x41b00000,
			 0x41800000,

			 /// vpu register f28
			 0x41f80000,
			 0x41100000,
			 0x41e00000,
			 0x41500000,
			 0x41500000,
			 0x41000000,
			 0x3f800000,
			 0x40400000,

			 /// vpu register f29
			 0x41100000,
			 0x41600000,
			 0x41980000,
			 0x42000000,
			 0x40400000,
			 0x41100000,
			 0x41600000,
			 0x41e00000,

			 /// vpu register f30
			 0x41e00000,
			 0x41e80000,
			 0x41200000,
			 0x41600000,
			 0x41500000,
			 0x40000000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f31
			 0x41d80000,
			 0x41400000,
			 0x40000000,
			 0x41c80000,
			 0x40800000,
			 0x41e80000,
			 0x41300000,
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
		"fgbl.ps f15, f1 (x12)\n"                             ///  0,     0
		"fclass.ps f18, f4\n"                                 ///  0,     1
		"fsrl.pi f29, f26, f26\n"                             ///  0,     2
		"cubefaceidx.ps f2, f24, f15\n"                       ///  0,     3
		"fmin.ps f14, f27, f15\n"                             ///  0,     4
		"fcvt.pw.ps f13, f6, rup\n"                           ///  0,     5
		"maskor m1, m2, m0\n"                                 ///  0,     6
		"fmsub.s f5, f27, f17, f25, rdn\n"                    ///  0,     7
		"fltu.pi f12, f8, f13\n"                              ///  0,     8
		"fsetm.pi m7, f24\n"                                  ///  0,     9
		"fmax.ps f16, f0, f10\n"                              ///  0,    10
		"fcvt.ps.pw f24, f25, rmm\n"                          ///  0,    11
		"fmulh.pi f13, f16, f0\n"                             ///  0,    12
		"fmul.pi f4, f9, f6\n"                                ///  0,    13
		"maskpopc x7, m5\n"                                   ///  0,    14
		"fscw.ps f1, f1 (x11)\n"                              ///  0,    15
		"fsgnjn.ps f10, f12, f17\n"                           ///  0,    16
		"fgbl.ps f2, f1 (x13)\n"                              ///  0,    17
		"fcvt.ps.pwu f2, f8, rdn\n"                           ///  0,    18
		"fcvt.pw.ps f29, f16, dyn\n"                          ///  0,    19
		"fmul.ps f29, f6, f30, dyn\n"                         ///  0,    20
		"fcvt.ps.pw f11, f2, rne\n"                           ///  0,    21
		"fandi.pi f13, f28, 32\n"                             ///  0,    22
		"fltu.pi f15, f17, f13\n"                             ///  0,    23
        
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
