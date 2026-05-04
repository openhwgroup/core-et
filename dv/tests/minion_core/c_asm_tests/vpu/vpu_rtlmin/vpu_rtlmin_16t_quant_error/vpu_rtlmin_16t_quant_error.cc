/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// tensor_fma32_vpu
// tensor_ima_vpu
// tensor_load_setup_b_vpu
// tensor_load_vpu
// tensor_store_vpu
//////////////////////////////////////////////////////////////////////// 
////// VPU FOCUS TEST: seed = 0x1f41  = 8001
////// VPU FOCUS TEST: isa_focus = vpu_perf_tensor
////// VPU FOCUS TEST: uarch_focus = uarch_default
//////////////////////////////////////////////////////////////////////// 
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"


/// Macros

#define VSNIP_RSV           : : [operands] "r" (operands), [m_11] "r" (m_11): "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
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
	//uint64_t i;
	mid = get_minion_id();
	tid = get_thread_id();

	volatile uint32_t m_11[2048] __attribute__ ((aligned (4096))) = {
			 0x8c765e8d, /// 0x0
			 0x2eb047bb, /// 0x4
			 0xa2c1cc00, /// 0x8
			 0x236db8a7, /// 0xc
			 0x6b631ffd, /// 0x10
			 0xfadc1661, /// 0x14
			 0x14d09a30, /// 0x18
			 0xde8fb133, /// 0x1c
			 0x8b329c7f, /// 0x20
			 0xea33cd58, /// 0x24
			 0x7c10e684, /// 0x28
			 0xe7ed055f, /// 0x2c
			 0x5af6dd4e, /// 0x30
			 0x2d21bcd9, /// 0x34
			 0xfdbef238, /// 0x38
			 0x0f84fc06, /// 0x3c
			 0xd97c51af, /// 0x40
			 0xe7281222, /// 0x44
			 0x4edeb4f9, /// 0x48
			 0xdc4b6df7, /// 0x4c
			 0x03e98e7c, /// 0x50
			 0x1528c358, /// 0x54
			 0x963872e0, /// 0x58
			 0xb02fcc2a, /// 0x5c
			 0x1849f222, /// 0x60
			 0x5ae1bce1, /// 0x64
			 0x24eb8304, /// 0x68
			 0x149e9c12, /// 0x6c
			 0xd7d64982, /// 0x70
			 0x9c5b8fcf, /// 0x74
			 0x6d19c927, /// 0x78
			 0xc43323f8, /// 0x7c
			 0x27674e74, /// 0x80
			 0xbd63f42e, /// 0x84
			 0xdb36518a, /// 0x88
			 0x3712daa1, /// 0x8c
			 0xb7e222ec, /// 0x90
			 0xdd2b71fd, /// 0x94
			 0x27f4b291, /// 0x98
			 0x963553dc, /// 0x9c
			 0xdbfa1cc4, /// 0xa0
			 0x481d50f8, /// 0xa4
			 0x46640a50, /// 0xa8
			 0xe9c47a4a, /// 0xac
			 0x0db26385, /// 0xb0
			 0x126b3575, /// 0xb4
			 0x1bd18398, /// 0xb8
			 0x03698085, /// 0xbc
			 0x76ef1e3b, /// 0xc0
			 0xe08663bd, /// 0xc4
			 0x2ef4c18a, /// 0xc8
			 0xa9f49acc, /// 0xcc
			 0xbc3ebea3, /// 0xd0
			 0xa9ffd837, /// 0xd4
			 0x4fa396cd, /// 0xd8
			 0xd0f2b6e4, /// 0xdc
			 0xfe988140, /// 0xe0
			 0x27548ad4, /// 0xe4
			 0xcd04dce7, /// 0xe8
			 0x4c5cfffe, /// 0xec
			 0x1beb632b, /// 0xf0
			 0xea197d35, /// 0xf4
			 0x5f009360, /// 0xf8
			 0xe2f721ac, /// 0xfc
			 0x543ae728, /// 0x100
			 0xed5f5555, /// 0x104
			 0xe4236bbd, /// 0x108
			 0xbf579006, /// 0x10c
			 0x695324b6, /// 0x110
			 0xd5b546b3, /// 0x114
			 0x00247081, /// 0x118
			 0x596244c0, /// 0x11c
			 0x188b258a, /// 0x120
			 0x335eb45e, /// 0x124
			 0x739fb66d, /// 0x128
			 0x7e7efed2, /// 0x12c
			 0xec64ebe3, /// 0x130
			 0xa777bd84, /// 0x134
			 0x4df271ea, /// 0x138
			 0xbea0045c, /// 0x13c
			 0x4690c147, /// 0x140
			 0xb42a56b7, /// 0x144
			 0x4b0b24ab, /// 0x148
			 0xb46039e7, /// 0x14c
			 0x6e4b18e0, /// 0x150
			 0x79e72b44, /// 0x154
			 0x2908a87f, /// 0x158
			 0xc6efb275, /// 0x15c
			 0xdcc7bda9, /// 0x160
			 0x3d220bf4, /// 0x164
			 0x02d42df8, /// 0x168
			 0x1cca15bf, /// 0x16c
			 0x773c903d, /// 0x170
			 0x4a8a36b2, /// 0x174
			 0xd29903dd, /// 0x178
			 0x78e2d1cf, /// 0x17c
			 0xc296326a, /// 0x180
			 0x04356cb7, /// 0x184
			 0xa9cc9de5, /// 0x188
			 0x69669095, /// 0x18c
			 0x77168780, /// 0x190
			 0xa2050ddc, /// 0x194
			 0x4e336f65, /// 0x198
			 0xf47d1eb4, /// 0x19c
			 0x373b73a5, /// 0x1a0
			 0x39251899, /// 0x1a4
			 0xad73fc9a, /// 0x1a8
			 0xa18132fb, /// 0x1ac
			 0x2f320152, /// 0x1b0
			 0x2629e52e, /// 0x1b4
			 0x02fed378, /// 0x1b8
			 0x1f46ef17, /// 0x1bc
			 0x2cd9f0b2, /// 0x1c0
			 0x646371df, /// 0x1c4
			 0x0772962e, /// 0x1c8
			 0x379ae94d, /// 0x1cc
			 0x45eb9db1, /// 0x1d0
			 0x0a634f6d, /// 0x1d4
			 0xd4330539, /// 0x1d8
			 0xf810644b, /// 0x1dc
			 0x00c3af21, /// 0x1e0
			 0xf78a0704, /// 0x1e4
			 0xc0cdc890, /// 0x1e8
			 0x88424489, /// 0x1ec
			 0x7a1bfb6d, /// 0x1f0
			 0xe05a91d3, /// 0x1f4
			 0xf2e59dc3, /// 0x1f8
			 0xf92e0ae2, /// 0x1fc
			 0x237f7c9a, /// 0x200
			 0x412e0561, /// 0x204
			 0xc8aa5db2, /// 0x208
			 0x845e303c, /// 0x20c
			 0x00b62ed7, /// 0x210
			 0x189fdf70, /// 0x214
			 0x1ff16afe, /// 0x218
			 0xf683a667, /// 0x21c
			 0x6accbbae, /// 0x220
			 0x2c931468, /// 0x224
			 0x2b09f0ca, /// 0x228
			 0x8e597a79, /// 0x22c
			 0xf15036cd, /// 0x230
			 0x5d97bcff, /// 0x234
			 0xc2ebf8b7, /// 0x238
			 0x56702158, /// 0x23c
			 0x636b0537, /// 0x240
			 0xe2cf3abd, /// 0x244
			 0x6e73b0c4, /// 0x248
			 0x29d0a51e, /// 0x24c
			 0x523cc931, /// 0x250
			 0xba751b7b, /// 0x254
			 0xff7a0f2a, /// 0x258
			 0x67a2e648, /// 0x25c
			 0xc579e87f, /// 0x260
			 0x97601318, /// 0x264
			 0x826dfd8f, /// 0x268
			 0xc563f70a, /// 0x26c
			 0x0dbf86be, /// 0x270
			 0x9839b638, /// 0x274
			 0xad4d1281, /// 0x278
			 0x530b04b3, /// 0x27c
			 0x47bae51e, /// 0x280
			 0x3e5c2d6e, /// 0x284
			 0xe2304839, /// 0x288
			 0xd0c57e84, /// 0x28c
			 0xf3eeb9ef, /// 0x290
			 0xa8f72a18, /// 0x294
			 0x6756908a, /// 0x298
			 0x5db2b05d, /// 0x29c
			 0x22e0bf73, /// 0x2a0
			 0x77ed232e, /// 0x2a4
			 0x1af27d39, /// 0x2a8
			 0x9d5a0f5b, /// 0x2ac
			 0xdc60fdfc, /// 0x2b0
			 0x2077c2b7, /// 0x2b4
			 0x6b58790e, /// 0x2b8
			 0x6b613e36, /// 0x2bc
			 0xa3efe804, /// 0x2c0
			 0x2d7148d8, /// 0x2c4
			 0x9af23cae, /// 0x2c8
			 0xc6bf4769, /// 0x2cc
			 0x41ca98e8, /// 0x2d0
			 0xda3922a7, /// 0x2d4
			 0xd30239df, /// 0x2d8
			 0xfeb7db81, /// 0x2dc
			 0x7e5bcd44, /// 0x2e0
			 0xab6b77dd, /// 0x2e4
			 0x47c6259d, /// 0x2e8
			 0xc3f50acd, /// 0x2ec
			 0x2606b49d, /// 0x2f0
			 0x25c12c65, /// 0x2f4
			 0x6f8227ac, /// 0x2f8
			 0x3eef4a3d, /// 0x2fc
			 0xbf891223, /// 0x300
			 0x1338b736, /// 0x304
			 0x74f02f37, /// 0x308
			 0x120bd1a1, /// 0x30c
			 0xf98c2a7d, /// 0x310
			 0x43a05800, /// 0x314
			 0x08de195a, /// 0x318
			 0x81b0e246, /// 0x31c
			 0xc9e8cfa5, /// 0x320
			 0x84b699c6, /// 0x324
			 0x9af85a2f, /// 0x328
			 0x3d99ab60, /// 0x32c
			 0x568007bc, /// 0x330
			 0x975b22ef, /// 0x334
			 0x20851de6, /// 0x338
			 0x60e3c48d, /// 0x33c
			 0x05152fc3, /// 0x340
			 0x76db5bd7, /// 0x344
			 0x5a3c79d8, /// 0x348
			 0xa0abae3e, /// 0x34c
			 0x3d68b483, /// 0x350
			 0xa46e376d, /// 0x354
			 0x08eb3bcf, /// 0x358
			 0x689fa409, /// 0x35c
			 0xc152bcb1, /// 0x360
			 0xb593b388, /// 0x364
			 0x7f1c4495, /// 0x368
			 0xbd63653c, /// 0x36c
			 0x3f5a7388, /// 0x370
			 0x5d6a862a, /// 0x374
			 0xae2bf8ce, /// 0x378
			 0xfdf6f0ad, /// 0x37c
			 0x6dafcbfc, /// 0x380
			 0x4f95266b, /// 0x384
			 0x42338a10, /// 0x388
			 0x77bdf00b, /// 0x38c
			 0x1f0adf15, /// 0x390
			 0xe175d80b, /// 0x394
			 0x29e1bb00, /// 0x398
			 0x9b04aa10, /// 0x39c
			 0xfc506498, /// 0x3a0
			 0x655085c6, /// 0x3a4
			 0xf50ff745, /// 0x3a8
			 0x5a9487f3, /// 0x3ac
			 0xeacc1b0a, /// 0x3b0
			 0x61bdf40b, /// 0x3b4
			 0x3c52c93b, /// 0x3b8
			 0xb3d6da6d, /// 0x3bc
			 0x03c48610, /// 0x3c0
			 0x7c6c8140, /// 0x3c4
			 0xcac73418, /// 0x3c8
			 0x79f1a4af, /// 0x3cc
			 0x63267b00, /// 0x3d0
			 0xbf0c2186, /// 0x3d4
			 0x5c5f5f69, /// 0x3d8
			 0xab343cd7, /// 0x3dc
			 0xdae0f5e9, /// 0x3e0
			 0x46d1cf38, /// 0x3e4
			 0xb798161c, /// 0x3e8
			 0x584aabdb, /// 0x3ec
			 0xc5f6df24, /// 0x3f0
			 0xe360e8fa, /// 0x3f4
			 0xd3f7b9ab, /// 0x3f8
			 0xf1ddca0d, /// 0x3fc
			 0x3a05e108, /// 0x400
			 0x1333dd3a, /// 0x404
			 0x78567877, /// 0x408
			 0xa0154c38, /// 0x40c
			 0x95aa568a, /// 0x410
			 0x9d2dc65f, /// 0x414
			 0xa91d2343, /// 0x418
			 0x7ea6faca, /// 0x41c
			 0xf1f39ddb, /// 0x420
			 0xdadda84f, /// 0x424
			 0x8ada20b6, /// 0x428
			 0x53b41b1e, /// 0x42c
			 0xf601c9f6, /// 0x430
			 0x77dcee0c, /// 0x434
			 0xe4d3d76c, /// 0x438
			 0x0cd3ee30, /// 0x43c
			 0x34afbb08, /// 0x440
			 0x2765d20e, /// 0x444
			 0xbd92c946, /// 0x448
			 0xcd7798c4, /// 0x44c
			 0xf9a18839, /// 0x450
			 0x78cf678d, /// 0x454
			 0x80262187, /// 0x458
			 0x883116e0, /// 0x45c
			 0x0312de75, /// 0x460
			 0x02d5d617, /// 0x464
			 0xca1d554e, /// 0x468
			 0x6c3fe6a6, /// 0x46c
			 0x02b15d38, /// 0x470
			 0xedb280c2, /// 0x474
			 0x7058605c, /// 0x478
			 0xb14decc7, /// 0x47c
			 0xfc467142, /// 0x480
			 0x68f25b79, /// 0x484
			 0xeac8d564, /// 0x488
			 0xa6721662, /// 0x48c
			 0x18cc8810, /// 0x490
			 0x37c61f64, /// 0x494
			 0xf8ecc5cf, /// 0x498
			 0x1974180d, /// 0x49c
			 0xdd38cc52, /// 0x4a0
			 0x37549203, /// 0x4a4
			 0x17774a9f, /// 0x4a8
			 0xb233fb1e, /// 0x4ac
			 0x50a354ea, /// 0x4b0
			 0x1db50f4c, /// 0x4b4
			 0x39490324, /// 0x4b8
			 0xf3943865, /// 0x4bc
			 0xafa1e8d7, /// 0x4c0
			 0xa2ff9f33, /// 0x4c4
			 0x4e7e34d3, /// 0x4c8
			 0x359d4975, /// 0x4cc
			 0xceadf223, /// 0x4d0
			 0xb8c4b4cd, /// 0x4d4
			 0x1dbaae90, /// 0x4d8
			 0x355d26e7, /// 0x4dc
			 0xbf8a7527, /// 0x4e0
			 0x03b0453c, /// 0x4e4
			 0x93e3a5a2, /// 0x4e8
			 0x27ad4f1f, /// 0x4ec
			 0x4ee6e20f, /// 0x4f0
			 0xbf72416c, /// 0x4f4
			 0x4e2d93dd, /// 0x4f8
			 0xfa804519, /// 0x4fc
			 0xe23e886b, /// 0x500
			 0x35f7b03d, /// 0x504
			 0x2de6d8fc, /// 0x508
			 0x4b586498, /// 0x50c
			 0x4b844f82, /// 0x510
			 0x3a176ebb, /// 0x514
			 0x3050fd2c, /// 0x518
			 0x27a297fa, /// 0x51c
			 0x44e68807, /// 0x520
			 0xed19826e, /// 0x524
			 0x0d75cba7, /// 0x528
			 0xccc0e96b, /// 0x52c
			 0xfb30e178, /// 0x530
			 0xb59f78ef, /// 0x534
			 0x906b397b, /// 0x538
			 0xae8ea81c, /// 0x53c
			 0xa33d318e, /// 0x540
			 0x8dbaa6d4, /// 0x544
			 0x2ef27a7f, /// 0x548
			 0x910e476a, /// 0x54c
			 0x9cd6e87c, /// 0x550
			 0x54079512, /// 0x554
			 0x16188861, /// 0x558
			 0x864604f3, /// 0x55c
			 0xb586d3b5, /// 0x560
			 0x0e95579c, /// 0x564
			 0xc18cb1de, /// 0x568
			 0x29d163fd, /// 0x56c
			 0xc90dbf86, /// 0x570
			 0xfed38b2e, /// 0x574
			 0xc3a2ab9c, /// 0x578
			 0x0b5062bc, /// 0x57c
			 0xdbb9d289, /// 0x580
			 0x9fdee879, /// 0x584
			 0x09ce797e, /// 0x588
			 0x989ee7f2, /// 0x58c
			 0xe6f4b103, /// 0x590
			 0xe5aea37e, /// 0x594
			 0x60816a2a, /// 0x598
			 0xa8ae0260, /// 0x59c
			 0xafbfc26f, /// 0x5a0
			 0xb889a251, /// 0x5a4
			 0x8a92fd57, /// 0x5a8
			 0xb79c67c7, /// 0x5ac
			 0x82525fc9, /// 0x5b0
			 0x03955132, /// 0x5b4
			 0x085925c8, /// 0x5b8
			 0xf5fd8657, /// 0x5bc
			 0x3fec0bc9, /// 0x5c0
			 0x5a8b7366, /// 0x5c4
			 0xd3a99ac5, /// 0x5c8
			 0x74e793d7, /// 0x5cc
			 0xef128c7c, /// 0x5d0
			 0xdfb11ba7, /// 0x5d4
			 0x8c7547c0, /// 0x5d8
			 0x3bdc8753, /// 0x5dc
			 0xf3771bb3, /// 0x5e0
			 0x6675e2b8, /// 0x5e4
			 0x99eec23b, /// 0x5e8
			 0xc8e65ee3, /// 0x5ec
			 0x0e9a1186, /// 0x5f0
			 0xc6bf357b, /// 0x5f4
			 0x98f216e8, /// 0x5f8
			 0x043cb742, /// 0x5fc
			 0x9429f3a4, /// 0x600
			 0x43b47307, /// 0x604
			 0x1627f765, /// 0x608
			 0xe9256326, /// 0x60c
			 0x4ea165be, /// 0x610
			 0x674c0287, /// 0x614
			 0xe36e830c, /// 0x618
			 0x90634775, /// 0x61c
			 0x3bf7ebb0, /// 0x620
			 0xe529bfc1, /// 0x624
			 0x400ee328, /// 0x628
			 0xa0568696, /// 0x62c
			 0x30346799, /// 0x630
			 0x69a8c1bf, /// 0x634
			 0x98861867, /// 0x638
			 0xc789733c, /// 0x63c
			 0xdbfa9503, /// 0x640
			 0x3cc2584a, /// 0x644
			 0x64df8c0d, /// 0x648
			 0x5d3c327c, /// 0x64c
			 0x82952acd, /// 0x650
			 0xa83bf16e, /// 0x654
			 0x2ea4dc87, /// 0x658
			 0xd17d668f, /// 0x65c
			 0x44f6695e, /// 0x660
			 0x3fa5e5ab, /// 0x664
			 0x00a334d6, /// 0x668
			 0x4da2e5e5, /// 0x66c
			 0x426d5530, /// 0x670
			 0x2678b622, /// 0x674
			 0x9b7e75b9, /// 0x678
			 0x7bc93506, /// 0x67c
			 0x46307c4f, /// 0x680
			 0xa195e365, /// 0x684
			 0xaa1c7341, /// 0x688
			 0x47d15381, /// 0x68c
			 0x284ad96b, /// 0x690
			 0xfca1634f, /// 0x694
			 0x665456b5, /// 0x698
			 0x6a4221bd, /// 0x69c
			 0xa616e827, /// 0x6a0
			 0xd94dc087, /// 0x6a4
			 0xaa33e4da, /// 0x6a8
			 0xb66a4810, /// 0x6ac
			 0x40f4408b, /// 0x6b0
			 0x8a6156ce, /// 0x6b4
			 0x5f6f35ff, /// 0x6b8
			 0xf33af1bb, /// 0x6bc
			 0xc77dd53b, /// 0x6c0
			 0xa011b613, /// 0x6c4
			 0xe4d2499f, /// 0x6c8
			 0x8710dc59, /// 0x6cc
			 0x74d1a6ec, /// 0x6d0
			 0x47e059e5, /// 0x6d4
			 0x4e6879c0, /// 0x6d8
			 0xc50b2b94, /// 0x6dc
			 0x62f539e7, /// 0x6e0
			 0x1d114a68, /// 0x6e4
			 0xf6e502e6, /// 0x6e8
			 0xb82f5eb2, /// 0x6ec
			 0x06a9f532, /// 0x6f0
			 0x43218283, /// 0x6f4
			 0x7987b3a8, /// 0x6f8
			 0x47dfc3a3, /// 0x6fc
			 0x523c509b, /// 0x700
			 0x01b9561f, /// 0x704
			 0x7de21d32, /// 0x708
			 0xcff3e211, /// 0x70c
			 0x21c4b2a0, /// 0x710
			 0xcad1c208, /// 0x714
			 0xb5938ac6, /// 0x718
			 0xe587e30d, /// 0x71c
			 0xaa238872, /// 0x720
			 0x48b31d0a, /// 0x724
			 0x0a404e1a, /// 0x728
			 0x6c12035e, /// 0x72c
			 0x7cc4e3e1, /// 0x730
			 0x6d031b99, /// 0x734
			 0xac0c27ec, /// 0x738
			 0x74d9cf26, /// 0x73c
			 0x4d20a080, /// 0x740
			 0x5b6a6845, /// 0x744
			 0xc01b6aa0, /// 0x748
			 0x8fe70528, /// 0x74c
			 0xa444a655, /// 0x750
			 0x999657ca, /// 0x754
			 0x02711e8a, /// 0x758
			 0xea393c2c, /// 0x75c
			 0x5849e344, /// 0x760
			 0xbde6d7bc, /// 0x764
			 0x1c04079e, /// 0x768
			 0xff73106b, /// 0x76c
			 0x3e5e99ad, /// 0x770
			 0x3577307f, /// 0x774
			 0x93ec573a, /// 0x778
			 0xebf53a5a, /// 0x77c
			 0xae1cc428, /// 0x780
			 0xc66340d2, /// 0x784
			 0xabb9edd6, /// 0x788
			 0xf213121b, /// 0x78c
			 0x7b1b7a09, /// 0x790
			 0x8d62b563, /// 0x794
			 0x48b845e4, /// 0x798
			 0xac4a550e, /// 0x79c
			 0xd04e8240, /// 0x7a0
			 0xb84f5c7b, /// 0x7a4
			 0x8c877ddb, /// 0x7a8
			 0xc2c23971, /// 0x7ac
			 0xc6d23036, /// 0x7b0
			 0x1cbfd36d, /// 0x7b4
			 0xb718de79, /// 0x7b8
			 0x6ce27a2f, /// 0x7bc
			 0x60eab3eb, /// 0x7c0
			 0x7901a438, /// 0x7c4
			 0x6a44b9e5, /// 0x7c8
			 0x7c293679, /// 0x7cc
			 0xc2060087, /// 0x7d0
			 0xa89623c6, /// 0x7d4
			 0x67426b48, /// 0x7d8
			 0xaad82857, /// 0x7dc
			 0xe0f3fe4f, /// 0x7e0
			 0x0b43b39e, /// 0x7e4
			 0x33d09f95, /// 0x7e8
			 0x8d6f23b4, /// 0x7ec
			 0x62c80386, /// 0x7f0
			 0x25a9ca24, /// 0x7f4
			 0x21c842a1, /// 0x7f8
			 0x100fa73c, /// 0x7fc
			 0x6a45134c, /// 0x800
			 0xc18b6c27, /// 0x804
			 0xc8eed987, /// 0x808
			 0x75e55601, /// 0x80c
			 0x7d692a4c, /// 0x810
			 0x8d1586c7, /// 0x814
			 0x4f8b4821, /// 0x818
			 0x529531a0, /// 0x81c
			 0x68c687a6, /// 0x820
			 0xd07bb1fb, /// 0x824
			 0xd2fd97fa, /// 0x828
			 0xeca5ec57, /// 0x82c
			 0xcfc2cafe, /// 0x830
			 0x93e30598, /// 0x834
			 0xd2ae98db, /// 0x838
			 0x76c0c8f5, /// 0x83c
			 0xda54a354, /// 0x840
			 0xb94625f1, /// 0x844
			 0x8d6b6038, /// 0x848
			 0xee5ac23b, /// 0x84c
			 0xd82f30e9, /// 0x850
			 0xdb399ca1, /// 0x854
			 0xa618a36c, /// 0x858
			 0x18e4b5d3, /// 0x85c
			 0x6981fb88, /// 0x860
			 0xdd0f5be0, /// 0x864
			 0xef3a748f, /// 0x868
			 0xe9a28858, /// 0x86c
			 0x3c9ca9c6, /// 0x870
			 0x125a3c22, /// 0x874
			 0x0d472182, /// 0x878
			 0x3d282baf, /// 0x87c
			 0xad3a4fc5, /// 0x880
			 0x584e4271, /// 0x884
			 0x650609d8, /// 0x888
			 0x7cb80c2f, /// 0x88c
			 0xf026cc20, /// 0x890
			 0x4fb38afd, /// 0x894
			 0xec27c20a, /// 0x898
			 0x6c5ed1af, /// 0x89c
			 0x786b3e1e, /// 0x8a0
			 0x4b7669c1, /// 0x8a4
			 0x794eda30, /// 0x8a8
			 0xc7ffe73b, /// 0x8ac
			 0x6ecd32b6, /// 0x8b0
			 0x79b657cd, /// 0x8b4
			 0x284c53a5, /// 0x8b8
			 0xde8233b0, /// 0x8bc
			 0x1e38d0b6, /// 0x8c0
			 0xf95e703f, /// 0x8c4
			 0x9c0acb02, /// 0x8c8
			 0x277fbe08, /// 0x8cc
			 0xf9492147, /// 0x8d0
			 0xffbc8927, /// 0x8d4
			 0x73314862, /// 0x8d8
			 0xeb14d309, /// 0x8dc
			 0x702f2d8b, /// 0x8e0
			 0xdbdb3566, /// 0x8e4
			 0x2f125d54, /// 0x8e8
			 0x27748cc3, /// 0x8ec
			 0x815839d6, /// 0x8f0
			 0x3055fc8c, /// 0x8f4
			 0x16c6e041, /// 0x8f8
			 0x83dda5e7, /// 0x8fc
			 0x629ef07d, /// 0x900
			 0xcc14e371, /// 0x904
			 0x80e9041e, /// 0x908
			 0x810bf5ac, /// 0x90c
			 0x6ed2a97f, /// 0x910
			 0x45541ad1, /// 0x914
			 0xca92b69a, /// 0x918
			 0xa8b3466a, /// 0x91c
			 0xede2cfc1, /// 0x920
			 0xe33d9b89, /// 0x924
			 0x0adf3559, /// 0x928
			 0x329d6870, /// 0x92c
			 0xc3962f29, /// 0x930
			 0xe08b4a6c, /// 0x934
			 0xff7635e2, /// 0x938
			 0x15dd6905, /// 0x93c
			 0x5bf76521, /// 0x940
			 0x0a9bad52, /// 0x944
			 0xd102138c, /// 0x948
			 0xa45e6a22, /// 0x94c
			 0xa6a29dc2, /// 0x950
			 0x0fab6a55, /// 0x954
			 0xc80708d4, /// 0x958
			 0x93e0ccce, /// 0x95c
			 0xc4cc6e94, /// 0x960
			 0x90f631a0, /// 0x964
			 0x43065245, /// 0x968
			 0xccd27f77, /// 0x96c
			 0x5bfbe679, /// 0x970
			 0xfa085ce2, /// 0x974
			 0x2da1c9a3, /// 0x978
			 0x278f39e5, /// 0x97c
			 0xb3f4e56a, /// 0x980
			 0xf8d1a696, /// 0x984
			 0x2bddecfc, /// 0x988
			 0x56edad9e, /// 0x98c
			 0x084377bf, /// 0x990
			 0x808ec475, /// 0x994
			 0xb1b66aaa, /// 0x998
			 0xb9374d73, /// 0x99c
			 0x2746a634, /// 0x9a0
			 0xb6f1caf3, /// 0x9a4
			 0x3a8d7f6f, /// 0x9a8
			 0x8590f4fb, /// 0x9ac
			 0x743c6455, /// 0x9b0
			 0xdc577a4c, /// 0x9b4
			 0xc55c37c0, /// 0x9b8
			 0x11cf6c3a, /// 0x9bc
			 0xcae979d7, /// 0x9c0
			 0xdb8d8e8b, /// 0x9c4
			 0x5f2bcec5, /// 0x9c8
			 0xc2953ae5, /// 0x9cc
			 0x4872330b, /// 0x9d0
			 0x3a0831a5, /// 0x9d4
			 0xc6f4129a, /// 0x9d8
			 0xfc94fb9a, /// 0x9dc
			 0x56c353d8, /// 0x9e0
			 0x604d51df, /// 0x9e4
			 0x98ef5b35, /// 0x9e8
			 0x3a96f330, /// 0x9ec
			 0xd4d5c06f, /// 0x9f0
			 0xf763ed84, /// 0x9f4
			 0x35b2e127, /// 0x9f8
			 0x3c7830ae, /// 0x9fc
			 0x8003e5a1, /// 0xa00
			 0x9f585d33, /// 0xa04
			 0xc93dd61d, /// 0xa08
			 0x258b9dd8, /// 0xa0c
			 0x3cc6ee51, /// 0xa10
			 0x571282a3, /// 0xa14
			 0x9934f906, /// 0xa18
			 0x4c060838, /// 0xa1c
			 0x12599fe9, /// 0xa20
			 0x577a67c3, /// 0xa24
			 0x63efb52c, /// 0xa28
			 0xad0a35cd, /// 0xa2c
			 0x45845ab5, /// 0xa30
			 0x59c78969, /// 0xa34
			 0x3ee715a4, /// 0xa38
			 0xa1d3cb9d, /// 0xa3c
			 0xb00b5049, /// 0xa40
			 0xcc8329d1, /// 0xa44
			 0xf08d6b32, /// 0xa48
			 0x214cd0d4, /// 0xa4c
			 0x3bda4bac, /// 0xa50
			 0x1653c01c, /// 0xa54
			 0x3b5aa09c, /// 0xa58
			 0x6fc1e523, /// 0xa5c
			 0x343de547, /// 0xa60
			 0x781bbbe5, /// 0xa64
			 0x3d312f44, /// 0xa68
			 0xfe3c3613, /// 0xa6c
			 0x241a91ed, /// 0xa70
			 0x1863aa1f, /// 0xa74
			 0xbb08752b, /// 0xa78
			 0xb793fb54, /// 0xa7c
			 0xfcbfb44e, /// 0xa80
			 0x5da51154, /// 0xa84
			 0xcf00dde4, /// 0xa88
			 0xeb12381d, /// 0xa8c
			 0x7c75bbd9, /// 0xa90
			 0x06fd1e3d, /// 0xa94
			 0x4da73de4, /// 0xa98
			 0xfb5f5d8e, /// 0xa9c
			 0xd3060520, /// 0xaa0
			 0xf16e30c6, /// 0xaa4
			 0x2a231cd6, /// 0xaa8
			 0x03706054, /// 0xaac
			 0x0b19528b, /// 0xab0
			 0x7a41aaf9, /// 0xab4
			 0xc85a4620, /// 0xab8
			 0xba4e090e, /// 0xabc
			 0x3c290d1c, /// 0xac0
			 0x924e91e9, /// 0xac4
			 0xbb083520, /// 0xac8
			 0x2b782fc1, /// 0xacc
			 0x912c2b86, /// 0xad0
			 0x3530372e, /// 0xad4
			 0x2f71fa02, /// 0xad8
			 0x4d5c2938, /// 0xadc
			 0xae349018, /// 0xae0
			 0x51cbaa63, /// 0xae4
			 0xc2773d8f, /// 0xae8
			 0x2b694abb, /// 0xaec
			 0x3cc05702, /// 0xaf0
			 0x47defcf0, /// 0xaf4
			 0xa724eb23, /// 0xaf8
			 0xf417bf11, /// 0xafc
			 0x158663d7, /// 0xb00
			 0x3ca03613, /// 0xb04
			 0x4a648222, /// 0xb08
			 0x0e775ade, /// 0xb0c
			 0xd3946a59, /// 0xb10
			 0xe0aad794, /// 0xb14
			 0xf833030e, /// 0xb18
			 0x07dc9be3, /// 0xb1c
			 0x53f9052f, /// 0xb20
			 0xb4db885e, /// 0xb24
			 0x9111f6b1, /// 0xb28
			 0x17decc90, /// 0xb2c
			 0x82e71ae8, /// 0xb30
			 0x895493f6, /// 0xb34
			 0x3ab874fa, /// 0xb38
			 0x3002b951, /// 0xb3c
			 0x0d67c487, /// 0xb40
			 0x2b0155b1, /// 0xb44
			 0xee1d99b7, /// 0xb48
			 0x6f049bea, /// 0xb4c
			 0x966d2848, /// 0xb50
			 0x3fbcc8cb, /// 0xb54
			 0x4a242508, /// 0xb58
			 0x89a1cb68, /// 0xb5c
			 0x8ef2178f, /// 0xb60
			 0x18a3970b, /// 0xb64
			 0x3bb09ab1, /// 0xb68
			 0xfc1116c3, /// 0xb6c
			 0xbc5fbdf8, /// 0xb70
			 0x133256e9, /// 0xb74
			 0xf194bfd9, /// 0xb78
			 0x5cdc25c6, /// 0xb7c
			 0xc0edeb29, /// 0xb80
			 0xe10df649, /// 0xb84
			 0xe9d282d7, /// 0xb88
			 0x522f7b72, /// 0xb8c
			 0x23c4687f, /// 0xb90
			 0xd0955e57, /// 0xb94
			 0xa15f3c7d, /// 0xb98
			 0x57daf490, /// 0xb9c
			 0x0a7e6ef2, /// 0xba0
			 0x81561210, /// 0xba4
			 0x176cca0e, /// 0xba8
			 0x0f39eed3, /// 0xbac
			 0x21670065, /// 0xbb0
			 0x6d843566, /// 0xbb4
			 0xf27a091c, /// 0xbb8
			 0xae16c361, /// 0xbbc
			 0x19c4f274, /// 0xbc0
			 0x1c96735d, /// 0xbc4
			 0x007e7431, /// 0xbc8
			 0x64f5c702, /// 0xbcc
			 0x9482a884, /// 0xbd0
			 0xa0cc46b6, /// 0xbd4
			 0x7aa2fa3f, /// 0xbd8
			 0x34e5fcec, /// 0xbdc
			 0x15e404fe, /// 0xbe0
			 0x0c420f26, /// 0xbe4
			 0xc5d6ca97, /// 0xbe8
			 0x9dee660f, /// 0xbec
			 0x95dee161, /// 0xbf0
			 0xc0a4ee56, /// 0xbf4
			 0xefe65faf, /// 0xbf8
			 0x3929da2c, /// 0xbfc
			 0x9bee868a, /// 0xc00
			 0x4d3622bc, /// 0xc04
			 0x1c96448c, /// 0xc08
			 0xd2e01d99, /// 0xc0c
			 0xbfd7db48, /// 0xc10
			 0x2ae90f88, /// 0xc14
			 0x60e562ff, /// 0xc18
			 0x0d6a2833, /// 0xc1c
			 0x0c62814a, /// 0xc20
			 0x26632702, /// 0xc24
			 0x89bbc6c8, /// 0xc28
			 0x08525244, /// 0xc2c
			 0x7e828fa1, /// 0xc30
			 0x818f9c98, /// 0xc34
			 0xf5c37131, /// 0xc38
			 0x68e57e69, /// 0xc3c
			 0x7b882efb, /// 0xc40
			 0x66d5742b, /// 0xc44
			 0xa409023e, /// 0xc48
			 0x8d18ff29, /// 0xc4c
			 0x92cd9fcc, /// 0xc50
			 0xb97b1b56, /// 0xc54
			 0xccaed183, /// 0xc58
			 0xa5cd73f2, /// 0xc5c
			 0x1ba89dc3, /// 0xc60
			 0x09c76b6e, /// 0xc64
			 0xfb59bd93, /// 0xc68
			 0xf18d4749, /// 0xc6c
			 0x56323403, /// 0xc70
			 0x3f48c717, /// 0xc74
			 0xeb9740fd, /// 0xc78
			 0x52104dce, /// 0xc7c
			 0x96b8a219, /// 0xc80
			 0xe32eb9c1, /// 0xc84
			 0x9451644d, /// 0xc88
			 0x444ed97e, /// 0xc8c
			 0xc84625bf, /// 0xc90
			 0xd31890e1, /// 0xc94
			 0xb1b9d481, /// 0xc98
			 0x76f2e5fc, /// 0xc9c
			 0xa6aca1e7, /// 0xca0
			 0x647d2fb9, /// 0xca4
			 0x3f9b1042, /// 0xca8
			 0x3dd8c6c2, /// 0xcac
			 0x94a5d459, /// 0xcb0
			 0x9dc22575, /// 0xcb4
			 0xe2db179e, /// 0xcb8
			 0xe5df8996, /// 0xcbc
			 0x19eb9547, /// 0xcc0
			 0x6bcc03a1, /// 0xcc4
			 0x22d007c2, /// 0xcc8
			 0x3bbf92f2, /// 0xccc
			 0xa89acaa7, /// 0xcd0
			 0xd136978d, /// 0xcd4
			 0x89e38354, /// 0xcd8
			 0x92b0db84, /// 0xcdc
			 0xa997d0ae, /// 0xce0
			 0x613dad65, /// 0xce4
			 0x04360364, /// 0xce8
			 0x8c453754, /// 0xcec
			 0x954c4796, /// 0xcf0
			 0xb39169c1, /// 0xcf4
			 0x01399eb2, /// 0xcf8
			 0x997913a5, /// 0xcfc
			 0x39e7279c, /// 0xd00
			 0x9f6d8b4e, /// 0xd04
			 0x3faf48da, /// 0xd08
			 0xf508c7a1, /// 0xd0c
			 0x4606ab97, /// 0xd10
			 0x6b024425, /// 0xd14
			 0x77d7f4fb, /// 0xd18
			 0xa1cb2b1f, /// 0xd1c
			 0xf4fa9b24, /// 0xd20
			 0xffa2cabe, /// 0xd24
			 0x50eb7e91, /// 0xd28
			 0xb42d600e, /// 0xd2c
			 0x793e9460, /// 0xd30
			 0x355954b0, /// 0xd34
			 0x8014a129, /// 0xd38
			 0xb1d8a361, /// 0xd3c
			 0x3fdbb09e, /// 0xd40
			 0xe7255245, /// 0xd44
			 0xafa330f6, /// 0xd48
			 0x6cb940da, /// 0xd4c
			 0xa53ecb62, /// 0xd50
			 0x44878586, /// 0xd54
			 0x2908863a, /// 0xd58
			 0x1ce85fcc, /// 0xd5c
			 0x170bd115, /// 0xd60
			 0xb13b10fa, /// 0xd64
			 0x7a4c04d0, /// 0xd68
			 0xe015ee89, /// 0xd6c
			 0xc5246571, /// 0xd70
			 0x3aba248c, /// 0xd74
			 0x3ed46329, /// 0xd78
			 0xe22479aa, /// 0xd7c
			 0x9413d456, /// 0xd80
			 0xd27d19ac, /// 0xd84
			 0x3fabe092, /// 0xd88
			 0x1b084743, /// 0xd8c
			 0xf0500790, /// 0xd90
			 0x65d8651f, /// 0xd94
			 0x3b9d2f59, /// 0xd98
			 0x1aad150a, /// 0xd9c
			 0x4de6963e, /// 0xda0
			 0xd8927345, /// 0xda4
			 0x47263798, /// 0xda8
			 0xb282d657, /// 0xdac
			 0x108cd7d5, /// 0xdb0
			 0xe4f6fcba, /// 0xdb4
			 0x8e7b6390, /// 0xdb8
			 0x4b1c5021, /// 0xdbc
			 0xf13eb210, /// 0xdc0
			 0xe2e38f2b, /// 0xdc4
			 0x32f5a152, /// 0xdc8
			 0x844a9e4c, /// 0xdcc
			 0x0518e4cb, /// 0xdd0
			 0x61286bcb, /// 0xdd4
			 0x451058a2, /// 0xdd8
			 0x56a8286c, /// 0xddc
			 0xe8d7a6ea, /// 0xde0
			 0x3e11cfad, /// 0xde4
			 0xbeaf584d, /// 0xde8
			 0x26533882, /// 0xdec
			 0x8be2dd99, /// 0xdf0
			 0x9e935afa, /// 0xdf4
			 0xaaf11da3, /// 0xdf8
			 0xe4b710a2, /// 0xdfc
			 0x50824c06, /// 0xe00
			 0x13db9896, /// 0xe04
			 0x748988a0, /// 0xe08
			 0x80abee43, /// 0xe0c
			 0x48ba0a56, /// 0xe10
			 0xae83c1ee, /// 0xe14
			 0x28f2da95, /// 0xe18
			 0x3d546860, /// 0xe1c
			 0x02937359, /// 0xe20
			 0x1a4b7ca4, /// 0xe24
			 0x8057bde9, /// 0xe28
			 0xddaf70cc, /// 0xe2c
			 0xdcb25386, /// 0xe30
			 0xf3245507, /// 0xe34
			 0x10067495, /// 0xe38
			 0xb3bc4368, /// 0xe3c
			 0x9945eb0a, /// 0xe40
			 0xd247696a, /// 0xe44
			 0x779f9a2b, /// 0xe48
			 0x270eccbb, /// 0xe4c
			 0xe58842c7, /// 0xe50
			 0xf1499ad3, /// 0xe54
			 0xd94f56ea, /// 0xe58
			 0x4744fbca, /// 0xe5c
			 0x492892c7, /// 0xe60
			 0x050af2c5, /// 0xe64
			 0xd196f90c, /// 0xe68
			 0x0d7ca381, /// 0xe6c
			 0x5d4e23ef, /// 0xe70
			 0xe52f790b, /// 0xe74
			 0x717b17ac, /// 0xe78
			 0x84ce56c6, /// 0xe7c
			 0x16fc1ac9, /// 0xe80
			 0x11724dcc, /// 0xe84
			 0x6708ec8a, /// 0xe88
			 0x3abe0471, /// 0xe8c
			 0xa51ca1f5, /// 0xe90
			 0xbbe0b98b, /// 0xe94
			 0x1ddb29f6, /// 0xe98
			 0xab40011e, /// 0xe9c
			 0xaeec1bb2, /// 0xea0
			 0x40697e9c, /// 0xea4
			 0x83e2256e, /// 0xea8
			 0x427c40b3, /// 0xeac
			 0x7276ac7a, /// 0xeb0
			 0x52c4bbf5, /// 0xeb4
			 0x6a0b81d1, /// 0xeb8
			 0x60d748a7, /// 0xebc
			 0x7e6ed4a4, /// 0xec0
			 0xff78e729, /// 0xec4
			 0x2cac5d1f, /// 0xec8
			 0x7c3da49f, /// 0xecc
			 0x798883a1, /// 0xed0
			 0xd0a42dfb, /// 0xed4
			 0x8e31e1b0, /// 0xed8
			 0xffbdba0b, /// 0xedc
			 0xfd425e3b, /// 0xee0
			 0xa20f9a96, /// 0xee4
			 0xac5c39f9, /// 0xee8
			 0x62aaf9e6, /// 0xeec
			 0xee7953f2, /// 0xef0
			 0x1b6ee294, /// 0xef4
			 0xfa5ddc6d, /// 0xef8
			 0x2daac8fb, /// 0xefc
			 0x8331d2c2, /// 0xf00
			 0xbf6d64f2, /// 0xf04
			 0xf0fca61e, /// 0xf08
			 0xeca359da, /// 0xf0c
			 0x2752465c, /// 0xf10
			 0x52444742, /// 0xf14
			 0xad73ff54, /// 0xf18
			 0x9858d161, /// 0xf1c
			 0xe87edbf9, /// 0xf20
			 0x565e4fd1, /// 0xf24
			 0x9a4b0ecb, /// 0xf28
			 0x1a3a6606, /// 0xf2c
			 0x611b39b5, /// 0xf30
			 0x17e5ede9, /// 0xf34
			 0x22448e74, /// 0xf38
			 0x3f225005, /// 0xf3c
			 0x0dcd7ba6, /// 0xf40
			 0x41279d8f, /// 0xf44
			 0x7649f63f, /// 0xf48
			 0x29c97aed, /// 0xf4c
			 0xa1c2e971, /// 0xf50
			 0xf64378dc, /// 0xf54
			 0xc9372a63, /// 0xf58
			 0xaaf24366, /// 0xf5c
			 0x7df7f2aa, /// 0xf60
			 0x153b617b, /// 0xf64
			 0x35492c82, /// 0xf68
			 0x08d88dff, /// 0xf6c
			 0xd6ecb527, /// 0xf70
			 0x078ec5bb, /// 0xf74
			 0x66fb0ca9, /// 0xf78
			 0x962eba35, /// 0xf7c
			 0x8473bb0d, /// 0xf80
			 0xd6a3bd5e, /// 0xf84
			 0x0bc9457e, /// 0xf88
			 0xa6f00def, /// 0xf8c
			 0x0171fca4, /// 0xf90
			 0xb64f479d, /// 0xf94
			 0xc18c8619, /// 0xf98
			 0xe8ae4d88, /// 0xf9c
			 0x2d1f0424, /// 0xfa0
			 0xda056acf, /// 0xfa4
			 0xe054708e, /// 0xfa8
			 0xa704829b, /// 0xfac
			 0xd17772ad, /// 0xfb0
			 0x8f00868c, /// 0xfb4
			 0x5d6d1da8, /// 0xfb8
			 0xdd187b8d, /// 0xfbc
			 0xd30a7e77, /// 0xfc0
			 0x1405874e, /// 0xfc4
			 0x9e32b0a7, /// 0xfc8
			 0x588b46dd, /// 0xfcc
			 0xa92581e6, /// 0xfd0
			 0x0a30f6b7, /// 0xfd4
			 0xf202c87d, /// 0xfd8
			 0x4d0e1b24, /// 0xfdc
			 0x26dec868, /// 0xfe0
			 0x44868405, /// 0xfe4
			 0x7d82f1c5, /// 0xfe8
			 0x5df22fb7, /// 0xfec
			 0xdbe41f31, /// 0xff0
			 0x9500a707, /// 0xff4
			 0x2b395fa4, /// 0xff8
			 0x5e706ceb, /// 0xffc
			 0xef49f059, /// 0x1000
			 0x6aeddcbd, /// 0x1004
			 0xdfffe897, /// 0x1008
			 0xa7056d1b, /// 0x100c
			 0xff426696, /// 0x1010
			 0xb10019ee, /// 0x1014
			 0xc3c5a0d4, /// 0x1018
			 0xff66fc9c, /// 0x101c
			 0x1232d46b, /// 0x1020
			 0x28ba85ce, /// 0x1024
			 0x42447855, /// 0x1028
			 0xb19e5154, /// 0x102c
			 0xef57402e, /// 0x1030
			 0xde06310d, /// 0x1034
			 0x55e3e35f, /// 0x1038
			 0x1563db0a, /// 0x103c
			 0x55e23320, /// 0x1040
			 0xa57ea55e, /// 0x1044
			 0xd3c25259, /// 0x1048
			 0x4619b058, /// 0x104c
			 0x59caca79, /// 0x1050
			 0x373be8f2, /// 0x1054
			 0xc31bd70e, /// 0x1058
			 0x7fcef4f7, /// 0x105c
			 0x6e063811, /// 0x1060
			 0xced5c479, /// 0x1064
			 0x5c00b916, /// 0x1068
			 0xfedf4d35, /// 0x106c
			 0xf1413b2f, /// 0x1070
			 0x091bb8f8, /// 0x1074
			 0x8a28afa6, /// 0x1078
			 0x8545f8e4, /// 0x107c
			 0xc440840f, /// 0x1080
			 0xc85cd63d, /// 0x1084
			 0xc33c7bf5, /// 0x1088
			 0xf13a1d20, /// 0x108c
			 0x4604abdc, /// 0x1090
			 0x5551a57e, /// 0x1094
			 0x5e74a670, /// 0x1098
			 0x8101d902, /// 0x109c
			 0xd23a5b85, /// 0x10a0
			 0x2dee0e29, /// 0x10a4
			 0x1f9a9b43, /// 0x10a8
			 0x54c41ece, /// 0x10ac
			 0xc06a390c, /// 0x10b0
			 0x23213477, /// 0x10b4
			 0xebf60e66, /// 0x10b8
			 0x5543a6cb, /// 0x10bc
			 0x13d58bac, /// 0x10c0
			 0xdbfe258f, /// 0x10c4
			 0x391abe9a, /// 0x10c8
			 0x19135551, /// 0x10cc
			 0x7ba46322, /// 0x10d0
			 0x957d4d8c, /// 0x10d4
			 0x0c878fb1, /// 0x10d8
			 0x53bf9a01, /// 0x10dc
			 0x3755d2fa, /// 0x10e0
			 0x43b1c4ac, /// 0x10e4
			 0x583466dd, /// 0x10e8
			 0x07a9b3a1, /// 0x10ec
			 0x26fc277e, /// 0x10f0
			 0x43563f99, /// 0x10f4
			 0x0328dc39, /// 0x10f8
			 0xc97cd4bb, /// 0x10fc
			 0x274e499c, /// 0x1100
			 0xdecfe747, /// 0x1104
			 0x5784298d, /// 0x1108
			 0x89b41c8c, /// 0x110c
			 0xa0ea99d2, /// 0x1110
			 0xbeb246d8, /// 0x1114
			 0x10971f9c, /// 0x1118
			 0x109aabb0, /// 0x111c
			 0xf83d6397, /// 0x1120
			 0x83f64ce8, /// 0x1124
			 0xddc64f6d, /// 0x1128
			 0x55948f01, /// 0x112c
			 0xaadfcbfa, /// 0x1130
			 0x490f540b, /// 0x1134
			 0x18e383d7, /// 0x1138
			 0x713fcf08, /// 0x113c
			 0xe3d1abbf, /// 0x1140
			 0xeea96f81, /// 0x1144
			 0x4913fa58, /// 0x1148
			 0x1dcda9ba, /// 0x114c
			 0xa35b5c79, /// 0x1150
			 0x4d0706cf, /// 0x1154
			 0xc18098b4, /// 0x1158
			 0xe757c380, /// 0x115c
			 0x5663d24f, /// 0x1160
			 0xe7990d5c, /// 0x1164
			 0x03bd7574, /// 0x1168
			 0x1daf812b, /// 0x116c
			 0x56293aac, /// 0x1170
			 0xa2e456c5, /// 0x1174
			 0xc42725ba, /// 0x1178
			 0xd3bebf3f, /// 0x117c
			 0x4f86363b, /// 0x1180
			 0xfb7a0f14, /// 0x1184
			 0x4caccff0, /// 0x1188
			 0x5ef9e35b, /// 0x118c
			 0xe6166a32, /// 0x1190
			 0xffd0e52c, /// 0x1194
			 0xfde0f846, /// 0x1198
			 0x142e4644, /// 0x119c
			 0x618d10c4, /// 0x11a0
			 0xb8f717d5, /// 0x11a4
			 0x263ebae8, /// 0x11a8
			 0x5b28593b, /// 0x11ac
			 0x00f6052f, /// 0x11b0
			 0xeb05d346, /// 0x11b4
			 0x24876759, /// 0x11b8
			 0x45c2505c, /// 0x11bc
			 0xcce343fd, /// 0x11c0
			 0xf9ba9e1c, /// 0x11c4
			 0x360bd10e, /// 0x11c8
			 0xaf6d7391, /// 0x11cc
			 0xd5eae8b5, /// 0x11d0
			 0x12fa903f, /// 0x11d4
			 0xa92309b1, /// 0x11d8
			 0x0780c7f2, /// 0x11dc
			 0x464283f6, /// 0x11e0
			 0xfe1161ae, /// 0x11e4
			 0xf7bb7a81, /// 0x11e8
			 0xb5c3c3a1, /// 0x11ec
			 0xfeefa4a2, /// 0x11f0
			 0xa56b51c6, /// 0x11f4
			 0x70caeda7, /// 0x11f8
			 0x2fbd3bbf, /// 0x11fc
			 0xd2618f4f, /// 0x1200
			 0xe58df21c, /// 0x1204
			 0xc39e7375, /// 0x1208
			 0x90af9464, /// 0x120c
			 0x61e1b02f, /// 0x1210
			 0xaba10bf5, /// 0x1214
			 0xb1c7d54e, /// 0x1218
			 0x2ac6aab9, /// 0x121c
			 0x517194bf, /// 0x1220
			 0x94d5a8b7, /// 0x1224
			 0x381fbd44, /// 0x1228
			 0x06d87f77, /// 0x122c
			 0x0ab8b1d0, /// 0x1230
			 0x70fdf496, /// 0x1234
			 0x47b98fc0, /// 0x1238
			 0xd8f98a1a, /// 0x123c
			 0xce4751dc, /// 0x1240
			 0x51b7b6eb, /// 0x1244
			 0x30d2c551, /// 0x1248
			 0xabfb3e83, /// 0x124c
			 0x300758ad, /// 0x1250
			 0x2521ea98, /// 0x1254
			 0x360f0b14, /// 0x1258
			 0xff98112a, /// 0x125c
			 0x05c41ef7, /// 0x1260
			 0x8fc06db8, /// 0x1264
			 0x20361965, /// 0x1268
			 0xcbabf960, /// 0x126c
			 0x19803b12, /// 0x1270
			 0x0322d8ba, /// 0x1274
			 0x80f4953c, /// 0x1278
			 0x38ea9185, /// 0x127c
			 0x7c62d15b, /// 0x1280
			 0xbbf532b2, /// 0x1284
			 0x6ea4cb7a, /// 0x1288
			 0x85c03adf, /// 0x128c
			 0x55050b1c, /// 0x1290
			 0x9aaa2ae4, /// 0x1294
			 0x9f80007b, /// 0x1298
			 0x24c8d10d, /// 0x129c
			 0x3615f715, /// 0x12a0
			 0x7390fa16, /// 0x12a4
			 0x4f75a98f, /// 0x12a8
			 0x2447768e, /// 0x12ac
			 0x316ce690, /// 0x12b0
			 0x390382cd, /// 0x12b4
			 0x18b46812, /// 0x12b8
			 0x871bbca5, /// 0x12bc
			 0x48b4c154, /// 0x12c0
			 0x8ceca3f1, /// 0x12c4
			 0x8ec6a979, /// 0x12c8
			 0xd13eaa22, /// 0x12cc
			 0xfd5e3121, /// 0x12d0
			 0xa8c6f181, /// 0x12d4
			 0xb8f3ccfe, /// 0x12d8
			 0x81123192, /// 0x12dc
			 0x81098d4e, /// 0x12e0
			 0x6f309f1b, /// 0x12e4
			 0x60c8b487, /// 0x12e8
			 0x71f383ee, /// 0x12ec
			 0xe7cfe932, /// 0x12f0
			 0x879be6de, /// 0x12f4
			 0x41eb91b2, /// 0x12f8
			 0x1364b86e, /// 0x12fc
			 0x3b3a3f3a, /// 0x1300
			 0x7e32f7d0, /// 0x1304
			 0x917d7dbc, /// 0x1308
			 0x26dce2e4, /// 0x130c
			 0x05490ac9, /// 0x1310
			 0xba097946, /// 0x1314
			 0x2caa9d97, /// 0x1318
			 0x114f54d1, /// 0x131c
			 0x9fd8d019, /// 0x1320
			 0x4166d401, /// 0x1324
			 0x9d7b481e, /// 0x1328
			 0xc605250a, /// 0x132c
			 0x7290d9a8, /// 0x1330
			 0x625be6ee, /// 0x1334
			 0x9a17b667, /// 0x1338
			 0xadd6062b, /// 0x133c
			 0xe7d753be, /// 0x1340
			 0xe5d0cd32, /// 0x1344
			 0xc3d38d67, /// 0x1348
			 0x8b2d9b8a, /// 0x134c
			 0x9d4d7fd1, /// 0x1350
			 0x0cf749d3, /// 0x1354
			 0x4b4fd6bb, /// 0x1358
			 0xb942e7e3, /// 0x135c
			 0x178e9664, /// 0x1360
			 0xcd099e0b, /// 0x1364
			 0xea8501eb, /// 0x1368
			 0x2aa92cca, /// 0x136c
			 0x74756f33, /// 0x1370
			 0x9f1e6938, /// 0x1374
			 0xa2a5b7e9, /// 0x1378
			 0x80547fba, /// 0x137c
			 0xf7412480, /// 0x1380
			 0xb1f0643a, /// 0x1384
			 0x890638a3, /// 0x1388
			 0xfffd87b2, /// 0x138c
			 0x5cc65ffc, /// 0x1390
			 0x90cc5388, /// 0x1394
			 0x630a0f14, /// 0x1398
			 0x57311784, /// 0x139c
			 0x041b3f52, /// 0x13a0
			 0xda0d0db5, /// 0x13a4
			 0xb77e3af7, /// 0x13a8
			 0xd6be099d, /// 0x13ac
			 0xfd8717f5, /// 0x13b0
			 0xf0de7001, /// 0x13b4
			 0x9955546e, /// 0x13b8
			 0xf4e2f67e, /// 0x13bc
			 0xa7d9d58a, /// 0x13c0
			 0xd131ce81, /// 0x13c4
			 0xd100cc2a, /// 0x13c8
			 0x1c02e954, /// 0x13cc
			 0x32d24a27, /// 0x13d0
			 0xc573368b, /// 0x13d4
			 0x41f2b9dd, /// 0x13d8
			 0x3ad23994, /// 0x13dc
			 0x6729e670, /// 0x13e0
			 0x26c57f21, /// 0x13e4
			 0x06d2b965, /// 0x13e8
			 0x78afd1f4, /// 0x13ec
			 0x6782bb29, /// 0x13f0
			 0xd338fc64, /// 0x13f4
			 0xe50b145e, /// 0x13f8
			 0xec49ea78, /// 0x13fc
			 0x8a5acc62, /// 0x1400
			 0x17b92f22, /// 0x1404
			 0x2299d66e, /// 0x1408
			 0x07cd2cdc, /// 0x140c
			 0x51202326, /// 0x1410
			 0xfe192233, /// 0x1414
			 0xa0aca46c, /// 0x1418
			 0xcafcf849, /// 0x141c
			 0x021642a6, /// 0x1420
			 0x62916320, /// 0x1424
			 0xf8a23e23, /// 0x1428
			 0x298cc998, /// 0x142c
			 0x38616967, /// 0x1430
			 0x46333f7f, /// 0x1434
			 0xd59388b8, /// 0x1438
			 0xf66c6135, /// 0x143c
			 0x1d30e63b, /// 0x1440
			 0x4de2b309, /// 0x1444
			 0x14a50624, /// 0x1448
			 0x648d4116, /// 0x144c
			 0x9e8fca83, /// 0x1450
			 0x72697bc5, /// 0x1454
			 0xd72dc8e8, /// 0x1458
			 0xf4dfa24a, /// 0x145c
			 0xf223f265, /// 0x1460
			 0x5cd8575f, /// 0x1464
			 0xfdc9947d, /// 0x1468
			 0x9077b93e, /// 0x146c
			 0x16085885, /// 0x1470
			 0x41563c87, /// 0x1474
			 0xc4a9baf0, /// 0x1478
			 0x55cbedfd, /// 0x147c
			 0x96a9b496, /// 0x1480
			 0xe63c9a6d, /// 0x1484
			 0x4ef12171, /// 0x1488
			 0x7ca15edd, /// 0x148c
			 0x7d65bb3d, /// 0x1490
			 0x4aee8b41, /// 0x1494
			 0x6aa0e001, /// 0x1498
			 0x33ee1a2b, /// 0x149c
			 0x46b69527, /// 0x14a0
			 0x30f5909c, /// 0x14a4
			 0x95666574, /// 0x14a8
			 0xce3be48b, /// 0x14ac
			 0x3b9cfd54, /// 0x14b0
			 0x618a7773, /// 0x14b4
			 0x36160db1, /// 0x14b8
			 0xf0870f47, /// 0x14bc
			 0xd4fcfcbe, /// 0x14c0
			 0xe7b80122, /// 0x14c4
			 0xf5afe21b, /// 0x14c8
			 0x916132e9, /// 0x14cc
			 0x0072f0a3, /// 0x14d0
			 0x40a1865b, /// 0x14d4
			 0xe6a80803, /// 0x14d8
			 0xef8e3451, /// 0x14dc
			 0x197e1e1e, /// 0x14e0
			 0xaffd9217, /// 0x14e4
			 0x3d301080, /// 0x14e8
			 0x76f79e6d, /// 0x14ec
			 0x674e53a2, /// 0x14f0
			 0x549df3fc, /// 0x14f4
			 0x4de0fd7b, /// 0x14f8
			 0xcb50be69, /// 0x14fc
			 0x23888261, /// 0x1500
			 0xd7c425d5, /// 0x1504
			 0xbd73e84b, /// 0x1508
			 0xbfa30bb6, /// 0x150c
			 0xd9643595, /// 0x1510
			 0xc4d77efa, /// 0x1514
			 0xf835469e, /// 0x1518
			 0x110b8ce0, /// 0x151c
			 0xc509ac34, /// 0x1520
			 0xa4007999, /// 0x1524
			 0xaaf3b3d8, /// 0x1528
			 0xd5f2d9fb, /// 0x152c
			 0xe9b2204e, /// 0x1530
			 0xb84df8e8, /// 0x1534
			 0x060852b2, /// 0x1538
			 0x5925e94c, /// 0x153c
			 0xc20952e5, /// 0x1540
			 0x16a7f782, /// 0x1544
			 0x6dc584ed, /// 0x1548
			 0x897d7d44, /// 0x154c
			 0xd88b3522, /// 0x1550
			 0x779c50f8, /// 0x1554
			 0xba277736, /// 0x1558
			 0x99bbcc1d, /// 0x155c
			 0x63b88a73, /// 0x1560
			 0xa834be3c, /// 0x1564
			 0x3a1373af, /// 0x1568
			 0x68eade27, /// 0x156c
			 0x1d74e69f, /// 0x1570
			 0xeab874b0, /// 0x1574
			 0x0a5f8b7c, /// 0x1578
			 0x367fc96b, /// 0x157c
			 0x59faf81d, /// 0x1580
			 0x0f18795f, /// 0x1584
			 0xc0497b13, /// 0x1588
			 0xa423b5bd, /// 0x158c
			 0xfa8e6b1f, /// 0x1590
			 0xc7988c8a, /// 0x1594
			 0x5c7f7de9, /// 0x1598
			 0xac7e01f8, /// 0x159c
			 0x83893af5, /// 0x15a0
			 0x372d2d4c, /// 0x15a4
			 0xb389c55a, /// 0x15a8
			 0x68309572, /// 0x15ac
			 0x73e75d4d, /// 0x15b0
			 0x972518aa, /// 0x15b4
			 0x128fae47, /// 0x15b8
			 0x41698a58, /// 0x15bc
			 0x2ac6df7b, /// 0x15c0
			 0xaee5a3c3, /// 0x15c4
			 0xc0954eab, /// 0x15c8
			 0xbceead63, /// 0x15cc
			 0x6f960b33, /// 0x15d0
			 0xfa7fe5a8, /// 0x15d4
			 0x90cb6c84, /// 0x15d8
			 0x52924fbd, /// 0x15dc
			 0xf7aa538d, /// 0x15e0
			 0x0403f10c, /// 0x15e4
			 0x37d73743, /// 0x15e8
			 0x2e40ee10, /// 0x15ec
			 0xcc4b236a, /// 0x15f0
			 0x0bea14a2, /// 0x15f4
			 0x961be528, /// 0x15f8
			 0x457e8adc, /// 0x15fc
			 0x765ef4e6, /// 0x1600
			 0xc6f785d6, /// 0x1604
			 0x5daae2ed, /// 0x1608
			 0xcd4f4d60, /// 0x160c
			 0x60b044f1, /// 0x1610
			 0xc223ba6f, /// 0x1614
			 0x9efbc72a, /// 0x1618
			 0xe416f133, /// 0x161c
			 0xd6829d87, /// 0x1620
			 0x6e7b7c8d, /// 0x1624
			 0x4cd53c43, /// 0x1628
			 0xe9e93cb7, /// 0x162c
			 0x400c7d6d, /// 0x1630
			 0x68abdd59, /// 0x1634
			 0x8dc12b14, /// 0x1638
			 0x60d1fdd9, /// 0x163c
			 0xad2d6e95, /// 0x1640
			 0xdd5daad7, /// 0x1644
			 0x5692c760, /// 0x1648
			 0x000fa1c9, /// 0x164c
			 0x22a9cd45, /// 0x1650
			 0x2f29cc18, /// 0x1654
			 0x55fe10bb, /// 0x1658
			 0xb711feba, /// 0x165c
			 0xcf844035, /// 0x1660
			 0x7bfdba1e, /// 0x1664
			 0x698ab807, /// 0x1668
			 0xc393ff0d, /// 0x166c
			 0xaec1b1a2, /// 0x1670
			 0x1c7eb04b, /// 0x1674
			 0x726069d8, /// 0x1678
			 0xc45229a7, /// 0x167c
			 0x1c353b0e, /// 0x1680
			 0xcbb39eca, /// 0x1684
			 0x9bb7a57e, /// 0x1688
			 0xc406af7b, /// 0x168c
			 0xd691c28f, /// 0x1690
			 0x40ec56d0, /// 0x1694
			 0x9944a795, /// 0x1698
			 0x64284f04, /// 0x169c
			 0x9916ad7f, /// 0x16a0
			 0x18f6c21d, /// 0x16a4
			 0x780d0591, /// 0x16a8
			 0xa0d894bf, /// 0x16ac
			 0x57914a83, /// 0x16b0
			 0xb5c67f36, /// 0x16b4
			 0x8f57f177, /// 0x16b8
			 0x3f6e4cd1, /// 0x16bc
			 0xecdcb617, /// 0x16c0
			 0x77eb53dd, /// 0x16c4
			 0xed642dce, /// 0x16c8
			 0xb401e16a, /// 0x16cc
			 0x77c46e54, /// 0x16d0
			 0xc7241e08, /// 0x16d4
			 0xa48ba773, /// 0x16d8
			 0x57ec894c, /// 0x16dc
			 0xe7ff343c, /// 0x16e0
			 0x4728625c, /// 0x16e4
			 0x092ce4a3, /// 0x16e8
			 0x34475bc6, /// 0x16ec
			 0x1b7a10a4, /// 0x16f0
			 0x27907623, /// 0x16f4
			 0x9fe56482, /// 0x16f8
			 0x0d5f0e1a, /// 0x16fc
			 0x0bd20c38, /// 0x1700
			 0x7fc7a2d7, /// 0x1704
			 0xd0b8a390, /// 0x1708
			 0xf645160e, /// 0x170c
			 0x0bea1935, /// 0x1710
			 0x28587a3f, /// 0x1714
			 0xb72d407c, /// 0x1718
			 0x97e73d53, /// 0x171c
			 0xae17c839, /// 0x1720
			 0xa15887b1, /// 0x1724
			 0xdc0451e9, /// 0x1728
			 0x8264464d, /// 0x172c
			 0x24fd219a, /// 0x1730
			 0x55cdab50, /// 0x1734
			 0xbb2b6fe2, /// 0x1738
			 0x2eb28492, /// 0x173c
			 0xc9dbee3b, /// 0x1740
			 0xee28369a, /// 0x1744
			 0xcc8b2c7a, /// 0x1748
			 0x4d9a9875, /// 0x174c
			 0xcc9651e9, /// 0x1750
			 0x6d0863d0, /// 0x1754
			 0x2237b36f, /// 0x1758
			 0x874d27e8, /// 0x175c
			 0xf07d82a6, /// 0x1760
			 0xd7c3d768, /// 0x1764
			 0xb2e8df3c, /// 0x1768
			 0x3b11177b, /// 0x176c
			 0x1f98aa41, /// 0x1770
			 0x79a435c9, /// 0x1774
			 0x4d62f03a, /// 0x1778
			 0xbdd58ad5, /// 0x177c
			 0x6717e86c, /// 0x1780
			 0xdb06e80c, /// 0x1784
			 0x9500fc24, /// 0x1788
			 0xf2d53ea6, /// 0x178c
			 0x495734cb, /// 0x1790
			 0x3b8620e9, /// 0x1794
			 0x6f8fbf62, /// 0x1798
			 0x2e74feea, /// 0x179c
			 0x7f8f1064, /// 0x17a0
			 0x1281714a, /// 0x17a4
			 0x23c98302, /// 0x17a8
			 0xe46610a7, /// 0x17ac
			 0xfd88b0d3, /// 0x17b0
			 0xe38d8496, /// 0x17b4
			 0x3a8384dc, /// 0x17b8
			 0x3c133bb0, /// 0x17bc
			 0x08a44b7c, /// 0x17c0
			 0xb2650509, /// 0x17c4
			 0x6a498bc5, /// 0x17c8
			 0xd4d7cdd0, /// 0x17cc
			 0x93f24177, /// 0x17d0
			 0xb81378f9, /// 0x17d4
			 0x5c8fccb0, /// 0x17d8
			 0x475edeb0, /// 0x17dc
			 0x2abe5518, /// 0x17e0
			 0x836fcd2d, /// 0x17e4
			 0x2fb33c76, /// 0x17e8
			 0x1ce64c04, /// 0x17ec
			 0xdf17f67b, /// 0x17f0
			 0x77ffebc7, /// 0x17f4
			 0x488adfe0, /// 0x17f8
			 0xb4bec586, /// 0x17fc
			 0x7dc0cc08, /// 0x1800
			 0x57689e7a, /// 0x1804
			 0x71e21b9e, /// 0x1808
			 0xf10fc1dd, /// 0x180c
			 0x60a85c40, /// 0x1810
			 0xba12051c, /// 0x1814
			 0x637c3988, /// 0x1818
			 0x4ca528f4, /// 0x181c
			 0x48da18eb, /// 0x1820
			 0xa666f34c, /// 0x1824
			 0xd639eef4, /// 0x1828
			 0x03fea6fa, /// 0x182c
			 0xc4e59729, /// 0x1830
			 0xd31d18f0, /// 0x1834
			 0x54cd8122, /// 0x1838
			 0x36deea2e, /// 0x183c
			 0x77fcdce4, /// 0x1840
			 0xe4602a05, /// 0x1844
			 0x898ce134, /// 0x1848
			 0x728426d3, /// 0x184c
			 0xfb6d4bdf, /// 0x1850
			 0x2dcee0f7, /// 0x1854
			 0xc66219bb, /// 0x1858
			 0xabf534b6, /// 0x185c
			 0x819cf5e1, /// 0x1860
			 0xc0a56505, /// 0x1864
			 0xc98a72cf, /// 0x1868
			 0xd24d1ae9, /// 0x186c
			 0xdc307549, /// 0x1870
			 0x5423dc4a, /// 0x1874
			 0x691b7a17, /// 0x1878
			 0x9b4d0729, /// 0x187c
			 0xbab1d7c1, /// 0x1880
			 0xcf52d32c, /// 0x1884
			 0x2c048da5, /// 0x1888
			 0x3bae4674, /// 0x188c
			 0x86ec901a, /// 0x1890
			 0xd996c73c, /// 0x1894
			 0xb37854a0, /// 0x1898
			 0x9effbe86, /// 0x189c
			 0x43378904, /// 0x18a0
			 0xb11542f3, /// 0x18a4
			 0xe523b35b, /// 0x18a8
			 0x0c544719, /// 0x18ac
			 0x82f3448f, /// 0x18b0
			 0x830282db, /// 0x18b4
			 0x227681d5, /// 0x18b8
			 0x5b95246f, /// 0x18bc
			 0x24bbc800, /// 0x18c0
			 0x2fe62e1d, /// 0x18c4
			 0xd039d338, /// 0x18c8
			 0x89eac109, /// 0x18cc
			 0x7f1889c9, /// 0x18d0
			 0x6722d83f, /// 0x18d4
			 0x20981fbd, /// 0x18d8
			 0xcea10b67, /// 0x18dc
			 0x719bcb37, /// 0x18e0
			 0xc3059654, /// 0x18e4
			 0x03060327, /// 0x18e8
			 0xa9aafa4b, /// 0x18ec
			 0x2bb4c914, /// 0x18f0
			 0xcb3ab92d, /// 0x18f4
			 0x097e7f30, /// 0x18f8
			 0xa38e039c, /// 0x18fc
			 0x43ce2c1a, /// 0x1900
			 0xf8147a6e, /// 0x1904
			 0x608a9659, /// 0x1908
			 0x4265d410, /// 0x190c
			 0x7e588ed7, /// 0x1910
			 0x656f9883, /// 0x1914
			 0x14da816d, /// 0x1918
			 0x56fb0fc9, /// 0x191c
			 0x4ed2f518, /// 0x1920
			 0x17db9de3, /// 0x1924
			 0x08aabeb7, /// 0x1928
			 0xf9c450a5, /// 0x192c
			 0x7d62f7dd, /// 0x1930
			 0x35885683, /// 0x1934
			 0x2da8dc90, /// 0x1938
			 0x17bd7b42, /// 0x193c
			 0x4ef54cab, /// 0x1940
			 0x43420406, /// 0x1944
			 0xa2106af5, /// 0x1948
			 0xa2cb42b5, /// 0x194c
			 0x2aa708c9, /// 0x1950
			 0xf6e32c20, /// 0x1954
			 0x9e585948, /// 0x1958
			 0xf07f4fb3, /// 0x195c
			 0x402489d9, /// 0x1960
			 0x917d47ef, /// 0x1964
			 0x39c90dc0, /// 0x1968
			 0x89821fee, /// 0x196c
			 0x0cb615ed, /// 0x1970
			 0x8f80c10d, /// 0x1974
			 0x99d615cd, /// 0x1978
			 0x043a5dc8, /// 0x197c
			 0xc5b247c1, /// 0x1980
			 0xe5a44085, /// 0x1984
			 0xdeb7a129, /// 0x1988
			 0xf3040f0a, /// 0x198c
			 0xc98d0411, /// 0x1990
			 0xe76daa40, /// 0x1994
			 0xc67429eb, /// 0x1998
			 0x0f32cdb3, /// 0x199c
			 0x1e66796d, /// 0x19a0
			 0xd9a9e118, /// 0x19a4
			 0xec4f43d8, /// 0x19a8
			 0x4fb7b08f, /// 0x19ac
			 0x85e2d9d9, /// 0x19b0
			 0x31ef7388, /// 0x19b4
			 0xa0b91dc9, /// 0x19b8
			 0xb0543003, /// 0x19bc
			 0x3db71904, /// 0x19c0
			 0x83bf33a7, /// 0x19c4
			 0x23f0f4bf, /// 0x19c8
			 0xfdd93236, /// 0x19cc
			 0x63526eef, /// 0x19d0
			 0x4d685d07, /// 0x19d4
			 0x90d864d5, /// 0x19d8
			 0x9587159c, /// 0x19dc
			 0x0dcf30bb, /// 0x19e0
			 0x33240c2d, /// 0x19e4
			 0x9dce5fc1, /// 0x19e8
			 0x993bedab, /// 0x19ec
			 0x1972c686, /// 0x19f0
			 0x1e47ba4e, /// 0x19f4
			 0xd2285803, /// 0x19f8
			 0xa0650f3f, /// 0x19fc
			 0xb16425eb, /// 0x1a00
			 0x3760399c, /// 0x1a04
			 0xf2eed144, /// 0x1a08
			 0x26bb80bc, /// 0x1a0c
			 0xf523f95b, /// 0x1a10
			 0x470c78c8, /// 0x1a14
			 0xaa7ca2ae, /// 0x1a18
			 0x66b1ef0c, /// 0x1a1c
			 0xe87c5f44, /// 0x1a20
			 0xc7bbba9d, /// 0x1a24
			 0xf4f18a66, /// 0x1a28
			 0x3f72e060, /// 0x1a2c
			 0xda4fec0c, /// 0x1a30
			 0xb458081d, /// 0x1a34
			 0x22d75d8a, /// 0x1a38
			 0x0fefd42d, /// 0x1a3c
			 0x9355a801, /// 0x1a40
			 0x1e7bdf6c, /// 0x1a44
			 0x54269e40, /// 0x1a48
			 0x06886153, /// 0x1a4c
			 0xae95baa5, /// 0x1a50
			 0x09b04528, /// 0x1a54
			 0x168e4132, /// 0x1a58
			 0xe9072d94, /// 0x1a5c
			 0xa10489df, /// 0x1a60
			 0x0427cfc6, /// 0x1a64
			 0x7d766b55, /// 0x1a68
			 0x59b8c808, /// 0x1a6c
			 0x088a1bea, /// 0x1a70
			 0xe85d7124, /// 0x1a74
			 0x494f8ffc, /// 0x1a78
			 0x69aed56b, /// 0x1a7c
			 0x12756a51, /// 0x1a80
			 0xc8c1feeb, /// 0x1a84
			 0xcb0743df, /// 0x1a88
			 0x4bc34494, /// 0x1a8c
			 0x8ec597d7, /// 0x1a90
			 0x84bbc3b0, /// 0x1a94
			 0x1863c98a, /// 0x1a98
			 0x9792ceec, /// 0x1a9c
			 0x8059eb40, /// 0x1aa0
			 0xf47dc03a, /// 0x1aa4
			 0x3cab33b5, /// 0x1aa8
			 0x457a0bcb, /// 0x1aac
			 0xbdaf69cd, /// 0x1ab0
			 0xc93bdd1b, /// 0x1ab4
			 0xd82b228c, /// 0x1ab8
			 0x7430bc0e, /// 0x1abc
			 0x1c14febf, /// 0x1ac0
			 0x377023c4, /// 0x1ac4
			 0x6e15cbc1, /// 0x1ac8
			 0xd533f70f, /// 0x1acc
			 0xc216e69d, /// 0x1ad0
			 0x1586726f, /// 0x1ad4
			 0xe204494c, /// 0x1ad8
			 0xc2124c59, /// 0x1adc
			 0xc7769cfc, /// 0x1ae0
			 0x53ea08d1, /// 0x1ae4
			 0x3b5c9520, /// 0x1ae8
			 0x6d93d5ab, /// 0x1aec
			 0x23f0b1cd, /// 0x1af0
			 0xe61e6e93, /// 0x1af4
			 0x56d4b9d7, /// 0x1af8
			 0xd359fe50, /// 0x1afc
			 0x3569803a, /// 0x1b00
			 0x1735d059, /// 0x1b04
			 0x44e41f60, /// 0x1b08
			 0xa3d72626, /// 0x1b0c
			 0xb647723d, /// 0x1b10
			 0x0a21648f, /// 0x1b14
			 0x204fe4d2, /// 0x1b18
			 0x6ab9cd86, /// 0x1b1c
			 0x398241c1, /// 0x1b20
			 0xc32983a5, /// 0x1b24
			 0xef9c69e0, /// 0x1b28
			 0xabe67d42, /// 0x1b2c
			 0x5ccda214, /// 0x1b30
			 0xf2a380bc, /// 0x1b34
			 0x599eca48, /// 0x1b38
			 0x62c0523f, /// 0x1b3c
			 0x2fcefde5, /// 0x1b40
			 0x295b2f1f, /// 0x1b44
			 0xef8a7372, /// 0x1b48
			 0x04521cee, /// 0x1b4c
			 0xeea01126, /// 0x1b50
			 0x6e8aa8b5, /// 0x1b54
			 0x53cde447, /// 0x1b58
			 0x4f59fd2b, /// 0x1b5c
			 0xf52c9b0c, /// 0x1b60
			 0xc1aca899, /// 0x1b64
			 0x74995212, /// 0x1b68
			 0xba5901f7, /// 0x1b6c
			 0xd8764da1, /// 0x1b70
			 0xd508f968, /// 0x1b74
			 0x9310a63a, /// 0x1b78
			 0x42802c9d, /// 0x1b7c
			 0x0f1507f4, /// 0x1b80
			 0xa3ae573c, /// 0x1b84
			 0xc1cd7797, /// 0x1b88
			 0x7265a39d, /// 0x1b8c
			 0xc099ad2d, /// 0x1b90
			 0xd3ed2889, /// 0x1b94
			 0xc3b4125f, /// 0x1b98
			 0x5337f85c, /// 0x1b9c
			 0xf8871337, /// 0x1ba0
			 0x8e37685b, /// 0x1ba4
			 0xd5bdeeef, /// 0x1ba8
			 0x8eb95ea9, /// 0x1bac
			 0x0b9f89b5, /// 0x1bb0
			 0x8a6e36d0, /// 0x1bb4
			 0xcd30209a, /// 0x1bb8
			 0x135fb33b, /// 0x1bbc
			 0x97003d8d, /// 0x1bc0
			 0x3f2e25c5, /// 0x1bc4
			 0x955b8818, /// 0x1bc8
			 0xbb8950ff, /// 0x1bcc
			 0x75731c34, /// 0x1bd0
			 0xcca48224, /// 0x1bd4
			 0x57294f0f, /// 0x1bd8
			 0x5f613387, /// 0x1bdc
			 0x64912a34, /// 0x1be0
			 0x92f1a091, /// 0x1be4
			 0x34c862ae, /// 0x1be8
			 0xd49770c6, /// 0x1bec
			 0x08544b0d, /// 0x1bf0
			 0xaa1b8f57, /// 0x1bf4
			 0xa2ab878f, /// 0x1bf8
			 0x6f771c9d, /// 0x1bfc
			 0x3dd90796, /// 0x1c00
			 0x4ca7dafc, /// 0x1c04
			 0xc328e00f, /// 0x1c08
			 0x300bb8ac, /// 0x1c0c
			 0x62f456de, /// 0x1c10
			 0x8e7b7573, /// 0x1c14
			 0x893902f9, /// 0x1c18
			 0x4d4db61e, /// 0x1c1c
			 0xce0d8b5e, /// 0x1c20
			 0x36d3d031, /// 0x1c24
			 0x8d8ef217, /// 0x1c28
			 0xdf960f93, /// 0x1c2c
			 0x396d168c, /// 0x1c30
			 0xb9b5886b, /// 0x1c34
			 0x8ec0f9e5, /// 0x1c38
			 0xb90cd6e5, /// 0x1c3c
			 0x9432f059, /// 0x1c40
			 0x70fb0fcd, /// 0x1c44
			 0x695f7adf, /// 0x1c48
			 0xe0faf28d, /// 0x1c4c
			 0xed7d6448, /// 0x1c50
			 0xd9527bf4, /// 0x1c54
			 0x8a612c56, /// 0x1c58
			 0x0cea6524, /// 0x1c5c
			 0xacd1a5fd, /// 0x1c60
			 0x5bccb708, /// 0x1c64
			 0x063137ca, /// 0x1c68
			 0x74357bdc, /// 0x1c6c
			 0x4fa6bb1c, /// 0x1c70
			 0xf9ea6fdd, /// 0x1c74
			 0x2e5e90b4, /// 0x1c78
			 0x26e98229, /// 0x1c7c
			 0x5805a471, /// 0x1c80
			 0xef3665ad, /// 0x1c84
			 0x42628486, /// 0x1c88
			 0xec18868c, /// 0x1c8c
			 0xe1a05cec, /// 0x1c90
			 0x52afbb26, /// 0x1c94
			 0xa89f0a09, /// 0x1c98
			 0x00ba8e2e, /// 0x1c9c
			 0x9e1b9929, /// 0x1ca0
			 0xf52ed520, /// 0x1ca4
			 0x546fa129, /// 0x1ca8
			 0xa5488fa6, /// 0x1cac
			 0x11fc2d8b, /// 0x1cb0
			 0xa53ffe2e, /// 0x1cb4
			 0xabc75ba0, /// 0x1cb8
			 0x80aad03f, /// 0x1cbc
			 0x07dccd51, /// 0x1cc0
			 0x611c934d, /// 0x1cc4
			 0x35ee19da, /// 0x1cc8
			 0x03ad8e12, /// 0x1ccc
			 0x25ad7654, /// 0x1cd0
			 0xd99012ea, /// 0x1cd4
			 0x04282f4b, /// 0x1cd8
			 0x7a163142, /// 0x1cdc
			 0x891d190d, /// 0x1ce0
			 0xc75a96cf, /// 0x1ce4
			 0x89a80369, /// 0x1ce8
			 0x05cc57b4, /// 0x1cec
			 0xe0c4d073, /// 0x1cf0
			 0x2e041940, /// 0x1cf4
			 0x77d679a6, /// 0x1cf8
			 0xed28fe43, /// 0x1cfc
			 0xcac7aed5, /// 0x1d00
			 0x33388764, /// 0x1d04
			 0xfbd93881, /// 0x1d08
			 0xf14cab95, /// 0x1d0c
			 0xf7a838da, /// 0x1d10
			 0xbabbd7fc, /// 0x1d14
			 0x47b232a8, /// 0x1d18
			 0xef6d235f, /// 0x1d1c
			 0xf96ca27b, /// 0x1d20
			 0xe33c2909, /// 0x1d24
			 0x8d46441e, /// 0x1d28
			 0xc2851fdb, /// 0x1d2c
			 0x8bad7132, /// 0x1d30
			 0x27e47d0c, /// 0x1d34
			 0x7697768a, /// 0x1d38
			 0xf52c4997, /// 0x1d3c
			 0x67918e43, /// 0x1d40
			 0x6990ca62, /// 0x1d44
			 0x857ff126, /// 0x1d48
			 0xa004b6b0, /// 0x1d4c
			 0x82361c97, /// 0x1d50
			 0xb0783372, /// 0x1d54
			 0x16ebee04, /// 0x1d58
			 0xc0614c20, /// 0x1d5c
			 0x5147853b, /// 0x1d60
			 0x85feaa77, /// 0x1d64
			 0x3c5b3106, /// 0x1d68
			 0x5cb9246c, /// 0x1d6c
			 0x2925c667, /// 0x1d70
			 0x3be4fe55, /// 0x1d74
			 0xedf64dcf, /// 0x1d78
			 0x89a492f4, /// 0x1d7c
			 0x52cd3796, /// 0x1d80
			 0xf50f68f8, /// 0x1d84
			 0x59d25909, /// 0x1d88
			 0xa6065d53, /// 0x1d8c
			 0x0b60425f, /// 0x1d90
			 0x70edff0b, /// 0x1d94
			 0x49a2c335, /// 0x1d98
			 0x728bf21c, /// 0x1d9c
			 0x28056eae, /// 0x1da0
			 0x42717a5e, /// 0x1da4
			 0xc842dd6d, /// 0x1da8
			 0x098af8b4, /// 0x1dac
			 0x2e5c979e, /// 0x1db0
			 0xb6941f09, /// 0x1db4
			 0xd613734a, /// 0x1db8
			 0xf6455f3a, /// 0x1dbc
			 0x1be675f9, /// 0x1dc0
			 0xc3e1ca5e, /// 0x1dc4
			 0x44b836fa, /// 0x1dc8
			 0xfa4dd9b9, /// 0x1dcc
			 0x3c1c5588, /// 0x1dd0
			 0xf73094db, /// 0x1dd4
			 0x2a95a3e5, /// 0x1dd8
			 0xb70083de, /// 0x1ddc
			 0x3ed5b0b1, /// 0x1de0
			 0x8b272ceb, /// 0x1de4
			 0x13536821, /// 0x1de8
			 0x6727ea76, /// 0x1dec
			 0xd8c832f4, /// 0x1df0
			 0xddc1210e, /// 0x1df4
			 0xb44df29e, /// 0x1df8
			 0xea94c3ff, /// 0x1dfc
			 0x72e91f4f, /// 0x1e00
			 0x3a9ab55c, /// 0x1e04
			 0x36be54c1, /// 0x1e08
			 0x7fd49251, /// 0x1e0c
			 0xc4b61758, /// 0x1e10
			 0xbc63a957, /// 0x1e14
			 0xf865588c, /// 0x1e18
			 0x93210d7b, /// 0x1e1c
			 0xb9176e8f, /// 0x1e20
			 0xb0979700, /// 0x1e24
			 0x33ad5458, /// 0x1e28
			 0x93b31f70, /// 0x1e2c
			 0xbe8a2686, /// 0x1e30
			 0x90c7cf9c, /// 0x1e34
			 0x3db28e7c, /// 0x1e38
			 0x40284306, /// 0x1e3c
			 0xa554044f, /// 0x1e40
			 0x192d9805, /// 0x1e44
			 0xe962aef0, /// 0x1e48
			 0x4acfbc6f, /// 0x1e4c
			 0x7cf23345, /// 0x1e50
			 0x6daf9b24, /// 0x1e54
			 0x131b5389, /// 0x1e58
			 0x35ea918d, /// 0x1e5c
			 0xd6103f89, /// 0x1e60
			 0xd833c23d, /// 0x1e64
			 0x66e7b1ad, /// 0x1e68
			 0xea5addc2, /// 0x1e6c
			 0xe0cb710d, /// 0x1e70
			 0x77351532, /// 0x1e74
			 0x30488352, /// 0x1e78
			 0x5afe0539, /// 0x1e7c
			 0xa9847dea, /// 0x1e80
			 0xc1e236a9, /// 0x1e84
			 0x697918b7, /// 0x1e88
			 0xa0e46e3c, /// 0x1e8c
			 0x3efac431, /// 0x1e90
			 0xab79633d, /// 0x1e94
			 0x03b87934, /// 0x1e98
			 0x2c51e595, /// 0x1e9c
			 0xcc63075e, /// 0x1ea0
			 0x6fcfc945, /// 0x1ea4
			 0xc0ff18ef, /// 0x1ea8
			 0xdf19b47d, /// 0x1eac
			 0xd99c9de5, /// 0x1eb0
			 0xf866e71e, /// 0x1eb4
			 0x95f5621a, /// 0x1eb8
			 0xe47c34bf, /// 0x1ebc
			 0x3ee15e26, /// 0x1ec0
			 0xd14fcbc2, /// 0x1ec4
			 0xf2019a8c, /// 0x1ec8
			 0x03d89a1b, /// 0x1ecc
			 0xa48a032d, /// 0x1ed0
			 0xd7448921, /// 0x1ed4
			 0xee8fc0bd, /// 0x1ed8
			 0x69c993d9, /// 0x1edc
			 0x3535f189, /// 0x1ee0
			 0x39d54b64, /// 0x1ee4
			 0xe35b31d8, /// 0x1ee8
			 0xc2656591, /// 0x1eec
			 0x4b0b803a, /// 0x1ef0
			 0x231057e8, /// 0x1ef4
			 0x32130bd9, /// 0x1ef8
			 0x9ba70258, /// 0x1efc
			 0xaa8bef95, /// 0x1f00
			 0x4ca05f39, /// 0x1f04
			 0xe84134aa, /// 0x1f08
			 0xaddc583f, /// 0x1f0c
			 0xeae9ab72, /// 0x1f10
			 0x03b82992, /// 0x1f14
			 0x28489260, /// 0x1f18
			 0x8cc2b901, /// 0x1f1c
			 0x80d76ece, /// 0x1f20
			 0xfe8b77bc, /// 0x1f24
			 0x686883e8, /// 0x1f28
			 0x5310c255, /// 0x1f2c
			 0xb62f7168, /// 0x1f30
			 0x956642e1, /// 0x1f34
			 0x240d5692, /// 0x1f38
			 0x9ffc578e, /// 0x1f3c
			 0x2ac3d0db, /// 0x1f40
			 0x24bca32c, /// 0x1f44
			 0x1fe0b9f5, /// 0x1f48
			 0xe243a830, /// 0x1f4c
			 0x93094479, /// 0x1f50
			 0x44a681b8, /// 0x1f54
			 0xf236ed64, /// 0x1f58
			 0x6e030245, /// 0x1f5c
			 0xf12e42e8, /// 0x1f60
			 0x566ceef5, /// 0x1f64
			 0xc268f013, /// 0x1f68
			 0x2479464a, /// 0x1f6c
			 0xfc8af615, /// 0x1f70
			 0x97a81023, /// 0x1f74
			 0xf3bc2fbd, /// 0x1f78
			 0x733dcdf5, /// 0x1f7c
			 0x133294a1, /// 0x1f80
			 0x9d7606f6, /// 0x1f84
			 0xfe4f12fc, /// 0x1f88
			 0xc748d92c, /// 0x1f8c
			 0xfdb0b3f4, /// 0x1f90
			 0xe1d75f28, /// 0x1f94
			 0x31424ae6, /// 0x1f98
			 0x20753a98, /// 0x1f9c
			 0x0d2b73f8, /// 0x1fa0
			 0x79720f30, /// 0x1fa4
			 0xb09c48f2, /// 0x1fa8
			 0x33d9d20d, /// 0x1fac
			 0xde3c494f, /// 0x1fb0
			 0xbef848cf, /// 0x1fb4
			 0x61e11b33, /// 0x1fb8
			 0x980872f4, /// 0x1fbc
			 0x469368e9, /// 0x1fc0
			 0x5575a4e5, /// 0x1fc4
			 0xf52a6a75, /// 0x1fc8
			 0xcd70d915, /// 0x1fcc
			 0x1443978c, /// 0x1fd0
			 0xbce233c1, /// 0x1fd4
			 0xb95de5e8, /// 0x1fd8
			 0x6904ae5d, /// 0x1fdc
			 0x5f32d116, /// 0x1fe0
			 0x927ff018, /// 0x1fe4
			 0x90679b28, /// 0x1fe8
			 0x8c2c3a86, /// 0x1fec
			 0x04a1ae6d, /// 0x1ff0
			 0x6c16a697, /// 0x1ff4
			 0x9772174c, /// 0x1ff8
			 0x11764d62 /// last
	};

	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			 // vpu register f0 is designated 0 register
			 0xde3c4940, /// 0x1fb0
			 0xde3c4941, /// 0x1fb0
			 0xde3c4942, /// 0x1fb0
			 0xde3c4943, /// 0x1fb0
			 0xde3c4944,/// 0x1fb0
			 0xde3c4945, /// 0x1fb0
			 0xde3c4946, /// 0x1fb0
			 0xde3c4947, /// 0x1fb0

			 // vpu register f1 is designated offset register for memory access ops
			 0x0f0003b0,
			 0x0f000234,
			 0x0f000560,
			 0x0f00046c,
			 0x0f000584,
			 0x0f000750,
			 0x0f0004f0,
			 0x0f00008c,

			 /// vpu register f2
			 0xf07ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x007ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ff800,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x70000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x000007ff,                                                  // Trailing 1s:                               

			 /// vpu register f3
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  

			 /// vpu register f4
			 0x0c100000,                                                  // Leading 1s:                                
			 0x7f1ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80120000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80101000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c1fff00,                                                  // Leading 1s:                                
			 0x0c1fe000,                                                  // Leading 1s:                                
			 0x8f1ffffe,                                                  // all bit of mantissa set upto -3ulp         

			 /// vpu register f5
			 0xff000000, // infinity                                      // SP - ve numbers                            
			 0x3f000000, // 1                                             // SP +ve numbers                             
			 0xbf000001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000001, // SNaN                                          // SP +ve numbers                             
			 0x0f0ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f0ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c0f8000,                                                  // Leading 1s:                                

			 /// vpu register f6
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             

			 /// vpu register f7
			 0xC0CCCCCC,                                                  // 4 random values                            
			 0x007fffe0,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x000003ff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xf07fffff, // max norm                                      // max norm -ve                               
			 0x707ffffe, // max norm - 1ulp                               // max norm +ve                               

			 /// vpu register f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 /// vpu register f9
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          

			 /// vpu register f10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            

			 /// vpu register f11
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                

			 /// vpu register f12
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xcb000000,                                                  // -8388608.0                                 
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       

			 /// vpu register f13
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            

			 /// vpu register f14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          

			 /// vpu register f15
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               

			 /// vpu register f16
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       

			 /// vpu register f17
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                

			 /// vpu register f18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x0e07ffff,                                                  // Trailing 1s:                               

			 /// vpu register f19
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                

			 /// vpu register f20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            

			 /// vpu register f21
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       

			 /// vpu register f22
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 /// vpu register f23
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                

			 /// vpu register f24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 /// vpu register f25
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       

			 /// vpu register f26
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                

			 /// vpu register f27
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c400000,                                                  // Leading 1s:                                

			 /// vpu register f28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 /// vpu register f29
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7e0000,                                                  // Leading 1s:                                

			 /// vpu register f30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // SP +ve numbers                             

			 /// vpu register f31
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x3f800000     /// 1.0                   
	};

	if (mid == 0 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M0_T0:\n"
		"j LBL_SETUP_EV_M0_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M0_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M0_T0\n"
	"LBL_SETUP_SCP_M0_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M0_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M0_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M0_T0:\n"
		"la x5, LBL_M_MODE_E_H_M0_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M0_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
		"mv x4, %[operands]\n"
		"flw.ps f0,  992 (x4)\n"
		"flw.ps f1,  960 (x4)\n"
		"flw.ps f2,  928 (x4)\n"
		"flw.ps f3,  896 (x4)\n"
		"flw.ps f4,  864 (x4)\n"
		"flw.ps f5,  832 (x4)\n"
		"flw.ps f6,  800 (x4)\n"
		"flw.ps f7,  768 (x4)\n"
		"flw.ps f8,  736 (x4)\n"
		"flw.ps f9,  704 (x4)\n"
		"flw.ps f10, 672 (x4)\n"
		"flw.ps f11, 640 (x4)\n"
		"flw.ps f12, 608 (x4)\n"
		"flw.ps f13, 576 (x4)\n"
		"flw.ps f14, 544 (x4)\n"
		"flw.ps f15, 512 (x4)\n"
		"flw.ps f16, 480 (x4)\n"
		"flw.ps f17, 448 (x4)\n"
		"flw.ps f18, 416 (x4)\n"
		"flw.ps f19, 384 (x4)\n"
		"flw.ps f20, 352 (x4)\n"
		"flw.ps f21, 320 (x4)\n"
		"flw.ps f22, 288 (x4)\n"
		"flw.ps f23, 256 (x4)\n"
		"flw.ps f24, 224 (x4)\n"
		"flw.ps f25, 192 (x4)\n"
		"flw.ps f26, 160 (x4)\n"
		"flw.ps f27, 128 (x4)\n"
		"flw.ps f28, 96  (x4)\n"
		"flw.ps f29, 64  (x4)\n"
		"flw.ps f30, 32  (x4)\n"
		"flw.ps f31, 0   (x4)\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (


                "li x3,  0xa \n "
                "li x1,  0x3 \n "

	        "reduce_M0_T0:\n"

                 "li x5,  0x10003 \n "
                 "li x6,  0x1 \n "
		 "add x6,x6,x1\n"        
                 /*"csrrw zero,  0x800, x5 \n "*/
                 "csrrw zero,  0x806, x6 \n "
		"faddi.pi f0, f0, 30\n"                              /// 25, 71032

		 /*
                 "li x5,  0x20003 \n "
                 "li x6,  0x200000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x30003 \n "
                 "li x6,  0x400000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x40003 \n "
                 "li x6,  0x600000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x50003 \n "
                 "li x6,  0x800000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x60003 \n "
                 "li x6,  0xa00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x70003 \n "
                 "li x6,  0xc00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x80003 \n "
                 "li x6,  0xe00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x90003 \n "
                 "li x6,  0x1000000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0xa0003 \n "
                 "li x6,  0x1200000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0xb0003 \n "
                 "li x6,  0x1400000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0xc0003 \n "
                 "li x6,  0x1600000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0xd0003 \n "
                 "li x6,  0x1800000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0xe0003 \n "
                 "li x6,  0x1a00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0xf0003 \n "
                 "li x6,  0x1c00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x100003 \n "
                 "li x6,  0x1e00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x110003 \n "
                 "li x6,  0x2000000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x120003 \n "
                 "li x6,  0x2200000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x130003 \n "
                 "li x6,  0x2400000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x140003 \n "
                 "li x6,  0x2600000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x150003 \n "
                 "li x6,  0x2800000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x160003 \n "
                 "li x6,  0x2a00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x170003 \n "
                 "li x6,  0x2c00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x180003 \n "
                 "li x6,  0x2e00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x190003 \n "
                 "li x6,  0x3000000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x1a0003 \n "
                 "li x6,  0x3200000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x1b0003 \n "
                 "li x6,  0x3400000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x1c0003 \n "
                 "li x6,  0x3600000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x1d0003 \n "
                 "li x6,  0x3800000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x1e0003 \n "
                 "li x6,  0x3a00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x1f0003 \n "
                 "li x6,  0x3c00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
                 "li x5,  0x200003 \n "
                 "li x6,  0x3e00000000000001 \n "
		 "add x6,x6,x1\n"        
                 "csrrw zero,  0x800, x5 \n "
                 "csrrw zero,  0x806, x6 \n "
		 "addi	x3,x3,-1\n"        
		 "addi	x1,x1, 1\n"        
		 "bne x3, x0, reduce_M0_T0\n"
*/
		"fence\n"


		"fence\n"

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M0_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 0 && tid == 1) { 

	C_TEST_START;

	C_TEST_PASS;
	return 0;
	};
	if (mid == 1 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M1_T0:\n"
		"j LBL_SETUP_EV_M1_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M1_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M1_T0\n"
	"LBL_SETUP_SCP_M1_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M1_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M1_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M1_T0:\n"
		"la x5, LBL_M_MODE_E_H_M1_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M1_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

		"fence\n"
                "li x3,  0xa \n "

	        /*"reduce_M1_T0:\n"
                "li x5,  0x10003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x20003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x30003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x40003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x50003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x60003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x70003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x80003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x90003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0xa0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0xb0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0xc0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0xd0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0xe0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0xf0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x100003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x110003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x120003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x130003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x140003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x150003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x160003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x170003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x180003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x190003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x1a0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x1b0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x1c0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x1d0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x1e0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x1f0003 \n "
                "csrrw zero,  0x800, x5 \n "
                "li x5,  0x200003 \n "
                "csrrw zero,  0x800, x5 \n "

		"addi	x3,x3,-1\n"        
		"bne x3, x0, reduce_M1_T0\n"

*/





		"fence\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M1_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 1 && tid == 1) { 

	C_TEST_START;
	C_TEST_PASS;
	return 0;
	};

	if (mid == 2 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M2_T0:\n"
		"j LBL_SETUP_EV_M2_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M2_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M2_T0\n"
	"LBL_SETUP_SCP_M2_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M2_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M2_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M2_T0:\n"
		"la x5, LBL_M_MODE_E_H_M2_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M2_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M2_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 2 && tid == 1) { 

	C_TEST_START;
	C_TEST_PASS;
	return 0;
	};
	if (mid == 3 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M3_T0:\n"
		"j LBL_SETUP_EV_M3_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M3_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M3_T0\n"
	"LBL_SETUP_SCP_M3_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M3_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M3_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M3_T0:\n"
		"la x5, LBL_M_MODE_E_H_M3_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M3_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

		"fence\n"

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M3_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 3 && tid == 1) { 

	C_TEST_START;
	C_TEST_PASS;
	return 0;
	};

	if (mid == 4 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M4_T0:\n"
		"j LBL_SETUP_EV_M4_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M4_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M4_T0\n"
	"LBL_SETUP_SCP_M4_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M4_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M4_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M4_T0:\n"
		"la x5, LBL_M_MODE_E_H_M4_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M4_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (


		"fence\n"


		"fence\n"

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M4_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 4 && tid == 1) { 

	C_TEST_START;
	C_TEST_PASS;
	return 0;
	};
	if (mid == 5 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M5_T0:\n"
		"j LBL_SETUP_EV_M5_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M5_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M5_T0\n"
	"LBL_SETUP_SCP_M5_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M5_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M5_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M5_T0:\n"
		"la x5, LBL_M_MODE_E_H_M5_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M5_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

		"fence\n"

		///tensor_load_vpu
        "li x31, 0x0\n"
        "add x5, x11, x31\n"


		///tensor_load_vpu
        "li x31, 0x0\n"

		"fence\n"

		"fence\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M5_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 5 && tid == 1) { 

	C_TEST_START;
	C_TEST_PASS;
	return 0;
	};

	if (mid == 6 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M6_T0:\n"
		"j LBL_SETUP_EV_M6_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M6_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M6_T0\n"
	"LBL_SETUP_SCP_M6_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M6_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M6_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M6_T0:\n"
		"la x5, LBL_M_MODE_E_H_M6_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M6_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (


		"fence\n"


		"fence\n"

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M6_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 6 && tid == 1) { 

	C_TEST_START;
	C_TEST_PASS;
	return 0;
	};
	if (mid == 7 && tid == 0) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M7_T0:\n"
		"j LBL_SETUP_EV_M7_T0\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M7_T0:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M7_T0\n"
	"LBL_SETUP_SCP_M7_T0:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M7_T0:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M7_T0:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M7_T0:\n"
		"la x5, LBL_M_MODE_E_H_M7_T0\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M7_T0\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

		"fence\n"

		///tensor_load_vpu
        "li x31, 0x0\n"
        "add x5, x11, x31\n"


		///tensor_load_vpu
        "li x31, 0x0\n"

		"fence\n"

		"fence\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M7_T0:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};
	if (mid == 7 && tid == 1) { 

	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START_M1_T1:\n"
		"j LBL_SETUP_EV_M1_T1\n"
		".align 12\n"
		"LBL_M_MODE_E_H_M1_T1:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR_M1_T1\n"
	"LBL_SETUP_SCP_M1_T1:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR_M1_T1:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H_M1_T1:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV_M1_T1:\n"
		"la x5, LBL_M_MODE_E_H_M1_T1\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H_M1_T1\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"li x5, 0x1000\n"
		"mv x11, %[m_11]\n"
		"add x11, x11, x5\n"
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
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

	"LBL_VPU_SEQ_START_15:\n"
		"mova.m.x x11\n"                                      /// 25, 71028
		"fmadd.ps f20, f30, f20, f7, rmm\n"                   /// 25, 71029
		"fmvs.x.ps x7, f22, 0\n"                              /// 25, 71030
		"fnmadd.ps f11, f29, f2, f0, rdn\n"                   /// 25, 71031
		"faddi.pi f4, f29, 30\n"                              /// 25, 71032
		"fsgnjn.ps f28, f27, f3\n"                            /// 25, 71033
		"fnmadd.ps f21, f15, f14, f8, rne\n"                  /// 25, 71079
		"fmsub.s f4, f28, f13, f10, rup\n"                    /// 25, 71080
		"fcvt.wu.s x6, f11, rne\n"                            /// 25, 71081
		"fadd.ps f26, f28, f3, rne\n"                         /// 25, 71082
		"fcmovm.ps f3, f27, f26\n"                            /// 25, 71083

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS_M1_T1:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
	};

}
