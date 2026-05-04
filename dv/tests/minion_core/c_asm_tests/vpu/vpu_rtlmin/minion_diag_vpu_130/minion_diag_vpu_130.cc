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
			 0x60fd8b49, /// 0x0
			 0x821c8dfd, /// 0x4
			 0x7ead9010, /// 0x8
			 0xd33da117, /// 0xc
			 0x26b2a8ad, /// 0x10
			 0x65e251ed, /// 0x14
			 0xeae2f643, /// 0x18
			 0xac0e1121, /// 0x1c
			 0xb6a86743, /// 0x20
			 0x65243745, /// 0x24
			 0x3d1b2f90, /// 0x28
			 0xae39984d, /// 0x2c
			 0x899a0930, /// 0x30
			 0x472a5fa2, /// 0x34
			 0x46679458, /// 0x38
			 0xc73bf16f, /// 0x3c
			 0x80082e8d, /// 0x40
			 0x8a94c12d, /// 0x44
			 0x890dffb8, /// 0x48
			 0x24782cb0, /// 0x4c
			 0xe5be00b2, /// 0x50
			 0xfeb94c7a, /// 0x54
			 0x0cef4ba3, /// 0x58
			 0x5475159d, /// 0x5c
			 0x2d685188, /// 0x60
			 0x5057c324, /// 0x64
			 0x2e1bb66d, /// 0x68
			 0x56d1663d, /// 0x6c
			 0x7eeea434, /// 0x70
			 0x916e0483, /// 0x74
			 0x38c4160d, /// 0x78
			 0xf13a9f68, /// 0x7c
			 0xbbff9b1e, /// 0x80
			 0x3f09b8f9, /// 0x84
			 0x208a49c1, /// 0x88
			 0x9a171263, /// 0x8c
			 0x48351fb7, /// 0x90
			 0x7817ca6e, /// 0x94
			 0x55449833, /// 0x98
			 0x591d0346, /// 0x9c
			 0x3c288cce, /// 0xa0
			 0x30ee11f6, /// 0xa4
			 0x9adef4f7, /// 0xa8
			 0x88ce71ea, /// 0xac
			 0xe07e7161, /// 0xb0
			 0xb8fb9d32, /// 0xb4
			 0xa9833a2a, /// 0xb8
			 0xca13eda0, /// 0xbc
			 0x9dc44d17, /// 0xc0
			 0x847ec668, /// 0xc4
			 0xf0cf593e, /// 0xc8
			 0x3f645e2b, /// 0xcc
			 0xe8130c1c, /// 0xd0
			 0xcb6d1798, /// 0xd4
			 0x04331ba7, /// 0xd8
			 0x8ed80061, /// 0xdc
			 0x3ae4f7b4, /// 0xe0
			 0x188fae3a, /// 0xe4
			 0xe8b3ad6d, /// 0xe8
			 0xddf8672b, /// 0xec
			 0x1cebd462, /// 0xf0
			 0x68e9da89, /// 0xf4
			 0x676640a6, /// 0xf8
			 0xda8fd0ab, /// 0xfc
			 0x02c91fb2, /// 0x100
			 0x5108ea70, /// 0x104
			 0xc2866a3a, /// 0x108
			 0xa7ffc9df, /// 0x10c
			 0xb152d498, /// 0x110
			 0x66d78cfa, /// 0x114
			 0xb2245d35, /// 0x118
			 0xa828e83a, /// 0x11c
			 0x4b7a6202, /// 0x120
			 0xaaf1e1e0, /// 0x124
			 0x06703f3c, /// 0x128
			 0x74149f98, /// 0x12c
			 0x3115bfdb, /// 0x130
			 0xcf296c77, /// 0x134
			 0x0dd2c3be, /// 0x138
			 0x47a88e13, /// 0x13c
			 0x420db9ac, /// 0x140
			 0xa8bb6171, /// 0x144
			 0x69590273, /// 0x148
			 0x8a95b91a, /// 0x14c
			 0x476d3955, /// 0x150
			 0x9ef39ff3, /// 0x154
			 0xc1778fd1, /// 0x158
			 0xa5e278ed, /// 0x15c
			 0x36f1f836, /// 0x160
			 0xc9183c50, /// 0x164
			 0xed61240d, /// 0x168
			 0xd71a54b4, /// 0x16c
			 0xfeff64a2, /// 0x170
			 0x11f2d40b, /// 0x174
			 0x7b253727, /// 0x178
			 0xb57ede38, /// 0x17c
			 0x01b701bc, /// 0x180
			 0xc24b3bf0, /// 0x184
			 0xa8a3cec2, /// 0x188
			 0x25942994, /// 0x18c
			 0xaca9a515, /// 0x190
			 0x4fc2c2b8, /// 0x194
			 0x2e3b6695, /// 0x198
			 0xc0681359, /// 0x19c
			 0x042fce06, /// 0x1a0
			 0xb8d7ddc0, /// 0x1a4
			 0x917c77c3, /// 0x1a8
			 0xc271ea86, /// 0x1ac
			 0x50ec1fde, /// 0x1b0
			 0x2fb5a344, /// 0x1b4
			 0x398c61e6, /// 0x1b8
			 0x0463a8ab, /// 0x1bc
			 0xc042642e, /// 0x1c0
			 0x038dcc64, /// 0x1c4
			 0x21a683ad, /// 0x1c8
			 0xa7a118e4, /// 0x1cc
			 0x51f61a31, /// 0x1d0
			 0x67eee79e, /// 0x1d4
			 0x8e5fa8f4, /// 0x1d8
			 0x2a33f520, /// 0x1dc
			 0x1e48f6ef, /// 0x1e0
			 0xcc180885, /// 0x1e4
			 0xe8ca79a8, /// 0x1e8
			 0x23f5e231, /// 0x1ec
			 0x1f6d9174, /// 0x1f0
			 0xb5963b14, /// 0x1f4
			 0x1cf3ebf5, /// 0x1f8
			 0x2f75b7d4, /// 0x1fc
			 0xf97dad04, /// 0x200
			 0xfd6a2ba2, /// 0x204
			 0x13e53a6f, /// 0x208
			 0x1dd97d4e, /// 0x20c
			 0xe47216d0, /// 0x210
			 0x53492299, /// 0x214
			 0xa56982bd, /// 0x218
			 0xa36165dd, /// 0x21c
			 0x087f0bab, /// 0x220
			 0x30046bd3, /// 0x224
			 0x3425161c, /// 0x228
			 0x36d223de, /// 0x22c
			 0x3640bc62, /// 0x230
			 0x4aec535b, /// 0x234
			 0x4a928f55, /// 0x238
			 0xb1b90c7a, /// 0x23c
			 0x855eceb2, /// 0x240
			 0x72fc0999, /// 0x244
			 0x83fddaba, /// 0x248
			 0x296ee345, /// 0x24c
			 0xf093c596, /// 0x250
			 0x15314636, /// 0x254
			 0x0905dc66, /// 0x258
			 0xf42a5e1c, /// 0x25c
			 0xa91dc54f, /// 0x260
			 0xf5f7899c, /// 0x264
			 0x76d8aab0, /// 0x268
			 0xe17cab07, /// 0x26c
			 0xad9644c2, /// 0x270
			 0xc61004f8, /// 0x274
			 0x6f2386c1, /// 0x278
			 0x05aedd68, /// 0x27c
			 0x9d6ac006, /// 0x280
			 0x68c427f1, /// 0x284
			 0xea61d031, /// 0x288
			 0x2660993c, /// 0x28c
			 0x08fa7877, /// 0x290
			 0xe8781d3c, /// 0x294
			 0xb53b381e, /// 0x298
			 0x6696529f, /// 0x29c
			 0x881408de, /// 0x2a0
			 0xda93cb11, /// 0x2a4
			 0xba87a27c, /// 0x2a8
			 0x28ad340e, /// 0x2ac
			 0x59a92cb0, /// 0x2b0
			 0x95903fe5, /// 0x2b4
			 0x00709ee3, /// 0x2b8
			 0x3e959cf5, /// 0x2bc
			 0x8030bd2b, /// 0x2c0
			 0xe2b05663, /// 0x2c4
			 0x02081025, /// 0x2c8
			 0xc668290c, /// 0x2cc
			 0xa9c17ae5, /// 0x2d0
			 0x8d8b42c1, /// 0x2d4
			 0xa7765b50, /// 0x2d8
			 0xfc546221, /// 0x2dc
			 0x8f755317, /// 0x2e0
			 0xa2ab0a09, /// 0x2e4
			 0x60f1bacb, /// 0x2e8
			 0xbf76fa5b, /// 0x2ec
			 0xc58f34c9, /// 0x2f0
			 0xf7d1df13, /// 0x2f4
			 0x6745b27e, /// 0x2f8
			 0x3efc7270, /// 0x2fc
			 0x9da9eec6, /// 0x300
			 0xe227c909, /// 0x304
			 0xc8d591f8, /// 0x308
			 0x12868cc0, /// 0x30c
			 0x7bdb2c0a, /// 0x310
			 0xfca2680d, /// 0x314
			 0x57b1c561, /// 0x318
			 0x370b7162, /// 0x31c
			 0x7a275318, /// 0x320
			 0x87ed07ed, /// 0x324
			 0x37286bae, /// 0x328
			 0x76693ff1, /// 0x32c
			 0x11bae468, /// 0x330
			 0x73f8852c, /// 0x334
			 0x0bdc73f3, /// 0x338
			 0x500193e1, /// 0x33c
			 0x0bc3927e, /// 0x340
			 0xd6a2c4cf, /// 0x344
			 0xc45dbe3c, /// 0x348
			 0x0690cc76, /// 0x34c
			 0xa9167f41, /// 0x350
			 0x410ed505, /// 0x354
			 0xb2dec249, /// 0x358
			 0xee7221e9, /// 0x35c
			 0x2f7e3c98, /// 0x360
			 0x259f8630, /// 0x364
			 0xf0fc8dfc, /// 0x368
			 0xecfee4c0, /// 0x36c
			 0x4c2de0a2, /// 0x370
			 0x6935c029, /// 0x374
			 0xb41bc455, /// 0x378
			 0xcb1d60dc, /// 0x37c
			 0xee64165c, /// 0x380
			 0xb8b40393, /// 0x384
			 0x104f4ec2, /// 0x388
			 0x161c0ffd, /// 0x38c
			 0x203a5112, /// 0x390
			 0xeda890d8, /// 0x394
			 0x1bc35788, /// 0x398
			 0xd15fa376, /// 0x39c
			 0xa4fe6e0d, /// 0x3a0
			 0x4e5a03eb, /// 0x3a4
			 0xc119e51c, /// 0x3a8
			 0x590ca307, /// 0x3ac
			 0x7dc083a3, /// 0x3b0
			 0x7a6aec4d, /// 0x3b4
			 0xc8760bdc, /// 0x3b8
			 0x6abfbad5, /// 0x3bc
			 0xddf73747, /// 0x3c0
			 0xf9a7a879, /// 0x3c4
			 0x8194add3, /// 0x3c8
			 0x5f39243e, /// 0x3cc
			 0xd80eee75, /// 0x3d0
			 0x208e4ea2, /// 0x3d4
			 0xec9f6914, /// 0x3d8
			 0xfdc213ff, /// 0x3dc
			 0xfe9f9670, /// 0x3e0
			 0x07245cbd, /// 0x3e4
			 0x106f8a4a, /// 0x3e8
			 0x1c9a2574, /// 0x3ec
			 0x00250e3b, /// 0x3f0
			 0x25c7cd28, /// 0x3f4
			 0x4247d500, /// 0x3f8
			 0x03e6f048, /// 0x3fc
			 0xa7dd93c5, /// 0x400
			 0x4778cf94, /// 0x404
			 0xa21e6c4b, /// 0x408
			 0x6686bdb4, /// 0x40c
			 0xd1d6213b, /// 0x410
			 0x8bbf469f, /// 0x414
			 0x9bc46db7, /// 0x418
			 0x55f9b3d3, /// 0x41c
			 0x98d25aa2, /// 0x420
			 0x7c6a3528, /// 0x424
			 0x1cf98085, /// 0x428
			 0xe2c31db0, /// 0x42c
			 0xe443d092, /// 0x430
			 0xd465ce18, /// 0x434
			 0x50fb63cd, /// 0x438
			 0x3e74577c, /// 0x43c
			 0xf47ca538, /// 0x440
			 0x82bdc60c, /// 0x444
			 0x5b360436, /// 0x448
			 0xc881e47a, /// 0x44c
			 0x469d7d3a, /// 0x450
			 0x1d4b3276, /// 0x454
			 0xe4f2b29d, /// 0x458
			 0x90c8c078, /// 0x45c
			 0x81e47e88, /// 0x460
			 0x7d35dc05, /// 0x464
			 0x1cc88740, /// 0x468
			 0x3ab60a96, /// 0x46c
			 0xdcbf16de, /// 0x470
			 0xdb74d299, /// 0x474
			 0x9246f5f8, /// 0x478
			 0xd027f2b5, /// 0x47c
			 0x6cd0ebe9, /// 0x480
			 0x5548e405, /// 0x484
			 0xad1c838e, /// 0x488
			 0x236c9ae0, /// 0x48c
			 0x4aa6025b, /// 0x490
			 0x257408e2, /// 0x494
			 0x3be98a2f, /// 0x498
			 0x2c55c55e, /// 0x49c
			 0x0a256e88, /// 0x4a0
			 0xefcd5b40, /// 0x4a4
			 0x313deee6, /// 0x4a8
			 0x3c0914fa, /// 0x4ac
			 0x8553a0fc, /// 0x4b0
			 0xf9019382, /// 0x4b4
			 0x87f46d9f, /// 0x4b8
			 0x8b04364d, /// 0x4bc
			 0x058ad296, /// 0x4c0
			 0x3f203077, /// 0x4c4
			 0xb99c17a3, /// 0x4c8
			 0x0768eb50, /// 0x4cc
			 0x1827ab74, /// 0x4d0
			 0x13b27427, /// 0x4d4
			 0xab56a037, /// 0x4d8
			 0xe39d49f5, /// 0x4dc
			 0xea262b6f, /// 0x4e0
			 0x580279a5, /// 0x4e4
			 0x4b270edc, /// 0x4e8
			 0x4f568582, /// 0x4ec
			 0x25818edc, /// 0x4f0
			 0x2724fcb2, /// 0x4f4
			 0x3f7f6ee8, /// 0x4f8
			 0xd1a94e23, /// 0x4fc
			 0x99fce4a1, /// 0x500
			 0x1a746ca0, /// 0x504
			 0xc2912dcd, /// 0x508
			 0x9b01f382, /// 0x50c
			 0xf7c5debb, /// 0x510
			 0xfcbfc58f, /// 0x514
			 0xad85266a, /// 0x518
			 0x2ec667d6, /// 0x51c
			 0x3496c170, /// 0x520
			 0x5b971c79, /// 0x524
			 0xea3227ac, /// 0x528
			 0xec204a70, /// 0x52c
			 0x457d725c, /// 0x530
			 0x7a6e60c7, /// 0x534
			 0x2c59ca8a, /// 0x538
			 0x7764da93, /// 0x53c
			 0x43d34d90, /// 0x540
			 0x6bd64cbb, /// 0x544
			 0x8a291f72, /// 0x548
			 0x23b58920, /// 0x54c
			 0xd04e34e5, /// 0x550
			 0x3ad0d00a, /// 0x554
			 0x16ad30aa, /// 0x558
			 0x28b5bab6, /// 0x55c
			 0x6a709cd2, /// 0x560
			 0x794c5066, /// 0x564
			 0xa4b74d62, /// 0x568
			 0x2598f005, /// 0x56c
			 0x4986a314, /// 0x570
			 0x19f7a7a1, /// 0x574
			 0x7bf38718, /// 0x578
			 0xd1f936b8, /// 0x57c
			 0x3f6d1cb6, /// 0x580
			 0x59be1dba, /// 0x584
			 0x98eda53c, /// 0x588
			 0x04f3bd03, /// 0x58c
			 0x9f4c26d4, /// 0x590
			 0xf3132908, /// 0x594
			 0x44d02583, /// 0x598
			 0xfed02a5f, /// 0x59c
			 0xd4c28464, /// 0x5a0
			 0x67495a0d, /// 0x5a4
			 0x45285b53, /// 0x5a8
			 0xa5e7c913, /// 0x5ac
			 0x8ddb964a, /// 0x5b0
			 0x8f40cdb7, /// 0x5b4
			 0x0161f774, /// 0x5b8
			 0x4bbfb3e3, /// 0x5bc
			 0x309ec581, /// 0x5c0
			 0x793ebc7e, /// 0x5c4
			 0x455bde51, /// 0x5c8
			 0x28099c5b, /// 0x5cc
			 0x67b133a3, /// 0x5d0
			 0xd37c8214, /// 0x5d4
			 0x3c76327c, /// 0x5d8
			 0x63d63cce, /// 0x5dc
			 0xfddd3120, /// 0x5e0
			 0x72c57086, /// 0x5e4
			 0xec6e1a32, /// 0x5e8
			 0xf2343cf3, /// 0x5ec
			 0xefd022cb, /// 0x5f0
			 0xa718819f, /// 0x5f4
			 0x835d2194, /// 0x5f8
			 0x0d6980bd, /// 0x5fc
			 0xa1cf0efb, /// 0x600
			 0x476b200e, /// 0x604
			 0x9205e88a, /// 0x608
			 0xc9089409, /// 0x60c
			 0x4f2a74d0, /// 0x610
			 0x1f00b953, /// 0x614
			 0x44badc37, /// 0x618
			 0xff8450f2, /// 0x61c
			 0xb430337b, /// 0x620
			 0x46576e27, /// 0x624
			 0x13e7983b, /// 0x628
			 0x62281a89, /// 0x62c
			 0xaa6584f1, /// 0x630
			 0x44a608ed, /// 0x634
			 0x6fb0c626, /// 0x638
			 0xd3699b7f, /// 0x63c
			 0xce8009af, /// 0x640
			 0x4476f9b4, /// 0x644
			 0xec359a18, /// 0x648
			 0x70101416, /// 0x64c
			 0xc748bd59, /// 0x650
			 0x2c129614, /// 0x654
			 0x86f0a4e2, /// 0x658
			 0xff7d9e1c, /// 0x65c
			 0x7afee0f9, /// 0x660
			 0xc85c07d1, /// 0x664
			 0x7bf6dc48, /// 0x668
			 0x2eff1029, /// 0x66c
			 0x2addeb99, /// 0x670
			 0x86bbb7c9, /// 0x674
			 0x05ef5538, /// 0x678
			 0x8033efcf, /// 0x67c
			 0x0bb954bc, /// 0x680
			 0xbfc5aa6d, /// 0x684
			 0xe164e618, /// 0x688
			 0x297ddba0, /// 0x68c
			 0xccf0d801, /// 0x690
			 0xdab3f09c, /// 0x694
			 0x205b82a7, /// 0x698
			 0x6e135781, /// 0x69c
			 0x8eb87bbe, /// 0x6a0
			 0xf0078bcf, /// 0x6a4
			 0x0929f76f, /// 0x6a8
			 0x587206e5, /// 0x6ac
			 0x0be27451, /// 0x6b0
			 0x155eab76, /// 0x6b4
			 0x65194466, /// 0x6b8
			 0xd06800ef, /// 0x6bc
			 0x359eb64e, /// 0x6c0
			 0x5ae4cc8f, /// 0x6c4
			 0x3a2d4e9a, /// 0x6c8
			 0x382169d9, /// 0x6cc
			 0x173478ef, /// 0x6d0
			 0x0d93af58, /// 0x6d4
			 0x2c41cf74, /// 0x6d8
			 0x3be119d9, /// 0x6dc
			 0xad2c21c9, /// 0x6e0
			 0x8f78faae, /// 0x6e4
			 0xcc8a1b81, /// 0x6e8
			 0x76363f1d, /// 0x6ec
			 0x2b48e1bd, /// 0x6f0
			 0xe0633aac, /// 0x6f4
			 0x99e88c5d, /// 0x6f8
			 0x9a61f103, /// 0x6fc
			 0x4af0408f, /// 0x700
			 0x83a9fa22, /// 0x704
			 0x6fedad12, /// 0x708
			 0xd3852bc5, /// 0x70c
			 0xa63bc4a6, /// 0x710
			 0x083e1635, /// 0x714
			 0x053858b2, /// 0x718
			 0x6526fc64, /// 0x71c
			 0xbedf9dc9, /// 0x720
			 0xc8ab3e17, /// 0x724
			 0x3ee6f846, /// 0x728
			 0xb1424867, /// 0x72c
			 0x235ab819, /// 0x730
			 0x08cde5b6, /// 0x734
			 0xa50e5836, /// 0x738
			 0xc22e93b1, /// 0x73c
			 0x8496e7b1, /// 0x740
			 0x1bba6c85, /// 0x744
			 0x0a824273, /// 0x748
			 0x49e39a5b, /// 0x74c
			 0x22c4be27, /// 0x750
			 0x035b54bb, /// 0x754
			 0x683461e4, /// 0x758
			 0xe12159ee, /// 0x75c
			 0xeff632b6, /// 0x760
			 0xae785a7e, /// 0x764
			 0x92b0dc89, /// 0x768
			 0xe6168529, /// 0x76c
			 0x327eeb26, /// 0x770
			 0x00b2e661, /// 0x774
			 0xf93aa871, /// 0x778
			 0x8c3398f1, /// 0x77c
			 0x9795c4f8, /// 0x780
			 0x6ea8c7ad, /// 0x784
			 0xe4715221, /// 0x788
			 0xe24ffed5, /// 0x78c
			 0x1a2630d0, /// 0x790
			 0x8d57b10e, /// 0x794
			 0x3b30342b, /// 0x798
			 0xc70ac25c, /// 0x79c
			 0xacae4d95, /// 0x7a0
			 0xb6435777, /// 0x7a4
			 0x1d477750, /// 0x7a8
			 0x41092539, /// 0x7ac
			 0xf7bd9740, /// 0x7b0
			 0x8b8ff880, /// 0x7b4
			 0x7d4e2fff, /// 0x7b8
			 0x3a2f629e, /// 0x7bc
			 0x32adc77c, /// 0x7c0
			 0x9cad51c7, /// 0x7c4
			 0x8fbad7e9, /// 0x7c8
			 0x7f3f0ab6, /// 0x7cc
			 0x5b93737e, /// 0x7d0
			 0x8739c333, /// 0x7d4
			 0x15d2df64, /// 0x7d8
			 0x9311c7d7, /// 0x7dc
			 0xa162527f, /// 0x7e0
			 0xae849525, /// 0x7e4
			 0xf2eb5446, /// 0x7e8
			 0x88f98128, /// 0x7ec
			 0x1047f03c, /// 0x7f0
			 0x27d21abb, /// 0x7f4
			 0x1fd9905c, /// 0x7f8
			 0xb5d858d9, /// 0x7fc
			 0x58774292, /// 0x800
			 0xdd6b9857, /// 0x804
			 0x48c76006, /// 0x808
			 0xa6c5022c, /// 0x80c
			 0x6a44c993, /// 0x810
			 0xd45db4f7, /// 0x814
			 0xe04f382c, /// 0x818
			 0xbb9ee0df, /// 0x81c
			 0x6e86e685, /// 0x820
			 0x20e73e15, /// 0x824
			 0x15f8b09d, /// 0x828
			 0xbe00f657, /// 0x82c
			 0x718a0489, /// 0x830
			 0xc940537e, /// 0x834
			 0x011822e2, /// 0x838
			 0x0d5578d4, /// 0x83c
			 0xd4a4398a, /// 0x840
			 0xefa2c3b7, /// 0x844
			 0x2076b130, /// 0x848
			 0x94de1aa4, /// 0x84c
			 0xcc6cc9fb, /// 0x850
			 0xb087cdcb, /// 0x854
			 0xfab2b438, /// 0x858
			 0xd1f522af, /// 0x85c
			 0x7e188c99, /// 0x860
			 0xefa77963, /// 0x864
			 0x75c71114, /// 0x868
			 0x32a52180, /// 0x86c
			 0xa266a288, /// 0x870
			 0x1d04e27f, /// 0x874
			 0xd1791be6, /// 0x878
			 0x2501809a, /// 0x87c
			 0x9752f040, /// 0x880
			 0x17094348, /// 0x884
			 0xdb93efe3, /// 0x888
			 0xcdf40bad, /// 0x88c
			 0xc82771b4, /// 0x890
			 0x7463c6f1, /// 0x894
			 0x92e4f059, /// 0x898
			 0xe8faae4f, /// 0x89c
			 0xbad59643, /// 0x8a0
			 0x591f328d, /// 0x8a4
			 0xefaf7b01, /// 0x8a8
			 0x691b1b4c, /// 0x8ac
			 0x1bf06214, /// 0x8b0
			 0x6a2f2e0c, /// 0x8b4
			 0x4c006c91, /// 0x8b8
			 0x7c5b9f49, /// 0x8bc
			 0x9dfdd0db, /// 0x8c0
			 0xda164090, /// 0x8c4
			 0xe39ffbaf, /// 0x8c8
			 0x9e71c597, /// 0x8cc
			 0x9020dd1b, /// 0x8d0
			 0x381ef4d3, /// 0x8d4
			 0x82d0f222, /// 0x8d8
			 0x93d4ed71, /// 0x8dc
			 0x454faae6, /// 0x8e0
			 0xec30bcb9, /// 0x8e4
			 0x9029ab45, /// 0x8e8
			 0x5800c8a3, /// 0x8ec
			 0xafe59e24, /// 0x8f0
			 0x956188af, /// 0x8f4
			 0xe4809a10, /// 0x8f8
			 0x969dfffb, /// 0x8fc
			 0x198c6257, /// 0x900
			 0xc92651e1, /// 0x904
			 0x117cab69, /// 0x908
			 0xa3b56f29, /// 0x90c
			 0x88a91143, /// 0x910
			 0x76e94e70, /// 0x914
			 0xeccd5713, /// 0x918
			 0x73b7897d, /// 0x91c
			 0x4a1e0e83, /// 0x920
			 0x0acae23e, /// 0x924
			 0xecd5e39a, /// 0x928
			 0x70795972, /// 0x92c
			 0xa5cca640, /// 0x930
			 0x2e4e12f1, /// 0x934
			 0x1380241a, /// 0x938
			 0x7b2f9efb, /// 0x93c
			 0xc69b05e9, /// 0x940
			 0xf543f4d3, /// 0x944
			 0x12560dd9, /// 0x948
			 0x3281c08a, /// 0x94c
			 0x1671c0c3, /// 0x950
			 0xea9660c8, /// 0x954
			 0x11f8112a, /// 0x958
			 0xe5d94757, /// 0x95c
			 0x82f5dc8d, /// 0x960
			 0xf1d98e13, /// 0x964
			 0x0e6287c2, /// 0x968
			 0x8ff637ec, /// 0x96c
			 0x12a3eca8, /// 0x970
			 0x1c37400f, /// 0x974
			 0x419a9aac, /// 0x978
			 0x9f095818, /// 0x97c
			 0x2ab36d4f, /// 0x980
			 0x4454b41b, /// 0x984
			 0xebbccb6d, /// 0x988
			 0x4549e667, /// 0x98c
			 0xd743f202, /// 0x990
			 0x928c8eeb, /// 0x994
			 0x125c376b, /// 0x998
			 0x07ff942c, /// 0x99c
			 0xaae09398, /// 0x9a0
			 0x10ebf48e, /// 0x9a4
			 0x5f1daf4a, /// 0x9a8
			 0x89c961ad, /// 0x9ac
			 0x2af98d25, /// 0x9b0
			 0xad66a214, /// 0x9b4
			 0x3e1c2fdd, /// 0x9b8
			 0x02538616, /// 0x9bc
			 0x3c03feb5, /// 0x9c0
			 0x160897dd, /// 0x9c4
			 0x972dc381, /// 0x9c8
			 0x7aa43315, /// 0x9cc
			 0x40810d5a, /// 0x9d0
			 0x4ec2e441, /// 0x9d4
			 0xa78c70fb, /// 0x9d8
			 0x4f692362, /// 0x9dc
			 0x79024792, /// 0x9e0
			 0xbe6c1acb, /// 0x9e4
			 0x6b1d6107, /// 0x9e8
			 0xdc09e32f, /// 0x9ec
			 0x7f8a48a6, /// 0x9f0
			 0xf4f209a7, /// 0x9f4
			 0xdbed0e02, /// 0x9f8
			 0x7ce55cbe, /// 0x9fc
			 0x633d6356, /// 0xa00
			 0xc663221a, /// 0xa04
			 0x184e4648, /// 0xa08
			 0x29a5b000, /// 0xa0c
			 0x27afa8d0, /// 0xa10
			 0x8e9c266d, /// 0xa14
			 0x97cb5d64, /// 0xa18
			 0x3e33cb8f, /// 0xa1c
			 0x4fc30e3d, /// 0xa20
			 0xb212eab8, /// 0xa24
			 0x6079cece, /// 0xa28
			 0xc1c2c439, /// 0xa2c
			 0x35ed6f07, /// 0xa30
			 0x46e4a96a, /// 0xa34
			 0xf5a08fe6, /// 0xa38
			 0x1b7c71d8, /// 0xa3c
			 0x68503a66, /// 0xa40
			 0x3cc0b426, /// 0xa44
			 0x76772f65, /// 0xa48
			 0xec8e72f7, /// 0xa4c
			 0xd62eb779, /// 0xa50
			 0x517c5785, /// 0xa54
			 0xe36ea0d2, /// 0xa58
			 0x1580dd78, /// 0xa5c
			 0x392b85e3, /// 0xa60
			 0x235d7a7a, /// 0xa64
			 0xb6f9befc, /// 0xa68
			 0x4b5dc587, /// 0xa6c
			 0xe56a2f6e, /// 0xa70
			 0x6947865e, /// 0xa74
			 0xafdc85e8, /// 0xa78
			 0x073f327d, /// 0xa7c
			 0x32e902f3, /// 0xa80
			 0x0415b27d, /// 0xa84
			 0x440c1334, /// 0xa88
			 0x0f76b303, /// 0xa8c
			 0xcf451d4d, /// 0xa90
			 0x3c14a189, /// 0xa94
			 0x447e7e22, /// 0xa98
			 0x99fd3435, /// 0xa9c
			 0xd5dd4b96, /// 0xaa0
			 0x62865b45, /// 0xaa4
			 0x18024134, /// 0xaa8
			 0x223de0ec, /// 0xaac
			 0xcbc1a9c6, /// 0xab0
			 0xc0f25c0e, /// 0xab4
			 0x40875fdf, /// 0xab8
			 0xd5783381, /// 0xabc
			 0xb43720b2, /// 0xac0
			 0xa1188412, /// 0xac4
			 0xe779c218, /// 0xac8
			 0x297a01e7, /// 0xacc
			 0x08410368, /// 0xad0
			 0x73a2bf26, /// 0xad4
			 0x4c0f0544, /// 0xad8
			 0x3cfd00d3, /// 0xadc
			 0xc7071d7f, /// 0xae0
			 0x7a488b16, /// 0xae4
			 0xe4a21baf, /// 0xae8
			 0x411297b4, /// 0xaec
			 0xe835b12a, /// 0xaf0
			 0xd5ece404, /// 0xaf4
			 0x5e323e18, /// 0xaf8
			 0x0d92fbb7, /// 0xafc
			 0xbdf295a5, /// 0xb00
			 0x47f4622c, /// 0xb04
			 0x183d736c, /// 0xb08
			 0x644e4a00, /// 0xb0c
			 0x121137ad, /// 0xb10
			 0xec56b311, /// 0xb14
			 0x3a86c260, /// 0xb18
			 0xe79f5089, /// 0xb1c
			 0x625b408c, /// 0xb20
			 0x96b7e8ae, /// 0xb24
			 0x175a5833, /// 0xb28
			 0xd48bac64, /// 0xb2c
			 0xdd455b1e, /// 0xb30
			 0x922ddc98, /// 0xb34
			 0x0cee6780, /// 0xb38
			 0x7ded1be7, /// 0xb3c
			 0xaefba755, /// 0xb40
			 0xaef36866, /// 0xb44
			 0xbb60bf05, /// 0xb48
			 0xa4080676, /// 0xb4c
			 0xd76cf65a, /// 0xb50
			 0x74e98952, /// 0xb54
			 0x92b3e351, /// 0xb58
			 0x8c51c644, /// 0xb5c
			 0x664429c7, /// 0xb60
			 0xd4df735f, /// 0xb64
			 0x0ca10f9e, /// 0xb68
			 0x1997120a, /// 0xb6c
			 0x6fb476e5, /// 0xb70
			 0x0dbaae5a, /// 0xb74
			 0x5ae88716, /// 0xb78
			 0xc0dc671f, /// 0xb7c
			 0x9f8cbe3b, /// 0xb80
			 0xa54a31ba, /// 0xb84
			 0x853b5604, /// 0xb88
			 0x3c4591ff, /// 0xb8c
			 0x34eb1417, /// 0xb90
			 0x02555efb, /// 0xb94
			 0xd3e74753, /// 0xb98
			 0x34d06bd8, /// 0xb9c
			 0x6b7d51c7, /// 0xba0
			 0xaf2d2a31, /// 0xba4
			 0x8ff14983, /// 0xba8
			 0x5906abd0, /// 0xbac
			 0x94a59f61, /// 0xbb0
			 0x2733d5a5, /// 0xbb4
			 0xd7500d59, /// 0xbb8
			 0xbfca4dbd, /// 0xbbc
			 0x866142c0, /// 0xbc0
			 0xc5940e5d, /// 0xbc4
			 0x7aa40b4c, /// 0xbc8
			 0xb7c8ec1e, /// 0xbcc
			 0x6074ecc3, /// 0xbd0
			 0xbaf4ded5, /// 0xbd4
			 0xba4f4add, /// 0xbd8
			 0xe7dd3024, /// 0xbdc
			 0xf31f7170, /// 0xbe0
			 0x04c2040f, /// 0xbe4
			 0xd46a33ec, /// 0xbe8
			 0xbd4c20ca, /// 0xbec
			 0xef711f07, /// 0xbf0
			 0xeef00bc3, /// 0xbf4
			 0xaafd8a80, /// 0xbf8
			 0xe2b304bc, /// 0xbfc
			 0x73dd2de8, /// 0xc00
			 0x1632a94d, /// 0xc04
			 0x7debd984, /// 0xc08
			 0x281d543b, /// 0xc0c
			 0x30fc2d08, /// 0xc10
			 0x0094c01e, /// 0xc14
			 0x222e0ac8, /// 0xc18
			 0xdb288353, /// 0xc1c
			 0x17ab3e33, /// 0xc20
			 0x7f500983, /// 0xc24
			 0x3ba0bd91, /// 0xc28
			 0xa426dd60, /// 0xc2c
			 0xa9a983c9, /// 0xc30
			 0xb571ef48, /// 0xc34
			 0x4d639e20, /// 0xc38
			 0x5f814d20, /// 0xc3c
			 0x1a6807e8, /// 0xc40
			 0x2673c775, /// 0xc44
			 0xf9aabc0e, /// 0xc48
			 0xc559266f, /// 0xc4c
			 0x2215c456, /// 0xc50
			 0x6dd28bce, /// 0xc54
			 0x79fdadde, /// 0xc58
			 0xdbd02f75, /// 0xc5c
			 0xab3a61db, /// 0xc60
			 0x475b578c, /// 0xc64
			 0x8d642daf, /// 0xc68
			 0xc72276a3, /// 0xc6c
			 0x507fb344, /// 0xc70
			 0xd8f3a5c3, /// 0xc74
			 0xf47c2a02, /// 0xc78
			 0x74647889, /// 0xc7c
			 0x5a58a943, /// 0xc80
			 0x15f8136b, /// 0xc84
			 0xf1067863, /// 0xc88
			 0xdf4c788a, /// 0xc8c
			 0x35544489, /// 0xc90
			 0x1f3437da, /// 0xc94
			 0x7aaa38f2, /// 0xc98
			 0x554a5291, /// 0xc9c
			 0x6c997774, /// 0xca0
			 0xde81ce82, /// 0xca4
			 0x0b02f3c7, /// 0xca8
			 0x35f9e1f1, /// 0xcac
			 0xecbe3e54, /// 0xcb0
			 0x94a0e948, /// 0xcb4
			 0xe1ed7a89, /// 0xcb8
			 0x8b9cd724, /// 0xcbc
			 0xce74a737, /// 0xcc0
			 0xe813d1d0, /// 0xcc4
			 0xef6d0a07, /// 0xcc8
			 0xf4e913e9, /// 0xccc
			 0xff1c2716, /// 0xcd0
			 0xe118aaab, /// 0xcd4
			 0xb076fc1a, /// 0xcd8
			 0xd803c3f7, /// 0xcdc
			 0xc93d4585, /// 0xce0
			 0x522955f1, /// 0xce4
			 0xe0145a32, /// 0xce8
			 0x59e93127, /// 0xcec
			 0x9457b063, /// 0xcf0
			 0xc89f9644, /// 0xcf4
			 0xc3f62c98, /// 0xcf8
			 0x31669a68, /// 0xcfc
			 0xf9290115, /// 0xd00
			 0xe44646ec, /// 0xd04
			 0x8d3442a0, /// 0xd08
			 0xc8dd29ff, /// 0xd0c
			 0x6c67165d, /// 0xd10
			 0x83ec26a3, /// 0xd14
			 0xdc47fe8f, /// 0xd18
			 0x0b6b3dc0, /// 0xd1c
			 0xe0680003, /// 0xd20
			 0x4ed2a390, /// 0xd24
			 0x4adccaea, /// 0xd28
			 0x8c2b6167, /// 0xd2c
			 0x1b61260f, /// 0xd30
			 0x5926bf29, /// 0xd34
			 0x148c16b2, /// 0xd38
			 0x5b92570e, /// 0xd3c
			 0x604da1a3, /// 0xd40
			 0xe046c717, /// 0xd44
			 0x428474ba, /// 0xd48
			 0x0b3efefc, /// 0xd4c
			 0x0efae2f6, /// 0xd50
			 0xe41697c9, /// 0xd54
			 0x69391560, /// 0xd58
			 0xaddf13a7, /// 0xd5c
			 0x887666d5, /// 0xd60
			 0xa19f96b0, /// 0xd64
			 0xc4230524, /// 0xd68
			 0x5074a13b, /// 0xd6c
			 0x68913e25, /// 0xd70
			 0xc66ff295, /// 0xd74
			 0x997fe1ee, /// 0xd78
			 0x953ccf86, /// 0xd7c
			 0xe280e51d, /// 0xd80
			 0xc6efc272, /// 0xd84
			 0x53c33921, /// 0xd88
			 0xa965e9ce, /// 0xd8c
			 0x693d8dd5, /// 0xd90
			 0xe5dce41f, /// 0xd94
			 0x9d2edc1e, /// 0xd98
			 0x304812d7, /// 0xd9c
			 0xa7fc0f7d, /// 0xda0
			 0x7c708dee, /// 0xda4
			 0x9ca40f84, /// 0xda8
			 0xc575d5ce, /// 0xdac
			 0xb17e36fe, /// 0xdb0
			 0x8cfd0d98, /// 0xdb4
			 0xc98fc94c, /// 0xdb8
			 0x328a4e46, /// 0xdbc
			 0x7f25b209, /// 0xdc0
			 0x919ae368, /// 0xdc4
			 0xfe934818, /// 0xdc8
			 0x57c496de, /// 0xdcc
			 0xf642285b, /// 0xdd0
			 0x4790c097, /// 0xdd4
			 0x357a6701, /// 0xdd8
			 0x995f1d42, /// 0xddc
			 0xe76920cb, /// 0xde0
			 0x3897d4cc, /// 0xde4
			 0x6b302c17, /// 0xde8
			 0xd42a8423, /// 0xdec
			 0x246dfae1, /// 0xdf0
			 0xebd920c0, /// 0xdf4
			 0x2ae3e472, /// 0xdf8
			 0xc2ac374d, /// 0xdfc
			 0xf27b1ec1, /// 0xe00
			 0xbf3b934a, /// 0xe04
			 0xbf2ee67b, /// 0xe08
			 0x656b840e, /// 0xe0c
			 0x72f377d9, /// 0xe10
			 0x29e2c92a, /// 0xe14
			 0x9548e834, /// 0xe18
			 0x3ba530d7, /// 0xe1c
			 0xdfb20ef3, /// 0xe20
			 0x29fd59c6, /// 0xe24
			 0x066b1e0f, /// 0xe28
			 0x96a0cc44, /// 0xe2c
			 0x30a40ab8, /// 0xe30
			 0xfa8abc54, /// 0xe34
			 0x23b38b31, /// 0xe38
			 0xd263a139, /// 0xe3c
			 0x41ecc2f4, /// 0xe40
			 0xc90f3d29, /// 0xe44
			 0xbe3dce33, /// 0xe48
			 0xf85a8c2e, /// 0xe4c
			 0xe6dec5c1, /// 0xe50
			 0xf15a0488, /// 0xe54
			 0x72533f9d, /// 0xe58
			 0xab322e0d, /// 0xe5c
			 0x0df2d333, /// 0xe60
			 0xb42ee51a, /// 0xe64
			 0x5ffe0f53, /// 0xe68
			 0xd81e4c4f, /// 0xe6c
			 0x6bbca00c, /// 0xe70
			 0x104ca04e, /// 0xe74
			 0x1c08485a, /// 0xe78
			 0x3b77d870, /// 0xe7c
			 0xfa8a5f28, /// 0xe80
			 0x5c4b2b42, /// 0xe84
			 0x0fece3e0, /// 0xe88
			 0x32c74fd1, /// 0xe8c
			 0x34981dc3, /// 0xe90
			 0x668aecd1, /// 0xe94
			 0x250b13b8, /// 0xe98
			 0x5e20bd96, /// 0xe9c
			 0x2e11fc0d, /// 0xea0
			 0x4159ce33, /// 0xea4
			 0xfbd199cb, /// 0xea8
			 0xff7417c4, /// 0xeac
			 0x28b461c8, /// 0xeb0
			 0x6fbf3272, /// 0xeb4
			 0x79000269, /// 0xeb8
			 0x4eff486d, /// 0xebc
			 0x72f64af3, /// 0xec0
			 0xf6eab9e5, /// 0xec4
			 0x64691157, /// 0xec8
			 0x28927751, /// 0xecc
			 0x85ec0e02, /// 0xed0
			 0x04cab9e4, /// 0xed4
			 0x0d3bc825, /// 0xed8
			 0x62e1baaf, /// 0xedc
			 0x9c0a8456, /// 0xee0
			 0x9ccafd89, /// 0xee4
			 0x3aab1bc1, /// 0xee8
			 0xd77ee43a, /// 0xeec
			 0xc4500b59, /// 0xef0
			 0x3558aa66, /// 0xef4
			 0xa8f51aea, /// 0xef8
			 0xc3e3e16d, /// 0xefc
			 0xef6febdc, /// 0xf00
			 0x87d2fbec, /// 0xf04
			 0x8fedddc1, /// 0xf08
			 0x99f1f49b, /// 0xf0c
			 0x170a111e, /// 0xf10
			 0xd14a9b2d, /// 0xf14
			 0x37d21be4, /// 0xf18
			 0xc57885e9, /// 0xf1c
			 0x76c08e6a, /// 0xf20
			 0xd6b72388, /// 0xf24
			 0x20923023, /// 0xf28
			 0xd1ea8528, /// 0xf2c
			 0x9c054b10, /// 0xf30
			 0xfe03e58e, /// 0xf34
			 0x0f881d66, /// 0xf38
			 0x19ae2dba, /// 0xf3c
			 0xf0ac7f14, /// 0xf40
			 0x1c819670, /// 0xf44
			 0x93bd0a79, /// 0xf48
			 0x7b6a8bcb, /// 0xf4c
			 0xa14d2cee, /// 0xf50
			 0x93dfeffa, /// 0xf54
			 0x7732c1dd, /// 0xf58
			 0x5426cc92, /// 0xf5c
			 0x07e354ca, /// 0xf60
			 0x894b725a, /// 0xf64
			 0x84d59d2e, /// 0xf68
			 0x141578f3, /// 0xf6c
			 0xd0538315, /// 0xf70
			 0x845ffa94, /// 0xf74
			 0x89aee559, /// 0xf78
			 0x7fb60263, /// 0xf7c
			 0x972bedad, /// 0xf80
			 0x26fd1974, /// 0xf84
			 0x19084f64, /// 0xf88
			 0x0349562d, /// 0xf8c
			 0xb7c8ac74, /// 0xf90
			 0x5bad60bd, /// 0xf94
			 0xd5226d6f, /// 0xf98
			 0xa7e39466, /// 0xf9c
			 0x4a3be4f8, /// 0xfa0
			 0x35ea1604, /// 0xfa4
			 0xdbe3f798, /// 0xfa8
			 0xfd90b2ac, /// 0xfac
			 0xf6d85cd3, /// 0xfb0
			 0x1a4a2ebe, /// 0xfb4
			 0x797f701d, /// 0xfb8
			 0xe476fc6a, /// 0xfbc
			 0xf2e92e3c, /// 0xfc0
			 0x5e13df8f, /// 0xfc4
			 0x84c0f7a3, /// 0xfc8
			 0xe7cd21e8, /// 0xfcc
			 0x4022c428, /// 0xfd0
			 0x6afa585d, /// 0xfd4
			 0x5e266402, /// 0xfd8
			 0x3daaadb6, /// 0xfdc
			 0x6bf9a364, /// 0xfe0
			 0x591e6f26, /// 0xfe4
			 0xb61e8363, /// 0xfe8
			 0x4507b934, /// 0xfec
			 0x6a49d695, /// 0xff0
			 0x4d427ec5, /// 0xff4
			 0xd4297f6e, /// 0xff8
			 0xba49145f /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff800000,                                                  // -inf                                        /// 00000
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00004
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00008
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0000c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00018
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00020
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00024
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0002c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00030
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00034
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00038
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0004c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80011111,                                                  // -9.7958E-41                                 /// 00054
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00058
			 0x33333333,                                                  // 4 random values                             /// 0005c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00060
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00064
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0006c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00074
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00078
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0007c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00080
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00088
			 0xffc00001,                                                  // -signaling NaN                              /// 0008c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00090
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00094
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00098
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0009c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00104
			 0x0c780000,                                                  // Leading 1s:                                 /// 00108
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00110
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00114
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00120
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00124
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00128
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0012c
			 0xffc00001,                                                  // -signaling NaN                              /// 00130
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00138
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0013c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00144
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0014c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00150
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00164
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0016c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00178
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0017c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00180
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0c400000,                                                  // Leading 1s:                                 /// 00188
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0018c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00190
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00198
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00204
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00208
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00210
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00214
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00218
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00224
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00230
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00234
			 0x7f800000,                                                  // inf                                         /// 00238
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00240
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00244
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00248
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0024c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0e000001,                                                  // Trailing 1s:                                /// 00258
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0025c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00264
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0026c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00270
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00274
			 0x3f028f5c,                                                  // 0.51                                        /// 00278
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0027c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00284
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00288
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0028c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00290
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00298
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00300
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00308
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00318
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0031c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00320
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00324
			 0x7f800000,                                                  // inf                                         /// 00328
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0032c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00330
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00334
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00340
			 0x0c780000,                                                  // Leading 1s:                                 /// 00344
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00348
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0034c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00354
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0036c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00374
			 0xcb000000,                                                  // -8388608.0                                  /// 00378
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00384
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00388
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0038c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0e000001,                                                  // Trailing 1s:                                /// 00394
			 0x7f800000,                                                  // inf                                         /// 00398
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0039c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003a4
			 0xbf028f5c,                                                  // -0.51                                       /// 003a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d4
			 0xff800000,                                                  // -inf                                        /// 003d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00400
			 0x0e000007,                                                  // Trailing 1s:                                /// 00404
			 0x80000000,                                                  // -zero                                       /// 00408
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0040c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00410
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00414
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00428
			 0x0c400000,                                                  // Leading 1s:                                 /// 0042c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00430
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00434
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0043c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0044c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00450
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x4b000000,                                                  // 8388608.0                                   /// 00458
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0046c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00474
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00000000,                                                  // zero                                        /// 0047c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00488
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00490
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00494
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0049c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0xffc00001,                                                  // -signaling NaN                              /// 004d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e0
			 0x80000000,                                                  // -zero                                       /// 004e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00500
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00508
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x33333333,                                                  // 4 random values                             /// 00514
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00520
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00528
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00530
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00534
			 0xff800000,                                                  // -inf                                        /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00540
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0054c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00550
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00554
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00558
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0055c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00564
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00568
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00570
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0057c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00584
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00588
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0058c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00590
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00594
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005cc
			 0xff800000,                                                  // -inf                                        /// 005d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00600
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00604
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00608
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00610
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00614
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00618
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0061c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00620
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00624
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00628
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x7f800000,                                                  // inf                                         /// 00640
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00644
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00648
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0064c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00650
			 0x0c700000,                                                  // Leading 1s:                                 /// 00654
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00658
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00660
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00670
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00674
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00680
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00684
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00688
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0068c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00694
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00698
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0069c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xff800000,                                                  // -inf                                        /// 006a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 006d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00700
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00704
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00708
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00714
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00720
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00724
			 0x0e000001,                                                  // Trailing 1s:                                /// 00728
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00730
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00734
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00738
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00740
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00744
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00748
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0074c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00750
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00754
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0xcb000000,                                                  // -8388608.0                                  /// 0075c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00760
			 0x00000000,                                                  // zero                                        /// 00764
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0076c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00778
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0077c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00784
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0078c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00800
			 0x0e000003,                                                  // Trailing 1s:                                /// 00804
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00808
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0080c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00810
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00814
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00818
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0081c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00820
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00824
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00828
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00834
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00838
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0083c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00840
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00844
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00850
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00858
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00860
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00870
			 0x3f028f5c,                                                  // 0.51                                        /// 00874
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00878
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0087c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00884
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0088c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00890
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008cc
			 0xcb000000,                                                  // -8388608.0                                  /// 008d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00904
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00908
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x7f800000,                                                  // inf                                         /// 00914
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00918
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00924
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00928
			 0x0c700000,                                                  // Leading 1s:                                 /// 0092c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00938
			 0xff800000,                                                  // -inf                                        /// 0093c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00944
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00948
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0094c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00950
			 0x0e000003,                                                  // Trailing 1s:                                /// 00954
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0095c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00964
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00968
			 0x0c400000,                                                  // Leading 1s:                                 /// 0096c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00970
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00011111,                                                  // 9.7958E-41                                  /// 00978
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c780000,                                                  // Leading 1s:                                 /// 00994
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00998
			 0x80000000,                                                  // -zero                                       /// 0099c
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a24
			 0x7f800000,                                                  // inf                                         /// 00a28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xcb000000,                                                  // -8388608.0                                  /// 00a38
			 0x00000000,                                                  // zero                                        /// 00a3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a44
			 0x7fc00001,                                                  // signaling NaN                               /// 00a48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a7c
			 0xff800000,                                                  // -inf                                        /// 00a80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ab4
			 0x7f800000,                                                  // inf                                         /// 00ab8
			 0x33333333,                                                  // 4 random values                             /// 00abc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00acc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ad0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ae0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ae8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b04
			 0x33333333,                                                  // 4 random values                             /// 00b08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x7fc00001,                                                  // signaling NaN                               /// 00b34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ba4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ba8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00be4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00be8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf0
			 0x33333333,                                                  // 4 random values                             /// 00bf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xff800000,                                                  // -inf                                        /// 00c00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c18
			 0x55555555,                                                  // 4 random values                             /// 00c1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x33333333,                                                  // 4 random values                             /// 00c2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ca4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ca8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0xff800000,                                                  // -inf                                        /// 00cb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ccc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd8
			 0x80000000,                                                  // -zero                                       /// 00cdc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cf4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d28
			 0x7fc00001,                                                  // signaling NaN                               /// 00d2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d54
			 0x7f800000,                                                  // inf                                         /// 00d58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x7fc00001,                                                  // signaling NaN                               /// 00d64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00da4
			 0x80000000,                                                  // -zero                                       /// 00da8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db8
			 0xff800000,                                                  // -inf                                        /// 00dbc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dcc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x55555555,                                                  // 4 random values                             /// 00ddc
			 0x7f800000,                                                  // inf                                         /// 00de0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00df4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dfc
			 0x33333333,                                                  // 4 random values                             /// 00e00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e04
			 0x7fc00001,                                                  // signaling NaN                               /// 00e08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x7f800000,                                                  // inf                                         /// 00e34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0xff800000,                                                  // -inf                                        /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ea0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ebc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ec0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ecc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f10
			 0xbf028f5c,                                                  // -0.51                                       /// 00f14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f1c
			 0x00000000,                                                  // zero                                        /// 00f20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f34
			 0xff800000,                                                  // -inf                                        /// 00f38
			 0xff800000,                                                  // -inf                                        /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f78
			 0xffc00001,                                                  // -signaling NaN                              /// 00f7c
			 0x00000000,                                                  // zero                                        /// 00f80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f94
			 0x4b000000,                                                  // 8388608.0                                   /// 00f98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x33333333,                                                  // 4 random values                             /// 00fcc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff8
			 0x7f7ffffe // max norm - 1ulp                               // max norm +ve                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x3799482e, /// 0x0
			 0x1e99fd25, /// 0x4
			 0x8718fb08, /// 0x8
			 0x4b6106d4, /// 0xc
			 0x3a1371e6, /// 0x10
			 0x0541fb5d, /// 0x14
			 0x21170960, /// 0x18
			 0x072063e5, /// 0x1c
			 0x1db42de6, /// 0x20
			 0xde628914, /// 0x24
			 0xb0d24043, /// 0x28
			 0xd138bbbb, /// 0x2c
			 0x1bdc8cb2, /// 0x30
			 0xe08e3383, /// 0x34
			 0x5981f03e, /// 0x38
			 0xbf318fce, /// 0x3c
			 0x4b5ffaf9, /// 0x40
			 0xaac21892, /// 0x44
			 0xe0e40b37, /// 0x48
			 0xd82c8bce, /// 0x4c
			 0xdaa33390, /// 0x50
			 0xb242c097, /// 0x54
			 0xeb6c8366, /// 0x58
			 0x5bf4b062, /// 0x5c
			 0x14409161, /// 0x60
			 0xcb8da394, /// 0x64
			 0x1e5ebae1, /// 0x68
			 0xf3b27209, /// 0x6c
			 0xb90b4018, /// 0x70
			 0xa8f4e949, /// 0x74
			 0x95b7d3df, /// 0x78
			 0x626b72b7, /// 0x7c
			 0xa869e8e2, /// 0x80
			 0x60f749ee, /// 0x84
			 0x99631e9b, /// 0x88
			 0xb41041aa, /// 0x8c
			 0x20ee4d35, /// 0x90
			 0x8c59b1b2, /// 0x94
			 0x8028e7e2, /// 0x98
			 0x7cd05d3b, /// 0x9c
			 0xf7872427, /// 0xa0
			 0x70f65db0, /// 0xa4
			 0xc86b922c, /// 0xa8
			 0x6b345906, /// 0xac
			 0x98f43e9a, /// 0xb0
			 0x603f53b7, /// 0xb4
			 0xbeac027e, /// 0xb8
			 0xbfdbfe76, /// 0xbc
			 0x762722b7, /// 0xc0
			 0xa1cb052d, /// 0xc4
			 0xa2425d23, /// 0xc8
			 0x338f9109, /// 0xcc
			 0x5b5c6068, /// 0xd0
			 0xd2746646, /// 0xd4
			 0xaa95700f, /// 0xd8
			 0x76fc512d, /// 0xdc
			 0x01ec7bc5, /// 0xe0
			 0x356668c9, /// 0xe4
			 0x2b5ddd6d, /// 0xe8
			 0x9b668350, /// 0xec
			 0x6d855d80, /// 0xf0
			 0x3031e57b, /// 0xf4
			 0x5bcddc1d, /// 0xf8
			 0x03c0fe33, /// 0xfc
			 0x591ba89b, /// 0x100
			 0xa39d9e89, /// 0x104
			 0xa841fb14, /// 0x108
			 0x0470e255, /// 0x10c
			 0xadea1803, /// 0x110
			 0xd248cf2a, /// 0x114
			 0xe0f0a81a, /// 0x118
			 0x23a56a47, /// 0x11c
			 0x125e75d5, /// 0x120
			 0xeccdd875, /// 0x124
			 0xc364a0b4, /// 0x128
			 0xa50dae93, /// 0x12c
			 0x7a4c352d, /// 0x130
			 0x2ed2c98c, /// 0x134
			 0x7c32b22c, /// 0x138
			 0xa033213c, /// 0x13c
			 0xacded06e, /// 0x140
			 0x7eacf3d2, /// 0x144
			 0xed5508b3, /// 0x148
			 0x6f0d5e7a, /// 0x14c
			 0x4bdae029, /// 0x150
			 0x453fa4de, /// 0x154
			 0x9f390be3, /// 0x158
			 0xcc62b88d, /// 0x15c
			 0x463966bd, /// 0x160
			 0xb1c9a820, /// 0x164
			 0x2dbcdb94, /// 0x168
			 0xfa4a4b95, /// 0x16c
			 0x3b03a854, /// 0x170
			 0xf253db9c, /// 0x174
			 0x66f2673c, /// 0x178
			 0xbbb58424, /// 0x17c
			 0xe2778f53, /// 0x180
			 0xa32d1b1c, /// 0x184
			 0x92677a8e, /// 0x188
			 0x18e066d4, /// 0x18c
			 0x74c25ed4, /// 0x190
			 0xefe421ea, /// 0x194
			 0xf853a93c, /// 0x198
			 0xc30abe83, /// 0x19c
			 0xc86237f8, /// 0x1a0
			 0xd3db1682, /// 0x1a4
			 0x2dec5f95, /// 0x1a8
			 0x83d69a5d, /// 0x1ac
			 0xf021b010, /// 0x1b0
			 0x5e598b24, /// 0x1b4
			 0x9245c34c, /// 0x1b8
			 0xfe516113, /// 0x1bc
			 0xeddeb3c3, /// 0x1c0
			 0x26a0cb81, /// 0x1c4
			 0xbf19dca4, /// 0x1c8
			 0xa98afc48, /// 0x1cc
			 0xba3fc8be, /// 0x1d0
			 0x6d7e2213, /// 0x1d4
			 0xc4ce05e6, /// 0x1d8
			 0x9424d9da, /// 0x1dc
			 0x3fd2ea32, /// 0x1e0
			 0xab1a6d4b, /// 0x1e4
			 0x7f282ef3, /// 0x1e8
			 0xdacc2703, /// 0x1ec
			 0xf4641394, /// 0x1f0
			 0x7d97d9c4, /// 0x1f4
			 0x1081fa3a, /// 0x1f8
			 0x5e661c4b, /// 0x1fc
			 0x823d2413, /// 0x200
			 0x0d5b2407, /// 0x204
			 0x6007893e, /// 0x208
			 0x2940cac1, /// 0x20c
			 0x6c849425, /// 0x210
			 0x7598d208, /// 0x214
			 0x84e62509, /// 0x218
			 0x6d4d6219, /// 0x21c
			 0x3fd36002, /// 0x220
			 0xc35732c4, /// 0x224
			 0xe72f3927, /// 0x228
			 0xac786b30, /// 0x22c
			 0x938cc6a0, /// 0x230
			 0xe1a225ec, /// 0x234
			 0xd126f234, /// 0x238
			 0xb9056752, /// 0x23c
			 0xa8c7982b, /// 0x240
			 0x1741852b, /// 0x244
			 0xb657bfed, /// 0x248
			 0xbe3a1f4a, /// 0x24c
			 0xef93905d, /// 0x250
			 0xfc2c7baf, /// 0x254
			 0xbeec558f, /// 0x258
			 0xc65a5a14, /// 0x25c
			 0x38757f52, /// 0x260
			 0x20fde8dd, /// 0x264
			 0xfc3b5ef7, /// 0x268
			 0x43789be0, /// 0x26c
			 0x34bf808e, /// 0x270
			 0x82a75e35, /// 0x274
			 0x9ac5c4bf, /// 0x278
			 0x24815ca5, /// 0x27c
			 0x1568dd57, /// 0x280
			 0x4194b247, /// 0x284
			 0xe39706ad, /// 0x288
			 0xa7cdfb3e, /// 0x28c
			 0x1e6be90e, /// 0x290
			 0x9f731722, /// 0x294
			 0x3ae8d3a3, /// 0x298
			 0xb33da2b9, /// 0x29c
			 0xd2684d26, /// 0x2a0
			 0x447b9fea, /// 0x2a4
			 0x12e65a34, /// 0x2a8
			 0x13490c73, /// 0x2ac
			 0x09f0c097, /// 0x2b0
			 0x3d878ca0, /// 0x2b4
			 0x8123e6c2, /// 0x2b8
			 0xd6ab2499, /// 0x2bc
			 0xd90cce29, /// 0x2c0
			 0xf8eae2d3, /// 0x2c4
			 0x170db893, /// 0x2c8
			 0xa9341d02, /// 0x2cc
			 0x0fd224ca, /// 0x2d0
			 0xd3294546, /// 0x2d4
			 0x052ecd1d, /// 0x2d8
			 0x2d41809a, /// 0x2dc
			 0x7dad9b3a, /// 0x2e0
			 0xc59b335c, /// 0x2e4
			 0x997d7997, /// 0x2e8
			 0x05bfadc3, /// 0x2ec
			 0xcd3f47c7, /// 0x2f0
			 0x39e1000d, /// 0x2f4
			 0x39c36605, /// 0x2f8
			 0x7c73cb65, /// 0x2fc
			 0xdfd8d670, /// 0x300
			 0x2aa43035, /// 0x304
			 0x1fa92527, /// 0x308
			 0x29380f36, /// 0x30c
			 0x32d253df, /// 0x310
			 0x1916af35, /// 0x314
			 0x5f3e4863, /// 0x318
			 0x16e43141, /// 0x31c
			 0x672d23f5, /// 0x320
			 0x49e5de35, /// 0x324
			 0x89f9f143, /// 0x328
			 0x974ab377, /// 0x32c
			 0x2fec1ff0, /// 0x330
			 0xd86fc868, /// 0x334
			 0x17ca6379, /// 0x338
			 0xf7bc5025, /// 0x33c
			 0x21943b6c, /// 0x340
			 0x642c82cd, /// 0x344
			 0x02e7ef87, /// 0x348
			 0x26745e0a, /// 0x34c
			 0x4fbad8f1, /// 0x350
			 0x730079a3, /// 0x354
			 0x12a8238f, /// 0x358
			 0x330de2a8, /// 0x35c
			 0xdcea78b9, /// 0x360
			 0xe0df40d8, /// 0x364
			 0xcb23a6fb, /// 0x368
			 0xbf56a5eb, /// 0x36c
			 0x96212a67, /// 0x370
			 0x9c0b3434, /// 0x374
			 0x6586d8eb, /// 0x378
			 0x1aab6de7, /// 0x37c
			 0x4b30ac80, /// 0x380
			 0x5416c279, /// 0x384
			 0xdef579e4, /// 0x388
			 0xbb2c5f7d, /// 0x38c
			 0xd511adaf, /// 0x390
			 0xef823c57, /// 0x394
			 0x94088a5c, /// 0x398
			 0xc220ccb2, /// 0x39c
			 0xcfa3c6cf, /// 0x3a0
			 0xc9019e44, /// 0x3a4
			 0xbadccec9, /// 0x3a8
			 0x599754ea, /// 0x3ac
			 0xcadde91a, /// 0x3b0
			 0xa8e0ade0, /// 0x3b4
			 0xf766789d, /// 0x3b8
			 0xe95be13a, /// 0x3bc
			 0x3a829230, /// 0x3c0
			 0x25c34502, /// 0x3c4
			 0xcd2bec51, /// 0x3c8
			 0x88268338, /// 0x3cc
			 0xb96a9c5f, /// 0x3d0
			 0x43aa6e00, /// 0x3d4
			 0x566f07ce, /// 0x3d8
			 0xee32a974, /// 0x3dc
			 0xe405d191, /// 0x3e0
			 0x8d71dcbf, /// 0x3e4
			 0x25f02ae6, /// 0x3e8
			 0x307850bb, /// 0x3ec
			 0xb13b01e7, /// 0x3f0
			 0x6b96b09b, /// 0x3f4
			 0x7d766d72, /// 0x3f8
			 0x57596637, /// 0x3fc
			 0xdf0b1412, /// 0x400
			 0xbc889b73, /// 0x404
			 0x8fcbd763, /// 0x408
			 0xc71998af, /// 0x40c
			 0xa1cae4f3, /// 0x410
			 0x7f58803b, /// 0x414
			 0xe91cfbc6, /// 0x418
			 0x8c7fb3a6, /// 0x41c
			 0x07cdd607, /// 0x420
			 0x9c073361, /// 0x424
			 0xd4920b8a, /// 0x428
			 0x06bcafac, /// 0x42c
			 0x59fae6fa, /// 0x430
			 0x145c1cfd, /// 0x434
			 0xf3bf2026, /// 0x438
			 0x2716454c, /// 0x43c
			 0x342e388c, /// 0x440
			 0x08f577df, /// 0x444
			 0xdb80987e, /// 0x448
			 0x8b08fa3f, /// 0x44c
			 0x6969276a, /// 0x450
			 0xf6edf607, /// 0x454
			 0xac13fdee, /// 0x458
			 0x4dff7607, /// 0x45c
			 0x5ff91000, /// 0x460
			 0x7313b93b, /// 0x464
			 0xd5320fa6, /// 0x468
			 0x5f3b0215, /// 0x46c
			 0x2156102b, /// 0x470
			 0x33094b13, /// 0x474
			 0x84f8c369, /// 0x478
			 0xff52a122, /// 0x47c
			 0x62b3b37c, /// 0x480
			 0x963fe35f, /// 0x484
			 0x5d3277d9, /// 0x488
			 0xe78f88e2, /// 0x48c
			 0xee6c5563, /// 0x490
			 0x8d927a44, /// 0x494
			 0x96731278, /// 0x498
			 0x6a5a6c3b, /// 0x49c
			 0x1df36d69, /// 0x4a0
			 0xd9eea086, /// 0x4a4
			 0xdd152251, /// 0x4a8
			 0x783e093a, /// 0x4ac
			 0x5f9295ab, /// 0x4b0
			 0xedaa031c, /// 0x4b4
			 0x4a867c78, /// 0x4b8
			 0x54eeaba2, /// 0x4bc
			 0x6df8bf24, /// 0x4c0
			 0xa370f9dc, /// 0x4c4
			 0x440c24e1, /// 0x4c8
			 0xcae9d331, /// 0x4cc
			 0xcefe4862, /// 0x4d0
			 0xc07a5d06, /// 0x4d4
			 0xadcd2177, /// 0x4d8
			 0x694323c5, /// 0x4dc
			 0x3e4c1536, /// 0x4e0
			 0x0560ac69, /// 0x4e4
			 0x15827f79, /// 0x4e8
			 0xe3a5fee5, /// 0x4ec
			 0x477b8122, /// 0x4f0
			 0x585ac678, /// 0x4f4
			 0x61ef6710, /// 0x4f8
			 0x3467e280, /// 0x4fc
			 0xa4c2f411, /// 0x500
			 0x155bd0dd, /// 0x504
			 0xb211e19f, /// 0x508
			 0x6126a516, /// 0x50c
			 0x5910326f, /// 0x510
			 0x6cac6368, /// 0x514
			 0x9bcb7441, /// 0x518
			 0x32794eb0, /// 0x51c
			 0x9154a8ee, /// 0x520
			 0xaee4d4c6, /// 0x524
			 0x441da572, /// 0x528
			 0xc52af976, /// 0x52c
			 0x9852b6d0, /// 0x530
			 0xbc004916, /// 0x534
			 0x246f6f8f, /// 0x538
			 0xbc67a984, /// 0x53c
			 0xa6f800eb, /// 0x540
			 0x4a1898ea, /// 0x544
			 0xe01faec3, /// 0x548
			 0x6fea591c, /// 0x54c
			 0x25f2c828, /// 0x550
			 0x31fe30d1, /// 0x554
			 0x68ab48ea, /// 0x558
			 0x9f007fb1, /// 0x55c
			 0x85172d20, /// 0x560
			 0x0e52e11e, /// 0x564
			 0x5e64af2d, /// 0x568
			 0x869fb3f4, /// 0x56c
			 0x1f77d333, /// 0x570
			 0xb9fb1b45, /// 0x574
			 0xc143a2a8, /// 0x578
			 0xaf95e7f1, /// 0x57c
			 0x9858feda, /// 0x580
			 0x70eae858, /// 0x584
			 0x226a888d, /// 0x588
			 0x45ee8745, /// 0x58c
			 0x4d7080f8, /// 0x590
			 0xd8a5798c, /// 0x594
			 0xfca42951, /// 0x598
			 0xbd5d65f2, /// 0x59c
			 0x611e76a9, /// 0x5a0
			 0xe803c577, /// 0x5a4
			 0x27c0d3c4, /// 0x5a8
			 0x602bed0a, /// 0x5ac
			 0xeaef2d98, /// 0x5b0
			 0x164bfce5, /// 0x5b4
			 0xe27545a7, /// 0x5b8
			 0xb7525222, /// 0x5bc
			 0xa97500b7, /// 0x5c0
			 0x2d2b127d, /// 0x5c4
			 0x33c743e5, /// 0x5c8
			 0x54477005, /// 0x5cc
			 0x68cddb20, /// 0x5d0
			 0xbbbce0d6, /// 0x5d4
			 0xcde0e43a, /// 0x5d8
			 0x5aafdbb8, /// 0x5dc
			 0xad900d75, /// 0x5e0
			 0x382a2b8f, /// 0x5e4
			 0x2573497a, /// 0x5e8
			 0x9f019916, /// 0x5ec
			 0x3bce9439, /// 0x5f0
			 0xe81044d9, /// 0x5f4
			 0xcf6a474a, /// 0x5f8
			 0x11029496, /// 0x5fc
			 0x2d579457, /// 0x600
			 0x3492057a, /// 0x604
			 0x91b6a486, /// 0x608
			 0x8a33debd, /// 0x60c
			 0x769e2f72, /// 0x610
			 0x351630c3, /// 0x614
			 0xebd0e8fe, /// 0x618
			 0x135acf54, /// 0x61c
			 0xda3bee13, /// 0x620
			 0xba80fc02, /// 0x624
			 0xff0a1982, /// 0x628
			 0xae5d80d6, /// 0x62c
			 0x2bdf94ae, /// 0x630
			 0x454c8a3c, /// 0x634
			 0xb42bd5e0, /// 0x638
			 0xabdfb8ae, /// 0x63c
			 0x6af9eea4, /// 0x640
			 0xf6ac3ced, /// 0x644
			 0x4269e8d0, /// 0x648
			 0xf57a3711, /// 0x64c
			 0xdac5aa67, /// 0x650
			 0x5e5d6372, /// 0x654
			 0xa77e15dd, /// 0x658
			 0xe3d3efce, /// 0x65c
			 0x290c4986, /// 0x660
			 0xdcce4d1e, /// 0x664
			 0x9d23930e, /// 0x668
			 0xf58e7a07, /// 0x66c
			 0xdaf4b517, /// 0x670
			 0xa6df2d8f, /// 0x674
			 0x5e3a06ab, /// 0x678
			 0x1045b57e, /// 0x67c
			 0x57f4232f, /// 0x680
			 0x2595b358, /// 0x684
			 0x4a55ef48, /// 0x688
			 0x6ab7fcc7, /// 0x68c
			 0x436de988, /// 0x690
			 0x3e51aa30, /// 0x694
			 0xe5adf267, /// 0x698
			 0xe24ea835, /// 0x69c
			 0xbcdeee7a, /// 0x6a0
			 0x7d988726, /// 0x6a4
			 0x6370b1e2, /// 0x6a8
			 0x81526416, /// 0x6ac
			 0x20c889fd, /// 0x6b0
			 0x470c9c6c, /// 0x6b4
			 0x22c318c1, /// 0x6b8
			 0x7f982a4c, /// 0x6bc
			 0x600d100c, /// 0x6c0
			 0xf3d9f4a1, /// 0x6c4
			 0x9cfd7d55, /// 0x6c8
			 0x1e418301, /// 0x6cc
			 0xf0361220, /// 0x6d0
			 0xf1d4d6de, /// 0x6d4
			 0x17bf12b2, /// 0x6d8
			 0x6754836a, /// 0x6dc
			 0xa3385795, /// 0x6e0
			 0x93af1e3d, /// 0x6e4
			 0x73c15943, /// 0x6e8
			 0x77d2e6d3, /// 0x6ec
			 0x63101feb, /// 0x6f0
			 0x5a2fba02, /// 0x6f4
			 0x5caaf596, /// 0x6f8
			 0xd4f59a9f, /// 0x6fc
			 0x58a34311, /// 0x700
			 0xfcdd0196, /// 0x704
			 0xc6089030, /// 0x708
			 0xe55162f8, /// 0x70c
			 0x56c9a1c8, /// 0x710
			 0xa2a472ff, /// 0x714
			 0xb058eaa7, /// 0x718
			 0x009cd5d5, /// 0x71c
			 0xf162b16b, /// 0x720
			 0x4c69b05c, /// 0x724
			 0x51464cfe, /// 0x728
			 0x12a8da45, /// 0x72c
			 0x9d124c1e, /// 0x730
			 0x142f9b63, /// 0x734
			 0xfc9d09c0, /// 0x738
			 0x634921e0, /// 0x73c
			 0xcd0d3fbf, /// 0x740
			 0x75dbc3f9, /// 0x744
			 0xd84aaefa, /// 0x748
			 0xd4e25c48, /// 0x74c
			 0xa5641435, /// 0x750
			 0xd22302eb, /// 0x754
			 0x79d9be7b, /// 0x758
			 0x771d106e, /// 0x75c
			 0x20fd6be0, /// 0x760
			 0x5997f91f, /// 0x764
			 0x82255349, /// 0x768
			 0x407593bd, /// 0x76c
			 0x09585cbf, /// 0x770
			 0xfce461df, /// 0x774
			 0x3d3d6981, /// 0x778
			 0xb659ccc9, /// 0x77c
			 0xc554a481, /// 0x780
			 0x8099d4c2, /// 0x784
			 0xf0f0502f, /// 0x788
			 0x83470305, /// 0x78c
			 0x228a9a6d, /// 0x790
			 0x96c06b11, /// 0x794
			 0xa36fdde0, /// 0x798
			 0x5ec310e2, /// 0x79c
			 0x1b93c7f4, /// 0x7a0
			 0x5ed0d35e, /// 0x7a4
			 0x60e1355b, /// 0x7a8
			 0xc472d36a, /// 0x7ac
			 0x37243d76, /// 0x7b0
			 0xfe936fb3, /// 0x7b4
			 0x20c8f842, /// 0x7b8
			 0x52ede462, /// 0x7bc
			 0x614b05a7, /// 0x7c0
			 0xa0eead36, /// 0x7c4
			 0xff598721, /// 0x7c8
			 0x1f776e93, /// 0x7cc
			 0x42d0d495, /// 0x7d0
			 0xb0e38d00, /// 0x7d4
			 0xb68256ca, /// 0x7d8
			 0x8455352e, /// 0x7dc
			 0xcfa2d729, /// 0x7e0
			 0x7504ef2b, /// 0x7e4
			 0x9e68336a, /// 0x7e8
			 0x625e06c3, /// 0x7ec
			 0xf548a9ea, /// 0x7f0
			 0xb86b8c96, /// 0x7f4
			 0x3b61e06f, /// 0x7f8
			 0xe90c0258, /// 0x7fc
			 0x21b45bed, /// 0x800
			 0xcca6f3d9, /// 0x804
			 0xdb834160, /// 0x808
			 0x2a075bc2, /// 0x80c
			 0xbef58025, /// 0x810
			 0xffe99e14, /// 0x814
			 0xef388882, /// 0x818
			 0xdc8f968a, /// 0x81c
			 0x1fafa1f3, /// 0x820
			 0xc9f0b10a, /// 0x824
			 0xe38c81cf, /// 0x828
			 0x0318a74f, /// 0x82c
			 0x51d03b61, /// 0x830
			 0x928a60bc, /// 0x834
			 0x077e1d1d, /// 0x838
			 0x10016a3d, /// 0x83c
			 0xab516e60, /// 0x840
			 0x75a5ac1c, /// 0x844
			 0x3c41760f, /// 0x848
			 0x7ddfa7ab, /// 0x84c
			 0x2f26b361, /// 0x850
			 0xd4c9e674, /// 0x854
			 0xc7e7531e, /// 0x858
			 0x04b654b3, /// 0x85c
			 0xe0e901c2, /// 0x860
			 0xb667dc1c, /// 0x864
			 0xa0f4ea71, /// 0x868
			 0x166f5cd7, /// 0x86c
			 0x561728a6, /// 0x870
			 0x445a660f, /// 0x874
			 0xce37ebc8, /// 0x878
			 0x68efa7cc, /// 0x87c
			 0xacf93406, /// 0x880
			 0x6a8f5269, /// 0x884
			 0x57389353, /// 0x888
			 0xed83c53f, /// 0x88c
			 0x0c168c58, /// 0x890
			 0x3ba1417c, /// 0x894
			 0xe38effbc, /// 0x898
			 0x6ace87df, /// 0x89c
			 0xf9a90b80, /// 0x8a0
			 0xb538d0b7, /// 0x8a4
			 0xc80d0f97, /// 0x8a8
			 0x968eae06, /// 0x8ac
			 0x33b8f013, /// 0x8b0
			 0x9adc521b, /// 0x8b4
			 0xffdfdace, /// 0x8b8
			 0xeefb07d2, /// 0x8bc
			 0xd4d5a4ba, /// 0x8c0
			 0xa35b9972, /// 0x8c4
			 0x135f6393, /// 0x8c8
			 0x3985f68e, /// 0x8cc
			 0xb75a28d9, /// 0x8d0
			 0x1e006e39, /// 0x8d4
			 0xd56e0298, /// 0x8d8
			 0xbed92c86, /// 0x8dc
			 0x148c164e, /// 0x8e0
			 0x65f24593, /// 0x8e4
			 0x80d85ca2, /// 0x8e8
			 0x809fd349, /// 0x8ec
			 0x05e04c64, /// 0x8f0
			 0xd6684646, /// 0x8f4
			 0x411a2774, /// 0x8f8
			 0x6eababab, /// 0x8fc
			 0xb8c7b230, /// 0x900
			 0x3b504a69, /// 0x904
			 0x3e279f13, /// 0x908
			 0x615fd4b6, /// 0x90c
			 0x9f44d6ed, /// 0x910
			 0xea7c3c40, /// 0x914
			 0xecdfec28, /// 0x918
			 0x0881260e, /// 0x91c
			 0x1da99e01, /// 0x920
			 0x809abc84, /// 0x924
			 0x9a541173, /// 0x928
			 0x841939f4, /// 0x92c
			 0x4760950d, /// 0x930
			 0xf53a2a0d, /// 0x934
			 0x25c3fa2a, /// 0x938
			 0x7d43f472, /// 0x93c
			 0x0bf354b1, /// 0x940
			 0xb9af5a1e, /// 0x944
			 0x2c8c538c, /// 0x948
			 0x68363ccd, /// 0x94c
			 0x3537a07c, /// 0x950
			 0x66f93e00, /// 0x954
			 0xee4a17b6, /// 0x958
			 0xd7f8b098, /// 0x95c
			 0x3bac3f39, /// 0x960
			 0x25231fd7, /// 0x964
			 0x915767de, /// 0x968
			 0x53b1d49c, /// 0x96c
			 0xfede20ed, /// 0x970
			 0x2697fa05, /// 0x974
			 0x902c0dcf, /// 0x978
			 0xebb2dc88, /// 0x97c
			 0x91831e3c, /// 0x980
			 0xc5048cc1, /// 0x984
			 0x8c4cb9a9, /// 0x988
			 0x506c6d18, /// 0x98c
			 0xfafbd2ca, /// 0x990
			 0x8cc51672, /// 0x994
			 0xb110a343, /// 0x998
			 0xba4b4d9f, /// 0x99c
			 0x003a858e, /// 0x9a0
			 0xb12eb3e1, /// 0x9a4
			 0x344bcb21, /// 0x9a8
			 0x3913c4f7, /// 0x9ac
			 0xee6f2c2e, /// 0x9b0
			 0xca0d4949, /// 0x9b4
			 0x8e5ff55e, /// 0x9b8
			 0x2f2c2129, /// 0x9bc
			 0xd1d70a36, /// 0x9c0
			 0xbb5ac8b4, /// 0x9c4
			 0x11f20d60, /// 0x9c8
			 0xf4dff106, /// 0x9cc
			 0x03a3633c, /// 0x9d0
			 0xf46a8844, /// 0x9d4
			 0xbd000e25, /// 0x9d8
			 0x58bfa24f, /// 0x9dc
			 0xda9066d9, /// 0x9e0
			 0x6842e166, /// 0x9e4
			 0xbe651518, /// 0x9e8
			 0x0ce74f5a, /// 0x9ec
			 0xaa82aefc, /// 0x9f0
			 0xb51029ed, /// 0x9f4
			 0x9db38f96, /// 0x9f8
			 0x5410c509, /// 0x9fc
			 0x7ccb8438, /// 0xa00
			 0xfa65c7b9, /// 0xa04
			 0x255bf42a, /// 0xa08
			 0x06ea47dd, /// 0xa0c
			 0xbf6b02b0, /// 0xa10
			 0x47c32ecc, /// 0xa14
			 0xc84cca89, /// 0xa18
			 0xa14fd143, /// 0xa1c
			 0xbd308f19, /// 0xa20
			 0x1005907d, /// 0xa24
			 0x5f8e0c2f, /// 0xa28
			 0x4c5328de, /// 0xa2c
			 0x5ba6e955, /// 0xa30
			 0x7c855b7e, /// 0xa34
			 0x485039ba, /// 0xa38
			 0x470c719c, /// 0xa3c
			 0xe2686d13, /// 0xa40
			 0xbfd4a5a3, /// 0xa44
			 0xbb08808e, /// 0xa48
			 0x353d49d6, /// 0xa4c
			 0x6f9fcfe0, /// 0xa50
			 0x82a34cc0, /// 0xa54
			 0xd03156c5, /// 0xa58
			 0xc8f0169a, /// 0xa5c
			 0x9892871b, /// 0xa60
			 0x378969ae, /// 0xa64
			 0xad138da3, /// 0xa68
			 0x8e105baf, /// 0xa6c
			 0x5c0f8bf6, /// 0xa70
			 0xeb656372, /// 0xa74
			 0x640344ae, /// 0xa78
			 0x1b518cbc, /// 0xa7c
			 0xfd43d98c, /// 0xa80
			 0x6f61dd96, /// 0xa84
			 0xcf5f11d6, /// 0xa88
			 0x6643768b, /// 0xa8c
			 0x0145e439, /// 0xa90
			 0xb756e0cf, /// 0xa94
			 0x1ee0de9a, /// 0xa98
			 0x30b08e86, /// 0xa9c
			 0xeedcfe1e, /// 0xaa0
			 0x111ee70d, /// 0xaa4
			 0x8af49eeb, /// 0xaa8
			 0xabc6ecc2, /// 0xaac
			 0x1a929a21, /// 0xab0
			 0xd4c19d79, /// 0xab4
			 0xa0c70c42, /// 0xab8
			 0x6870d676, /// 0xabc
			 0xe73c41b7, /// 0xac0
			 0x65f7db14, /// 0xac4
			 0xff13c379, /// 0xac8
			 0xbfc671a7, /// 0xacc
			 0xb65e577c, /// 0xad0
			 0xc5f0923e, /// 0xad4
			 0x83b8813e, /// 0xad8
			 0xa8fa2300, /// 0xadc
			 0x2910bdf9, /// 0xae0
			 0x4d5995e4, /// 0xae4
			 0xabbe4042, /// 0xae8
			 0xa02e295c, /// 0xaec
			 0xa6277bd4, /// 0xaf0
			 0x85ed97a2, /// 0xaf4
			 0x7f4f27fd, /// 0xaf8
			 0x78219e9c, /// 0xafc
			 0x4bb56539, /// 0xb00
			 0xdcd6f101, /// 0xb04
			 0xa134fe29, /// 0xb08
			 0x97e1cea5, /// 0xb0c
			 0x40ad8520, /// 0xb10
			 0x18db3290, /// 0xb14
			 0x5faf353b, /// 0xb18
			 0x795bb761, /// 0xb1c
			 0xa74f1f2c, /// 0xb20
			 0x38322de9, /// 0xb24
			 0x859bb17c, /// 0xb28
			 0x0fefed33, /// 0xb2c
			 0xbf3bc5fe, /// 0xb30
			 0xb03e9e8e, /// 0xb34
			 0x9a28f6ed, /// 0xb38
			 0x3f20081a, /// 0xb3c
			 0x8f1c54b7, /// 0xb40
			 0x06f1f09d, /// 0xb44
			 0x01b8ed35, /// 0xb48
			 0x9a36f7f4, /// 0xb4c
			 0x0e654373, /// 0xb50
			 0x60f56210, /// 0xb54
			 0x9a7fa9d8, /// 0xb58
			 0xe5304eb0, /// 0xb5c
			 0x7431e5e2, /// 0xb60
			 0x459191ca, /// 0xb64
			 0xd0cf5b18, /// 0xb68
			 0xebb697c8, /// 0xb6c
			 0x20aeb46c, /// 0xb70
			 0x2eb338c3, /// 0xb74
			 0xc3f08440, /// 0xb78
			 0x5d705a14, /// 0xb7c
			 0x2f3deaad, /// 0xb80
			 0x08cb1e56, /// 0xb84
			 0x3ff38335, /// 0xb88
			 0xa29bcfd8, /// 0xb8c
			 0xa69051ee, /// 0xb90
			 0x9ad0f9df, /// 0xb94
			 0x51929ee5, /// 0xb98
			 0x03471112, /// 0xb9c
			 0x8b976a17, /// 0xba0
			 0x119fcac9, /// 0xba4
			 0xc71e1577, /// 0xba8
			 0xd61dc986, /// 0xbac
			 0x41eae500, /// 0xbb0
			 0xec748b38, /// 0xbb4
			 0x6b39a54b, /// 0xbb8
			 0x82922dbd, /// 0xbbc
			 0xe8e069dd, /// 0xbc0
			 0x4927f4a6, /// 0xbc4
			 0x22642503, /// 0xbc8
			 0xee2304a4, /// 0xbcc
			 0x26ee720b, /// 0xbd0
			 0x101e000b, /// 0xbd4
			 0x3a967bf5, /// 0xbd8
			 0x86ad3761, /// 0xbdc
			 0x6f1414d2, /// 0xbe0
			 0xb7723389, /// 0xbe4
			 0x1c5e6cbb, /// 0xbe8
			 0x853281a0, /// 0xbec
			 0x33df2f33, /// 0xbf0
			 0xbf947429, /// 0xbf4
			 0x01d685fa, /// 0xbf8
			 0x81656874, /// 0xbfc
			 0x07647447, /// 0xc00
			 0x7ff4288a, /// 0xc04
			 0xb5e31bba, /// 0xc08
			 0xee277b20, /// 0xc0c
			 0x8fe50f32, /// 0xc10
			 0x34ac46b3, /// 0xc14
			 0xa6a66f4e, /// 0xc18
			 0xf433f4c7, /// 0xc1c
			 0x5b62ab8b, /// 0xc20
			 0xd46ab5cb, /// 0xc24
			 0x257bdf99, /// 0xc28
			 0xbb4fd339, /// 0xc2c
			 0x3b279ce3, /// 0xc30
			 0xc39f2a7a, /// 0xc34
			 0xdcae4306, /// 0xc38
			 0x734908a1, /// 0xc3c
			 0xfb8b91a6, /// 0xc40
			 0x7bf78a57, /// 0xc44
			 0x22a481a5, /// 0xc48
			 0xdbfbf480, /// 0xc4c
			 0x63461000, /// 0xc50
			 0x29f4b2d4, /// 0xc54
			 0x9943260d, /// 0xc58
			 0x9877f873, /// 0xc5c
			 0x4a1b7bd2, /// 0xc60
			 0xdbf94f21, /// 0xc64
			 0x1a08e6ef, /// 0xc68
			 0xb4ceb1a5, /// 0xc6c
			 0xe2590ad2, /// 0xc70
			 0xd58f85d5, /// 0xc74
			 0x1f68a0a9, /// 0xc78
			 0x636adb3e, /// 0xc7c
			 0x412224f5, /// 0xc80
			 0x8c8c8b43, /// 0xc84
			 0x5ea0d861, /// 0xc88
			 0x380d971e, /// 0xc8c
			 0x27ae3db4, /// 0xc90
			 0x9f20f6b0, /// 0xc94
			 0xe2e926e1, /// 0xc98
			 0x7cdce672, /// 0xc9c
			 0x664abb82, /// 0xca0
			 0x6d0e9cd2, /// 0xca4
			 0x160f070e, /// 0xca8
			 0xc2e584bf, /// 0xcac
			 0x09e74e29, /// 0xcb0
			 0xf7c9bf36, /// 0xcb4
			 0x57d3edfa, /// 0xcb8
			 0x27005289, /// 0xcbc
			 0x498cbf7a, /// 0xcc0
			 0xd37dad73, /// 0xcc4
			 0x4387ba7f, /// 0xcc8
			 0x1508a092, /// 0xccc
			 0x3ba9a2a8, /// 0xcd0
			 0x24d31fd3, /// 0xcd4
			 0xad878617, /// 0xcd8
			 0x937f94af, /// 0xcdc
			 0x7c1406f9, /// 0xce0
			 0x68dae371, /// 0xce4
			 0xe36f6833, /// 0xce8
			 0x3a055793, /// 0xcec
			 0x451da4c2, /// 0xcf0
			 0x92bbcb92, /// 0xcf4
			 0x60e7d24a, /// 0xcf8
			 0x46d38cf5, /// 0xcfc
			 0x98931c5e, /// 0xd00
			 0xfea20a48, /// 0xd04
			 0x99d6a421, /// 0xd08
			 0x8242f21f, /// 0xd0c
			 0xe654060a, /// 0xd10
			 0xb4b2cfd4, /// 0xd14
			 0xb7484100, /// 0xd18
			 0x6fd14e87, /// 0xd1c
			 0x9cbd9469, /// 0xd20
			 0x27caef0f, /// 0xd24
			 0x6c1e38a0, /// 0xd28
			 0xc1eb1d9c, /// 0xd2c
			 0x9f4e99c2, /// 0xd30
			 0x01d82773, /// 0xd34
			 0x590e0123, /// 0xd38
			 0x8f7d6c2c, /// 0xd3c
			 0xd9b827ac, /// 0xd40
			 0x8f2f8fc7, /// 0xd44
			 0x98296ae3, /// 0xd48
			 0x88774672, /// 0xd4c
			 0xa2197095, /// 0xd50
			 0xfe27a5dd, /// 0xd54
			 0x36c6d0a2, /// 0xd58
			 0x0d568dc4, /// 0xd5c
			 0xeddf024a, /// 0xd60
			 0xafc37b7e, /// 0xd64
			 0x6bf8dc17, /// 0xd68
			 0x2317d6a8, /// 0xd6c
			 0x26046283, /// 0xd70
			 0x67291ab7, /// 0xd74
			 0xc0b3d714, /// 0xd78
			 0x0b3c977d, /// 0xd7c
			 0x1b36e15b, /// 0xd80
			 0x29c9721d, /// 0xd84
			 0xdf22b874, /// 0xd88
			 0x4a7ec049, /// 0xd8c
			 0xdcb23328, /// 0xd90
			 0x5a1b5ff6, /// 0xd94
			 0x8a9111a7, /// 0xd98
			 0xbe6ae588, /// 0xd9c
			 0x05cea9dd, /// 0xda0
			 0x588cb7b6, /// 0xda4
			 0x9db35ce0, /// 0xda8
			 0x446d1e45, /// 0xdac
			 0x1e9da42c, /// 0xdb0
			 0x70801fa9, /// 0xdb4
			 0x50d206d3, /// 0xdb8
			 0x003b1c79, /// 0xdbc
			 0x0609f188, /// 0xdc0
			 0xb56c8713, /// 0xdc4
			 0xdf867cd0, /// 0xdc8
			 0x45f6ba5f, /// 0xdcc
			 0xa9c0dc6c, /// 0xdd0
			 0xc9fe0a78, /// 0xdd4
			 0x5b4508d9, /// 0xdd8
			 0xf33fdecd, /// 0xddc
			 0x4241659b, /// 0xde0
			 0xa04ca03f, /// 0xde4
			 0xa4a02201, /// 0xde8
			 0x3dd18403, /// 0xdec
			 0xc15826fc, /// 0xdf0
			 0x562c05cd, /// 0xdf4
			 0xae6f233f, /// 0xdf8
			 0x8196f004, /// 0xdfc
			 0x8825cd60, /// 0xe00
			 0x2156d4c3, /// 0xe04
			 0x5958b378, /// 0xe08
			 0xd955212a, /// 0xe0c
			 0x4707265c, /// 0xe10
			 0x51f5ffb7, /// 0xe14
			 0x8bc88d29, /// 0xe18
			 0x53d29440, /// 0xe1c
			 0x709e2a55, /// 0xe20
			 0xd15e2abc, /// 0xe24
			 0x1e2523a3, /// 0xe28
			 0x1f2114d7, /// 0xe2c
			 0xe448339c, /// 0xe30
			 0x51b61b33, /// 0xe34
			 0x2b7ad43a, /// 0xe38
			 0x3b0d1bc1, /// 0xe3c
			 0x39a33d4e, /// 0xe40
			 0xd1fe402f, /// 0xe44
			 0x2a6f7052, /// 0xe48
			 0x572cf1ab, /// 0xe4c
			 0x422d6797, /// 0xe50
			 0xe36a6923, /// 0xe54
			 0x94dda326, /// 0xe58
			 0x2e8fecd6, /// 0xe5c
			 0x66bc6767, /// 0xe60
			 0x77738d42, /// 0xe64
			 0x62c98d52, /// 0xe68
			 0x82dae279, /// 0xe6c
			 0x7e145403, /// 0xe70
			 0x327b0950, /// 0xe74
			 0xabfbb1a7, /// 0xe78
			 0xfc84f2d4, /// 0xe7c
			 0x189bc917, /// 0xe80
			 0x894900b6, /// 0xe84
			 0x14dc3a66, /// 0xe88
			 0x201a4353, /// 0xe8c
			 0xeae32eaa, /// 0xe90
			 0x44edbe6d, /// 0xe94
			 0x5fd4dd2e, /// 0xe98
			 0x1452010f, /// 0xe9c
			 0xe6fffb28, /// 0xea0
			 0xe29a9f03, /// 0xea4
			 0xbfcf452c, /// 0xea8
			 0xe5bbb1bf, /// 0xeac
			 0xf3575775, /// 0xeb0
			 0xae76a026, /// 0xeb4
			 0x672718b9, /// 0xeb8
			 0x7ac7ada6, /// 0xebc
			 0xeded6ae2, /// 0xec0
			 0xdf5e2b01, /// 0xec4
			 0xdf6b153b, /// 0xec8
			 0x55f4bece, /// 0xecc
			 0x465539a6, /// 0xed0
			 0x18c03677, /// 0xed4
			 0xad7977f0, /// 0xed8
			 0x99a3d1bf, /// 0xedc
			 0xf723c68d, /// 0xee0
			 0x47416460, /// 0xee4
			 0x3e112b5e, /// 0xee8
			 0xee16655d, /// 0xeec
			 0x643db50b, /// 0xef0
			 0x64236bb5, /// 0xef4
			 0x2c2b9cdc, /// 0xef8
			 0xae8b2e00, /// 0xefc
			 0xadf8b21c, /// 0xf00
			 0x07a0d1bf, /// 0xf04
			 0xb43af403, /// 0xf08
			 0x50c3add1, /// 0xf0c
			 0x8f5540a6, /// 0xf10
			 0x0d140079, /// 0xf14
			 0x38c62d3a, /// 0xf18
			 0xbbd2acff, /// 0xf1c
			 0x22034c66, /// 0xf20
			 0x2885b706, /// 0xf24
			 0xeaea35df, /// 0xf28
			 0x5fd91366, /// 0xf2c
			 0xbddc22ec, /// 0xf30
			 0x153bf12e, /// 0xf34
			 0x5db9aaba, /// 0xf38
			 0x7268255c, /// 0xf3c
			 0xc8cb8bf4, /// 0xf40
			 0x383a1222, /// 0xf44
			 0x69f2a7cc, /// 0xf48
			 0x544d2ea3, /// 0xf4c
			 0x2742cc4c, /// 0xf50
			 0xa6dcfb89, /// 0xf54
			 0xbb4743ea, /// 0xf58
			 0xfde35587, /// 0xf5c
			 0xb0edbb75, /// 0xf60
			 0x19307804, /// 0xf64
			 0x219b2125, /// 0xf68
			 0x7c503091, /// 0xf6c
			 0x9b094105, /// 0xf70
			 0xaa01cde6, /// 0xf74
			 0xe26e6a83, /// 0xf78
			 0xe8e1a10c, /// 0xf7c
			 0x76804e56, /// 0xf80
			 0x094601cb, /// 0xf84
			 0x107483d7, /// 0xf88
			 0x75c3c03d, /// 0xf8c
			 0xe93a0e70, /// 0xf90
			 0x2304f06c, /// 0xf94
			 0x180a469a, /// 0xf98
			 0x72f7bccb, /// 0xf9c
			 0x2ebe9177, /// 0xfa0
			 0x8985b539, /// 0xfa4
			 0xad081106, /// 0xfa8
			 0x2562a2d2, /// 0xfac
			 0xc462c2fe, /// 0xfb0
			 0x98ee11d9, /// 0xfb4
			 0x27550789, /// 0xfb8
			 0xebfd9f2e, /// 0xfbc
			 0xf26d761c, /// 0xfc0
			 0x3507558a, /// 0xfc4
			 0xb529d065, /// 0xfc8
			 0xec279513, /// 0xfcc
			 0x070ee9d9, /// 0xfd0
			 0x5a85b5ef, /// 0xfd4
			 0x86c30db3, /// 0xfd8
			 0x2f242713, /// 0xfdc
			 0xca485c06, /// 0xfe0
			 0x095ebcfd, /// 0xfe4
			 0x66aa54a9, /// 0xfe8
			 0xa88f5b86, /// 0xfec
			 0x0acc7b91, /// 0xff0
			 0x07d8bc98, /// 0xff4
			 0x56ecbd33, /// 0xff8
			 0x6bcae014 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0000c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00010
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00014
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00018
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0001c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00020
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00024
			 0xbf028f5c,                                                  // -0.51                                       /// 00028
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00034
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0003c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00048
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00050
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00054
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00058
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0005c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00060
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00068
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0006c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x7f800000,                                                  // inf                                         /// 00074
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0007c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x3f028f5c,                                                  // 0.51                                        /// 00084
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00088
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00090
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0009c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e0
			 0x7f800000,                                                  // inf                                         /// 000e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00110
			 0xcb000000,                                                  // -8388608.0                                  /// 00114
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00118
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00120
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0012c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00130
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00134
			 0x0c700000,                                                  // Leading 1s:                                 /// 00138
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0013c
			 0x7f800000,                                                  // inf                                         /// 00140
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0014c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00150
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00154
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00160
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00164
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00168
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0016c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00174
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00178
			 0x0e000001,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00180
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0018c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00190
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00194
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00198
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00200
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00208
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0020c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00210
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00214
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00220
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00224
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x0c700000,                                                  // Leading 1s:                                 /// 0022c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00230
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x80011111,                                                  // -9.7958E-41                                 /// 00238
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0023c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00240
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00244
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00248
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0024c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00254
			 0x0c700000,                                                  // Leading 1s:                                 /// 00258
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00260
			 0x00011111,                                                  // 9.7958E-41                                  /// 00264
			 0xbf028f5c,                                                  // -0.51                                       /// 00268
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0026c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00274
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00278
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00280
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0028c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00290
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00298
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x00000000,                                                  // zero                                        /// 002b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x00000000,                                                  // zero                                        /// 002cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002e4
			 0x3f028f5c,                                                  // 0.51                                        /// 002e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00304
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00308
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0030c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00310
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00314
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00320
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00324
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00328
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00334
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00338
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0033c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00340
			 0x0e000003,                                                  // Trailing 1s:                                /// 00344
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00348
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00350
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00374
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00378
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0037c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00380
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x55555555,                                                  // 4 random values                             /// 0038c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00390
			 0x0c700000,                                                  // Leading 1s:                                 /// 00394
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ec
			 0x80000000,                                                  // -zero                                       /// 003f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003f8
			 0x4b000000,                                                  // 8388608.0                                   /// 003fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00400
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00404
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0040c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00418
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00428
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0042c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00430
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00434
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00444
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00448
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00450
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00454
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00458
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00460
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00468
			 0xcb000000,                                                  // -8388608.0                                  /// 0046c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00470
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00478
			 0x55555555,                                                  // 4 random values                             /// 0047c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00480
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00488
			 0xffc00001,                                                  // -signaling NaN                              /// 0048c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00490
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00494
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00498
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004bc
			 0xff800000,                                                  // -inf                                        /// 004c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x33333333,                                                  // 4 random values                             /// 004d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e4
			 0xff800000,                                                  // -inf                                        /// 004e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00504
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00508
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0050c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00510
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00524
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0052c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00530
			 0x00011111,                                                  // 9.7958E-41                                  /// 00534
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x0e000007,                                                  // Trailing 1s:                                /// 0053c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00548
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00554
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00558
			 0x3f028f5c,                                                  // 0.51                                        /// 0055c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00560
			 0x0c400000,                                                  // Leading 1s:                                 /// 00564
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00570
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00574
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00580
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00584
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00588
			 0xffc00001,                                                  // -signaling NaN                              /// 0058c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80011111,                                                  // -9.7958E-41                                 /// 00594
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00598
			 0x80011111,                                                  // -9.7958E-41                                 /// 0059c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a0
			 0xcb000000,                                                  // -8388608.0                                  /// 005a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005e8
			 0x4b000000,                                                  // 8388608.0                                   /// 005ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00600
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00604
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00608
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0060c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00610
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00618
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0061c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00620
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00624
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00628
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00630
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00638
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00640
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00644
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0064c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00654
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00658
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0065c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00660
			 0x00011111,                                                  // 9.7958E-41                                  /// 00664
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00674
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00680
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00684
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00688
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0068c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00694
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00698
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0c780000,                                                  // Leading 1s:                                 /// 00708
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0070c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00714
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00718
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00720
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00724
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00728
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00730
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0073c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00744
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00748
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0074c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00750
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00760
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00764
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00768
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0076c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00770
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00774
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00778
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0077c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00780
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00788
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00790
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00794
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0079c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a8
			 0x4b000000,                                                  // 8388608.0                                   /// 007ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007dc
			 0x3f028f5c,                                                  // 0.51                                        /// 007e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007f4
			 0x80000000,                                                  // -zero                                       /// 007f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00800
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00808
			 0xffc00001,                                                  // -signaling NaN                              /// 0080c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00810
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00818
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0081c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00820
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00824
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00828
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0082c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00838
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0083c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00840
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00854
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x55555555,                                                  // 4 random values                             /// 00860
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00864
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0086c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00870
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00878
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0087c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00880
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00884
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x0c700000,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00890
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00894
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00898
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0089c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00900
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00904
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00908
			 0xff800000,                                                  // -inf                                        /// 0090c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00910
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00914
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0091c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00920
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x4b000000,                                                  // 8388608.0                                   /// 00928
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00934
			 0x0c700000,                                                  // Leading 1s:                                 /// 00938
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0093c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00940
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00944
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00948
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0094c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00950
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00954
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0095c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x4b000000,                                                  // 8388608.0                                   /// 00964
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00968
			 0xbf028f5c,                                                  // -0.51                                       /// 0096c
			 0x55555555,                                                  // 4 random values                             /// 00970
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00974
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00978
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00998
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0099c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009b8
			 0x4b000000,                                                  // 8388608.0                                   /// 009bc
			 0x7f800000,                                                  // inf                                         /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x4b000000,                                                  // 8388608.0                                   /// 009ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00a00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x33333333,                                                  // 4 random values                             /// 00a34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a88
			 0x7fc00001,                                                  // signaling NaN                               /// 00a8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ab4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ab8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00000000,                                                  // zero                                        /// 00ac8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00acc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ad8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00adc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ae4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x55555555,                                                  // 4 random values                             /// 00b44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b68
			 0xbf028f5c,                                                  // -0.51                                       /// 00b6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bf0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf8
			 0x55555555,                                                  // 4 random values                             /// 00bfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c04
			 0x7fc00001,                                                  // signaling NaN                               /// 00c08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x55555555,                                                  // 4 random values                             /// 00c14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c2c
			 0xff800000,                                                  // -inf                                        /// 00c30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c34
			 0xffc00001,                                                  // -signaling NaN                              /// 00c38
			 0x7f800000,                                                  // inf                                         /// 00c3c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cdc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cf0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cf8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0xcb000000,                                                  // -8388608.0                                  /// 00d1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d30
			 0x7fc00001,                                                  // signaling NaN                               /// 00d34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x33333333,                                                  // 4 random values                             /// 00d94
			 0x00000000,                                                  // zero                                        /// 00d98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00da4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00da8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ddc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00df0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00df8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e98
			 0xbf028f5c,                                                  // -0.51                                       /// 00e9c
			 0x33333333,                                                  // 4 random values                             /// 00ea0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ec8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ecc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ed0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00eec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ef0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ef4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f00
			 0x55555555,                                                  // 4 random values                             /// 00f04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0xcb000000,                                                  // -8388608.0                                  /// 00f38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fa4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fa8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fcc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fe0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ff0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff8
			 0xff7ffffe // max norm - 1ulp                               // SP - ve numbers                             /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00000
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0000c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00010
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00014
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0x7f800000,                                                  // inf                                         /// 00020
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0002c
			 0x00000000,                                                  // zero                                        /// 00030
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00038
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0003c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00040
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00044
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x0c400000,                                                  // Leading 1s:                                 /// 00058
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00060
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00068
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0006c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00070
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0007c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00080
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00088
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0008c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00098
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 000b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d0
			 0x7f800000,                                                  // inf                                         /// 000d4
			 0x3f028f5c,                                                  // 0.51                                        /// 000d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00100
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00104
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00108
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0010c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00114
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00120
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00124
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00128
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0012c
			 0xff800000,                                                  // -inf                                        /// 00130
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00134
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00138
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0013c
			 0x7f800000,                                                  // inf                                         /// 00140
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0014c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00164
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00168
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0016c
			 0x4b000000,                                                  // 8388608.0                                   /// 00170
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00174
			 0x0e000003,                                                  // Trailing 1s:                                /// 00178
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xffc00001,                                                  // -signaling NaN                              /// 00184
			 0x0c400000,                                                  // Leading 1s:                                 /// 00188
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00190
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00194
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00198
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0019c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a4
			 0x7fc00001,                                                  // signaling NaN                               /// 001a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x4b000000,                                                  // 8388608.0                                   /// 001d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001f8
			 0xffc00001,                                                  // -signaling NaN                              /// 001fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00204
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00208
			 0x0c700000,                                                  // Leading 1s:                                 /// 0020c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00210
			 0x0e000001,                                                  // Trailing 1s:                                /// 00214
			 0x00011111,                                                  // 9.7958E-41                                  /// 00218
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0021c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00228
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0022c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00234
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00238
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0023c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00240
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00244
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00248
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0024c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00250
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00254
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00258
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0025c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0026c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00274
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00278
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00280
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00294
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0029c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00300
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00308
			 0x0e000007,                                                  // Trailing 1s:                                /// 0030c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00310
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0031c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00328
			 0x0e000003,                                                  // Trailing 1s:                                /// 0032c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00330
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00338
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0033c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00340
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00344
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0034c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00354
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00358
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00360
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00364
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00370
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00374
			 0x3f028f5c,                                                  // 0.51                                        /// 00378
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0037c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00388
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00390
			 0xbf028f5c,                                                  // -0.51                                       /// 00394
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0039c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00404
			 0x7fc00001,                                                  // signaling NaN                               /// 00408
			 0x0c780000,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00410
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00414
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0041c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0042c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x7f800000,                                                  // inf                                         /// 00434
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0043c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00440
			 0xcb000000,                                                  // -8388608.0                                  /// 00444
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00448
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00450
			 0x4b000000,                                                  // 8388608.0                                   /// 00454
			 0x80011111,                                                  // -9.7958E-41                                 /// 00458
			 0x4b000000,                                                  // 8388608.0                                   /// 0045c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00460
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00468
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0046c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00470
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00474
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00478
			 0x4b000000,                                                  // 8388608.0                                   /// 0047c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c700000,                                                  // Leading 1s:                                 /// 00484
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c700000,                                                  // Leading 1s:                                 /// 00498
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0049c
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a4
			 0x4b000000,                                                  // 8388608.0                                   /// 004a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004bc
			 0x55555555,                                                  // 4 random values                             /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f8
			 0x3f028f5c,                                                  // 0.51                                        /// 004fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00500
			 0x0e000001,                                                  // Trailing 1s:                                /// 00504
			 0x0e000001,                                                  // Trailing 1s:                                /// 00508
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0050c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00510
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00514
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00518
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0051c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00520
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00524
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00528
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00530
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0053c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00548
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0054c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00550
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0055c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00564
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xcb000000,                                                  // -8388608.0                                  /// 0056c
			 0xcb000000,                                                  // -8388608.0                                  /// 00570
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00574
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00580
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00588
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0058c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00594
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00598
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0059c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x3f028f5c,                                                  // 0.51                                        /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00600
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00608
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x4b000000,                                                  // 8388608.0                                   /// 0061c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00624
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00628
			 0xcb000000,                                                  // -8388608.0                                  /// 0062c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00630
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00638
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0063c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00640
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0064c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00650
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0c600000,                                                  // Leading 1s:                                 /// 00658
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0065c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00660
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00668
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00670
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00674
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00678
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0067c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00680
			 0x0e000001,                                                  // Trailing 1s:                                /// 00684
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00694
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00698
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006e0
			 0x3f028f5c,                                                  // 0.51                                        /// 006e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0070c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00710
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00714
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00718
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00724
			 0x0c700000,                                                  // Leading 1s:                                 /// 00728
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00730
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0073c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00740
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00748
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0074c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00750
			 0x0c400000,                                                  // Leading 1s:                                 /// 00754
			 0x0c700000,                                                  // Leading 1s:                                 /// 00758
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0075c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00760
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00764
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00768
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00774
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00778
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0077c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x33333333,                                                  // 4 random values                             /// 00788
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0078c
			 0x00000000,                                                  // zero                                        /// 00790
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00794
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00798
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00804
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0080c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00810
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00814
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00820
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00824
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0082c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00830
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0083c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00840
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00844
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00848
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0084c
			 0xcb000000,                                                  // -8388608.0                                  /// 00850
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00854
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00858
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0085c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00860
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00864
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0086c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0087c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00880
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00884
			 0x55555555,                                                  // 4 random values                             /// 00888
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0088c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00894
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00910
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00918
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00924
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00928
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0092c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00930
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00934
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00944
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0c700000,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00950
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00954
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xcb000000,                                                  // -8388608.0                                  /// 00960
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00964
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00968
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0096c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00974
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00978
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00980
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00984
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0xbf028f5c,                                                  // -0.51                                       /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00994
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00998
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0099c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x3f028f5c,                                                  // 0.51                                        /// 009e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a04
			 0x7fc00001,                                                  // signaling NaN                               /// 00a08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a3c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a88
			 0x4b000000,                                                  // 8388608.0                                   /// 00a8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0xff800000,                                                  // -inf                                        /// 00a98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aa0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00abc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00000000,                                                  // zero                                        /// 00af0
			 0x80000000,                                                  // -zero                                       /// 00af4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00afc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x3f028f5c,                                                  // 0.51                                        /// 00b24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b64
			 0x55555555,                                                  // 4 random values                             /// 00b68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b78
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ba0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bd4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00be0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000000,                                                  // -zero                                       /// 00be8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf4
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bfc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x80000000,                                                  // -zero                                       /// 00c28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ca0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x80000000,                                                  // -zero                                       /// 00cc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cfc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xffc00001,                                                  // -signaling NaN                              /// 00d1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d34
			 0xbf028f5c,                                                  // -0.51                                       /// 00d38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d64
			 0x7f800000,                                                  // inf                                         /// 00d68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da0
			 0x00000000,                                                  // zero                                        /// 00da4
			 0xff800000,                                                  // -inf                                        /// 00da8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00db0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000000,                                                  // -zero                                       /// 00de0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de4
			 0x55555555,                                                  // 4 random values                             /// 00de8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dfc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e20
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e44
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ea0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x7f800000,                                                  // inf                                         /// 00ea8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ec0
			 0x7f800000,                                                  // inf                                         /// 00ec4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ecc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00edc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ee0
			 0x00000000,                                                  // zero                                        /// 00ee4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f00
			 0x80000000,                                                  // -zero                                       /// 00f04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x55555555,                                                  // 4 random values                             /// 00f0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc0
			 0x7f800000,                                                  // inf                                         /// 00fc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ff4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff8
			 0x00004000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x000004d8,
			 0x00000754,
			 0x000004e4,
			 0x000001c8,
			 0x00000210,
			 0x00000164,
			 0x0000003c,
			 0x00000000,

			 /// vpu register f2
			 0x41800000,
			 0x40a00000,
			 0x40a00000,
			 0x42000000,
			 0x41980000,
			 0x41800000,
			 0x41b00000,
			 0x41a00000,

			 /// vpu register f3
			 0x41400000,
			 0x41700000,
			 0x41b00000,
			 0x41980000,
			 0x40800000,
			 0x40e00000,
			 0x41a00000,
			 0x41400000,

			 /// vpu register f4
			 0x41600000,
			 0x41200000,
			 0x41600000,
			 0x41f80000,
			 0x41000000,
			 0x41400000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f5
			 0x41f00000,
			 0x41300000,
			 0x41980000,
			 0x40000000,
			 0x40000000,
			 0x41f80000,
			 0x41b00000,
			 0x40400000,

			 /// vpu register f6
			 0x41a80000,
			 0x41600000,
			 0x41000000,
			 0x41800000,
			 0x40a00000,
			 0x41100000,
			 0x41300000,
			 0x41d80000,

			 /// vpu register f7
			 0x41f00000,
			 0x41a00000,
			 0x41700000,
			 0x41400000,
			 0x41f80000,
			 0x42000000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f8
			 0x40800000,
			 0x41a00000,
			 0x40400000,
			 0x41880000,
			 0x41100000,
			 0x41700000,
			 0x41b80000,
			 0x41000000,

			 /// vpu register f9
			 0x41a00000,
			 0x41c00000,
			 0x41700000,
			 0x41c00000,
			 0x41a80000,
			 0x41e00000,
			 0x41e80000,
			 0x41a80000,

			 /// vpu register f10
			 0x41f80000,
			 0x41400000,
			 0x41880000,
			 0x41b00000,
			 0x41a80000,
			 0x41e00000,
			 0x41b80000,
			 0x41700000,

			 /// vpu register f11
			 0x41200000,
			 0x41f00000,
			 0x41a80000,
			 0x41100000,
			 0x41600000,
			 0x41300000,
			 0x41500000,
			 0x41200000,

			 /// vpu register f12
			 0x41100000,
			 0x41000000,
			 0x41e80000,
			 0x40e00000,
			 0x41400000,
			 0x41000000,
			 0x41600000,
			 0x41700000,

			 /// vpu register f13
			 0x41600000,
			 0x41600000,
			 0x3f800000,
			 0x41f80000,
			 0x41f80000,
			 0x41c80000,
			 0x41e80000,
			 0x41800000,

			 /// vpu register f14
			 0x42000000,
			 0x41400000,
			 0x41f00000,
			 0x41000000,
			 0x3f800000,
			 0x40400000,
			 0x42000000,
			 0x41c80000,

			 /// vpu register f15
			 0x40e00000,
			 0x41980000,
			 0x41a80000,
			 0x41b00000,
			 0x41d00000,
			 0x41a00000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f16
			 0x41e00000,
			 0x41a80000,
			 0x41800000,
			 0x41000000,
			 0x40800000,
			 0x41d80000,
			 0x41c00000,
			 0x41100000,

			 /// vpu register f17
			 0x40c00000,
			 0x40400000,
			 0x41000000,
			 0x41f80000,
			 0x40000000,
			 0x41200000,
			 0x40e00000,
			 0x41980000,

			 /// vpu register f18
			 0x41c00000,
			 0x41880000,
			 0x41980000,
			 0x41900000,
			 0x3f800000,
			 0x41800000,
			 0x41700000,
			 0x41200000,

			 /// vpu register f19
			 0x40000000,
			 0x41700000,
			 0x41500000,
			 0x40400000,
			 0x41c80000,
			 0x41880000,
			 0x41800000,
			 0x40c00000,

			 /// vpu register f20
			 0x41980000,
			 0x41400000,
			 0x3f800000,
			 0x41c80000,
			 0x41100000,
			 0x41a00000,
			 0x40800000,
			 0x41600000,

			 /// vpu register f21
			 0x41d80000,
			 0x41f00000,
			 0x40800000,
			 0x41e00000,
			 0x41c00000,
			 0x41400000,
			 0x41700000,
			 0x41500000,

			 /// vpu register f22
			 0x41b00000,
			 0x41900000,
			 0x41b80000,
			 0x41900000,
			 0x41000000,
			 0x41500000,
			 0x41800000,
			 0x41f00000,

			 /// vpu register f23
			 0x3f800000,
			 0x41500000,
			 0x41c00000,
			 0x40800000,
			 0x41d80000,
			 0x40000000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f24
			 0x41300000,
			 0x41d80000,
			 0x41e80000,
			 0x40800000,
			 0x41e00000,
			 0x41100000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f25
			 0x41d80000,
			 0x41000000,
			 0x40c00000,
			 0x41980000,
			 0x41a00000,
			 0x41880000,
			 0x42000000,
			 0x41d00000,

			 /// vpu register f26
			 0x3f800000,
			 0x40c00000,
			 0x41a00000,
			 0x41880000,
			 0x41d00000,
			 0x41b00000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f27
			 0x3f800000,
			 0x42000000,
			 0x41f80000,
			 0x41f00000,
			 0x41800000,
			 0x41200000,
			 0x41c80000,
			 0x42000000,

			 /// vpu register f28
			 0x41880000,
			 0x41b80000,
			 0x41000000,
			 0x41a80000,
			 0x41880000,
			 0x40e00000,
			 0x41600000,
			 0x41d80000,

			 /// vpu register f29
			 0x41300000,
			 0x41300000,
			 0x40a00000,
			 0x40000000,
			 0x42000000,
			 0x41100000,
			 0x41300000,
			 0x41d00000,

			 /// vpu register f30
			 0x40400000,
			 0x41880000,
			 0x3f800000,
			 0x41e00000,
			 0x40400000,
			 0x41700000,
			 0x40000000,
			 0x41b00000,

			 /// vpu register f31
			 0x40000000,
			 0x41900000,
			 0x41300000,
			 0x40a00000,
			 0x41800000,
			 0x41f80000,
			 0x41600000,
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
		"flt.s x7, f16, f2\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f11, f18\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f18, f10\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f22, f7\n"                                 ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f29, f25\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f14, f22\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f7, f6\n"                                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f8, f3\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f18, f23\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f24, f3\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f7, f21\n"                                 ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f25, f21\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f3, f24\n"                                 ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f30, f22\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f5, f23\n"                                 ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f26, f21\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f6, f18\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f25, f21\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f19, f22\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f16, f7\n"                                 ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f7, f15\n"                                 ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f4, f0\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f30, f22\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f23, f16\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f19, f6\n"                                 ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f22, f6\n"                                 ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f17, f21\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f17, f22\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f25, f7\n"                                 ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f6, f19\n"                                 ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f17, f18\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f6, f18\n"                                 ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f13, f18\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f30, f9\n"                                 ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f2, f0\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f22, f9\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f6, f3\n"                                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f19, f22\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f25, f8\n"                                 ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f29, f12\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f6, f19\n"                                 ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f25, f9\n"                                 ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f24, f11\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f5, f3\n"                                  ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f21, f12\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f7, f10\n"                                 ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f1, f13\n"                                 ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f11, f20\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f27, f1\n"                                 ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f26, f13\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f8, f24\n"                                 ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f5, f23\n"                                 ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f5, f18\n"                                 ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f4, f22\n"                                 ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f1, f10\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f4, f1\n"                                  ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f11, f26\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f3, f19\n"                                 ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f26, f14\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f27, f3\n"                                 ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f16, f16\n"                                ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f22, f20\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f7, f27\n"                                 ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f11, f18\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f22, f22\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f9, f27\n"                                 ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f20, f21\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f20, f13\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f0, f20\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f17, f19\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f19, f14\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f7, f24\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f20, f5\n"                                 ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f2, f10\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f30, f29\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f26, f8\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f28, f17\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f21, f13\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f27, f17\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f14, f25\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f12, f8\n"                                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f0, f5\n"                                  ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f6, f3\n"                                  ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f0, f21\n"                                 ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f1, f26\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f15, f26\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f22, f0\n"                                 ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x7, f10, f19\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f24, f18\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f2, f14\n"                                 ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f23, f21\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f21, f8\n"                                 ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x6, f3, f0\n"                                  ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x9, f25, f15\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f29, f5\n"                                 ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f30, f20\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x4, f6, f8\n"                                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x5, f28, f6\n"                                 ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.s x3, f13, f1\n"                                 ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
