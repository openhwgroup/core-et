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
			 0x04ee3fe0, /// 0x0
			 0x6cd90b82, /// 0x4
			 0xa355e363, /// 0x8
			 0x4b4cbacf, /// 0xc
			 0x0dca19b7, /// 0x10
			 0x74ba2f89, /// 0x14
			 0x25f05f03, /// 0x18
			 0xc917f15e, /// 0x1c
			 0x5ec9a634, /// 0x20
			 0xa0f2265d, /// 0x24
			 0x9c85ca74, /// 0x28
			 0xefa10162, /// 0x2c
			 0x3d89d0e2, /// 0x30
			 0x514c2498, /// 0x34
			 0xcb5cc638, /// 0x38
			 0x5019b2d3, /// 0x3c
			 0x4367b18c, /// 0x40
			 0xf0e76bd9, /// 0x44
			 0x9fdf777e, /// 0x48
			 0x294b3d8c, /// 0x4c
			 0x0d1ff3e0, /// 0x50
			 0x2baa0e36, /// 0x54
			 0x6e2144bd, /// 0x58
			 0x1d585eae, /// 0x5c
			 0x5c8ad012, /// 0x60
			 0x153dac94, /// 0x64
			 0x918a8e23, /// 0x68
			 0x716a84c7, /// 0x6c
			 0x01e746bd, /// 0x70
			 0x715bf97b, /// 0x74
			 0xd8cc6a7e, /// 0x78
			 0xbbfda45b, /// 0x7c
			 0x918441b6, /// 0x80
			 0x2b9d6289, /// 0x84
			 0x87f71fef, /// 0x88
			 0xbd95c9cb, /// 0x8c
			 0x34581317, /// 0x90
			 0xaae4673b, /// 0x94
			 0xb792e76c, /// 0x98
			 0xe029a829, /// 0x9c
			 0x854b7865, /// 0xa0
			 0xe6050af6, /// 0xa4
			 0x380b30df, /// 0xa8
			 0xe63bef76, /// 0xac
			 0x011c9734, /// 0xb0
			 0xe8ec2df1, /// 0xb4
			 0x8be48812, /// 0xb8
			 0xb3b1bbf5, /// 0xbc
			 0x1d8bbbd4, /// 0xc0
			 0x00c8088e, /// 0xc4
			 0x39fea510, /// 0xc8
			 0x8e4c81e5, /// 0xcc
			 0xbc37d904, /// 0xd0
			 0x83335d06, /// 0xd4
			 0x18493c39, /// 0xd8
			 0xc7dd6724, /// 0xdc
			 0x757cca34, /// 0xe0
			 0x6b11e0fb, /// 0xe4
			 0x3654ee27, /// 0xe8
			 0xdf52a936, /// 0xec
			 0x7d3c397d, /// 0xf0
			 0x95bbc757, /// 0xf4
			 0x7b2ba9a0, /// 0xf8
			 0xd5c4a4a0, /// 0xfc
			 0x6bad96f2, /// 0x100
			 0x8c9cd8d9, /// 0x104
			 0x7425417c, /// 0x108
			 0x0ec4638c, /// 0x10c
			 0x6c1eb255, /// 0x110
			 0x40cfe0dd, /// 0x114
			 0x86ea9b73, /// 0x118
			 0xff19a838, /// 0x11c
			 0xc728b41e, /// 0x120
			 0xa156fb59, /// 0x124
			 0x4be49955, /// 0x128
			 0xde5a4293, /// 0x12c
			 0xfd3d2b71, /// 0x130
			 0xe7185d74, /// 0x134
			 0xdb6ba573, /// 0x138
			 0x5d67b68a, /// 0x13c
			 0xecfc23db, /// 0x140
			 0xffd63460, /// 0x144
			 0x264eba5d, /// 0x148
			 0x7314c60d, /// 0x14c
			 0x3dd6185d, /// 0x150
			 0x824703ec, /// 0x154
			 0x0a2a91f2, /// 0x158
			 0xdd969050, /// 0x15c
			 0xead9a5dc, /// 0x160
			 0x37052cb5, /// 0x164
			 0x1d208978, /// 0x168
			 0x442143d9, /// 0x16c
			 0x287e011c, /// 0x170
			 0x2904db8f, /// 0x174
			 0x8d0e6aec, /// 0x178
			 0x8ddcd4a3, /// 0x17c
			 0xba01f2bc, /// 0x180
			 0xbb4c9be9, /// 0x184
			 0x8c7c135a, /// 0x188
			 0xacec721e, /// 0x18c
			 0x46ec2c05, /// 0x190
			 0x50baa313, /// 0x194
			 0xc468990f, /// 0x198
			 0xaba03788, /// 0x19c
			 0x1dfe37de, /// 0x1a0
			 0x0ab73f48, /// 0x1a4
			 0xa0ee3faa, /// 0x1a8
			 0x364cf243, /// 0x1ac
			 0xbbf0be2a, /// 0x1b0
			 0x31e4f5ba, /// 0x1b4
			 0xea2b8d21, /// 0x1b8
			 0x971df0cf, /// 0x1bc
			 0x432bc9ba, /// 0x1c0
			 0x5f0ae4a5, /// 0x1c4
			 0x5c3028b4, /// 0x1c8
			 0x3231cd50, /// 0x1cc
			 0x9cbc6bfd, /// 0x1d0
			 0x55db6515, /// 0x1d4
			 0x546c8476, /// 0x1d8
			 0xd3db428f, /// 0x1dc
			 0xeaf63314, /// 0x1e0
			 0x41e384b3, /// 0x1e4
			 0x271320f5, /// 0x1e8
			 0xa51929cb, /// 0x1ec
			 0x4c790306, /// 0x1f0
			 0x0d72c253, /// 0x1f4
			 0x92fd57a1, /// 0x1f8
			 0xf882e759, /// 0x1fc
			 0x1cf229d4, /// 0x200
			 0x8971fa03, /// 0x204
			 0xa408c89f, /// 0x208
			 0x2e2ad2f8, /// 0x20c
			 0x47e54489, /// 0x210
			 0x3506ec46, /// 0x214
			 0x3536bb18, /// 0x218
			 0xf125ce31, /// 0x21c
			 0x172d71f5, /// 0x220
			 0xf5ed6355, /// 0x224
			 0xe89c9544, /// 0x228
			 0x1cf88bfb, /// 0x22c
			 0x54d9786e, /// 0x230
			 0xc5c9ac8d, /// 0x234
			 0xf81fcbf4, /// 0x238
			 0x7f395e7b, /// 0x23c
			 0x27806e22, /// 0x240
			 0x6c06c758, /// 0x244
			 0xd14797b1, /// 0x248
			 0x672350ed, /// 0x24c
			 0xa1b8434b, /// 0x250
			 0x802aeca2, /// 0x254
			 0xb54996d3, /// 0x258
			 0x5663b0fb, /// 0x25c
			 0x89ddbb21, /// 0x260
			 0x4b1482b0, /// 0x264
			 0x19fb491a, /// 0x268
			 0xe224da45, /// 0x26c
			 0xd1c3395e, /// 0x270
			 0xa131e1be, /// 0x274
			 0xb48c5aed, /// 0x278
			 0x88d1c231, /// 0x27c
			 0xda5b158a, /// 0x280
			 0x1480709a, /// 0x284
			 0xc4d7e61c, /// 0x288
			 0x065d76ae, /// 0x28c
			 0xaf8441da, /// 0x290
			 0x2bd1f0c0, /// 0x294
			 0x5be59525, /// 0x298
			 0xa924e4a3, /// 0x29c
			 0x417953e2, /// 0x2a0
			 0xdd4b90da, /// 0x2a4
			 0xbbcfb82f, /// 0x2a8
			 0x05f9886b, /// 0x2ac
			 0xce6cf02d, /// 0x2b0
			 0xb23970de, /// 0x2b4
			 0xf3b638c7, /// 0x2b8
			 0x6e8a8885, /// 0x2bc
			 0xaa374e47, /// 0x2c0
			 0xa0903cec, /// 0x2c4
			 0x46a3e463, /// 0x2c8
			 0x838bebe9, /// 0x2cc
			 0xc184eabe, /// 0x2d0
			 0x2588fcde, /// 0x2d4
			 0xd1a19b73, /// 0x2d8
			 0x075736f0, /// 0x2dc
			 0x3c7a4ddb, /// 0x2e0
			 0x10075c9b, /// 0x2e4
			 0x8ed598f9, /// 0x2e8
			 0x9aff15bf, /// 0x2ec
			 0x44c55e62, /// 0x2f0
			 0x9e8f031c, /// 0x2f4
			 0x96b0f2e2, /// 0x2f8
			 0x81e60d32, /// 0x2fc
			 0xccccac44, /// 0x300
			 0xf14467e7, /// 0x304
			 0x06d33831, /// 0x308
			 0x061716b6, /// 0x30c
			 0xdd7af6d5, /// 0x310
			 0xf7b749c0, /// 0x314
			 0xd48b97fa, /// 0x318
			 0x899de57a, /// 0x31c
			 0xd5f55738, /// 0x320
			 0xcd738e1a, /// 0x324
			 0x89831d8c, /// 0x328
			 0xf5741c15, /// 0x32c
			 0x3a9bcdf3, /// 0x330
			 0x2ae96642, /// 0x334
			 0xfe6e0657, /// 0x338
			 0x1fd9a325, /// 0x33c
			 0x0d7241ad, /// 0x340
			 0x94256c18, /// 0x344
			 0xe7932a2f, /// 0x348
			 0x87928f55, /// 0x34c
			 0xe0ffd0ef, /// 0x350
			 0xc5463d6a, /// 0x354
			 0x0d35d9e8, /// 0x358
			 0x8b28a9f1, /// 0x35c
			 0xe2ad844f, /// 0x360
			 0xaf6ba8a3, /// 0x364
			 0xf520f0d9, /// 0x368
			 0x81968b00, /// 0x36c
			 0x67c3f915, /// 0x370
			 0xe6132964, /// 0x374
			 0x9768230d, /// 0x378
			 0xa2e143e3, /// 0x37c
			 0x6bf1d811, /// 0x380
			 0x147cdd85, /// 0x384
			 0x26c2d5fb, /// 0x388
			 0x4d860472, /// 0x38c
			 0x90eea59d, /// 0x390
			 0x1602663a, /// 0x394
			 0xbf78bb87, /// 0x398
			 0x0590dba6, /// 0x39c
			 0x47afb3fb, /// 0x3a0
			 0xa0a5dd8b, /// 0x3a4
			 0xe5a6530b, /// 0x3a8
			 0x195d2926, /// 0x3ac
			 0x5d028926, /// 0x3b0
			 0x46e709d5, /// 0x3b4
			 0x52bf5ca9, /// 0x3b8
			 0xfa664bbf, /// 0x3bc
			 0x2c14b80a, /// 0x3c0
			 0xfa0647bd, /// 0x3c4
			 0x4bf74328, /// 0x3c8
			 0x36e188d1, /// 0x3cc
			 0x6ddfba33, /// 0x3d0
			 0x10e52536, /// 0x3d4
			 0x864d49b9, /// 0x3d8
			 0x098d4e14, /// 0x3dc
			 0xaa1c5952, /// 0x3e0
			 0x72456198, /// 0x3e4
			 0x865ee60f, /// 0x3e8
			 0x7133e878, /// 0x3ec
			 0xf8e4112f, /// 0x3f0
			 0x4a7be39f, /// 0x3f4
			 0x2afb3503, /// 0x3f8
			 0x5a2af37f, /// 0x3fc
			 0x08799667, /// 0x400
			 0x94c48f96, /// 0x404
			 0xbe2e29a8, /// 0x408
			 0x6acbacbb, /// 0x40c
			 0x8b11ee13, /// 0x410
			 0x3a3497d7, /// 0x414
			 0x709ef0a2, /// 0x418
			 0x2d35c43c, /// 0x41c
			 0xce085bc8, /// 0x420
			 0x239e8336, /// 0x424
			 0x90e0180b, /// 0x428
			 0xbed1d7d0, /// 0x42c
			 0xcc86aaa5, /// 0x430
			 0x9f92ba27, /// 0x434
			 0xda3f4bc9, /// 0x438
			 0x2c50fc9e, /// 0x43c
			 0xf5a7dd8d, /// 0x440
			 0xd5d3655f, /// 0x444
			 0x76284b70, /// 0x448
			 0x3477b881, /// 0x44c
			 0x7005517a, /// 0x450
			 0x4c415081, /// 0x454
			 0xaa00d972, /// 0x458
			 0x4b262761, /// 0x45c
			 0xdc8b8ebe, /// 0x460
			 0x0d969d58, /// 0x464
			 0x24aba035, /// 0x468
			 0xf11a6388, /// 0x46c
			 0x20912ef7, /// 0x470
			 0xa787d3f1, /// 0x474
			 0x1d51b923, /// 0x478
			 0x138b4e2a, /// 0x47c
			 0x0db87d57, /// 0x480
			 0x19a01eb9, /// 0x484
			 0x2ecce5a5, /// 0x488
			 0xee206df7, /// 0x48c
			 0x0b18d7a9, /// 0x490
			 0xaef83479, /// 0x494
			 0xff044b2f, /// 0x498
			 0xf178dbec, /// 0x49c
			 0xcd836136, /// 0x4a0
			 0x0e85abcb, /// 0x4a4
			 0x5ed710a9, /// 0x4a8
			 0x03e7096e, /// 0x4ac
			 0xc04590be, /// 0x4b0
			 0xee349111, /// 0x4b4
			 0x2a608b29, /// 0x4b8
			 0xbe8fae66, /// 0x4bc
			 0x378429d2, /// 0x4c0
			 0x91ec4fc7, /// 0x4c4
			 0x054f6838, /// 0x4c8
			 0xc2d0eb1b, /// 0x4cc
			 0xb7eee655, /// 0x4d0
			 0x73a67585, /// 0x4d4
			 0xdeaddfc7, /// 0x4d8
			 0xb50828f8, /// 0x4dc
			 0xcf2b0a62, /// 0x4e0
			 0x6405c094, /// 0x4e4
			 0x4f457c9e, /// 0x4e8
			 0x085d9bcc, /// 0x4ec
			 0x156fc953, /// 0x4f0
			 0x31ab9d5c, /// 0x4f4
			 0xbead0c8e, /// 0x4f8
			 0x6545266c, /// 0x4fc
			 0x3d7ca297, /// 0x500
			 0xaa1b8781, /// 0x504
			 0xe47008c9, /// 0x508
			 0xae5e1eba, /// 0x50c
			 0xf6a14d4c, /// 0x510
			 0x3c00c50d, /// 0x514
			 0xc50d64fc, /// 0x518
			 0x6f276168, /// 0x51c
			 0x16e5cfed, /// 0x520
			 0x826d1092, /// 0x524
			 0x5b8bf4c4, /// 0x528
			 0x502f32a2, /// 0x52c
			 0x5e02abfe, /// 0x530
			 0x7d668c80, /// 0x534
			 0x388123c6, /// 0x538
			 0x18271b73, /// 0x53c
			 0x937751b2, /// 0x540
			 0x8848bb49, /// 0x544
			 0xef5622ff, /// 0x548
			 0x83d8bf54, /// 0x54c
			 0x8b801d67, /// 0x550
			 0xc45cd540, /// 0x554
			 0x1533ccf8, /// 0x558
			 0xd4603819, /// 0x55c
			 0x4966a9a8, /// 0x560
			 0x4056e339, /// 0x564
			 0xf4902d94, /// 0x568
			 0x47ae1744, /// 0x56c
			 0x06f90b79, /// 0x570
			 0xb8aeaada, /// 0x574
			 0x37984583, /// 0x578
			 0xaf8a9c57, /// 0x57c
			 0xd5907942, /// 0x580
			 0xd400691f, /// 0x584
			 0xdcc665cd, /// 0x588
			 0x99b539e0, /// 0x58c
			 0x58870f61, /// 0x590
			 0x79e40e6c, /// 0x594
			 0x6aa77687, /// 0x598
			 0x89c6131b, /// 0x59c
			 0x74db0245, /// 0x5a0
			 0xb9c3c825, /// 0x5a4
			 0x08d4b999, /// 0x5a8
			 0x5dd2c5fc, /// 0x5ac
			 0x2c263e72, /// 0x5b0
			 0x764fbf9a, /// 0x5b4
			 0xb4f188b6, /// 0x5b8
			 0xc4919ab6, /// 0x5bc
			 0xbaa47240, /// 0x5c0
			 0x58177116, /// 0x5c4
			 0x9a38e8f0, /// 0x5c8
			 0x2dbf9c23, /// 0x5cc
			 0xac39f442, /// 0x5d0
			 0x811bffb3, /// 0x5d4
			 0x19776af4, /// 0x5d8
			 0x96c71d04, /// 0x5dc
			 0x98e749bd, /// 0x5e0
			 0x3f3f8db5, /// 0x5e4
			 0x974ac858, /// 0x5e8
			 0xf20a6d98, /// 0x5ec
			 0xec36b7af, /// 0x5f0
			 0x72b396a1, /// 0x5f4
			 0x2017604a, /// 0x5f8
			 0x5adb5890, /// 0x5fc
			 0x8bce144f, /// 0x600
			 0x433c60ef, /// 0x604
			 0x0631f477, /// 0x608
			 0xe26b04df, /// 0x60c
			 0xe2ba4123, /// 0x610
			 0x370f8638, /// 0x614
			 0xf15668f9, /// 0x618
			 0x8f7b6965, /// 0x61c
			 0xdbf96dea, /// 0x620
			 0xac1ffb64, /// 0x624
			 0xb7bbbbe7, /// 0x628
			 0xc5da0bd2, /// 0x62c
			 0xca0f43e2, /// 0x630
			 0x0dce6e0a, /// 0x634
			 0x4eded3f4, /// 0x638
			 0x95678dea, /// 0x63c
			 0xb9e8952f, /// 0x640
			 0x74f05813, /// 0x644
			 0x2a53ad47, /// 0x648
			 0x680fe367, /// 0x64c
			 0x524233a0, /// 0x650
			 0xe472f1dd, /// 0x654
			 0xcc934aa2, /// 0x658
			 0x65fc1dd1, /// 0x65c
			 0xc14009b5, /// 0x660
			 0xa0a26625, /// 0x664
			 0x03e9533b, /// 0x668
			 0xa0320ea5, /// 0x66c
			 0xa9f717a5, /// 0x670
			 0x4547c482, /// 0x674
			 0x8a54b44c, /// 0x678
			 0x336923e3, /// 0x67c
			 0x85713276, /// 0x680
			 0x59d7a552, /// 0x684
			 0x5cc19448, /// 0x688
			 0xe833ad59, /// 0x68c
			 0xf50950fe, /// 0x690
			 0x737eb261, /// 0x694
			 0xe6f1ea0a, /// 0x698
			 0x4e468583, /// 0x69c
			 0x8fc20bb1, /// 0x6a0
			 0xae498d6b, /// 0x6a4
			 0x6a7f7cb3, /// 0x6a8
			 0x82ffa1d5, /// 0x6ac
			 0x00345904, /// 0x6b0
			 0x98818a18, /// 0x6b4
			 0xe01afcde, /// 0x6b8
			 0xbca0f113, /// 0x6bc
			 0x7013c73c, /// 0x6c0
			 0x9b4e5ceb, /// 0x6c4
			 0xc22575bd, /// 0x6c8
			 0x43f80335, /// 0x6cc
			 0x8db306ed, /// 0x6d0
			 0x55218a4f, /// 0x6d4
			 0xa59421b3, /// 0x6d8
			 0x9227705c, /// 0x6dc
			 0x9557b681, /// 0x6e0
			 0x672d25ae, /// 0x6e4
			 0x904e1ccf, /// 0x6e8
			 0xbe36b04f, /// 0x6ec
			 0x2616e1ed, /// 0x6f0
			 0x75d807d7, /// 0x6f4
			 0x2b85787d, /// 0x6f8
			 0x3ed074db, /// 0x6fc
			 0x7e91caab, /// 0x700
			 0x81f637d2, /// 0x704
			 0x1a54b263, /// 0x708
			 0x8f700f61, /// 0x70c
			 0xc630deb7, /// 0x710
			 0x6b797650, /// 0x714
			 0xa613e50c, /// 0x718
			 0x343c9f50, /// 0x71c
			 0x8a697327, /// 0x720
			 0xf1ebf772, /// 0x724
			 0x82718e30, /// 0x728
			 0x4587adb2, /// 0x72c
			 0xe7a5592c, /// 0x730
			 0xb2dba172, /// 0x734
			 0x23a7cd20, /// 0x738
			 0xe9af1e50, /// 0x73c
			 0xa142ea1a, /// 0x740
			 0x8b27803a, /// 0x744
			 0xe5d12ac0, /// 0x748
			 0x7a4a7e27, /// 0x74c
			 0xae144d12, /// 0x750
			 0x0296c80a, /// 0x754
			 0xeef02d39, /// 0x758
			 0x2ab83be6, /// 0x75c
			 0x36296439, /// 0x760
			 0x79741ff3, /// 0x764
			 0x5dd59f78, /// 0x768
			 0x97987879, /// 0x76c
			 0x48f4b129, /// 0x770
			 0x7ba1e012, /// 0x774
			 0x991c9d44, /// 0x778
			 0x6581b02f, /// 0x77c
			 0x8cf04ec7, /// 0x780
			 0x34a1816f, /// 0x784
			 0xe418829c, /// 0x788
			 0xceb876df, /// 0x78c
			 0x053f1e92, /// 0x790
			 0xeb6ae8dc, /// 0x794
			 0x4c02c92d, /// 0x798
			 0x0f735776, /// 0x79c
			 0x6e220a47, /// 0x7a0
			 0xa370f85a, /// 0x7a4
			 0x2080f730, /// 0x7a8
			 0xef88aff3, /// 0x7ac
			 0x92da929e, /// 0x7b0
			 0x10cde115, /// 0x7b4
			 0xaff1a8ea, /// 0x7b8
			 0xfd06c7d6, /// 0x7bc
			 0x54a274d2, /// 0x7c0
			 0xf3a41013, /// 0x7c4
			 0x90ca5d8a, /// 0x7c8
			 0xdb428b8f, /// 0x7cc
			 0x9bb1b28d, /// 0x7d0
			 0xeeb78137, /// 0x7d4
			 0x435f96b7, /// 0x7d8
			 0x2ce8f272, /// 0x7dc
			 0xfeda5981, /// 0x7e0
			 0xf5f9e430, /// 0x7e4
			 0xfb9c770b, /// 0x7e8
			 0xaf66caa2, /// 0x7ec
			 0xeff7f34c, /// 0x7f0
			 0xa86b0af3, /// 0x7f4
			 0x4a15c45d, /// 0x7f8
			 0x0cf6d1ec, /// 0x7fc
			 0xdc154dea, /// 0x800
			 0x736f4b42, /// 0x804
			 0xfac28099, /// 0x808
			 0xc89861c3, /// 0x80c
			 0xa1a8f13c, /// 0x810
			 0xa0f35b58, /// 0x814
			 0x03631029, /// 0x818
			 0x1748309b, /// 0x81c
			 0xa5c3c346, /// 0x820
			 0x014cd72c, /// 0x824
			 0xf6c4af0c, /// 0x828
			 0x96d2a5fc, /// 0x82c
			 0x96ed885c, /// 0x830
			 0x89a041b0, /// 0x834
			 0x155eb1dd, /// 0x838
			 0xa2a7eda2, /// 0x83c
			 0xae039ad1, /// 0x840
			 0x3efd92be, /// 0x844
			 0x195a61e0, /// 0x848
			 0x4300072b, /// 0x84c
			 0x576c3b2d, /// 0x850
			 0x1dc1e9c6, /// 0x854
			 0x1f161202, /// 0x858
			 0x5eb6778e, /// 0x85c
			 0x932aaa18, /// 0x860
			 0x13616504, /// 0x864
			 0x2485d780, /// 0x868
			 0xefdcac11, /// 0x86c
			 0xba5515be, /// 0x870
			 0x9af5da48, /// 0x874
			 0x16c4fd40, /// 0x878
			 0xfe1bcdfd, /// 0x87c
			 0x1c25be72, /// 0x880
			 0x7cd62acc, /// 0x884
			 0x26162ba1, /// 0x888
			 0xf1cfbd96, /// 0x88c
			 0x04a3e896, /// 0x890
			 0xc6cbdd4a, /// 0x894
			 0xc571d022, /// 0x898
			 0xcac1f62f, /// 0x89c
			 0x4b6d224a, /// 0x8a0
			 0x25fe13c6, /// 0x8a4
			 0x71af0e83, /// 0x8a8
			 0x3daa57cf, /// 0x8ac
			 0xf5843577, /// 0x8b0
			 0xc910a3c1, /// 0x8b4
			 0x15c35724, /// 0x8b8
			 0x70b445b7, /// 0x8bc
			 0x6c62c16e, /// 0x8c0
			 0xeaac7d10, /// 0x8c4
			 0xfec86a26, /// 0x8c8
			 0x76c7bb9f, /// 0x8cc
			 0x6e680f6a, /// 0x8d0
			 0xfa5ded6e, /// 0x8d4
			 0xa1e8e406, /// 0x8d8
			 0x6844e958, /// 0x8dc
			 0xdb157535, /// 0x8e0
			 0x6f255383, /// 0x8e4
			 0xb90480b0, /// 0x8e8
			 0xe1602e92, /// 0x8ec
			 0x06827f73, /// 0x8f0
			 0xaf8450bc, /// 0x8f4
			 0x092df409, /// 0x8f8
			 0xcf0a4a3b, /// 0x8fc
			 0xec1d7e14, /// 0x900
			 0x9b37d416, /// 0x904
			 0x57729470, /// 0x908
			 0x57f12ce8, /// 0x90c
			 0x28067f21, /// 0x910
			 0x4128313a, /// 0x914
			 0x193ba75c, /// 0x918
			 0xea8ebe80, /// 0x91c
			 0x5a7cc0a2, /// 0x920
			 0xc9017ae5, /// 0x924
			 0x91fc0afb, /// 0x928
			 0xafc4a2ac, /// 0x92c
			 0xa830bfa9, /// 0x930
			 0x6bd65f3d, /// 0x934
			 0xa534a90c, /// 0x938
			 0xf2bd1105, /// 0x93c
			 0x11c8f451, /// 0x940
			 0x403767bb, /// 0x944
			 0x487921a3, /// 0x948
			 0xa768d65e, /// 0x94c
			 0x65bfe54f, /// 0x950
			 0x1664ccf3, /// 0x954
			 0x61d445da, /// 0x958
			 0x7375cf2c, /// 0x95c
			 0xe27942bd, /// 0x960
			 0x45a82dd2, /// 0x964
			 0x1b186b42, /// 0x968
			 0x47acfa31, /// 0x96c
			 0x31ad404c, /// 0x970
			 0x1cda0bcd, /// 0x974
			 0x27827272, /// 0x978
			 0x605a0568, /// 0x97c
			 0xac6f2e78, /// 0x980
			 0xeb4599ba, /// 0x984
			 0x2860342a, /// 0x988
			 0x7e96bc51, /// 0x98c
			 0xe55f4c75, /// 0x990
			 0xb744ede2, /// 0x994
			 0xab4c750b, /// 0x998
			 0xb1bd4241, /// 0x99c
			 0x2c67b6f8, /// 0x9a0
			 0x24e8df89, /// 0x9a4
			 0xaabb2fbd, /// 0x9a8
			 0xb13f153e, /// 0x9ac
			 0x72245cfe, /// 0x9b0
			 0x677a2059, /// 0x9b4
			 0xae485ecd, /// 0x9b8
			 0x37e60e4a, /// 0x9bc
			 0x9087f4be, /// 0x9c0
			 0x5cb41eb7, /// 0x9c4
			 0xde71d28c, /// 0x9c8
			 0x7150896e, /// 0x9cc
			 0xd715024d, /// 0x9d0
			 0xcee49e32, /// 0x9d4
			 0x35c1fed5, /// 0x9d8
			 0xa9be767b, /// 0x9dc
			 0x5eb0756f, /// 0x9e0
			 0xf6f6aec7, /// 0x9e4
			 0x22303e55, /// 0x9e8
			 0xc3fe24fb, /// 0x9ec
			 0x992aa984, /// 0x9f0
			 0xdf5729e1, /// 0x9f4
			 0x7959d84c, /// 0x9f8
			 0xb122d8aa, /// 0x9fc
			 0x2ea6aead, /// 0xa00
			 0x88d20b3b, /// 0xa04
			 0x28e46eec, /// 0xa08
			 0x58862ad9, /// 0xa0c
			 0x3f237918, /// 0xa10
			 0x11b0d454, /// 0xa14
			 0xb9c01f67, /// 0xa18
			 0x33702f07, /// 0xa1c
			 0xb4dd5a24, /// 0xa20
			 0x88e91cf1, /// 0xa24
			 0xb519d746, /// 0xa28
			 0xde06c7d2, /// 0xa2c
			 0x9739b4f9, /// 0xa30
			 0x0b018c81, /// 0xa34
			 0xc3b3702d, /// 0xa38
			 0x1c8f0f02, /// 0xa3c
			 0xf5ecf491, /// 0xa40
			 0xf14d22ef, /// 0xa44
			 0xed898faf, /// 0xa48
			 0xab47547f, /// 0xa4c
			 0xf3da82dd, /// 0xa50
			 0xcde8b106, /// 0xa54
			 0xb6c71abd, /// 0xa58
			 0x3407b5f4, /// 0xa5c
			 0x8dba373e, /// 0xa60
			 0xf51ac5d0, /// 0xa64
			 0xc875ee54, /// 0xa68
			 0xebba83ad, /// 0xa6c
			 0x4916a103, /// 0xa70
			 0x2b51bb0e, /// 0xa74
			 0x16466045, /// 0xa78
			 0xe61c7da3, /// 0xa7c
			 0x28d054fe, /// 0xa80
			 0x46669db9, /// 0xa84
			 0xc47b0d10, /// 0xa88
			 0x61b247e8, /// 0xa8c
			 0x906229f3, /// 0xa90
			 0xa66010d5, /// 0xa94
			 0xf3d1879c, /// 0xa98
			 0x3b79a90f, /// 0xa9c
			 0x2e7b5563, /// 0xaa0
			 0xd1e5e2d6, /// 0xaa4
			 0x10401014, /// 0xaa8
			 0x2701464d, /// 0xaac
			 0x6dfe3f04, /// 0xab0
			 0xbf1ab96f, /// 0xab4
			 0xbb9fc388, /// 0xab8
			 0x828e63b8, /// 0xabc
			 0xf44fca40, /// 0xac0
			 0x95ce66ea, /// 0xac4
			 0xedbc4863, /// 0xac8
			 0xba5269b4, /// 0xacc
			 0xdf6d0f92, /// 0xad0
			 0xca5e2316, /// 0xad4
			 0x43f7b46d, /// 0xad8
			 0xb5d77301, /// 0xadc
			 0xf56df351, /// 0xae0
			 0xe8d9eaef, /// 0xae4
			 0x2e884ee0, /// 0xae8
			 0x415f95a6, /// 0xaec
			 0x6c503e3f, /// 0xaf0
			 0x1ac87c51, /// 0xaf4
			 0x84e02a43, /// 0xaf8
			 0x394d6bd4, /// 0xafc
			 0x579446c1, /// 0xb00
			 0xb144e62c, /// 0xb04
			 0x5b6af07a, /// 0xb08
			 0x0961c618, /// 0xb0c
			 0x97fdcd15, /// 0xb10
			 0xcf9ac675, /// 0xb14
			 0xdc7f3950, /// 0xb18
			 0x4ce4ae60, /// 0xb1c
			 0x6bffc7e6, /// 0xb20
			 0xc583ae43, /// 0xb24
			 0xb32515e1, /// 0xb28
			 0x2af1c5c5, /// 0xb2c
			 0xba12839f, /// 0xb30
			 0x3c26e11d, /// 0xb34
			 0xe9fa5bfb, /// 0xb38
			 0x55237a24, /// 0xb3c
			 0x50a0d535, /// 0xb40
			 0x9ceab8e4, /// 0xb44
			 0xf02b54b4, /// 0xb48
			 0x88a23802, /// 0xb4c
			 0xed682d2d, /// 0xb50
			 0xe4930039, /// 0xb54
			 0xecfa1a9e, /// 0xb58
			 0xa386b195, /// 0xb5c
			 0xcfaba4b4, /// 0xb60
			 0xca234169, /// 0xb64
			 0x341c9d15, /// 0xb68
			 0xa3e66fde, /// 0xb6c
			 0xb7945506, /// 0xb70
			 0xae058aa4, /// 0xb74
			 0x3a25f9fc, /// 0xb78
			 0xed2170d1, /// 0xb7c
			 0xb7afcba5, /// 0xb80
			 0xe050762f, /// 0xb84
			 0x4cf3c048, /// 0xb88
			 0xfe278e12, /// 0xb8c
			 0xdea7f396, /// 0xb90
			 0x3251f3c0, /// 0xb94
			 0xe72803df, /// 0xb98
			 0x7d0acfe7, /// 0xb9c
			 0x56b7ad60, /// 0xba0
			 0xb9ec7ef4, /// 0xba4
			 0x853dd12e, /// 0xba8
			 0xc02e7ea3, /// 0xbac
			 0x45b3087a, /// 0xbb0
			 0x112f0099, /// 0xbb4
			 0x9443f34a, /// 0xbb8
			 0xadf26a82, /// 0xbbc
			 0x45ce9a1e, /// 0xbc0
			 0x05c3982a, /// 0xbc4
			 0x469bf391, /// 0xbc8
			 0x6f370981, /// 0xbcc
			 0xfc3f1380, /// 0xbd0
			 0x0491739e, /// 0xbd4
			 0x7734a8e2, /// 0xbd8
			 0x325ac784, /// 0xbdc
			 0xec04779f, /// 0xbe0
			 0x0a408454, /// 0xbe4
			 0x2c964616, /// 0xbe8
			 0xa4a3968c, /// 0xbec
			 0x8fe473b2, /// 0xbf0
			 0x6edb9c8b, /// 0xbf4
			 0xbf1fce5e, /// 0xbf8
			 0x6ed54b9f, /// 0xbfc
			 0xaf3a2413, /// 0xc00
			 0x1a9b972a, /// 0xc04
			 0x0efea671, /// 0xc08
			 0x5ef646f6, /// 0xc0c
			 0x5d0cfd01, /// 0xc10
			 0xf564923e, /// 0xc14
			 0x04080554, /// 0xc18
			 0x20654fef, /// 0xc1c
			 0x396dd60d, /// 0xc20
			 0x3ffe8f9c, /// 0xc24
			 0x5a545c84, /// 0xc28
			 0x6e9a5be6, /// 0xc2c
			 0xbe9ce263, /// 0xc30
			 0xf599d078, /// 0xc34
			 0xc1b9a227, /// 0xc38
			 0x1dc01cab, /// 0xc3c
			 0xf8fb8fa9, /// 0xc40
			 0x1d8be869, /// 0xc44
			 0x4c1d413d, /// 0xc48
			 0x18d3b8dd, /// 0xc4c
			 0x27197f1a, /// 0xc50
			 0x0839284d, /// 0xc54
			 0xb9cb3a2f, /// 0xc58
			 0x7242119c, /// 0xc5c
			 0xf458415c, /// 0xc60
			 0xff52a474, /// 0xc64
			 0x1b7fae91, /// 0xc68
			 0xfbe3c2c6, /// 0xc6c
			 0x4708ee4f, /// 0xc70
			 0x81d044dc, /// 0xc74
			 0xa5e7605a, /// 0xc78
			 0xcab13c9d, /// 0xc7c
			 0xb6ad04c4, /// 0xc80
			 0x4c7053db, /// 0xc84
			 0x108b8e68, /// 0xc88
			 0x1c3167a7, /// 0xc8c
			 0x679b6fb5, /// 0xc90
			 0xc584376f, /// 0xc94
			 0x1bdca76b, /// 0xc98
			 0xb2a606ed, /// 0xc9c
			 0x4c3c1121, /// 0xca0
			 0x0f864e92, /// 0xca4
			 0xe6ccdffd, /// 0xca8
			 0x32cf8ccc, /// 0xcac
			 0xfcf6a476, /// 0xcb0
			 0x5cd7ede5, /// 0xcb4
			 0x7211028e, /// 0xcb8
			 0x7c14e975, /// 0xcbc
			 0x98a7a989, /// 0xcc0
			 0xab80ac33, /// 0xcc4
			 0xb9b567d9, /// 0xcc8
			 0xaba22609, /// 0xccc
			 0xb8c91633, /// 0xcd0
			 0xc45ecd60, /// 0xcd4
			 0x97216fa1, /// 0xcd8
			 0x8df7c793, /// 0xcdc
			 0x794b1b98, /// 0xce0
			 0x04e68cbb, /// 0xce4
			 0x85fa9d26, /// 0xce8
			 0x57ef3435, /// 0xcec
			 0xd7f4e62e, /// 0xcf0
			 0xcb107ef7, /// 0xcf4
			 0x2ab54cca, /// 0xcf8
			 0xb438d576, /// 0xcfc
			 0x251d4ea5, /// 0xd00
			 0x3cdb9799, /// 0xd04
			 0x52442642, /// 0xd08
			 0x9e3ffc1c, /// 0xd0c
			 0x45d43e15, /// 0xd10
			 0x934c4540, /// 0xd14
			 0xe5d71e6c, /// 0xd18
			 0xf4c9a8ea, /// 0xd1c
			 0x003be5d4, /// 0xd20
			 0x54415da9, /// 0xd24
			 0x4cc4d4c7, /// 0xd28
			 0xc9aafa5e, /// 0xd2c
			 0x3484682a, /// 0xd30
			 0xb28b6d15, /// 0xd34
			 0x085e85dd, /// 0xd38
			 0xb6de346f, /// 0xd3c
			 0xc57b5348, /// 0xd40
			 0xd161013a, /// 0xd44
			 0x1a966495, /// 0xd48
			 0xb20fc43e, /// 0xd4c
			 0x621ce9b2, /// 0xd50
			 0xcd31ff73, /// 0xd54
			 0xfa642a30, /// 0xd58
			 0xc36154f5, /// 0xd5c
			 0x58e49616, /// 0xd60
			 0x6922cce2, /// 0xd64
			 0xd6cc23e7, /// 0xd68
			 0xfa903036, /// 0xd6c
			 0xfbf136c5, /// 0xd70
			 0x58ce98c9, /// 0xd74
			 0xb0d18d3b, /// 0xd78
			 0x7005729e, /// 0xd7c
			 0x85a01a50, /// 0xd80
			 0x9faade2b, /// 0xd84
			 0xe7375250, /// 0xd88
			 0xe4518521, /// 0xd8c
			 0x070f6a75, /// 0xd90
			 0x5389ede5, /// 0xd94
			 0x281962a3, /// 0xd98
			 0x10b3cbcb, /// 0xd9c
			 0x17063cd9, /// 0xda0
			 0xd9bdf090, /// 0xda4
			 0xf202a864, /// 0xda8
			 0x8763599d, /// 0xdac
			 0x6ff173bd, /// 0xdb0
			 0xd10bf8b1, /// 0xdb4
			 0x5daefb2b, /// 0xdb8
			 0xd8759a6b, /// 0xdbc
			 0x26495f6e, /// 0xdc0
			 0x12e21037, /// 0xdc4
			 0x0a14c07e, /// 0xdc8
			 0x468a9cbf, /// 0xdcc
			 0x4037d0f6, /// 0xdd0
			 0x45bcf4bf, /// 0xdd4
			 0x556ccc46, /// 0xdd8
			 0x61321987, /// 0xddc
			 0x5b8b1ebc, /// 0xde0
			 0x57255d4c, /// 0xde4
			 0x3c22282b, /// 0xde8
			 0x92363c3d, /// 0xdec
			 0x98842edf, /// 0xdf0
			 0x45b846af, /// 0xdf4
			 0xd8bad15a, /// 0xdf8
			 0x65083ccb, /// 0xdfc
			 0x834ba2ef, /// 0xe00
			 0xd29ec563, /// 0xe04
			 0xea894541, /// 0xe08
			 0x3e69d47d, /// 0xe0c
			 0x8aa8d69b, /// 0xe10
			 0xe095b82f, /// 0xe14
			 0x3e4ee43a, /// 0xe18
			 0x8b3fd5eb, /// 0xe1c
			 0x2f9e505b, /// 0xe20
			 0xab8c4387, /// 0xe24
			 0x8d2d8ceb, /// 0xe28
			 0x71bcc8bf, /// 0xe2c
			 0x6722b34b, /// 0xe30
			 0x8033780b, /// 0xe34
			 0x5cb8b5fc, /// 0xe38
			 0x600c29ed, /// 0xe3c
			 0xf60ad2fa, /// 0xe40
			 0xec463fdf, /// 0xe44
			 0x47eb2837, /// 0xe48
			 0x503c876b, /// 0xe4c
			 0x1aa6af60, /// 0xe50
			 0x9daae8f6, /// 0xe54
			 0xc85c9704, /// 0xe58
			 0x54f9a770, /// 0xe5c
			 0x9d0b8e79, /// 0xe60
			 0x6ff75017, /// 0xe64
			 0xfd740bcd, /// 0xe68
			 0x8c7ccb74, /// 0xe6c
			 0xcc6da207, /// 0xe70
			 0x55acbcf1, /// 0xe74
			 0x6b911d2a, /// 0xe78
			 0xff3bddfa, /// 0xe7c
			 0x80a7ca1f, /// 0xe80
			 0x1167de7f, /// 0xe84
			 0xd3dcd14c, /// 0xe88
			 0x31697860, /// 0xe8c
			 0x72293abb, /// 0xe90
			 0xc21242da, /// 0xe94
			 0xed9822c0, /// 0xe98
			 0x9f2af0b8, /// 0xe9c
			 0xd0f89087, /// 0xea0
			 0xa450ca8d, /// 0xea4
			 0x3e2208ba, /// 0xea8
			 0x95bb8e5b, /// 0xeac
			 0x5e2575e9, /// 0xeb0
			 0x2662cc51, /// 0xeb4
			 0xd059ca9c, /// 0xeb8
			 0xfe8c81cd, /// 0xebc
			 0xa082cb2e, /// 0xec0
			 0x3e282795, /// 0xec4
			 0x1911e4ac, /// 0xec8
			 0xc71fd6b4, /// 0xecc
			 0x8559f913, /// 0xed0
			 0x312a4b1e, /// 0xed4
			 0x7887c6c0, /// 0xed8
			 0xfb27e1f9, /// 0xedc
			 0x25a6cb90, /// 0xee0
			 0xa2ecff2f, /// 0xee4
			 0x785d39a9, /// 0xee8
			 0x08cb6cf8, /// 0xeec
			 0x615a0901, /// 0xef0
			 0x23581b40, /// 0xef4
			 0x3db9e934, /// 0xef8
			 0x3019a611, /// 0xefc
			 0xb8a8fcec, /// 0xf00
			 0xd3075fdb, /// 0xf04
			 0xf473bcec, /// 0xf08
			 0x8307fb81, /// 0xf0c
			 0xfba0bcf2, /// 0xf10
			 0x1feaf720, /// 0xf14
			 0xa867c5cf, /// 0xf18
			 0x8c57f3dd, /// 0xf1c
			 0xba00c3ae, /// 0xf20
			 0xecc32ee2, /// 0xf24
			 0x402874e9, /// 0xf28
			 0x73fd588f, /// 0xf2c
			 0xb6fcb26d, /// 0xf30
			 0x503b22fd, /// 0xf34
			 0x4eceacd4, /// 0xf38
			 0xe8c187a6, /// 0xf3c
			 0x32ae2e08, /// 0xf40
			 0x2f5a66be, /// 0xf44
			 0x4cd8f5b4, /// 0xf48
			 0x9baca37b, /// 0xf4c
			 0x13eb9a80, /// 0xf50
			 0xc6b9eba8, /// 0xf54
			 0xf0233831, /// 0xf58
			 0x544b6585, /// 0xf5c
			 0xcf0f4168, /// 0xf60
			 0xeb7bb0f9, /// 0xf64
			 0x37cbd483, /// 0xf68
			 0x17ae8c9c, /// 0xf6c
			 0x4c159978, /// 0xf70
			 0x3ea30452, /// 0xf74
			 0xe65b9db4, /// 0xf78
			 0x61a646db, /// 0xf7c
			 0xc758d599, /// 0xf80
			 0xd7fbef2a, /// 0xf84
			 0x0966b402, /// 0xf88
			 0x2885101a, /// 0xf8c
			 0x3c573379, /// 0xf90
			 0x4fcb77cc, /// 0xf94
			 0x551f8943, /// 0xf98
			 0x05de81c9, /// 0xf9c
			 0x37635fa3, /// 0xfa0
			 0x868ab592, /// 0xfa4
			 0x5b4deb05, /// 0xfa8
			 0x25ed00c1, /// 0xfac
			 0x066fbe3f, /// 0xfb0
			 0x367c0c9a, /// 0xfb4
			 0x522e45fe, /// 0xfb8
			 0xccafff41, /// 0xfbc
			 0x56d6d807, /// 0xfc0
			 0xa85c56aa, /// 0xfc4
			 0x5f598015, /// 0xfc8
			 0x5c4a8832, /// 0xfcc
			 0x3835a294, /// 0xfd0
			 0x9b378e9d, /// 0xfd4
			 0x6aa85919, /// 0xfd8
			 0x328c28ab, /// 0xfdc
			 0x0741e5c8, /// 0xfe0
			 0xfb3775fb, /// 0xfe4
			 0x03d70573, /// 0xfe8
			 0x9d33e2e3, /// 0xfec
			 0x58f041f3, /// 0xff0
			 0x95ac85ff, /// 0xff4
			 0x711a7e8b, /// 0xff8
			 0x62858d09 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00000
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00004
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00008
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0000c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00018
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0001c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00020
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00024
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00028
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00030
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00038
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0003c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00040
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00044
			 0x7f800000,                                                  // inf                                         /// 00048
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00050
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00054
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00058
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0005c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00060
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00068
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00070
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00074
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0007c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0008c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00090
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0xffc00001,                                                  // -signaling NaN                              /// 000e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00118
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00120
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00130
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00000000,                                                  // zero                                        /// 0013c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00144
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00148
			 0x0e000007,                                                  // Trailing 1s:                                /// 0014c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00150
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00154
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00158
			 0x0e000001,                                                  // Trailing 1s:                                /// 0015c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0xffc00001,                                                  // -signaling NaN                              /// 00164
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00168
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0016c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00174
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00178
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0017c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00184
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0018c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00190
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000000,                                                  // zero                                        /// 0019c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00200
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00204
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00208
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xcb000000,                                                  // -8388608.0                                  /// 00210
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00214
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0021c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00230
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00240
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00244
			 0x0c700000,                                                  // Leading 1s:                                 /// 00248
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00250
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00258
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00260
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00264
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00268
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0026c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00278
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x55555555,                                                  // 4 random values                             /// 00284
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00288
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0028c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00294
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00298
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0029c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002ec
			 0x4b000000,                                                  // 8388608.0                                   /// 002f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00300
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00308
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0030c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00310
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00314
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00320
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00330
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00334
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00338
			 0x00011111,                                                  // 9.7958E-41                                  /// 0033c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00340
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00344
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00348
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0034c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00350
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00358
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00360
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00364
			 0xbf028f5c,                                                  // -0.51                                       /// 00368
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0036c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00370
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00374
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0037c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00380
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00384
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00388
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00398
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xbf028f5c,                                                  // -0.51                                       /// 003d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 003f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00400
			 0x3f028f5c,                                                  // 0.51                                        /// 00404
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00408
			 0x0c400000,                                                  // Leading 1s:                                 /// 0040c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00410
			 0x00011111,                                                  // 9.7958E-41                                  /// 00414
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00418
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0041c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00420
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00424
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00428
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00430
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00434
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00438
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00444
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00450
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0045c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00464
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0046c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00470
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00474
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00478
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0047c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00484
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00488
			 0x00000000,                                                  // zero                                        /// 0048c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00494
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00498
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004a0
			 0xffc00001,                                                  // -signaling NaN                              /// 004a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f8
			 0x55555555,                                                  // 4 random values                             /// 004fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00500
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00504
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00508
			 0x3f028f5c,                                                  // 0.51                                        /// 0050c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00510
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00514
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00518
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00524
			 0x3f028f5c,                                                  // 0.51                                        /// 00528
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0052c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00530
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00534
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x7f800000,                                                  // inf                                         /// 0053c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00540
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00544
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00554
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00558
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x0c780000,                                                  // Leading 1s:                                 /// 00564
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00568
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0056c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00574
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00578
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00580
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00588
			 0x3f028f5c,                                                  // 0.51                                        /// 0058c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00594
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005d8
			 0x55555555,                                                  // 4 random values                             /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e8
			 0xffc00001,                                                  // -signaling NaN                              /// 005ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00604
			 0x33333333,                                                  // 4 random values                             /// 00608
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0060c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00610
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00614
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00624
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0062c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00630
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e000001,                                                  // Trailing 1s:                                /// 00638
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00640
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00644
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0064c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00654
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0065c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00660
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00664
			 0xbf028f5c,                                                  // -0.51                                       /// 00668
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00674
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0067c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00680
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00684
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0068c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00690
			 0x0e000007,                                                  // Trailing 1s:                                /// 00694
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00698
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x55555555,                                                  // 4 random values                             /// 006a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0xcb000000,                                                  // -8388608.0                                  /// 006b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0xff800000,                                                  // -inf                                        /// 006b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e0
			 0xcb000000,                                                  // -8388608.0                                  /// 006e4
			 0x3f028f5c,                                                  // 0.51                                        /// 006e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00704
			 0x00000000,                                                  // zero                                        /// 00708
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e000007,                                                  // Trailing 1s:                                /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00720
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00724
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00728
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0073c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0074c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00750
			 0xff800000,                                                  // -inf                                        /// 00754
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00764
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00770
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00778
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00780
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00784
			 0x0e000007,                                                  // Trailing 1s:                                /// 00788
			 0x3f028f5c,                                                  // 0.51                                        /// 0078c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00790
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00794
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0079c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00800
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00804
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0080c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00810
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00814
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00818
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0081c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x7fc00001,                                                  // signaling NaN                               /// 00824
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00828
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00830
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00834
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00840
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0084c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00854
			 0xcb000000,                                                  // -8388608.0                                  /// 00858
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0085c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00860
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00868
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00870
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00878
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00880
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00884
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0089c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x4b000000,                                                  // 8388608.0                                   /// 008a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008b0
			 0x7f800000,                                                  // inf                                         /// 008b4
			 0x3f028f5c,                                                  // 0.51                                        /// 008b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00900
			 0x00000000,                                                  // zero                                        /// 00904
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0090c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00910
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00918
			 0x0c700000,                                                  // Leading 1s:                                 /// 0091c
			 0x3f028f5c,                                                  // 0.51                                        /// 00920
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00928
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00930
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00934
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00938
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00940
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00944
			 0x3f028f5c,                                                  // 0.51                                        /// 00948
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00950
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00958
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00960
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00968
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00970
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00978
			 0x33333333,                                                  // 4 random values                             /// 0097c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00980
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00984
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0098c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00990
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00994
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00998
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0099c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d0
			 0x4b000000,                                                  // 8388608.0                                   /// 009d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x7f800000,                                                  // inf                                         /// 00a60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a98
			 0x3f028f5c,                                                  // 0.51                                        /// 00a9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aa4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ac0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00acc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ad0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ad4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x33333333,                                                  // 4 random values                             /// 00aec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00afc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x33333333,                                                  // 4 random values                             /// 00b04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x7f800000,                                                  // inf                                         /// 00b0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b24
			 0xbf028f5c,                                                  // -0.51                                       /// 00b28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b38
			 0xffc00001,                                                  // -signaling NaN                              /// 00b3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0xffc00001,                                                  // -signaling NaN                              /// 00b58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00000000,                                                  // zero                                        /// 00b74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ba4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ba8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bcc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00be4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00be8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bfc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c40
			 0x00000000,                                                  // zero                                        /// 00c44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c6c
			 0xff800000,                                                  // -inf                                        /// 00c70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x7fc00001,                                                  // signaling NaN                               /// 00c84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb0
			 0x80000000,                                                  // -zero                                       /// 00cb4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80000000,                                                  // -zero                                       /// 00cc8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cdc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cf4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cf8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cfc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d14
			 0x33333333,                                                  // 4 random values                             /// 00d18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d30
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d4c
			 0x7f800000,                                                  // inf                                         /// 00d50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d58
			 0x33333333,                                                  // 4 random values                             /// 00d5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d94
			 0x7fc00001,                                                  // signaling NaN                               /// 00d98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00da4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00de0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00df0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e74
			 0x55555555,                                                  // 4 random values                             /// 00e78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x3f028f5c,                                                  // 0.51                                        /// 00e88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ea0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ea8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ec8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ee8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f90
			 0xbf028f5c,                                                  // -0.51                                       /// 00f94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb4
			 0x33333333,                                                  // 4 random values                             /// 00fb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fdc
			 0x7f800000,                                                  // inf                                         /// 00fe0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ff4
			 0x80000000,                                                  // -zero                                       /// 00ff8
			 0x00040000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xdb932c15, /// 0x0
			 0x82719dae, /// 0x4
			 0xb80878a2, /// 0x8
			 0xe894a5f8, /// 0xc
			 0x1e7c2553, /// 0x10
			 0xa7c99934, /// 0x14
			 0x6608f517, /// 0x18
			 0x40c83dba, /// 0x1c
			 0x186c2207, /// 0x20
			 0x7080f5df, /// 0x24
			 0x383507f0, /// 0x28
			 0x1d6a80fc, /// 0x2c
			 0x4552ff5c, /// 0x30
			 0x72435071, /// 0x34
			 0x76fef8e6, /// 0x38
			 0x2ef8a9b5, /// 0x3c
			 0xef5a0cc6, /// 0x40
			 0x49aad318, /// 0x44
			 0xd194e334, /// 0x48
			 0x8f101c30, /// 0x4c
			 0x45bde5d0, /// 0x50
			 0xaf5747e5, /// 0x54
			 0x4ca0a162, /// 0x58
			 0x88290b40, /// 0x5c
			 0x2ebc8744, /// 0x60
			 0xcdc7b8c0, /// 0x64
			 0xa2700396, /// 0x68
			 0xb145327f, /// 0x6c
			 0xf3b5d496, /// 0x70
			 0x556c09b1, /// 0x74
			 0x2f65d182, /// 0x78
			 0x11f21bf9, /// 0x7c
			 0x4aa731d4, /// 0x80
			 0xe4e81517, /// 0x84
			 0xd2d733da, /// 0x88
			 0x5b28c53c, /// 0x8c
			 0x8ed60f03, /// 0x90
			 0x9601eea2, /// 0x94
			 0x352ee76b, /// 0x98
			 0x44bec0ef, /// 0x9c
			 0xd58694ca, /// 0xa0
			 0x5aeeb0e3, /// 0xa4
			 0xbf0d0fb0, /// 0xa8
			 0xe374f8f2, /// 0xac
			 0x97696bac, /// 0xb0
			 0x9f2681a3, /// 0xb4
			 0x76d6c6cf, /// 0xb8
			 0x6d50e8b8, /// 0xbc
			 0xa3e23e5d, /// 0xc0
			 0x609ad30f, /// 0xc4
			 0x73ab24df, /// 0xc8
			 0xdf3c2b4a, /// 0xcc
			 0xa81f5ffc, /// 0xd0
			 0x12d57d54, /// 0xd4
			 0xe31fd9c0, /// 0xd8
			 0x4cd25b7c, /// 0xdc
			 0x1b15e8f7, /// 0xe0
			 0x644644e8, /// 0xe4
			 0xd66d9837, /// 0xe8
			 0xae592014, /// 0xec
			 0x9ab24a2d, /// 0xf0
			 0x2595e714, /// 0xf4
			 0x8c9ef1e2, /// 0xf8
			 0xea0cc3a0, /// 0xfc
			 0x61ee9920, /// 0x100
			 0x4b6bbb30, /// 0x104
			 0xb1fb4b37, /// 0x108
			 0xa43df259, /// 0x10c
			 0xb19d4e6e, /// 0x110
			 0x0cdb67eb, /// 0x114
			 0xd8b11783, /// 0x118
			 0x564cd7e6, /// 0x11c
			 0x4146f373, /// 0x120
			 0xc01fd694, /// 0x124
			 0x93d9fe24, /// 0x128
			 0x6493d12b, /// 0x12c
			 0xc5f1b328, /// 0x130
			 0x62cf75c4, /// 0x134
			 0xbf4f8825, /// 0x138
			 0x6d0b272c, /// 0x13c
			 0xaf25a2ba, /// 0x140
			 0xf7cd1156, /// 0x144
			 0x365c5e34, /// 0x148
			 0xf5abb799, /// 0x14c
			 0x56fb1902, /// 0x150
			 0x6a61107c, /// 0x154
			 0xddd3e5fd, /// 0x158
			 0xf24faaef, /// 0x15c
			 0x73ff2171, /// 0x160
			 0x72e35eaf, /// 0x164
			 0x8f8e9663, /// 0x168
			 0xd20d3fb9, /// 0x16c
			 0xb36d7a0e, /// 0x170
			 0x63339220, /// 0x174
			 0x23bbb055, /// 0x178
			 0x29337a55, /// 0x17c
			 0x1ba60481, /// 0x180
			 0x67771147, /// 0x184
			 0x7b942ae9, /// 0x188
			 0x0870c7c7, /// 0x18c
			 0xc8abc7aa, /// 0x190
			 0x172f03a3, /// 0x194
			 0x7976e25e, /// 0x198
			 0xa164457e, /// 0x19c
			 0xcfaa4371, /// 0x1a0
			 0xe12b33f5, /// 0x1a4
			 0xfd0a64ce, /// 0x1a8
			 0xe7a32f9b, /// 0x1ac
			 0x8f866e51, /// 0x1b0
			 0xa6c2215c, /// 0x1b4
			 0xbda83899, /// 0x1b8
			 0x40b46525, /// 0x1bc
			 0x11f9e645, /// 0x1c0
			 0x5e738bb1, /// 0x1c4
			 0x0d85a5f4, /// 0x1c8
			 0x4d0eb67a, /// 0x1cc
			 0x4d16c456, /// 0x1d0
			 0xa4f443d4, /// 0x1d4
			 0xb33d5d22, /// 0x1d8
			 0x3d4afa6f, /// 0x1dc
			 0x736ad336, /// 0x1e0
			 0xc370da25, /// 0x1e4
			 0x8297dd63, /// 0x1e8
			 0x7d2cdce4, /// 0x1ec
			 0x4cf9f6b8, /// 0x1f0
			 0x854707c1, /// 0x1f4
			 0x04197009, /// 0x1f8
			 0x328655b2, /// 0x1fc
			 0x985a0372, /// 0x200
			 0xc4f2c3ce, /// 0x204
			 0xf3bd9521, /// 0x208
			 0x44ff7669, /// 0x20c
			 0xbf9e1dbc, /// 0x210
			 0x6953b377, /// 0x214
			 0xa4216d66, /// 0x218
			 0x1e8e32f2, /// 0x21c
			 0x39da69c0, /// 0x220
			 0x1ac24be3, /// 0x224
			 0x0e69d55f, /// 0x228
			 0xb6c34e22, /// 0x22c
			 0x5740d76b, /// 0x230
			 0x1f6877c5, /// 0x234
			 0x5e34eb89, /// 0x238
			 0x05c38524, /// 0x23c
			 0x3213210d, /// 0x240
			 0x8a790aa6, /// 0x244
			 0x3320c201, /// 0x248
			 0x12b58c3f, /// 0x24c
			 0x25db4fec, /// 0x250
			 0x2a926815, /// 0x254
			 0x1218eb9d, /// 0x258
			 0x45653284, /// 0x25c
			 0xfb53ca30, /// 0x260
			 0xc55c4aec, /// 0x264
			 0x1589c22b, /// 0x268
			 0xb1f6c9aa, /// 0x26c
			 0x75d856b5, /// 0x270
			 0x7cde02ef, /// 0x274
			 0x7342c4b5, /// 0x278
			 0xae6705e0, /// 0x27c
			 0x0b3eb223, /// 0x280
			 0x0ecafed5, /// 0x284
			 0xcef0e2b7, /// 0x288
			 0x9b67bb48, /// 0x28c
			 0xc59f00fe, /// 0x290
			 0xeabb9758, /// 0x294
			 0x0cd839f4, /// 0x298
			 0x63901589, /// 0x29c
			 0x87f544c6, /// 0x2a0
			 0x81517326, /// 0x2a4
			 0x531d1ed5, /// 0x2a8
			 0x5c2ba50f, /// 0x2ac
			 0xec5ed454, /// 0x2b0
			 0xee02cf92, /// 0x2b4
			 0xbb931300, /// 0x2b8
			 0x2b1fa915, /// 0x2bc
			 0x4f9ec6c9, /// 0x2c0
			 0x160ebbfe, /// 0x2c4
			 0xb4d2119d, /// 0x2c8
			 0x681fab9f, /// 0x2cc
			 0x9c745222, /// 0x2d0
			 0xec62c424, /// 0x2d4
			 0x9f2ade8a, /// 0x2d8
			 0x4bd85940, /// 0x2dc
			 0xfb5097a5, /// 0x2e0
			 0xdad56d03, /// 0x2e4
			 0xdbbef778, /// 0x2e8
			 0xc27bb057, /// 0x2ec
			 0x5b662c0a, /// 0x2f0
			 0x1a331208, /// 0x2f4
			 0xece6d5a4, /// 0x2f8
			 0x7e924803, /// 0x2fc
			 0x8181dec6, /// 0x300
			 0x8894a710, /// 0x304
			 0x31c9d6d0, /// 0x308
			 0x1f8fd136, /// 0x30c
			 0x35abb88a, /// 0x310
			 0x19e2dee4, /// 0x314
			 0x4c01774b, /// 0x318
			 0xf2dc3e63, /// 0x31c
			 0x9b3c7d22, /// 0x320
			 0xc42de055, /// 0x324
			 0xd85a8899, /// 0x328
			 0x2e9e1280, /// 0x32c
			 0x58ee7095, /// 0x330
			 0xfc905dc0, /// 0x334
			 0x4f18f8ff, /// 0x338
			 0xfdfcf9d7, /// 0x33c
			 0xc8bb2116, /// 0x340
			 0x2ee385d3, /// 0x344
			 0xcf29b215, /// 0x348
			 0xaa1cff52, /// 0x34c
			 0x0f0ba717, /// 0x350
			 0x93e1d13c, /// 0x354
			 0x00b744dd, /// 0x358
			 0xa6ac51a6, /// 0x35c
			 0x9a04206f, /// 0x360
			 0xc5614000, /// 0x364
			 0x78d2c197, /// 0x368
			 0x9cd51941, /// 0x36c
			 0xd3c99050, /// 0x370
			 0xa4d4a32a, /// 0x374
			 0xf5bd368b, /// 0x378
			 0x684631be, /// 0x37c
			 0x0dfb9feb, /// 0x380
			 0xa32e4166, /// 0x384
			 0xfe117610, /// 0x388
			 0x8c7497ac, /// 0x38c
			 0x833e214f, /// 0x390
			 0x49865061, /// 0x394
			 0x083bc615, /// 0x398
			 0xe23bc21d, /// 0x39c
			 0x2d3ad16e, /// 0x3a0
			 0xbe886978, /// 0x3a4
			 0x9cfc1638, /// 0x3a8
			 0xb8232d72, /// 0x3ac
			 0x8b4ac778, /// 0x3b0
			 0x18b3a2bc, /// 0x3b4
			 0x38b0a52a, /// 0x3b8
			 0x42f89ede, /// 0x3bc
			 0xd3b335f6, /// 0x3c0
			 0x1c750b13, /// 0x3c4
			 0xccffbdd8, /// 0x3c8
			 0x7f15b57d, /// 0x3cc
			 0x93f5f641, /// 0x3d0
			 0x51578e47, /// 0x3d4
			 0xc6e764d5, /// 0x3d8
			 0xe1240ee8, /// 0x3dc
			 0x827c353a, /// 0x3e0
			 0xe98d798d, /// 0x3e4
			 0x78ccace0, /// 0x3e8
			 0x8db20a08, /// 0x3ec
			 0xdbdce37d, /// 0x3f0
			 0x7c97783b, /// 0x3f4
			 0xa3602d61, /// 0x3f8
			 0xca58866b, /// 0x3fc
			 0xee80535a, /// 0x400
			 0xf2c66ebd, /// 0x404
			 0x9709a0de, /// 0x408
			 0x9cba56f1, /// 0x40c
			 0xa9289391, /// 0x410
			 0xda5d87e2, /// 0x414
			 0x634bf5bf, /// 0x418
			 0x83cb0e3f, /// 0x41c
			 0xfca559ae, /// 0x420
			 0x181d042a, /// 0x424
			 0x8c81dbcc, /// 0x428
			 0x3683ecdb, /// 0x42c
			 0x7151cea1, /// 0x430
			 0x4d3d0529, /// 0x434
			 0xb48a80e2, /// 0x438
			 0xbf105410, /// 0x43c
			 0x89625663, /// 0x440
			 0x0455e52c, /// 0x444
			 0xea7575d3, /// 0x448
			 0xda6a4488, /// 0x44c
			 0x7c19b6f2, /// 0x450
			 0x4f3f4766, /// 0x454
			 0xa50ee95b, /// 0x458
			 0xcc67d09b, /// 0x45c
			 0x8144d584, /// 0x460
			 0x9b28d523, /// 0x464
			 0x738c8d35, /// 0x468
			 0x9925f923, /// 0x46c
			 0x915d7af1, /// 0x470
			 0xe3a19b8b, /// 0x474
			 0xe35b5dc5, /// 0x478
			 0x72594941, /// 0x47c
			 0x2c0e9351, /// 0x480
			 0x330f7778, /// 0x484
			 0x55b2c19f, /// 0x488
			 0xb7b5332a, /// 0x48c
			 0x0d9a6764, /// 0x490
			 0x84586596, /// 0x494
			 0xbbf3e1a6, /// 0x498
			 0x1c04e0ad, /// 0x49c
			 0x0642e75e, /// 0x4a0
			 0x3d359007, /// 0x4a4
			 0x0af3d37f, /// 0x4a8
			 0x21220083, /// 0x4ac
			 0xb260ac1b, /// 0x4b0
			 0x60731c0d, /// 0x4b4
			 0xe9b4f098, /// 0x4b8
			 0x136469de, /// 0x4bc
			 0x78b8999f, /// 0x4c0
			 0xf4cbbea1, /// 0x4c4
			 0x364b69eb, /// 0x4c8
			 0x8eb1f9e6, /// 0x4cc
			 0x9189187e, /// 0x4d0
			 0xf6dba362, /// 0x4d4
			 0x88fac55a, /// 0x4d8
			 0x90cb1992, /// 0x4dc
			 0x2e43ad99, /// 0x4e0
			 0x8b8d17f9, /// 0x4e4
			 0x37085ad7, /// 0x4e8
			 0x133e3bb3, /// 0x4ec
			 0x2bda017a, /// 0x4f0
			 0x60a2d67d, /// 0x4f4
			 0x184b1d8c, /// 0x4f8
			 0xf25ab620, /// 0x4fc
			 0xc0699855, /// 0x500
			 0xabf85d8d, /// 0x504
			 0xc10caab3, /// 0x508
			 0x854ffd01, /// 0x50c
			 0xfaf433cf, /// 0x510
			 0x212dcb18, /// 0x514
			 0xb5f01d4f, /// 0x518
			 0x0b1e5135, /// 0x51c
			 0x06fa76aa, /// 0x520
			 0x5eb3d2af, /// 0x524
			 0x8595ebe2, /// 0x528
			 0x87a7751e, /// 0x52c
			 0x7ba8302f, /// 0x530
			 0x83061b71, /// 0x534
			 0x1c716439, /// 0x538
			 0x3cf48392, /// 0x53c
			 0x8cf98a4c, /// 0x540
			 0x8eb569a6, /// 0x544
			 0xc22e4029, /// 0x548
			 0x5417a4e9, /// 0x54c
			 0x8354533f, /// 0x550
			 0xdd9e2fdd, /// 0x554
			 0xf919df2b, /// 0x558
			 0xa33f90a5, /// 0x55c
			 0xf63dc948, /// 0x560
			 0xea9e63eb, /// 0x564
			 0xef4aeb86, /// 0x568
			 0x304bb395, /// 0x56c
			 0xf8703f70, /// 0x570
			 0x5f415eef, /// 0x574
			 0xec72fe12, /// 0x578
			 0xaf32cafe, /// 0x57c
			 0xf6b1ffc2, /// 0x580
			 0x9f0dcdff, /// 0x584
			 0xf93665fa, /// 0x588
			 0x608a510a, /// 0x58c
			 0x1920770d, /// 0x590
			 0x8d3aa2b9, /// 0x594
			 0xc48191d5, /// 0x598
			 0xb7566240, /// 0x59c
			 0x58d43251, /// 0x5a0
			 0x5c94c756, /// 0x5a4
			 0x684be67f, /// 0x5a8
			 0xce835103, /// 0x5ac
			 0xaf710696, /// 0x5b0
			 0x7b081e8f, /// 0x5b4
			 0xcb0ffb3d, /// 0x5b8
			 0x037b1c3b, /// 0x5bc
			 0x0694962c, /// 0x5c0
			 0x4b90a133, /// 0x5c4
			 0x5e75c858, /// 0x5c8
			 0xd305b5cd, /// 0x5cc
			 0xe0f5257e, /// 0x5d0
			 0x100bf84a, /// 0x5d4
			 0xcb0db58a, /// 0x5d8
			 0xfcd04fec, /// 0x5dc
			 0xaed974c4, /// 0x5e0
			 0x08a6e4ad, /// 0x5e4
			 0x02895019, /// 0x5e8
			 0x1d22f6a4, /// 0x5ec
			 0xdd0a37d4, /// 0x5f0
			 0xe657df38, /// 0x5f4
			 0xecec55b0, /// 0x5f8
			 0x98b32051, /// 0x5fc
			 0x060ffdfa, /// 0x600
			 0xd01dd774, /// 0x604
			 0xbacedc6e, /// 0x608
			 0xdf6faac8, /// 0x60c
			 0x8346e8c9, /// 0x610
			 0xef670220, /// 0x614
			 0x6f7ff061, /// 0x618
			 0x462713c2, /// 0x61c
			 0x447a2ec7, /// 0x620
			 0xf580516f, /// 0x624
			 0xdce04a1d, /// 0x628
			 0xc98f3da3, /// 0x62c
			 0xb18c58fc, /// 0x630
			 0xd99a757a, /// 0x634
			 0x072b6bec, /// 0x638
			 0x4965e360, /// 0x63c
			 0x2eced965, /// 0x640
			 0x8471a337, /// 0x644
			 0x6ef2d9e0, /// 0x648
			 0xa071f5d2, /// 0x64c
			 0xe97f1261, /// 0x650
			 0x02ec3b7e, /// 0x654
			 0x2a0dc311, /// 0x658
			 0x09fde549, /// 0x65c
			 0x0d01ccbf, /// 0x660
			 0x7aac9cff, /// 0x664
			 0x5f955004, /// 0x668
			 0x93cb5263, /// 0x66c
			 0x5810fc12, /// 0x670
			 0xdb6897ca, /// 0x674
			 0x9e0f5a63, /// 0x678
			 0x18d5bde2, /// 0x67c
			 0x551a09e3, /// 0x680
			 0x1e7c2895, /// 0x684
			 0x1a062944, /// 0x688
			 0x1fc66552, /// 0x68c
			 0x6d16fa9f, /// 0x690
			 0xd23d35b6, /// 0x694
			 0xd0698486, /// 0x698
			 0x301defee, /// 0x69c
			 0xeaa0ff14, /// 0x6a0
			 0x139f60b2, /// 0x6a4
			 0xfbd58f9c, /// 0x6a8
			 0xc12051bc, /// 0x6ac
			 0x2adfb7f4, /// 0x6b0
			 0x1e987f7d, /// 0x6b4
			 0x550e6796, /// 0x6b8
			 0x5211e9d4, /// 0x6bc
			 0x1b157333, /// 0x6c0
			 0x3ebb87e6, /// 0x6c4
			 0x4ce75a6f, /// 0x6c8
			 0x7e363fae, /// 0x6cc
			 0x96ac1811, /// 0x6d0
			 0xff0fdf87, /// 0x6d4
			 0xa157297e, /// 0x6d8
			 0xae5297c7, /// 0x6dc
			 0xf37ab5ae, /// 0x6e0
			 0xaa011d0f, /// 0x6e4
			 0xf3f27bab, /// 0x6e8
			 0xa1c5a833, /// 0x6ec
			 0xdf64d1cc, /// 0x6f0
			 0x646da9be, /// 0x6f4
			 0xd3e95e4f, /// 0x6f8
			 0x2f5bb9ef, /// 0x6fc
			 0x7a023270, /// 0x700
			 0xeb17b164, /// 0x704
			 0x500182a3, /// 0x708
			 0x56ba70b8, /// 0x70c
			 0xb7c96ed6, /// 0x710
			 0xdce5c9a6, /// 0x714
			 0x89f3d251, /// 0x718
			 0xbe8f18bb, /// 0x71c
			 0x1322a622, /// 0x720
			 0xd89e97b6, /// 0x724
			 0x35cd1025, /// 0x728
			 0x0f1e691a, /// 0x72c
			 0x8d9fb121, /// 0x730
			 0x4eeee46b, /// 0x734
			 0xf807abbd, /// 0x738
			 0xaa4a863d, /// 0x73c
			 0xfe01d71b, /// 0x740
			 0x8d490d88, /// 0x744
			 0xccb8e083, /// 0x748
			 0xec6e1674, /// 0x74c
			 0xdb4c3add, /// 0x750
			 0xcb4627a3, /// 0x754
			 0xa82fdcc5, /// 0x758
			 0x33c617cf, /// 0x75c
			 0x238d8a18, /// 0x760
			 0x2200a92c, /// 0x764
			 0x352c12f9, /// 0x768
			 0xb71a9574, /// 0x76c
			 0x4a9976eb, /// 0x770
			 0x12b53b2e, /// 0x774
			 0xaf83b439, /// 0x778
			 0xd462dd2d, /// 0x77c
			 0x3eded4df, /// 0x780
			 0x38b85737, /// 0x784
			 0xf878c0dd, /// 0x788
			 0x46d15374, /// 0x78c
			 0x3450bfc6, /// 0x790
			 0x1eca3e57, /// 0x794
			 0x51ab638d, /// 0x798
			 0xc9f2f82f, /// 0x79c
			 0xa27f866c, /// 0x7a0
			 0xf29bfb1a, /// 0x7a4
			 0x8bfec597, /// 0x7a8
			 0xbf4f5a7d, /// 0x7ac
			 0x342ad136, /// 0x7b0
			 0xb3bc9db8, /// 0x7b4
			 0xaf5f381f, /// 0x7b8
			 0x2d8c67c6, /// 0x7bc
			 0xfaed22e6, /// 0x7c0
			 0x0b3445f2, /// 0x7c4
			 0x8974686e, /// 0x7c8
			 0x6e886706, /// 0x7cc
			 0x4a263a86, /// 0x7d0
			 0xf4e70702, /// 0x7d4
			 0x4bfc2842, /// 0x7d8
			 0x2fe1fafa, /// 0x7dc
			 0x0a3a57c9, /// 0x7e0
			 0xdd6c5b89, /// 0x7e4
			 0x8c2ed28d, /// 0x7e8
			 0x2d4ba459, /// 0x7ec
			 0x070aa4c1, /// 0x7f0
			 0x3cce9786, /// 0x7f4
			 0xaee183bb, /// 0x7f8
			 0x885feb14, /// 0x7fc
			 0x0993913e, /// 0x800
			 0x25a3adb3, /// 0x804
			 0x0b19ca7a, /// 0x808
			 0xc948771f, /// 0x80c
			 0x47281a9c, /// 0x810
			 0x740e4c48, /// 0x814
			 0xae73e281, /// 0x818
			 0x88868d41, /// 0x81c
			 0xf641a1d9, /// 0x820
			 0x2a3a7130, /// 0x824
			 0xf4fd5d10, /// 0x828
			 0x24fa00ea, /// 0x82c
			 0x97bac87f, /// 0x830
			 0xbc2a22c3, /// 0x834
			 0xdf2007fd, /// 0x838
			 0x04964ee8, /// 0x83c
			 0xe45bdfdf, /// 0x840
			 0xf6504bbb, /// 0x844
			 0x78087d67, /// 0x848
			 0x073d86fc, /// 0x84c
			 0xbef6263f, /// 0x850
			 0x38bfc20b, /// 0x854
			 0x601ca485, /// 0x858
			 0x99aae052, /// 0x85c
			 0x10cd5cfe, /// 0x860
			 0xc62944fc, /// 0x864
			 0x398a5672, /// 0x868
			 0x7685ded9, /// 0x86c
			 0x0775cefd, /// 0x870
			 0x08bb81e3, /// 0x874
			 0xd0ccac0a, /// 0x878
			 0xaa220b8a, /// 0x87c
			 0xf6b56ee0, /// 0x880
			 0x4040dd17, /// 0x884
			 0x8d421566, /// 0x888
			 0x3f0c87a4, /// 0x88c
			 0x3d829228, /// 0x890
			 0x7f83475e, /// 0x894
			 0x9b41ce91, /// 0x898
			 0x75331bc5, /// 0x89c
			 0x5796af92, /// 0x8a0
			 0xce4f7fe7, /// 0x8a4
			 0xed4e6245, /// 0x8a8
			 0x56fe4284, /// 0x8ac
			 0xebf6e908, /// 0x8b0
			 0xe934d100, /// 0x8b4
			 0x5abb988a, /// 0x8b8
			 0x202d56c9, /// 0x8bc
			 0xe7038f4e, /// 0x8c0
			 0x82fc3ada, /// 0x8c4
			 0x3045f4d7, /// 0x8c8
			 0x1e725ad0, /// 0x8cc
			 0x1389906f, /// 0x8d0
			 0x3dc3f317, /// 0x8d4
			 0xbbe983c9, /// 0x8d8
			 0x4dd2dbdb, /// 0x8dc
			 0xec42ae58, /// 0x8e0
			 0x61289589, /// 0x8e4
			 0x90f21034, /// 0x8e8
			 0xc914991a, /// 0x8ec
			 0xce0065dc, /// 0x8f0
			 0x5f79a201, /// 0x8f4
			 0xe4be1050, /// 0x8f8
			 0x27ed3aca, /// 0x8fc
			 0x86e39f72, /// 0x900
			 0x286ece4c, /// 0x904
			 0x63bf3d54, /// 0x908
			 0x276efc2a, /// 0x90c
			 0x7672bc4f, /// 0x910
			 0xb1b20715, /// 0x914
			 0xb2829398, /// 0x918
			 0x9dd085c8, /// 0x91c
			 0xf23f4767, /// 0x920
			 0x974e8908, /// 0x924
			 0xb1e69a41, /// 0x928
			 0x9095cd19, /// 0x92c
			 0x3fd2e6d1, /// 0x930
			 0x0ee1135e, /// 0x934
			 0xa32d3b0b, /// 0x938
			 0xa084bcfa, /// 0x93c
			 0xc4a03cd9, /// 0x940
			 0x477e1587, /// 0x944
			 0xd268c934, /// 0x948
			 0x6f187e8c, /// 0x94c
			 0x3ce4272b, /// 0x950
			 0x8d7df529, /// 0x954
			 0xf8dd52c1, /// 0x958
			 0xa927e455, /// 0x95c
			 0x1ca80eb4, /// 0x960
			 0x83952cbd, /// 0x964
			 0xb52a8f07, /// 0x968
			 0x782e005b, /// 0x96c
			 0xe6ccbfc6, /// 0x970
			 0x44ca537c, /// 0x974
			 0xde3b4a81, /// 0x978
			 0x91bd0b98, /// 0x97c
			 0x1100ae43, /// 0x980
			 0x3c41b1f1, /// 0x984
			 0x95674cce, /// 0x988
			 0x8fb418a2, /// 0x98c
			 0x2722d0a7, /// 0x990
			 0xed6188f1, /// 0x994
			 0x61e32f49, /// 0x998
			 0x77e64edd, /// 0x99c
			 0x6d28cc05, /// 0x9a0
			 0xd04f807e, /// 0x9a4
			 0x361cccdb, /// 0x9a8
			 0xb0768b60, /// 0x9ac
			 0x92296062, /// 0x9b0
			 0x2aae71ae, /// 0x9b4
			 0xbe8c4393, /// 0x9b8
			 0x4f6d9e62, /// 0x9bc
			 0x1224719a, /// 0x9c0
			 0x5d11b53c, /// 0x9c4
			 0x18388221, /// 0x9c8
			 0xfb1767bf, /// 0x9cc
			 0x663df7e3, /// 0x9d0
			 0xe1444fab, /// 0x9d4
			 0x56dc06fb, /// 0x9d8
			 0x5746aea1, /// 0x9dc
			 0xe5e2a5f4, /// 0x9e0
			 0xfdc5591d, /// 0x9e4
			 0xf99744bf, /// 0x9e8
			 0x2157ef8b, /// 0x9ec
			 0xe9676c5f, /// 0x9f0
			 0x5cf4fdb5, /// 0x9f4
			 0x5c7bac48, /// 0x9f8
			 0x919fd236, /// 0x9fc
			 0x65962bcd, /// 0xa00
			 0xd8b6713e, /// 0xa04
			 0x6de8685d, /// 0xa08
			 0xc725d5af, /// 0xa0c
			 0x15b3a809, /// 0xa10
			 0x20bcdc81, /// 0xa14
			 0x8a929971, /// 0xa18
			 0x089bf8d0, /// 0xa1c
			 0xef84cc7c, /// 0xa20
			 0x70d5af74, /// 0xa24
			 0x1dd4df47, /// 0xa28
			 0x0ec0511d, /// 0xa2c
			 0x1feba3fa, /// 0xa30
			 0xaa513bf8, /// 0xa34
			 0xa6849e3b, /// 0xa38
			 0xe9de26f8, /// 0xa3c
			 0xfa7e5fd3, /// 0xa40
			 0x37c2e88b, /// 0xa44
			 0xdc65748d, /// 0xa48
			 0x72b8196b, /// 0xa4c
			 0xc4f16989, /// 0xa50
			 0x13741d82, /// 0xa54
			 0x56bba5f9, /// 0xa58
			 0x6bc33de0, /// 0xa5c
			 0x3b0799b9, /// 0xa60
			 0x82e9b07d, /// 0xa64
			 0xc9add413, /// 0xa68
			 0x25c92f55, /// 0xa6c
			 0xffb80388, /// 0xa70
			 0x867ab008, /// 0xa74
			 0x9e7560c8, /// 0xa78
			 0xcb031930, /// 0xa7c
			 0x97adeab8, /// 0xa80
			 0xc53c095c, /// 0xa84
			 0xbad489f5, /// 0xa88
			 0x16703519, /// 0xa8c
			 0x2e1c7e5b, /// 0xa90
			 0x4f88a5d1, /// 0xa94
			 0xbcaf0576, /// 0xa98
			 0x8daff3c2, /// 0xa9c
			 0xe1d7af08, /// 0xaa0
			 0x5983c5d0, /// 0xaa4
			 0x72d10d09, /// 0xaa8
			 0x79e1161c, /// 0xaac
			 0x62b9ea20, /// 0xab0
			 0x4cb2702c, /// 0xab4
			 0xfd82b234, /// 0xab8
			 0x3191b166, /// 0xabc
			 0xc639c527, /// 0xac0
			 0xa2b38dcc, /// 0xac4
			 0x6416efe8, /// 0xac8
			 0x9bc7efbd, /// 0xacc
			 0xc3543f2c, /// 0xad0
			 0x1c8a33eb, /// 0xad4
			 0x0ea0b6d1, /// 0xad8
			 0x9c53e690, /// 0xadc
			 0xa83d93eb, /// 0xae0
			 0xab8e710d, /// 0xae4
			 0xf828c5fb, /// 0xae8
			 0x01b29342, /// 0xaec
			 0xa6640a5b, /// 0xaf0
			 0x53a91f58, /// 0xaf4
			 0x11506498, /// 0xaf8
			 0x91f8bf82, /// 0xafc
			 0x5e13da3e, /// 0xb00
			 0x188b0436, /// 0xb04
			 0x82329e5c, /// 0xb08
			 0xf56565f4, /// 0xb0c
			 0xbe7d013a, /// 0xb10
			 0xd42eaa42, /// 0xb14
			 0x8aa6ab1a, /// 0xb18
			 0xc7a2b218, /// 0xb1c
			 0x92d7e48d, /// 0xb20
			 0xd326668c, /// 0xb24
			 0x03bfa438, /// 0xb28
			 0x1174108e, /// 0xb2c
			 0xe6756b08, /// 0xb30
			 0x72efc822, /// 0xb34
			 0x733d612a, /// 0xb38
			 0x06d8a9cc, /// 0xb3c
			 0xf7debe2d, /// 0xb40
			 0xfa623594, /// 0xb44
			 0xdedcd411, /// 0xb48
			 0xbe06cf5a, /// 0xb4c
			 0xcbe3b1e5, /// 0xb50
			 0x9f3537b3, /// 0xb54
			 0x82c2f807, /// 0xb58
			 0xdda5bdc9, /// 0xb5c
			 0x80a5f18c, /// 0xb60
			 0xb400c368, /// 0xb64
			 0xba6b1314, /// 0xb68
			 0xca10914f, /// 0xb6c
			 0x5e03d03d, /// 0xb70
			 0x46cc3625, /// 0xb74
			 0xa7d5b0ac, /// 0xb78
			 0x41777fc3, /// 0xb7c
			 0x5fb4cdd6, /// 0xb80
			 0x92dcfb90, /// 0xb84
			 0x009eff07, /// 0xb88
			 0xf93df472, /// 0xb8c
			 0xc8c69ca4, /// 0xb90
			 0x3b2c8919, /// 0xb94
			 0xc6bf8030, /// 0xb98
			 0x3de9e709, /// 0xb9c
			 0x0dfc2142, /// 0xba0
			 0x59a36d70, /// 0xba4
			 0x3c391314, /// 0xba8
			 0x18458aa2, /// 0xbac
			 0xb8c59fcd, /// 0xbb0
			 0xe6b6fec5, /// 0xbb4
			 0x14604bf8, /// 0xbb8
			 0xa01e6d7a, /// 0xbbc
			 0xc86b0ace, /// 0xbc0
			 0x20f589fb, /// 0xbc4
			 0x1a3b06bf, /// 0xbc8
			 0xa85a8f97, /// 0xbcc
			 0x0d32a427, /// 0xbd0
			 0xd0a26848, /// 0xbd4
			 0x8fbd7a1b, /// 0xbd8
			 0x4c90b86c, /// 0xbdc
			 0x4e9cb7b1, /// 0xbe0
			 0xbb693a85, /// 0xbe4
			 0xb7a44921, /// 0xbe8
			 0x73bdb50c, /// 0xbec
			 0x448c17df, /// 0xbf0
			 0xde576869, /// 0xbf4
			 0xe31b4b1b, /// 0xbf8
			 0x0b7bb8d0, /// 0xbfc
			 0x41471871, /// 0xc00
			 0x0af3b187, /// 0xc04
			 0x457b4a80, /// 0xc08
			 0xc59d21c3, /// 0xc0c
			 0x98d8069c, /// 0xc10
			 0xaec60e12, /// 0xc14
			 0xd87eb04a, /// 0xc18
			 0xa92af8e4, /// 0xc1c
			 0x7ebe5f7c, /// 0xc20
			 0xc676f4f8, /// 0xc24
			 0xcf2a4d2e, /// 0xc28
			 0x777676a2, /// 0xc2c
			 0x90ec2767, /// 0xc30
			 0x2b377005, /// 0xc34
			 0x58a246cc, /// 0xc38
			 0x3791bf2d, /// 0xc3c
			 0x4e22f300, /// 0xc40
			 0xc0b93a48, /// 0xc44
			 0xdc2b324f, /// 0xc48
			 0x79dd652b, /// 0xc4c
			 0x6fea91d8, /// 0xc50
			 0xe1b98140, /// 0xc54
			 0x76fe3f97, /// 0xc58
			 0xdadbe2d0, /// 0xc5c
			 0x189522c7, /// 0xc60
			 0x8faf4482, /// 0xc64
			 0x1b216f06, /// 0xc68
			 0x3d41c7f9, /// 0xc6c
			 0x8b40333c, /// 0xc70
			 0x011a9def, /// 0xc74
			 0x2017328a, /// 0xc78
			 0xb7e9c313, /// 0xc7c
			 0x761d120f, /// 0xc80
			 0xe71ad8b9, /// 0xc84
			 0x9a0e97fb, /// 0xc88
			 0xc141bc27, /// 0xc8c
			 0x87466ed6, /// 0xc90
			 0xc9b023bf, /// 0xc94
			 0x9f03153b, /// 0xc98
			 0x6435fc02, /// 0xc9c
			 0xf6bf4456, /// 0xca0
			 0xc5fdfa35, /// 0xca4
			 0x8aa65b67, /// 0xca8
			 0xd9a0c885, /// 0xcac
			 0xe6807b18, /// 0xcb0
			 0x4d5c4da6, /// 0xcb4
			 0x9f3efb62, /// 0xcb8
			 0xd75ee158, /// 0xcbc
			 0x16498c42, /// 0xcc0
			 0x82fb74b4, /// 0xcc4
			 0x0968c338, /// 0xcc8
			 0x5bf19dd1, /// 0xccc
			 0xdecdd4b1, /// 0xcd0
			 0x370096c1, /// 0xcd4
			 0x3b70aea4, /// 0xcd8
			 0xce9174b1, /// 0xcdc
			 0x2622c3cd, /// 0xce0
			 0x69bd783e, /// 0xce4
			 0xdb996370, /// 0xce8
			 0x62dc8a4e, /// 0xcec
			 0x8b31fa36, /// 0xcf0
			 0x43ddcf26, /// 0xcf4
			 0xd20bb687, /// 0xcf8
			 0x99167fc7, /// 0xcfc
			 0xeb45666b, /// 0xd00
			 0x399c77f7, /// 0xd04
			 0xfb0c5a55, /// 0xd08
			 0xbd29bdd0, /// 0xd0c
			 0x57bd883a, /// 0xd10
			 0x2d7b5a74, /// 0xd14
			 0x95d7f435, /// 0xd18
			 0x7a0bc39d, /// 0xd1c
			 0x3f1fe806, /// 0xd20
			 0xfe65c9ea, /// 0xd24
			 0x229b057e, /// 0xd28
			 0x04e8ddcf, /// 0xd2c
			 0xbc3454fb, /// 0xd30
			 0x5d083d78, /// 0xd34
			 0x97469c10, /// 0xd38
			 0x89341eb8, /// 0xd3c
			 0x18dd31ac, /// 0xd40
			 0xb6f37957, /// 0xd44
			 0x9ed54188, /// 0xd48
			 0x2e71fdd9, /// 0xd4c
			 0xfabe9177, /// 0xd50
			 0x04625a37, /// 0xd54
			 0xfce6cbaa, /// 0xd58
			 0x65b4a3e8, /// 0xd5c
			 0x17335f98, /// 0xd60
			 0x22f3102b, /// 0xd64
			 0x050c1600, /// 0xd68
			 0x37b94d64, /// 0xd6c
			 0xe9afed21, /// 0xd70
			 0xe8b57031, /// 0xd74
			 0x5592b4b4, /// 0xd78
			 0x5f036886, /// 0xd7c
			 0xc0def543, /// 0xd80
			 0x199e2527, /// 0xd84
			 0x5ae85dbc, /// 0xd88
			 0xdae1dc14, /// 0xd8c
			 0xd5bba211, /// 0xd90
			 0xf7fff426, /// 0xd94
			 0xb684e1a4, /// 0xd98
			 0xf7b66118, /// 0xd9c
			 0xb7b7bbd3, /// 0xda0
			 0xc24a8a63, /// 0xda4
			 0xfbe9e8db, /// 0xda8
			 0xacb2e5d6, /// 0xdac
			 0x412d289f, /// 0xdb0
			 0x9b817bc8, /// 0xdb4
			 0xc9f6af1a, /// 0xdb8
			 0x4f31c2c0, /// 0xdbc
			 0x3365dfa1, /// 0xdc0
			 0xdc5701c4, /// 0xdc4
			 0x9b59639c, /// 0xdc8
			 0xfbcb1695, /// 0xdcc
			 0xfee175d2, /// 0xdd0
			 0x4df9ba53, /// 0xdd4
			 0x49e0d89e, /// 0xdd8
			 0x3b6cab1e, /// 0xddc
			 0xb98a21fe, /// 0xde0
			 0x20093c5a, /// 0xde4
			 0x2606386c, /// 0xde8
			 0x6c819eb5, /// 0xdec
			 0xb3b111a1, /// 0xdf0
			 0xf97f68af, /// 0xdf4
			 0x9e89bcc3, /// 0xdf8
			 0x1fd08698, /// 0xdfc
			 0xe9881b42, /// 0xe00
			 0x63873ba0, /// 0xe04
			 0xc4dc634e, /// 0xe08
			 0xc893549f, /// 0xe0c
			 0x7dc689ca, /// 0xe10
			 0x43e18296, /// 0xe14
			 0x0515c3fd, /// 0xe18
			 0x06edacdd, /// 0xe1c
			 0xfcf90a69, /// 0xe20
			 0x498a1dd0, /// 0xe24
			 0x76d3b487, /// 0xe28
			 0x15cd7b12, /// 0xe2c
			 0xad138d7e, /// 0xe30
			 0x59a139ee, /// 0xe34
			 0x7b613af7, /// 0xe38
			 0x6f6c63d0, /// 0xe3c
			 0xc3c3a6b4, /// 0xe40
			 0xb1b0347b, /// 0xe44
			 0x648b1efa, /// 0xe48
			 0x4d7797c3, /// 0xe4c
			 0xc78d29fc, /// 0xe50
			 0x374de7d8, /// 0xe54
			 0xc5332b0f, /// 0xe58
			 0x2b09a6c9, /// 0xe5c
			 0x4d9e9bfd, /// 0xe60
			 0xf5fd4321, /// 0xe64
			 0x7522e540, /// 0xe68
			 0x43670514, /// 0xe6c
			 0x474abe83, /// 0xe70
			 0xbc55e3b0, /// 0xe74
			 0xd47c6f74, /// 0xe78
			 0x3af99099, /// 0xe7c
			 0x62bb46f5, /// 0xe80
			 0x806b9280, /// 0xe84
			 0x48ce6805, /// 0xe88
			 0x4d105cfc, /// 0xe8c
			 0xb8f6c981, /// 0xe90
			 0x2ec122f6, /// 0xe94
			 0x5d39a99c, /// 0xe98
			 0x48abae43, /// 0xe9c
			 0xf15f65e6, /// 0xea0
			 0x6c33867d, /// 0xea4
			 0x9c1bbb12, /// 0xea8
			 0x671e3082, /// 0xeac
			 0xbb0a05c4, /// 0xeb0
			 0x132cbbc6, /// 0xeb4
			 0x3711e561, /// 0xeb8
			 0xffb0190b, /// 0xebc
			 0xb4833306, /// 0xec0
			 0x66182773, /// 0xec4
			 0x3c1b0379, /// 0xec8
			 0x6a7e894f, /// 0xecc
			 0x2ae3839e, /// 0xed0
			 0x9b1dfa87, /// 0xed4
			 0x8af8ecb9, /// 0xed8
			 0x6a8db01e, /// 0xedc
			 0x417f7881, /// 0xee0
			 0x8fb6e884, /// 0xee4
			 0x8b403ac7, /// 0xee8
			 0x860b80ce, /// 0xeec
			 0x380b0c2f, /// 0xef0
			 0x0e937c37, /// 0xef4
			 0xf48b1eed, /// 0xef8
			 0x7dae4fc3, /// 0xefc
			 0xa1634edb, /// 0xf00
			 0x4c37ff82, /// 0xf04
			 0x61b10d32, /// 0xf08
			 0x8ccb95cc, /// 0xf0c
			 0xa4999c18, /// 0xf10
			 0xd93a3be8, /// 0xf14
			 0xf8cafa2b, /// 0xf18
			 0x01516b98, /// 0xf1c
			 0xb9108047, /// 0xf20
			 0x89ceafa4, /// 0xf24
			 0x1916fc4c, /// 0xf28
			 0x6c5682a6, /// 0xf2c
			 0x359bf6db, /// 0xf30
			 0x0531f283, /// 0xf34
			 0xed7844c2, /// 0xf38
			 0x9a57eddb, /// 0xf3c
			 0x05e1d055, /// 0xf40
			 0xed96ff95, /// 0xf44
			 0x4ddabcb6, /// 0xf48
			 0xaf7dcf84, /// 0xf4c
			 0x28e4b3ff, /// 0xf50
			 0xf40dac2b, /// 0xf54
			 0xbfb6c06a, /// 0xf58
			 0x30537af3, /// 0xf5c
			 0xd855655f, /// 0xf60
			 0x83405f7c, /// 0xf64
			 0xf6f89d50, /// 0xf68
			 0x1628cdbc, /// 0xf6c
			 0x7889d418, /// 0xf70
			 0xf743e406, /// 0xf74
			 0x5e94631f, /// 0xf78
			 0x8f045559, /// 0xf7c
			 0xbe9a9fe4, /// 0xf80
			 0x11afb05e, /// 0xf84
			 0xa5a9877b, /// 0xf88
			 0x8c987e2e, /// 0xf8c
			 0xad8de237, /// 0xf90
			 0xa1df3a25, /// 0xf94
			 0x8a68ca2e, /// 0xf98
			 0xb1304198, /// 0xf9c
			 0xd1c2e886, /// 0xfa0
			 0x8aca9cd5, /// 0xfa4
			 0x177b41e3, /// 0xfa8
			 0xdac651ab, /// 0xfac
			 0x9df01fde, /// 0xfb0
			 0x26c79ddd, /// 0xfb4
			 0x36e199b7, /// 0xfb8
			 0xa693db61, /// 0xfbc
			 0x75770c71, /// 0xfc0
			 0x4a8a2078, /// 0xfc4
			 0xf7245b74, /// 0xfc8
			 0x38113970, /// 0xfcc
			 0x9fa6971f, /// 0xfd0
			 0x38680c6d, /// 0xfd4
			 0xd2b12cc2, /// 0xfd8
			 0x54bc07e3, /// 0xfdc
			 0xa7f933d2, /// 0xfe0
			 0x491553f4, /// 0xfe4
			 0xccbcff0f, /// 0xfe8
			 0x20f35662, /// 0xfec
			 0x4ffe9cae, /// 0xff0
			 0xcdc5e288, /// 0xff4
			 0x2ecbd33b, /// 0xff8
			 0xdc9e8e33 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00000
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00008
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0000c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00014
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00018
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0001c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00020
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00024
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00030
			 0x3f028f5c,                                                  // 0.51                                        /// 00034
			 0x0c400000,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0003c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00040
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00050
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00054
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00058
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0005c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00060
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00068
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0006c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xffc00001,                                                  // -signaling NaN                              /// 00078
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00080
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00088
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00098
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0009c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c4
			 0x00000000,                                                  // zero                                        /// 000c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f0
			 0x7fc00001,                                                  // signaling NaN                               /// 000f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00104
			 0x55555555,                                                  // 4 random values                             /// 00108
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0010c
			 0x7fc00001,                                                  // signaling NaN                               /// 00110
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00114
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00118
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0011c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00124
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0012c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00134
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0013c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00140
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00144
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00148
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0014c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00150
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0xff800000,                                                  // -inf                                        /// 00168
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00170
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00174
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00178
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0017c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00180
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00184
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00188
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00194
			 0x3f028f5c,                                                  // 0.51                                        /// 00198
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 001e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00200
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00204
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xff800000,                                                  // -inf                                        /// 0020c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00210
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00214
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00218
			 0x55555555,                                                  // 4 random values                             /// 0021c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00220
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00228
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00230
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00234
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00238
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0023c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00240
			 0xffc00001,                                                  // -signaling NaN                              /// 00244
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00248
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0024c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00250
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00254
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00258
			 0x4b000000,                                                  // 8388608.0                                   /// 0025c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00260
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00264
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00270
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00274
			 0x0e000007,                                                  // Trailing 1s:                                /// 00278
			 0x80000000,                                                  // -zero                                       /// 0027c
			 0x33333333,                                                  // 4 random values                             /// 00280
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00284
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00298
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0029c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x3f028f5c,                                                  // 0.51                                        /// 002f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00300
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00304
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00308
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0030c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00320
			 0x55555555,                                                  // 4 random values                             /// 00324
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00328
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0032c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00334
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00338
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00340
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0034c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00350
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00354
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0035c
			 0xbf028f5c,                                                  // -0.51                                       /// 00360
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00364
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00370
			 0x33333333,                                                  // 4 random values                             /// 00374
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0037c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00384
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00388
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0038c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00390
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00398
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00400
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00404
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0040c
			 0x00000000,                                                  // zero                                        /// 00410
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00418
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0041c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00420
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0042c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0043c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00444
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00448
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0044c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00458
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0045c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00460
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00464
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e000003,                                                  // Trailing 1s:                                /// 0046c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00000000,                                                  // zero                                        /// 0047c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00480
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00488
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0048c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00498
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0049c
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b8
			 0x3f028f5c,                                                  // 0.51                                        /// 004bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00500
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00504
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00508
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0050c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00518
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00520
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00524
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00528
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000000,                                                  // zero                                        /// 00530
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00534
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00538
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00540
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00548
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0054c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00550
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00554
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x7f800000,                                                  // inf                                         /// 0055c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x00000000,                                                  // zero                                        /// 00564
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0056c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00574
			 0xffc00001,                                                  // -signaling NaN                              /// 00578
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0057c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00584
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00594
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00598
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0059c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80000000,                                                  // -zero                                       /// 005ec
			 0x7fc00001,                                                  // signaling NaN                               /// 005f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00600
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00604
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0060c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00610
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00618
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0061c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00620
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00624
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0062c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00630
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00634
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00640
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00644
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00664
			 0x33333333,                                                  // 4 random values                             /// 00668
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x7fc00001,                                                  // signaling NaN                               /// 00670
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0xffc00001,                                                  // -signaling NaN                              /// 0067c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00680
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00688
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0068c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00694
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00698
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0069c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006b4
			 0x80000000,                                                  // -zero                                       /// 006b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006bc
			 0x7f800000,                                                  // inf                                         /// 006c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d4
			 0x80000000,                                                  // -zero                                       /// 006d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x4b000000,                                                  // 8388608.0                                   /// 006e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00700
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00704
			 0x33333333,                                                  // 4 random values                             /// 00708
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000000,                                                  // zero                                        /// 00714
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00718
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0071c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00720
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00724
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00730
			 0x00011111,                                                  // 9.7958E-41                                  /// 00734
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00738
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0073c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00740
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00744
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00748
			 0x7fc00001,                                                  // signaling NaN                               /// 0074c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00754
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00758
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0075c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00760
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00764
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0076c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00770
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00778
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0077c
			 0x7fc00001,                                                  // signaling NaN                               /// 00780
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00784
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00788
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0078c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00011111,                                                  // 9.7958E-41                                  /// 00794
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0079c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00810
			 0xbf028f5c,                                                  // -0.51                                       /// 00814
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00818
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0081c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00820
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00828
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0082c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00830
			 0x80011111,                                                  // -9.7958E-41                                 /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00844
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0084c
			 0x7f800000,                                                  // inf                                         /// 00850
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00854
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00858
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0086c
			 0x55555555,                                                  // 4 random values                             /// 00870
			 0x33333333,                                                  // 4 random values                             /// 00874
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00878
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0087c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00880
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00888
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0x0c780000,                                                  // Leading 1s:                                 /// 00894
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00898
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0089c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x33333333,                                                  // 4 random values                             /// 008a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x00000000,                                                  // zero                                        /// 008ec
			 0x55555555,                                                  // 4 random values                             /// 008f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00900
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00904
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00908
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00914
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00918
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0091c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00920
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00924
			 0x0e000007,                                                  // Trailing 1s:                                /// 00928
			 0x00011111,                                                  // 9.7958E-41                                  /// 0092c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0093c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00940
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00944
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00948
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00950
			 0x80000000,                                                  // -zero                                       /// 00954
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0095c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00960
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00970
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00978
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00984
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0098c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00994
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0099c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009c0
			 0xcb000000,                                                  // -8388608.0                                  /// 009c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009cc
			 0x00000000,                                                  // zero                                        /// 009d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a30
			 0x33333333,                                                  // 4 random values                             /// 00a34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x7f800000,                                                  // inf                                         /// 00a80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ab8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00acc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ad0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00adc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00afc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0xcb000000,                                                  // -8388608.0                                  /// 00b64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x7fc00001,                                                  // signaling NaN                               /// 00b84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ba4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bfc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c14
			 0x80000000,                                                  // -zero                                       /// 00c18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000000,                                                  // zero                                        /// 00c20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0xff800000,                                                  // -inf                                        /// 00c30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c44
			 0x55555555,                                                  // 4 random values                             /// 00c48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ca0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ccc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ce4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d30
			 0xffc00001,                                                  // -signaling NaN                              /// 00d34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d9c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00db0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00db8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dcc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ddc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00de8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00df0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80000000,                                                  // -zero                                       /// 00e1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x3f028f5c,                                                  // 0.51                                        /// 00e48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e54
			 0x7f800000,                                                  // inf                                         /// 00e58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e5c
			 0x33333333,                                                  // 4 random values                             /// 00e60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ea4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ea8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec4
			 0x55555555,                                                  // 4 random values                             /// 00ec8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ecc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00edc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ee8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00efc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f00
			 0x7f800000,                                                  // inf                                         /// 00f04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0xff800000,                                                  // -inf                                        /// 00f7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0xcb000000,                                                  // -8388608.0                                  /// 00f88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80000000,                                                  // -zero                                       /// 00fc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fcc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fe8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ff8
			 0x80000001                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xf0837331, /// 0x0
			 0x6fb4b80b, /// 0x4
			 0x2926d39c, /// 0x8
			 0x722b18a6, /// 0xc
			 0x8b2790dd, /// 0x10
			 0x09d08a49, /// 0x14
			 0xeb9b697f, /// 0x18
			 0xbd30be6b, /// 0x1c
			 0x54f45908, /// 0x20
			 0x42f35496, /// 0x24
			 0x81dd0203, /// 0x28
			 0xbea4814a, /// 0x2c
			 0x0ceb7036, /// 0x30
			 0x5b500dc1, /// 0x34
			 0x217e1a14, /// 0x38
			 0x366fd0b5, /// 0x3c
			 0xb06e4428, /// 0x40
			 0x428c2597, /// 0x44
			 0x399c5fb0, /// 0x48
			 0xeee38822, /// 0x4c
			 0x16d93101, /// 0x50
			 0xbee977e7, /// 0x54
			 0xe08f8a70, /// 0x58
			 0x9d1be9c8, /// 0x5c
			 0xa8ee49e9, /// 0x60
			 0x44a61d76, /// 0x64
			 0x75b33a50, /// 0x68
			 0x3d738847, /// 0x6c
			 0xf3fed0fb, /// 0x70
			 0x846e67e5, /// 0x74
			 0x14a2dec3, /// 0x78
			 0x7ad812bd, /// 0x7c
			 0xf16ac341, /// 0x80
			 0xc858833b, /// 0x84
			 0x43984cc8, /// 0x88
			 0xa3832137, /// 0x8c
			 0xbf22ec63, /// 0x90
			 0xb48fe3c9, /// 0x94
			 0x0819b597, /// 0x98
			 0x5fcafa6a, /// 0x9c
			 0x34b27ca2, /// 0xa0
			 0x8f5dee13, /// 0xa4
			 0xf451d9d9, /// 0xa8
			 0x6d7c6d44, /// 0xac
			 0xeae0c306, /// 0xb0
			 0x623768ff, /// 0xb4
			 0xe5363237, /// 0xb8
			 0x3e870b7c, /// 0xbc
			 0x123b4454, /// 0xc0
			 0xcfcd04b6, /// 0xc4
			 0x19bf10d3, /// 0xc8
			 0xf354d757, /// 0xcc
			 0xdcad0aaa, /// 0xd0
			 0x8e7a661d, /// 0xd4
			 0x0a6d102d, /// 0xd8
			 0x34600047, /// 0xdc
			 0xc721ca56, /// 0xe0
			 0x5879a46e, /// 0xe4
			 0xe8ca89b0, /// 0xe8
			 0xd881af78, /// 0xec
			 0x0f00c3da, /// 0xf0
			 0xf3f2c67f, /// 0xf4
			 0xea317074, /// 0xf8
			 0x4a8fd883, /// 0xfc
			 0x7229112f, /// 0x100
			 0x9210f521, /// 0x104
			 0xd4b572a4, /// 0x108
			 0x47346ec1, /// 0x10c
			 0x7fd7d261, /// 0x110
			 0xd4b44572, /// 0x114
			 0x6bc2e99f, /// 0x118
			 0x543cc84f, /// 0x11c
			 0x0f124046, /// 0x120
			 0x958030b6, /// 0x124
			 0x3213fda7, /// 0x128
			 0x404eb645, /// 0x12c
			 0x3231e667, /// 0x130
			 0x8a8dbf1f, /// 0x134
			 0xdc04cd4a, /// 0x138
			 0x26600dcc, /// 0x13c
			 0x9890744a, /// 0x140
			 0x60d6cd3a, /// 0x144
			 0x8a1d5363, /// 0x148
			 0xd431bf5d, /// 0x14c
			 0xb2b24f58, /// 0x150
			 0xa675eb3a, /// 0x154
			 0x1e0bfedf, /// 0x158
			 0x915af2bb, /// 0x15c
			 0x12643e5b, /// 0x160
			 0xec397605, /// 0x164
			 0xe6c180d3, /// 0x168
			 0xd900da85, /// 0x16c
			 0x036c881f, /// 0x170
			 0x9f9a2288, /// 0x174
			 0x7ed5da54, /// 0x178
			 0x7f1cb7e5, /// 0x17c
			 0x16a6b2f8, /// 0x180
			 0xf652e97e, /// 0x184
			 0x33b61db1, /// 0x188
			 0x8f88012f, /// 0x18c
			 0x16cf6348, /// 0x190
			 0xf5731fba, /// 0x194
			 0x58f7e4aa, /// 0x198
			 0xf06c155e, /// 0x19c
			 0x1591d1cb, /// 0x1a0
			 0xcdc50828, /// 0x1a4
			 0x22a5b1f1, /// 0x1a8
			 0x44ba89a0, /// 0x1ac
			 0x9b174b0c, /// 0x1b0
			 0x2d78f2e6, /// 0x1b4
			 0x75b676b4, /// 0x1b8
			 0x7963e51e, /// 0x1bc
			 0x3902e6da, /// 0x1c0
			 0x26ba1d37, /// 0x1c4
			 0x6a7db878, /// 0x1c8
			 0x16ac70f1, /// 0x1cc
			 0xe50807a1, /// 0x1d0
			 0x96e5da07, /// 0x1d4
			 0x1c752cb0, /// 0x1d8
			 0x7d5445b0, /// 0x1dc
			 0x34380421, /// 0x1e0
			 0x3f7d6eba, /// 0x1e4
			 0x5a4bd4f7, /// 0x1e8
			 0x5934d121, /// 0x1ec
			 0x34ce393e, /// 0x1f0
			 0xe02dc552, /// 0x1f4
			 0x818fbf0a, /// 0x1f8
			 0x276661be, /// 0x1fc
			 0x0baeef0a, /// 0x200
			 0x2bcabc9d, /// 0x204
			 0x855f79e1, /// 0x208
			 0xde93529c, /// 0x20c
			 0x3159b9b5, /// 0x210
			 0xde8c87b9, /// 0x214
			 0xbb6bfcf9, /// 0x218
			 0x7bdf7af5, /// 0x21c
			 0xc01f49f3, /// 0x220
			 0x9b99dfcc, /// 0x224
			 0xfb337e9f, /// 0x228
			 0x76c0c8ac, /// 0x22c
			 0x9e78b978, /// 0x230
			 0xa2432161, /// 0x234
			 0x8f619cd2, /// 0x238
			 0xbf44b8a6, /// 0x23c
			 0x34896d36, /// 0x240
			 0x5995c188, /// 0x244
			 0xa58e9f15, /// 0x248
			 0x22367ee2, /// 0x24c
			 0xb85105e3, /// 0x250
			 0x0839f669, /// 0x254
			 0xf41b5468, /// 0x258
			 0x04d11e66, /// 0x25c
			 0x5df3e8a9, /// 0x260
			 0x5eb34271, /// 0x264
			 0x04850151, /// 0x268
			 0xa0019d7d, /// 0x26c
			 0xd55ddc23, /// 0x270
			 0xe9c1eb36, /// 0x274
			 0xe1d99889, /// 0x278
			 0x36f1979b, /// 0x27c
			 0xfb7f36fc, /// 0x280
			 0x764ad744, /// 0x284
			 0x420a79c9, /// 0x288
			 0x8985752b, /// 0x28c
			 0x74656916, /// 0x290
			 0x4ed9afb5, /// 0x294
			 0xa5241af9, /// 0x298
			 0xe0edc3ae, /// 0x29c
			 0x6c59154f, /// 0x2a0
			 0xb4c1baf8, /// 0x2a4
			 0x1c78b501, /// 0x2a8
			 0x4f23399d, /// 0x2ac
			 0x5a6905f7, /// 0x2b0
			 0xd601f92a, /// 0x2b4
			 0x2c4f8b09, /// 0x2b8
			 0x40e4d2a1, /// 0x2bc
			 0x6dbdc50b, /// 0x2c0
			 0xc95e4076, /// 0x2c4
			 0xae2e58a3, /// 0x2c8
			 0x698c8565, /// 0x2cc
			 0x42964d2c, /// 0x2d0
			 0x80f61688, /// 0x2d4
			 0xde382f3e, /// 0x2d8
			 0xd0ee0cd3, /// 0x2dc
			 0x37056021, /// 0x2e0
			 0xf4df0614, /// 0x2e4
			 0x4ae458b1, /// 0x2e8
			 0xca3b9641, /// 0x2ec
			 0xd2b22a15, /// 0x2f0
			 0x4485667e, /// 0x2f4
			 0xa2a71411, /// 0x2f8
			 0x4ffd30ee, /// 0x2fc
			 0x2a3ec007, /// 0x300
			 0xf6d13fce, /// 0x304
			 0xcfe0f4bc, /// 0x308
			 0x5d79ebf4, /// 0x30c
			 0x3e48c4c0, /// 0x310
			 0xba5dabf4, /// 0x314
			 0x9bcff7b9, /// 0x318
			 0x2fc8eecd, /// 0x31c
			 0xa75dba73, /// 0x320
			 0x6ad00fe4, /// 0x324
			 0xbcfbf441, /// 0x328
			 0x39e6689f, /// 0x32c
			 0xff4258d4, /// 0x330
			 0xa3f4767f, /// 0x334
			 0xb71644e1, /// 0x338
			 0x4ea80743, /// 0x33c
			 0x87770e39, /// 0x340
			 0xf5d9a36d, /// 0x344
			 0xff87d90b, /// 0x348
			 0x3533156b, /// 0x34c
			 0xcb4593f6, /// 0x350
			 0xa6e75e09, /// 0x354
			 0xbbc979c8, /// 0x358
			 0x22f222c2, /// 0x35c
			 0x49828c3e, /// 0x360
			 0x2dbbd189, /// 0x364
			 0xcbea9aa5, /// 0x368
			 0x902cfb62, /// 0x36c
			 0xef856310, /// 0x370
			 0x6f6c635e, /// 0x374
			 0x68d14909, /// 0x378
			 0x38d59f80, /// 0x37c
			 0xf909d38f, /// 0x380
			 0x61ab828a, /// 0x384
			 0x1dc56ace, /// 0x388
			 0xa40d72e6, /// 0x38c
			 0x1646478c, /// 0x390
			 0xc3d5d5ea, /// 0x394
			 0xee13be0e, /// 0x398
			 0x01766b05, /// 0x39c
			 0x15a11d24, /// 0x3a0
			 0x6172c622, /// 0x3a4
			 0xd7af4573, /// 0x3a8
			 0x888d939e, /// 0x3ac
			 0x944ab0ea, /// 0x3b0
			 0x38c81959, /// 0x3b4
			 0x787e5968, /// 0x3b8
			 0x9724996f, /// 0x3bc
			 0xbb190f86, /// 0x3c0
			 0x9c09ef55, /// 0x3c4
			 0x19780ec4, /// 0x3c8
			 0x68453949, /// 0x3cc
			 0xd7384ddd, /// 0x3d0
			 0xb0cc5938, /// 0x3d4
			 0x9c86d136, /// 0x3d8
			 0x942bee67, /// 0x3dc
			 0xad7fcf2f, /// 0x3e0
			 0xf498af87, /// 0x3e4
			 0x8655a5f9, /// 0x3e8
			 0x4ff73a64, /// 0x3ec
			 0x20a1c90c, /// 0x3f0
			 0x1b6f82d0, /// 0x3f4
			 0xfbd54207, /// 0x3f8
			 0x3c12bf65, /// 0x3fc
			 0x33fd4307, /// 0x400
			 0x22d4b858, /// 0x404
			 0xb5395505, /// 0x408
			 0x0b633e65, /// 0x40c
			 0x9b41bbb7, /// 0x410
			 0xf1ba6fba, /// 0x414
			 0x041a3c4a, /// 0x418
			 0xc0b8012c, /// 0x41c
			 0x64c4abe5, /// 0x420
			 0x471aaa74, /// 0x424
			 0x84a9c838, /// 0x428
			 0xf274776c, /// 0x42c
			 0x46feb270, /// 0x430
			 0x19936a58, /// 0x434
			 0xf99ea6b7, /// 0x438
			 0x8841da82, /// 0x43c
			 0x8153d56e, /// 0x440
			 0xc2571d68, /// 0x444
			 0xe0d61c09, /// 0x448
			 0x3287dfa9, /// 0x44c
			 0x4ea0a7a2, /// 0x450
			 0x42f6eae6, /// 0x454
			 0x8ee0a6d0, /// 0x458
			 0xe647766e, /// 0x45c
			 0x1d21f99e, /// 0x460
			 0xcb0bd882, /// 0x464
			 0xd2914e16, /// 0x468
			 0xb8bc2f50, /// 0x46c
			 0x204002db, /// 0x470
			 0xe1967080, /// 0x474
			 0xe3906925, /// 0x478
			 0xb388f69f, /// 0x47c
			 0x54cc69cd, /// 0x480
			 0x47b1b524, /// 0x484
			 0x829a63be, /// 0x488
			 0x300506d6, /// 0x48c
			 0xb13a9d86, /// 0x490
			 0x71a55075, /// 0x494
			 0x7592b695, /// 0x498
			 0x387a1552, /// 0x49c
			 0x599e9544, /// 0x4a0
			 0xab527bf4, /// 0x4a4
			 0x3e9a88cb, /// 0x4a8
			 0xb6ed4432, /// 0x4ac
			 0x895488dd, /// 0x4b0
			 0x4647eb6c, /// 0x4b4
			 0xff14b141, /// 0x4b8
			 0xb40f2216, /// 0x4bc
			 0xf8232891, /// 0x4c0
			 0xfe743f2d, /// 0x4c4
			 0xc3916059, /// 0x4c8
			 0x4cbf60f7, /// 0x4cc
			 0xfadb4d3f, /// 0x4d0
			 0x43652f9e, /// 0x4d4
			 0xcafac4d8, /// 0x4d8
			 0x13a70605, /// 0x4dc
			 0xc1eefca3, /// 0x4e0
			 0xb42966f9, /// 0x4e4
			 0x99ee24b4, /// 0x4e8
			 0xd855171b, /// 0x4ec
			 0x324c8397, /// 0x4f0
			 0x56f4f6d0, /// 0x4f4
			 0x9166fb6e, /// 0x4f8
			 0x38ee40f7, /// 0x4fc
			 0x363881b9, /// 0x500
			 0x084d30fe, /// 0x504
			 0xa45fbe5a, /// 0x508
			 0x147738dd, /// 0x50c
			 0x63def5b5, /// 0x510
			 0x5b90eb25, /// 0x514
			 0x8af82d77, /// 0x518
			 0xb408d6a8, /// 0x51c
			 0x5ed0397d, /// 0x520
			 0x6d832fa4, /// 0x524
			 0x11957d94, /// 0x528
			 0x249ec701, /// 0x52c
			 0x13c62e94, /// 0x530
			 0x3bdb3945, /// 0x534
			 0x2614e284, /// 0x538
			 0xadd4485f, /// 0x53c
			 0x4a3732a6, /// 0x540
			 0xe53f75de, /// 0x544
			 0xab471455, /// 0x548
			 0x8241addc, /// 0x54c
			 0x7d939a7f, /// 0x550
			 0x2ada0502, /// 0x554
			 0x0c08383f, /// 0x558
			 0x3f5626db, /// 0x55c
			 0x5b8ea0a8, /// 0x560
			 0xcf35bcd1, /// 0x564
			 0x94649201, /// 0x568
			 0x27d329c1, /// 0x56c
			 0xe6af9750, /// 0x570
			 0x22cec152, /// 0x574
			 0xc78239a0, /// 0x578
			 0x85e28ed0, /// 0x57c
			 0x668360cc, /// 0x580
			 0xb0e64efc, /// 0x584
			 0x87c403e3, /// 0x588
			 0xc01dbdd8, /// 0x58c
			 0xca35525f, /// 0x590
			 0x2fb7ae77, /// 0x594
			 0xd9ff1b6d, /// 0x598
			 0x838c67f0, /// 0x59c
			 0xaf244f04, /// 0x5a0
			 0x7c764188, /// 0x5a4
			 0x203476cd, /// 0x5a8
			 0xe66dbf6b, /// 0x5ac
			 0x5bcf3516, /// 0x5b0
			 0x52ac4634, /// 0x5b4
			 0xecc7da29, /// 0x5b8
			 0xcb6dc950, /// 0x5bc
			 0x13644c24, /// 0x5c0
			 0x63d6fc1a, /// 0x5c4
			 0xe319acec, /// 0x5c8
			 0x0e8cdb79, /// 0x5cc
			 0x7e4b7c7f, /// 0x5d0
			 0x452c07e2, /// 0x5d4
			 0xf8a360d6, /// 0x5d8
			 0xe7e81e72, /// 0x5dc
			 0xfead151d, /// 0x5e0
			 0x4f8cb687, /// 0x5e4
			 0x1eb06d05, /// 0x5e8
			 0x8827d826, /// 0x5ec
			 0xacd8264c, /// 0x5f0
			 0x0502ac2a, /// 0x5f4
			 0xdca9fdce, /// 0x5f8
			 0xa02ec397, /// 0x5fc
			 0x69b83333, /// 0x600
			 0x562f02f1, /// 0x604
			 0x1835b20f, /// 0x608
			 0x3ccd6337, /// 0x60c
			 0x5c2cd2f0, /// 0x610
			 0x2ec169e9, /// 0x614
			 0x1cf2edee, /// 0x618
			 0x693f8259, /// 0x61c
			 0x53baa428, /// 0x620
			 0x6ff62137, /// 0x624
			 0x7145bae4, /// 0x628
			 0xb5a6b6fc, /// 0x62c
			 0x24d1f23f, /// 0x630
			 0x557125ce, /// 0x634
			 0x885d372f, /// 0x638
			 0x39f3bec5, /// 0x63c
			 0x202f9bfa, /// 0x640
			 0x52930a27, /// 0x644
			 0xcc980597, /// 0x648
			 0x7e381f3f, /// 0x64c
			 0xdfc206c5, /// 0x650
			 0x38e5a814, /// 0x654
			 0xab6364b2, /// 0x658
			 0x45569310, /// 0x65c
			 0x18aceeb7, /// 0x660
			 0x97a253ed, /// 0x664
			 0x988ebd74, /// 0x668
			 0xc57163f8, /// 0x66c
			 0xeb5e2be4, /// 0x670
			 0x3709bfb5, /// 0x674
			 0x91db49dc, /// 0x678
			 0x1581520f, /// 0x67c
			 0x55d01618, /// 0x680
			 0x34a00338, /// 0x684
			 0xf76c2ad4, /// 0x688
			 0x613cfdc7, /// 0x68c
			 0xb49d0c46, /// 0x690
			 0xeede82e9, /// 0x694
			 0x68ccb8f5, /// 0x698
			 0xe041836d, /// 0x69c
			 0xe30ad745, /// 0x6a0
			 0x35bfb429, /// 0x6a4
			 0x3920e171, /// 0x6a8
			 0x8baaec6d, /// 0x6ac
			 0x2efd892b, /// 0x6b0
			 0x026c4e82, /// 0x6b4
			 0x26ade830, /// 0x6b8
			 0x4878ff53, /// 0x6bc
			 0x75f3a3bb, /// 0x6c0
			 0x2882107a, /// 0x6c4
			 0xc2e0676d, /// 0x6c8
			 0xf18cd02c, /// 0x6cc
			 0x8768bd09, /// 0x6d0
			 0xb4f876b0, /// 0x6d4
			 0x1b257137, /// 0x6d8
			 0xd329c25f, /// 0x6dc
			 0x99df5725, /// 0x6e0
			 0xb5da4d04, /// 0x6e4
			 0x7de964c2, /// 0x6e8
			 0xbcc16544, /// 0x6ec
			 0xc3a8c35f, /// 0x6f0
			 0x3ac85e5a, /// 0x6f4
			 0x1a53ab7f, /// 0x6f8
			 0x6e34e7aa, /// 0x6fc
			 0xb2509a7c, /// 0x700
			 0x648248f5, /// 0x704
			 0xef6eca63, /// 0x708
			 0x3560c7af, /// 0x70c
			 0x95e27f98, /// 0x710
			 0xd79c0efe, /// 0x714
			 0x11dbc177, /// 0x718
			 0x6d42875b, /// 0x71c
			 0xd4d22f65, /// 0x720
			 0x14a7026c, /// 0x724
			 0x4307fc8e, /// 0x728
			 0xeebdbd55, /// 0x72c
			 0xc67a3015, /// 0x730
			 0x6f029379, /// 0x734
			 0x99792c7a, /// 0x738
			 0xe5223da4, /// 0x73c
			 0x6f1265e2, /// 0x740
			 0xbc7e89e6, /// 0x744
			 0xd50e36bb, /// 0x748
			 0x847cc1b2, /// 0x74c
			 0x266aa44d, /// 0x750
			 0x56909705, /// 0x754
			 0xa61528c1, /// 0x758
			 0x3dc5c80c, /// 0x75c
			 0xeacd01ba, /// 0x760
			 0x8e20d58b, /// 0x764
			 0x1348e375, /// 0x768
			 0xf448e201, /// 0x76c
			 0x634706b7, /// 0x770
			 0x6b40e98e, /// 0x774
			 0x64791761, /// 0x778
			 0xb80b5124, /// 0x77c
			 0x9ea17b9f, /// 0x780
			 0x0da77589, /// 0x784
			 0xeb79b84a, /// 0x788
			 0x4833918e, /// 0x78c
			 0x27437ed7, /// 0x790
			 0x4c3e4060, /// 0x794
			 0xa7246518, /// 0x798
			 0xf1ef6d6e, /// 0x79c
			 0xfbc69433, /// 0x7a0
			 0x2462c809, /// 0x7a4
			 0xd52895b8, /// 0x7a8
			 0xc29c9287, /// 0x7ac
			 0xf3d5ee9b, /// 0x7b0
			 0x9e8df107, /// 0x7b4
			 0x4448210a, /// 0x7b8
			 0xff26957e, /// 0x7bc
			 0xbf854fa1, /// 0x7c0
			 0x00166b4d, /// 0x7c4
			 0xb7a6e561, /// 0x7c8
			 0x582074ad, /// 0x7cc
			 0x4011ed11, /// 0x7d0
			 0xe9666420, /// 0x7d4
			 0x9c4770c0, /// 0x7d8
			 0xe0ad4036, /// 0x7dc
			 0xff771fc6, /// 0x7e0
			 0x71db7c4e, /// 0x7e4
			 0xb28a39ba, /// 0x7e8
			 0x2813d906, /// 0x7ec
			 0x959f3c75, /// 0x7f0
			 0xe5017d29, /// 0x7f4
			 0xa3fb2633, /// 0x7f8
			 0xb4784a02, /// 0x7fc
			 0x3afaf420, /// 0x800
			 0xeaeb1f1f, /// 0x804
			 0xa5afe9e9, /// 0x808
			 0x3faffb5a, /// 0x80c
			 0x0b867bfa, /// 0x810
			 0xd7834445, /// 0x814
			 0x28964e55, /// 0x818
			 0xa0fb9eb7, /// 0x81c
			 0x9a654b63, /// 0x820
			 0x753b4211, /// 0x824
			 0x2961d90c, /// 0x828
			 0xc11819ff, /// 0x82c
			 0x2ec70f09, /// 0x830
			 0xa80f9f9f, /// 0x834
			 0x6a772322, /// 0x838
			 0x35cabf09, /// 0x83c
			 0x38ec6610, /// 0x840
			 0x11b24745, /// 0x844
			 0x7c4f4480, /// 0x848
			 0x39115b87, /// 0x84c
			 0x2ddf11c8, /// 0x850
			 0x73cabf77, /// 0x854
			 0x90103165, /// 0x858
			 0x81972ada, /// 0x85c
			 0xd429eac7, /// 0x860
			 0x27d5f6a0, /// 0x864
			 0x109949b4, /// 0x868
			 0xf8e8ed85, /// 0x86c
			 0xb87c4a35, /// 0x870
			 0x294120f1, /// 0x874
			 0x19860b98, /// 0x878
			 0xd1be95ad, /// 0x87c
			 0xad4cf393, /// 0x880
			 0xfc579a41, /// 0x884
			 0x90c4d0b5, /// 0x888
			 0x3d3ed5e9, /// 0x88c
			 0xa4cbb047, /// 0x890
			 0x65ccb33f, /// 0x894
			 0x2d620ccb, /// 0x898
			 0x1b616070, /// 0x89c
			 0x98c422a2, /// 0x8a0
			 0x7f0e5476, /// 0x8a4
			 0x84b6a596, /// 0x8a8
			 0xc3c131c9, /// 0x8ac
			 0x8411c376, /// 0x8b0
			 0x0ca07b38, /// 0x8b4
			 0xa96cf911, /// 0x8b8
			 0x97b601b7, /// 0x8bc
			 0x13e492d5, /// 0x8c0
			 0x4adf4298, /// 0x8c4
			 0x0c04e50e, /// 0x8c8
			 0xebde0c06, /// 0x8cc
			 0x823459db, /// 0x8d0
			 0xf845494a, /// 0x8d4
			 0x9ab359ec, /// 0x8d8
			 0xa7480aa1, /// 0x8dc
			 0x198a61a6, /// 0x8e0
			 0xb44df599, /// 0x8e4
			 0x9c7a92f1, /// 0x8e8
			 0x737499a0, /// 0x8ec
			 0x4ad709f5, /// 0x8f0
			 0x45ab3c62, /// 0x8f4
			 0xda5e08ec, /// 0x8f8
			 0xf633f567, /// 0x8fc
			 0x991042db, /// 0x900
			 0xb649448e, /// 0x904
			 0x64a82627, /// 0x908
			 0x77d170fa, /// 0x90c
			 0x2bf7d4e6, /// 0x910
			 0xd9426e45, /// 0x914
			 0x16abc00d, /// 0x918
			 0x462e916a, /// 0x91c
			 0x7c38bafc, /// 0x920
			 0xa29225d9, /// 0x924
			 0x8a73768e, /// 0x928
			 0x0548e665, /// 0x92c
			 0x323cf519, /// 0x930
			 0x4751374b, /// 0x934
			 0x362657c1, /// 0x938
			 0xd910f987, /// 0x93c
			 0xf499472e, /// 0x940
			 0x59bdad11, /// 0x944
			 0x6fa8249c, /// 0x948
			 0xc13ddaec, /// 0x94c
			 0xbdd660b6, /// 0x950
			 0x27d49496, /// 0x954
			 0x8fc12a9d, /// 0x958
			 0x28042d7e, /// 0x95c
			 0x6a79dddd, /// 0x960
			 0xa5739936, /// 0x964
			 0x34d9065a, /// 0x968
			 0xb2dbb678, /// 0x96c
			 0x68880d2c, /// 0x970
			 0x7ac20505, /// 0x974
			 0xf95aeb6d, /// 0x978
			 0xa43c8555, /// 0x97c
			 0xb7ff0b70, /// 0x980
			 0x0b981f19, /// 0x984
			 0xb83dba1c, /// 0x988
			 0x644d29ff, /// 0x98c
			 0xe0b36fed, /// 0x990
			 0x84243aab, /// 0x994
			 0x47d45ff3, /// 0x998
			 0xda9b0fda, /// 0x99c
			 0xc877c2e4, /// 0x9a0
			 0xbc584c0f, /// 0x9a4
			 0x0ec1a0a3, /// 0x9a8
			 0x567933c2, /// 0x9ac
			 0x14c28de4, /// 0x9b0
			 0x33b55403, /// 0x9b4
			 0x51decfcd, /// 0x9b8
			 0x1a218566, /// 0x9bc
			 0x52a15e55, /// 0x9c0
			 0xd818fc46, /// 0x9c4
			 0xb7065935, /// 0x9c8
			 0xc011369e, /// 0x9cc
			 0xf1fc7544, /// 0x9d0
			 0x1d158243, /// 0x9d4
			 0x9f4ead87, /// 0x9d8
			 0x023f0540, /// 0x9dc
			 0x92ee2861, /// 0x9e0
			 0xb858df9f, /// 0x9e4
			 0x93873ba5, /// 0x9e8
			 0xe7503254, /// 0x9ec
			 0x4ed3a277, /// 0x9f0
			 0x524cd666, /// 0x9f4
			 0x09ccdd6c, /// 0x9f8
			 0x6018a335, /// 0x9fc
			 0xb0114a87, /// 0xa00
			 0xdbf81590, /// 0xa04
			 0xb971b152, /// 0xa08
			 0x89c8a8b4, /// 0xa0c
			 0xfe0f637e, /// 0xa10
			 0x94826de4, /// 0xa14
			 0x8ac69524, /// 0xa18
			 0x2aad84a8, /// 0xa1c
			 0x0215054c, /// 0xa20
			 0xe5d5a9e9, /// 0xa24
			 0xddbcbd04, /// 0xa28
			 0x112f3dd9, /// 0xa2c
			 0x5838b8d0, /// 0xa30
			 0x32ba6b76, /// 0xa34
			 0x27b815a9, /// 0xa38
			 0x7043ae50, /// 0xa3c
			 0x74615b46, /// 0xa40
			 0xa27ba170, /// 0xa44
			 0x23ea5d44, /// 0xa48
			 0xac99b2d9, /// 0xa4c
			 0xb8a6d0f5, /// 0xa50
			 0x34db6698, /// 0xa54
			 0x18edb520, /// 0xa58
			 0xe9d818cd, /// 0xa5c
			 0x454dd5b9, /// 0xa60
			 0xac0d5e88, /// 0xa64
			 0x292a6466, /// 0xa68
			 0x1b79b941, /// 0xa6c
			 0xda097e48, /// 0xa70
			 0x9cec2587, /// 0xa74
			 0xe594f6c9, /// 0xa78
			 0x38344e40, /// 0xa7c
			 0x33db13c7, /// 0xa80
			 0x175d8464, /// 0xa84
			 0xc5faf8f3, /// 0xa88
			 0x20285b35, /// 0xa8c
			 0x8dc84827, /// 0xa90
			 0xd88f170b, /// 0xa94
			 0xef95e010, /// 0xa98
			 0x8fed186c, /// 0xa9c
			 0xa14d0450, /// 0xaa0
			 0xa4d91f2d, /// 0xaa4
			 0x3b36d47f, /// 0xaa8
			 0x6f784f5d, /// 0xaac
			 0x24a640a0, /// 0xab0
			 0xfd938e8b, /// 0xab4
			 0xfcdc74fe, /// 0xab8
			 0x6814e85f, /// 0xabc
			 0x9805a65b, /// 0xac0
			 0x2a68a89b, /// 0xac4
			 0xcb1f3d2f, /// 0xac8
			 0x7f8f9b7e, /// 0xacc
			 0x430fe95a, /// 0xad0
			 0x35530e87, /// 0xad4
			 0xf98cb23a, /// 0xad8
			 0x2e9a8b9f, /// 0xadc
			 0x341e5169, /// 0xae0
			 0xe1c21fbc, /// 0xae4
			 0xd9f5776b, /// 0xae8
			 0xc5e869cd, /// 0xaec
			 0x3e06b039, /// 0xaf0
			 0xec911507, /// 0xaf4
			 0x61dc3840, /// 0xaf8
			 0x0211f7f0, /// 0xafc
			 0x4d946862, /// 0xb00
			 0x8306f512, /// 0xb04
			 0xf1aeeb09, /// 0xb08
			 0x9e9066a2, /// 0xb0c
			 0x73b3ec9d, /// 0xb10
			 0x76beb8d0, /// 0xb14
			 0xd3786ca3, /// 0xb18
			 0x58f09265, /// 0xb1c
			 0x2f8f5cfd, /// 0xb20
			 0x089c2ea8, /// 0xb24
			 0x409be504, /// 0xb28
			 0x3e65601a, /// 0xb2c
			 0x25b96ec5, /// 0xb30
			 0xe86aeaea, /// 0xb34
			 0x7db088b7, /// 0xb38
			 0xee250b24, /// 0xb3c
			 0x641b6e86, /// 0xb40
			 0x346547c9, /// 0xb44
			 0x69557167, /// 0xb48
			 0x38a97d47, /// 0xb4c
			 0x2d2ccfe1, /// 0xb50
			 0xeb64af82, /// 0xb54
			 0xc59fac63, /// 0xb58
			 0xadd37294, /// 0xb5c
			 0xd13465c2, /// 0xb60
			 0x611aa3fe, /// 0xb64
			 0x389614e5, /// 0xb68
			 0x0f789b43, /// 0xb6c
			 0xdc9244f0, /// 0xb70
			 0x4bc94bf5, /// 0xb74
			 0x1899073a, /// 0xb78
			 0x977a2885, /// 0xb7c
			 0xa6333427, /// 0xb80
			 0xaa3956dc, /// 0xb84
			 0x5ffc0be4, /// 0xb88
			 0xc593c8c5, /// 0xb8c
			 0xe3034cd2, /// 0xb90
			 0xab45db24, /// 0xb94
			 0x0f9c0f84, /// 0xb98
			 0x860aa05c, /// 0xb9c
			 0xc3d4249d, /// 0xba0
			 0x8e34e431, /// 0xba4
			 0x10a296fd, /// 0xba8
			 0x6a36172e, /// 0xbac
			 0xa612e328, /// 0xbb0
			 0x4e0a9a8c, /// 0xbb4
			 0xb640c066, /// 0xbb8
			 0x3ec58bf3, /// 0xbbc
			 0xb5761810, /// 0xbc0
			 0x304e44cb, /// 0xbc4
			 0x041ed726, /// 0xbc8
			 0x6416ff9f, /// 0xbcc
			 0x4c3f73eb, /// 0xbd0
			 0xce792d90, /// 0xbd4
			 0x95ac20ee, /// 0xbd8
			 0xe17ccf48, /// 0xbdc
			 0x169cd7bb, /// 0xbe0
			 0x0fc8d7c7, /// 0xbe4
			 0xdf071931, /// 0xbe8
			 0x70dd6868, /// 0xbec
			 0xd61c4973, /// 0xbf0
			 0xa1c2b989, /// 0xbf4
			 0xbc586933, /// 0xbf8
			 0xff0ebcd1, /// 0xbfc
			 0x3eb9402f, /// 0xc00
			 0x3b76c8ef, /// 0xc04
			 0x74f57baa, /// 0xc08
			 0x8f594d1d, /// 0xc0c
			 0xbe47018a, /// 0xc10
			 0xc21689a1, /// 0xc14
			 0x5995af45, /// 0xc18
			 0xbc7d0179, /// 0xc1c
			 0xf8f739d6, /// 0xc20
			 0x2e3e1958, /// 0xc24
			 0xe45cb544, /// 0xc28
			 0xa7140e8e, /// 0xc2c
			 0xce829784, /// 0xc30
			 0xf1e9852a, /// 0xc34
			 0x6764c2b1, /// 0xc38
			 0x94b8f744, /// 0xc3c
			 0x3aa10862, /// 0xc40
			 0xec3b2a31, /// 0xc44
			 0x7fca7a52, /// 0xc48
			 0xede1d8a5, /// 0xc4c
			 0xc5bf2f76, /// 0xc50
			 0x0a41e09c, /// 0xc54
			 0x72d895a7, /// 0xc58
			 0x8a583bdc, /// 0xc5c
			 0xcf02da85, /// 0xc60
			 0xb86bb10d, /// 0xc64
			 0xf75ab138, /// 0xc68
			 0x1125315b, /// 0xc6c
			 0x860367d5, /// 0xc70
			 0x484a3eb1, /// 0xc74
			 0x0e41543f, /// 0xc78
			 0x4b120a92, /// 0xc7c
			 0x560c4671, /// 0xc80
			 0xe319fa54, /// 0xc84
			 0xfc2aa446, /// 0xc88
			 0x0fba2773, /// 0xc8c
			 0x868dc6ab, /// 0xc90
			 0xff7ba5da, /// 0xc94
			 0xa0a7ab46, /// 0xc98
			 0x6fb4a1cb, /// 0xc9c
			 0xb7f67934, /// 0xca0
			 0x7a248847, /// 0xca4
			 0xf214fd2c, /// 0xca8
			 0x3a7ff9e7, /// 0xcac
			 0xf99e2049, /// 0xcb0
			 0x3f65b8c6, /// 0xcb4
			 0x21d67d18, /// 0xcb8
			 0x6fc06809, /// 0xcbc
			 0x4a18f0bd, /// 0xcc0
			 0x8f8b9f22, /// 0xcc4
			 0xb7086e8d, /// 0xcc8
			 0x61c3e7bf, /// 0xccc
			 0x05e870a7, /// 0xcd0
			 0x781509d4, /// 0xcd4
			 0x38b77666, /// 0xcd8
			 0xd85c3748, /// 0xcdc
			 0x27d7a0ff, /// 0xce0
			 0x464a7c53, /// 0xce4
			 0x045949b4, /// 0xce8
			 0xb6d907c8, /// 0xcec
			 0x3ab24c9f, /// 0xcf0
			 0x3ad600ae, /// 0xcf4
			 0x0c067d0d, /// 0xcf8
			 0x3f42dd46, /// 0xcfc
			 0x363403d6, /// 0xd00
			 0xcb0c3e7d, /// 0xd04
			 0x65790a97, /// 0xd08
			 0xba2e0028, /// 0xd0c
			 0xebd8a536, /// 0xd10
			 0x52729d0b, /// 0xd14
			 0xa27ddf05, /// 0xd18
			 0x41c603f6, /// 0xd1c
			 0xfc628b5c, /// 0xd20
			 0x44010ba2, /// 0xd24
			 0x10979cd7, /// 0xd28
			 0xc80d171a, /// 0xd2c
			 0x3ebb5dff, /// 0xd30
			 0xe9e1e1a7, /// 0xd34
			 0x3fd4c073, /// 0xd38
			 0x7c316c1b, /// 0xd3c
			 0x8cc3fadd, /// 0xd40
			 0x23403d20, /// 0xd44
			 0x0cd6cbbe, /// 0xd48
			 0x82ee8a68, /// 0xd4c
			 0xa593e778, /// 0xd50
			 0xb735b98c, /// 0xd54
			 0x867f7bb1, /// 0xd58
			 0x971a9407, /// 0xd5c
			 0x883dd3cb, /// 0xd60
			 0x4285beb2, /// 0xd64
			 0x2eae5f1f, /// 0xd68
			 0x8530b84f, /// 0xd6c
			 0xd8fa0c75, /// 0xd70
			 0x5f7cd350, /// 0xd74
			 0x3b183945, /// 0xd78
			 0x663d6114, /// 0xd7c
			 0x626f7663, /// 0xd80
			 0xc1c043a7, /// 0xd84
			 0x4456c30e, /// 0xd88
			 0xcdeee973, /// 0xd8c
			 0x69773a24, /// 0xd90
			 0xc05c761c, /// 0xd94
			 0x56113070, /// 0xd98
			 0xd1101f35, /// 0xd9c
			 0x889cf73f, /// 0xda0
			 0x7d342f1d, /// 0xda4
			 0xea1caaf2, /// 0xda8
			 0xc6c4bbbb, /// 0xdac
			 0xc7f05e72, /// 0xdb0
			 0xad95ba74, /// 0xdb4
			 0x1ea26d31, /// 0xdb8
			 0x3a5d5a78, /// 0xdbc
			 0x9c3b73dd, /// 0xdc0
			 0xb93399f5, /// 0xdc4
			 0xbe4fdcda, /// 0xdc8
			 0x1e428b73, /// 0xdcc
			 0x98c2c7c0, /// 0xdd0
			 0xdcb67a32, /// 0xdd4
			 0xd64f7115, /// 0xdd8
			 0x5916956b, /// 0xddc
			 0xccd761c4, /// 0xde0
			 0x35a7c474, /// 0xde4
			 0xbb578fa0, /// 0xde8
			 0xa29a9432, /// 0xdec
			 0x21177569, /// 0xdf0
			 0x3a820605, /// 0xdf4
			 0xa393b645, /// 0xdf8
			 0xa66eedf0, /// 0xdfc
			 0x8f921881, /// 0xe00
			 0xc22dbaa5, /// 0xe04
			 0x075e4e90, /// 0xe08
			 0x1995570d, /// 0xe0c
			 0xd38861e8, /// 0xe10
			 0x4f42c1f3, /// 0xe14
			 0x41a7c262, /// 0xe18
			 0x26c586b5, /// 0xe1c
			 0x171ca043, /// 0xe20
			 0x07565913, /// 0xe24
			 0xbb0afa1b, /// 0xe28
			 0xc9035b8d, /// 0xe2c
			 0xdeedcc06, /// 0xe30
			 0xde337e57, /// 0xe34
			 0xf7ea3845, /// 0xe38
			 0x28a55f7b, /// 0xe3c
			 0x864d1970, /// 0xe40
			 0x9aee9389, /// 0xe44
			 0x4561963e, /// 0xe48
			 0x25e86f94, /// 0xe4c
			 0xad5f582b, /// 0xe50
			 0xbb213588, /// 0xe54
			 0xa9d7dfe6, /// 0xe58
			 0x147e57ff, /// 0xe5c
			 0x1dda899f, /// 0xe60
			 0x5d4ce9a8, /// 0xe64
			 0xa3b0baeb, /// 0xe68
			 0x95d52eee, /// 0xe6c
			 0x0bb811e9, /// 0xe70
			 0xaef358b8, /// 0xe74
			 0xcaacc5e4, /// 0xe78
			 0xf31d3f98, /// 0xe7c
			 0xdc9086ad, /// 0xe80
			 0x6e3c56bc, /// 0xe84
			 0x6b9c19a2, /// 0xe88
			 0xe4ffebb7, /// 0xe8c
			 0x8e511db3, /// 0xe90
			 0x4fb97d22, /// 0xe94
			 0x2b50c7e1, /// 0xe98
			 0xcf7b411b, /// 0xe9c
			 0x594a866a, /// 0xea0
			 0x4ed0c00b, /// 0xea4
			 0x51798c88, /// 0xea8
			 0x0d36e10b, /// 0xeac
			 0xcec1ef03, /// 0xeb0
			 0x75b82237, /// 0xeb4
			 0x4b5b6b0d, /// 0xeb8
			 0x672970f2, /// 0xebc
			 0x35877cc4, /// 0xec0
			 0x8815f6c5, /// 0xec4
			 0x15c38225, /// 0xec8
			 0xce570724, /// 0xecc
			 0x12b99df3, /// 0xed0
			 0x3c458f98, /// 0xed4
			 0x1ba4fbe6, /// 0xed8
			 0x56060b23, /// 0xedc
			 0x3e9c6beb, /// 0xee0
			 0x4ecb63c9, /// 0xee4
			 0xd9131846, /// 0xee8
			 0x7700780b, /// 0xeec
			 0x0b8c0e3c, /// 0xef0
			 0x8e8ddb6d, /// 0xef4
			 0x04451ef8, /// 0xef8
			 0xe0ea3c6c, /// 0xefc
			 0x653ad7ab, /// 0xf00
			 0x2b6335e4, /// 0xf04
			 0x2d9855e0, /// 0xf08
			 0xca7d54b2, /// 0xf0c
			 0x0eb93ce9, /// 0xf10
			 0x4da77550, /// 0xf14
			 0x16ef20c9, /// 0xf18
			 0xd5fe7c5f, /// 0xf1c
			 0x867c5494, /// 0xf20
			 0x01dc0bba, /// 0xf24
			 0xf636db99, /// 0xf28
			 0x489d4f35, /// 0xf2c
			 0xfb792dd9, /// 0xf30
			 0xc5998681, /// 0xf34
			 0xf03c8a75, /// 0xf38
			 0x2521bfb4, /// 0xf3c
			 0x4c91f94e, /// 0xf40
			 0x8dfd2530, /// 0xf44
			 0xff713400, /// 0xf48
			 0x79eea7e0, /// 0xf4c
			 0xbea4aafe, /// 0xf50
			 0x8d73779c, /// 0xf54
			 0x12080327, /// 0xf58
			 0xc48e9caa, /// 0xf5c
			 0xabd805f1, /// 0xf60
			 0xc47d415d, /// 0xf64
			 0xdf058e48, /// 0xf68
			 0x61fbfd01, /// 0xf6c
			 0x22e1322b, /// 0xf70
			 0x3e45b709, /// 0xf74
			 0xabf41a73, /// 0xf78
			 0x2f980aa0, /// 0xf7c
			 0x17ddc66e, /// 0xf80
			 0xe1981a2b, /// 0xf84
			 0x85d74ae7, /// 0xf88
			 0x4736785c, /// 0xf8c
			 0x0180fe37, /// 0xf90
			 0xffd6e779, /// 0xf94
			 0x8375c181, /// 0xf98
			 0x5e9f6413, /// 0xf9c
			 0x636c9c05, /// 0xfa0
			 0xf59f17f8, /// 0xfa4
			 0xc01bd36a, /// 0xfa8
			 0xa1bfcfb3, /// 0xfac
			 0x79eadfc7, /// 0xfb0
			 0x4bd2bfc9, /// 0xfb4
			 0x1a5b25eb, /// 0xfb8
			 0x285b37a0, /// 0xfbc
			 0x307c0b81, /// 0xfc0
			 0xdf774c4d, /// 0xfc4
			 0x6e01ca9f, /// 0xfc8
			 0x3a6d4270, /// 0xfcc
			 0x7a61a2b3, /// 0xfd0
			 0x8e621136, /// 0xfd4
			 0x03663124, /// 0xfd8
			 0x7f9809d3, /// 0xfdc
			 0xaf25e187, /// 0xfe0
			 0x650bac15, /// 0xfe4
			 0xeb011068, /// 0xfe8
			 0x159bdcb9, /// 0xfec
			 0x76513a67, /// 0xff0
			 0x33ac206e, /// 0xff4
			 0x5a1c7ae3, /// 0xff8
			 0x1441b46c /// last
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
			 0x00000500,
			 0x000006c4,
			 0x000006c8,
			 0x00000790,
			 0x000001e0,
			 0x000002a0,
			 0x0000076c,
			 0x00000224,

			 /// vpu register f2
			 0x41a80000,
			 0x41a00000,
			 0x41800000,
			 0x41b80000,
			 0x42000000,
			 0x41d80000,
			 0x41900000,
			 0x41a80000,

			 /// vpu register f3
			 0x41980000,
			 0x40c00000,
			 0x41f80000,
			 0x41c00000,
			 0x41300000,
			 0x41c00000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f4
			 0x40a00000,
			 0x41880000,
			 0x41400000,
			 0x41500000,
			 0x3f800000,
			 0x41f80000,
			 0x41900000,
			 0x41e80000,

			 /// vpu register f5
			 0x40c00000,
			 0x41700000,
			 0x3f800000,
			 0x41200000,
			 0x40400000,
			 0x40000000,
			 0x41980000,
			 0x41a00000,

			 /// vpu register f6
			 0x41a80000,
			 0x40000000,
			 0x40800000,
			 0x41300000,
			 0x41400000,
			 0x41500000,
			 0x41d00000,
			 0x41f00000,

			 /// vpu register f7
			 0x41a80000,
			 0x41c00000,
			 0x40400000,
			 0x41c00000,
			 0x41400000,
			 0x3f800000,
			 0x41500000,
			 0x41c80000,

			 /// vpu register f8
			 0x41b80000,
			 0x41800000,
			 0x41f80000,
			 0x41f00000,
			 0x41000000,
			 0x41b80000,
			 0x41f00000,
			 0x41500000,

			 /// vpu register f9
			 0x41f00000,
			 0x41000000,
			 0x41e80000,
			 0x41500000,
			 0x40e00000,
			 0x41a00000,
			 0x41800000,
			 0x41a80000,

			 /// vpu register f10
			 0x41c00000,
			 0x41d00000,
			 0x40400000,
			 0x41200000,
			 0x40400000,
			 0x41b00000,
			 0x41300000,
			 0x41900000,

			 /// vpu register f11
			 0x41900000,
			 0x40000000,
			 0x41600000,
			 0x41a00000,
			 0x41c80000,
			 0x41c00000,
			 0x41800000,
			 0x41980000,

			 /// vpu register f12
			 0x40c00000,
			 0x3f800000,
			 0x41c00000,
			 0x40800000,
			 0x40400000,
			 0x41300000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f13
			 0x42000000,
			 0x41900000,
			 0x41b80000,
			 0x41a00000,
			 0x41980000,
			 0x40a00000,
			 0x41000000,
			 0x41700000,

			 /// vpu register f14
			 0x41200000,
			 0x41d00000,
			 0x41000000,
			 0x41700000,
			 0x41a80000,
			 0x41700000,
			 0x41b00000,
			 0x41d00000,

			 /// vpu register f15
			 0x41d80000,
			 0x41a80000,
			 0x41900000,
			 0x40c00000,
			 0x41f80000,
			 0x3f800000,
			 0x41f00000,
			 0x41980000,

			 /// vpu register f16
			 0x40c00000,
			 0x41f80000,
			 0x41900000,
			 0x41c00000,
			 0x41200000,
			 0x40e00000,
			 0x41200000,
			 0x41e00000,

			 /// vpu register f17
			 0x41f80000,
			 0x40e00000,
			 0x41600000,
			 0x41c80000,
			 0x41000000,
			 0x41e00000,
			 0x41880000,
			 0x40a00000,

			 /// vpu register f18
			 0x41900000,
			 0x40800000,
			 0x41880000,
			 0x41d80000,
			 0x41600000,
			 0x41b00000,
			 0x41e00000,
			 0x40c00000,

			 /// vpu register f19
			 0x41000000,
			 0x41200000,
			 0x41980000,
			 0x40c00000,
			 0x40e00000,
			 0x40800000,
			 0x41f00000,
			 0x3f800000,

			 /// vpu register f20
			 0x41400000,
			 0x41400000,
			 0x41e00000,
			 0x41b00000,
			 0x41e80000,
			 0x41e80000,
			 0x41d00000,
			 0x41e80000,

			 /// vpu register f21
			 0x41c80000,
			 0x41900000,
			 0x41b00000,
			 0x41f00000,
			 0x41f00000,
			 0x40a00000,
			 0x41980000,
			 0x40000000,

			 /// vpu register f22
			 0x41c80000,
			 0x41400000,
			 0x41a00000,
			 0x41a00000,
			 0x40800000,
			 0x41f80000,
			 0x41100000,
			 0x41100000,

			 /// vpu register f23
			 0x42000000,
			 0x41b80000,
			 0x41700000,
			 0x3f800000,
			 0x41f00000,
			 0x41d80000,
			 0x41300000,
			 0x41d00000,

			 /// vpu register f24
			 0x41f80000,
			 0x41e80000,
			 0x41600000,
			 0x40a00000,
			 0x40a00000,
			 0x41600000,
			 0x41100000,
			 0x41c80000,

			 /// vpu register f25
			 0x41b00000,
			 0x41700000,
			 0x40000000,
			 0x41b80000,
			 0x40e00000,
			 0x41100000,
			 0x41900000,
			 0x41400000,

			 /// vpu register f26
			 0x41d00000,
			 0x41900000,
			 0x41f00000,
			 0x41200000,
			 0x41100000,
			 0x41a00000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f27
			 0x41d80000,
			 0x41300000,
			 0x41c80000,
			 0x41300000,
			 0x40800000,
			 0x41d80000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f28
			 0x41b80000,
			 0x41d00000,
			 0x41b80000,
			 0x40000000,
			 0x41300000,
			 0x41c80000,
			 0x41f80000,
			 0x41400000,

			 /// vpu register f29
			 0x41a80000,
			 0x41800000,
			 0x41300000,
			 0x42000000,
			 0x40400000,
			 0x41e00000,
			 0x40e00000,
			 0x3f800000,

			 /// vpu register f30
			 0x42000000,
			 0x41300000,
			 0x41a80000,
			 0x41d80000,
			 0x41d00000,
			 0x41200000,
			 0x41000000,
			 0x41800000,

			 /// vpu register f31
			 0x41900000,
			 0x41d80000,
			 0x41f80000,
			 0x41200000,
			 0x41800000,
			 0x41700000,
			 0x41d80000,
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
		"nop\n"
		"nop\n"
		"fgw.ps f24, f1 (x11)\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f12, f1 (x11)\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f24, f1 (x13)\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f11, f1 (x11)\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f13, f1 (x11)\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f23, f1 (x13)\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x12)\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f9, f1 (x12)\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x14)\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f10, f1 (x11)\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f9, f1 (x12)\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x12)\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f29, f1 (x11)\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f11, f1 (x14)\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f11, f1 (x11)\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f4, f1 (x15)\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f26, f1 (x14)\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f3, f1 (x12)\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f2, f1 (x11)\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f20, f1 (x12)\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f20, f1 (x15)\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f29, f1 (x14)\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f19, f1 (x15)\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f16, f1 (x11)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x13)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f2, f1 (x15)\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f29, f1 (x14)\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f3, f1 (x15)\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f20, f1 (x12)\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f7, f1 (x13)\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f10, f1 (x13)\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f23, f1 (x14)\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x15)\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f13, f1 (x12)\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f8, f1 (x15)\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f29, f1 (x11)\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f11, f1 (x14)\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f5, f1 (x11)\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f16, f1 (x11)\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f4, f1 (x11)\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f15, f1 (x11)\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f18, f1 (x11)\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f2, f1 (x11)\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f24, f1 (x13)\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f20, f1 (x14)\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f22, f1 (x15)\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f15, f1 (x13)\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x13)\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f13, f1 (x11)\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f13, f1 (x15)\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f21, f1 (x13)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f17, f1 (x15)\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f7, f1 (x12)\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f26, f1 (x11)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x13)\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x12)\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f19, f1 (x12)\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f21, f1 (x12)\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f7, f1 (x11)\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f13, f1 (x11)\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f30, f1 (x13)\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f15, f1 (x11)\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f7, f1 (x14)\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f5, f1 (x15)\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f11, f1 (x15)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f28, f1 (x13)\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x14)\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f30, f1 (x15)\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f3, f1 (x13)\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f12, f1 (x14)\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f21, f1 (x11)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f3, f1 (x12)\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x13)\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f7, f1 (x12)\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f9, f1 (x11)\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f24, f1 (x12)\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f14, f1 (x14)\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f8, f1 (x15)\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f4, f1 (x11)\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f26, f1 (x15)\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x13)\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f9, f1 (x11)\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f24, f1 (x12)\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f7, f1 (x11)\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f21, f1 (x14)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f19, f1 (x12)\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f10, f1 (x14)\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x14)\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f17, f1 (x11)\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f14, f1 (x14)\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f29, f1 (x14)\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f2, f1 (x15)\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f6, f1 (x15)\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f26, f1 (x12)\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f27, f1 (x14)\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f9, f1 (x11)\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f11, f1 (x12)\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f9, f1 (x11)\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgw.ps f18, f1 (x12)\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
