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
			 0xe6e62063, /// 0x0
			 0xa3382d2a, /// 0x4
			 0xaf6a5384, /// 0x8
			 0x7933dbf9, /// 0xc
			 0x73f3da3e, /// 0x10
			 0x00f46a41, /// 0x14
			 0x1bf6bfad, /// 0x18
			 0x596b5dd8, /// 0x1c
			 0x66f042dc, /// 0x20
			 0xb5d12a18, /// 0x24
			 0x50c06c80, /// 0x28
			 0x8be53603, /// 0x2c
			 0x55cf50a6, /// 0x30
			 0x52a0aa17, /// 0x34
			 0xb0b4010c, /// 0x38
			 0x73db9b2c, /// 0x3c
			 0x81dc952a, /// 0x40
			 0xccced5f7, /// 0x44
			 0xc0dcb28c, /// 0x48
			 0x92b4df26, /// 0x4c
			 0x7e3d1dde, /// 0x50
			 0x42127363, /// 0x54
			 0x024ec82c, /// 0x58
			 0x1c6f67ae, /// 0x5c
			 0x4b1e25fd, /// 0x60
			 0xc65f7c6d, /// 0x64
			 0x1283fc5e, /// 0x68
			 0xd8b78426, /// 0x6c
			 0x8486fbf7, /// 0x70
			 0x61437d44, /// 0x74
			 0x845bd675, /// 0x78
			 0x23be707c, /// 0x7c
			 0xa8c9a7dc, /// 0x80
			 0x0ac3ebc8, /// 0x84
			 0xb9eb4cdc, /// 0x88
			 0xb10fa18a, /// 0x8c
			 0x00a5c49f, /// 0x90
			 0x5c60d8de, /// 0x94
			 0x85ff939b, /// 0x98
			 0xf9b804f1, /// 0x9c
			 0x946d93c7, /// 0xa0
			 0x105d79ea, /// 0xa4
			 0x35d71b28, /// 0xa8
			 0xfa64e4ee, /// 0xac
			 0x407ece99, /// 0xb0
			 0xac8dc7fb, /// 0xb4
			 0xc3c95bb1, /// 0xb8
			 0x5a5c3d47, /// 0xbc
			 0x75609e61, /// 0xc0
			 0x6c1ffa0c, /// 0xc4
			 0x5652baeb, /// 0xc8
			 0x30d6e77a, /// 0xcc
			 0x703c6f8f, /// 0xd0
			 0xa72c8a9a, /// 0xd4
			 0xe65d268e, /// 0xd8
			 0x1c36824f, /// 0xdc
			 0x32e8aa43, /// 0xe0
			 0x47cb88a5, /// 0xe4
			 0x7c3b878f, /// 0xe8
			 0xf873f8d2, /// 0xec
			 0x2bffff9f, /// 0xf0
			 0x8e7ce518, /// 0xf4
			 0x61e74d59, /// 0xf8
			 0x10aad60a, /// 0xfc
			 0x9736ae1b, /// 0x100
			 0x464109d9, /// 0x104
			 0xc5092eee, /// 0x108
			 0xbbbbece3, /// 0x10c
			 0xa36b9315, /// 0x110
			 0x8ac3831c, /// 0x114
			 0xbbccd771, /// 0x118
			 0x70a8e3f5, /// 0x11c
			 0x61c31075, /// 0x120
			 0x3e9c9597, /// 0x124
			 0xadec787b, /// 0x128
			 0x24a0f09b, /// 0x12c
			 0x49611509, /// 0x130
			 0x6f778897, /// 0x134
			 0x49407b80, /// 0x138
			 0x24c34586, /// 0x13c
			 0x84f73c82, /// 0x140
			 0x5d9b6ab7, /// 0x144
			 0x554524a2, /// 0x148
			 0xea3cc016, /// 0x14c
			 0xfa406226, /// 0x150
			 0xaffe96ff, /// 0x154
			 0xfc8a4ff3, /// 0x158
			 0xa5601961, /// 0x15c
			 0xe1f5399b, /// 0x160
			 0xc77c693b, /// 0x164
			 0xdb698405, /// 0x168
			 0xbccc9594, /// 0x16c
			 0x6459c00c, /// 0x170
			 0xaf76c2c0, /// 0x174
			 0x22f9a111, /// 0x178
			 0x95dd53a1, /// 0x17c
			 0xc18e2abb, /// 0x180
			 0x3e7a50bf, /// 0x184
			 0x8bfd5444, /// 0x188
			 0x394fb150, /// 0x18c
			 0xa0d09023, /// 0x190
			 0x804f4323, /// 0x194
			 0x20747661, /// 0x198
			 0x18a2340a, /// 0x19c
			 0xe3ce5f5a, /// 0x1a0
			 0xc18a7aa7, /// 0x1a4
			 0x896881cf, /// 0x1a8
			 0xe51cef6e, /// 0x1ac
			 0xa0ad2e9c, /// 0x1b0
			 0xae0ef1f6, /// 0x1b4
			 0x9724bcc5, /// 0x1b8
			 0xd0b397aa, /// 0x1bc
			 0x4d6eb061, /// 0x1c0
			 0x994f9960, /// 0x1c4
			 0x4dc56357, /// 0x1c8
			 0x111c2638, /// 0x1cc
			 0xb5037c35, /// 0x1d0
			 0x6b9f70d1, /// 0x1d4
			 0x14a4aa4c, /// 0x1d8
			 0xa9ad3075, /// 0x1dc
			 0xa70f9698, /// 0x1e0
			 0xbd53f984, /// 0x1e4
			 0x6fe06a9c, /// 0x1e8
			 0x830c6676, /// 0x1ec
			 0x14ef403c, /// 0x1f0
			 0xcd277f82, /// 0x1f4
			 0x92f5a36f, /// 0x1f8
			 0xce7b46da, /// 0x1fc
			 0xad90324f, /// 0x200
			 0xbfb60183, /// 0x204
			 0x45c19548, /// 0x208
			 0x55c4b918, /// 0x20c
			 0x0d47474d, /// 0x210
			 0x53c15292, /// 0x214
			 0x9751842f, /// 0x218
			 0xc917bee0, /// 0x21c
			 0x20695cdf, /// 0x220
			 0xcb7bb5d8, /// 0x224
			 0x18adffab, /// 0x228
			 0x005ac608, /// 0x22c
			 0xd5a5a4c1, /// 0x230
			 0xaa934b29, /// 0x234
			 0xb1b75d67, /// 0x238
			 0x8e28eeed, /// 0x23c
			 0x4571d4f5, /// 0x240
			 0x0f460ea3, /// 0x244
			 0x2dfb336a, /// 0x248
			 0xdc918597, /// 0x24c
			 0x210b8691, /// 0x250
			 0xa5a9ca66, /// 0x254
			 0x8e24a680, /// 0x258
			 0x6d20b175, /// 0x25c
			 0xee8a69de, /// 0x260
			 0x43f960b9, /// 0x264
			 0x319f4a02, /// 0x268
			 0xb1d48e1a, /// 0x26c
			 0x65d01c9d, /// 0x270
			 0x96907402, /// 0x274
			 0x017d167e, /// 0x278
			 0x71b5ccf3, /// 0x27c
			 0xba5e3430, /// 0x280
			 0xbbb68873, /// 0x284
			 0x1fd35bba, /// 0x288
			 0x6a5c31e8, /// 0x28c
			 0xecdd183b, /// 0x290
			 0x7ee2f299, /// 0x294
			 0xd2d7b248, /// 0x298
			 0x495bf5c0, /// 0x29c
			 0x124ffe65, /// 0x2a0
			 0xafb50975, /// 0x2a4
			 0x9e5ec576, /// 0x2a8
			 0x25278ec6, /// 0x2ac
			 0x758bf42e, /// 0x2b0
			 0xa250f85a, /// 0x2b4
			 0xe79acf0f, /// 0x2b8
			 0x649e4143, /// 0x2bc
			 0xa4123efb, /// 0x2c0
			 0x2c6226c4, /// 0x2c4
			 0xb89953e5, /// 0x2c8
			 0xf2fd9e70, /// 0x2cc
			 0xc6d9921e, /// 0x2d0
			 0xbdc9d2ed, /// 0x2d4
			 0x7eca06ef, /// 0x2d8
			 0xd60de516, /// 0x2dc
			 0xae616e1c, /// 0x2e0
			 0x4588334c, /// 0x2e4
			 0x4cd4912a, /// 0x2e8
			 0x4dba9982, /// 0x2ec
			 0xf51977a9, /// 0x2f0
			 0x88c046c5, /// 0x2f4
			 0x19d1c254, /// 0x2f8
			 0x504d91dc, /// 0x2fc
			 0x7a9958d9, /// 0x300
			 0x96299a43, /// 0x304
			 0x45db4cbf, /// 0x308
			 0x86fb14e4, /// 0x30c
			 0x73052a80, /// 0x310
			 0xf41d8b25, /// 0x314
			 0xf63b1da7, /// 0x318
			 0xc57de0f8, /// 0x31c
			 0x97012e8c, /// 0x320
			 0xe842dc3f, /// 0x324
			 0x6339a9ca, /// 0x328
			 0x5f3c0214, /// 0x32c
			 0x1fa873b4, /// 0x330
			 0x1b32d9e2, /// 0x334
			 0x6920873b, /// 0x338
			 0x5b16f909, /// 0x33c
			 0x5cb7a824, /// 0x340
			 0x5596a8a0, /// 0x344
			 0x3796048b, /// 0x348
			 0x63934a58, /// 0x34c
			 0x1ef8c4b8, /// 0x350
			 0x9e9fd56c, /// 0x354
			 0xf19ab15a, /// 0x358
			 0x18c7f693, /// 0x35c
			 0x4906718d, /// 0x360
			 0x2b1d8aa5, /// 0x364
			 0xf1e07205, /// 0x368
			 0x47f88e3d, /// 0x36c
			 0x428197cd, /// 0x370
			 0x7b68b760, /// 0x374
			 0xc8c6120c, /// 0x378
			 0xb60d3727, /// 0x37c
			 0xcaec5b5f, /// 0x380
			 0x0bec2a8e, /// 0x384
			 0xbd6b778e, /// 0x388
			 0x765c8657, /// 0x38c
			 0x86a53b13, /// 0x390
			 0x9284ef0b, /// 0x394
			 0x1b01786e, /// 0x398
			 0x7333b1ef, /// 0x39c
			 0xc6ae808a, /// 0x3a0
			 0x4437d2c3, /// 0x3a4
			 0x16c77dee, /// 0x3a8
			 0x18279458, /// 0x3ac
			 0xfde6baed, /// 0x3b0
			 0x83158c44, /// 0x3b4
			 0xa64543f2, /// 0x3b8
			 0x2b99f7eb, /// 0x3bc
			 0xe1a1785b, /// 0x3c0
			 0xa5bda322, /// 0x3c4
			 0xdd0ec89d, /// 0x3c8
			 0x9d0a3abe, /// 0x3cc
			 0x2b002c78, /// 0x3d0
			 0x9b2566e4, /// 0x3d4
			 0xaefa9f51, /// 0x3d8
			 0x2f75c28d, /// 0x3dc
			 0x48f31261, /// 0x3e0
			 0x525f51b8, /// 0x3e4
			 0x919c766a, /// 0x3e8
			 0x3073f0ea, /// 0x3ec
			 0xea827d49, /// 0x3f0
			 0x10c63aa2, /// 0x3f4
			 0x0e6f12f3, /// 0x3f8
			 0x4ef72868, /// 0x3fc
			 0x9304ad99, /// 0x400
			 0x420bd577, /// 0x404
			 0x54fa67c2, /// 0x408
			 0x02e301c2, /// 0x40c
			 0xbfc559bc, /// 0x410
			 0xaaa39e87, /// 0x414
			 0x12d20daf, /// 0x418
			 0x8491f4ed, /// 0x41c
			 0x9afa5a08, /// 0x420
			 0x5ee94b82, /// 0x424
			 0xefb197ef, /// 0x428
			 0x3ab16752, /// 0x42c
			 0x82d80545, /// 0x430
			 0x8d312bea, /// 0x434
			 0x61fcb2a1, /// 0x438
			 0xb40ad46d, /// 0x43c
			 0xf57a8d92, /// 0x440
			 0x4b0a4177, /// 0x444
			 0x813ca14d, /// 0x448
			 0x8a6526bc, /// 0x44c
			 0x36387595, /// 0x450
			 0xc3148981, /// 0x454
			 0x722c70b4, /// 0x458
			 0xda66d45c, /// 0x45c
			 0x1e1bc60a, /// 0x460
			 0xae169e9e, /// 0x464
			 0xc51dd9d0, /// 0x468
			 0x4ce17047, /// 0x46c
			 0xb927f293, /// 0x470
			 0xcc2d20d3, /// 0x474
			 0xc92c677f, /// 0x478
			 0xb5016b00, /// 0x47c
			 0x59adf23a, /// 0x480
			 0x9178bedc, /// 0x484
			 0x1ccb0bc4, /// 0x488
			 0x651d98d4, /// 0x48c
			 0xa4a22a55, /// 0x490
			 0x06bb3108, /// 0x494
			 0x8cf2d342, /// 0x498
			 0x3232d4c7, /// 0x49c
			 0x918618fb, /// 0x4a0
			 0x2ab0c0fe, /// 0x4a4
			 0xe9dfc4e3, /// 0x4a8
			 0x0ca1c11c, /// 0x4ac
			 0x7f3b2046, /// 0x4b0
			 0xf749a209, /// 0x4b4
			 0x24cb5944, /// 0x4b8
			 0x905331ee, /// 0x4bc
			 0xc47c5269, /// 0x4c0
			 0x810b200b, /// 0x4c4
			 0xbf589730, /// 0x4c8
			 0x69ae48b6, /// 0x4cc
			 0xbf508763, /// 0x4d0
			 0x11013419, /// 0x4d4
			 0x37aa89d4, /// 0x4d8
			 0x181cceef, /// 0x4dc
			 0x5811a1f3, /// 0x4e0
			 0x7d7c8d85, /// 0x4e4
			 0x88fc3452, /// 0x4e8
			 0x41f32d20, /// 0x4ec
			 0xaa2821b2, /// 0x4f0
			 0x85de08f8, /// 0x4f4
			 0x524af746, /// 0x4f8
			 0x73e8b9fb, /// 0x4fc
			 0xc7e2489d, /// 0x500
			 0x5108b148, /// 0x504
			 0xe22e0c7f, /// 0x508
			 0xd1365976, /// 0x50c
			 0x96e3ede6, /// 0x510
			 0x6ebb7e32, /// 0x514
			 0x5b710fd2, /// 0x518
			 0x9b2057ba, /// 0x51c
			 0x204d435d, /// 0x520
			 0x74392363, /// 0x524
			 0x9d95ef24, /// 0x528
			 0x038784df, /// 0x52c
			 0x8575ee39, /// 0x530
			 0xc6987c0f, /// 0x534
			 0x9a55972f, /// 0x538
			 0x45255e65, /// 0x53c
			 0x05f61a37, /// 0x540
			 0x34d97c34, /// 0x544
			 0x52f986a1, /// 0x548
			 0x239a8013, /// 0x54c
			 0x97c647e4, /// 0x550
			 0x9864f27f, /// 0x554
			 0x8b946c92, /// 0x558
			 0xdedbe03c, /// 0x55c
			 0xebc7381d, /// 0x560
			 0x6fdffdec, /// 0x564
			 0x94144cc0, /// 0x568
			 0x7eb9c13b, /// 0x56c
			 0xc3eccbdb, /// 0x570
			 0x4ba9c5be, /// 0x574
			 0xf4acbe04, /// 0x578
			 0x979e4e86, /// 0x57c
			 0x94ec2e26, /// 0x580
			 0xa74bf44e, /// 0x584
			 0xce0b8b6e, /// 0x588
			 0xf090d7cd, /// 0x58c
			 0xc51733ac, /// 0x590
			 0x4b507c0b, /// 0x594
			 0x2c3ea010, /// 0x598
			 0x6c285ecb, /// 0x59c
			 0x574701bc, /// 0x5a0
			 0x50bf6b21, /// 0x5a4
			 0x1a7627a0, /// 0x5a8
			 0x832cfd3e, /// 0x5ac
			 0x0ebebfd4, /// 0x5b0
			 0x29a4eb6d, /// 0x5b4
			 0xaa411372, /// 0x5b8
			 0xa3cb9835, /// 0x5bc
			 0x890dc8b8, /// 0x5c0
			 0xe878bd1a, /// 0x5c4
			 0x7442575b, /// 0x5c8
			 0x75163f79, /// 0x5cc
			 0x53f72d29, /// 0x5d0
			 0xc4110882, /// 0x5d4
			 0xe3046555, /// 0x5d8
			 0x7dbb7f84, /// 0x5dc
			 0x7b8d1b1c, /// 0x5e0
			 0xca25ee82, /// 0x5e4
			 0xa9f86cb0, /// 0x5e8
			 0x2759d6aa, /// 0x5ec
			 0x26f72ffe, /// 0x5f0
			 0x4a69f7be, /// 0x5f4
			 0xf57ea27a, /// 0x5f8
			 0x12390ba0, /// 0x5fc
			 0xe3461b8e, /// 0x600
			 0x32680a97, /// 0x604
			 0x5fbddbc6, /// 0x608
			 0x1be44858, /// 0x60c
			 0xd10e03d2, /// 0x610
			 0x651b31b2, /// 0x614
			 0x6f663d82, /// 0x618
			 0xeb4cff94, /// 0x61c
			 0xdbc3fa48, /// 0x620
			 0xa7defcf0, /// 0x624
			 0xbd31227b, /// 0x628
			 0xa6fe081d, /// 0x62c
			 0x06f06fc3, /// 0x630
			 0xde8fd968, /// 0x634
			 0xee6dfa62, /// 0x638
			 0x0d5b50c2, /// 0x63c
			 0xf1a17af4, /// 0x640
			 0x38a103a6, /// 0x644
			 0xa112426f, /// 0x648
			 0xdfc4b1dc, /// 0x64c
			 0x822526de, /// 0x650
			 0x99c9ce8f, /// 0x654
			 0xf28e542a, /// 0x658
			 0xbc92ae9a, /// 0x65c
			 0x020c17cf, /// 0x660
			 0x87741a82, /// 0x664
			 0xd5eff279, /// 0x668
			 0x6aaa2ad6, /// 0x66c
			 0xff81fce1, /// 0x670
			 0x77bbe13c, /// 0x674
			 0x25c9cad5, /// 0x678
			 0x0e437d4e, /// 0x67c
			 0x09206f9d, /// 0x680
			 0xf025491a, /// 0x684
			 0x7249ba98, /// 0x688
			 0xbee0f21d, /// 0x68c
			 0x8ad15494, /// 0x690
			 0x69bfdb15, /// 0x694
			 0x34a8c029, /// 0x698
			 0x1c83add5, /// 0x69c
			 0x29378eda, /// 0x6a0
			 0x431005d0, /// 0x6a4
			 0x983d4139, /// 0x6a8
			 0x3b54e066, /// 0x6ac
			 0x9f8a9de6, /// 0x6b0
			 0x036d3c4f, /// 0x6b4
			 0x2f9f6ba3, /// 0x6b8
			 0xad819c15, /// 0x6bc
			 0x5914ae71, /// 0x6c0
			 0x308f14cd, /// 0x6c4
			 0xced1c5d5, /// 0x6c8
			 0x7293d71b, /// 0x6cc
			 0x877fca40, /// 0x6d0
			 0x9fc6d3b8, /// 0x6d4
			 0xf6b23f88, /// 0x6d8
			 0x1ca74a90, /// 0x6dc
			 0xeb681d99, /// 0x6e0
			 0x21e4661f, /// 0x6e4
			 0xbcd14d3b, /// 0x6e8
			 0x2cf0c1f6, /// 0x6ec
			 0xaa23e162, /// 0x6f0
			 0x019661a4, /// 0x6f4
			 0xfc615e31, /// 0x6f8
			 0x2d552b33, /// 0x6fc
			 0xbba970a8, /// 0x700
			 0x0b8cd32b, /// 0x704
			 0x5925c74e, /// 0x708
			 0x9e6515aa, /// 0x70c
			 0x328b38a1, /// 0x710
			 0x4437541c, /// 0x714
			 0xa07f03d5, /// 0x718
			 0xaa0b1a56, /// 0x71c
			 0x788ae5c9, /// 0x720
			 0xe2ff649a, /// 0x724
			 0x26433df2, /// 0x728
			 0x5617457a, /// 0x72c
			 0xebd666c9, /// 0x730
			 0xf45a8a61, /// 0x734
			 0x0629d93b, /// 0x738
			 0x7a760447, /// 0x73c
			 0x1a81d16f, /// 0x740
			 0xf3701e79, /// 0x744
			 0x28683359, /// 0x748
			 0x58f0ea93, /// 0x74c
			 0xe608817c, /// 0x750
			 0x36339688, /// 0x754
			 0x571a2500, /// 0x758
			 0xdb98281f, /// 0x75c
			 0x9c7cde6c, /// 0x760
			 0x81589c9d, /// 0x764
			 0x1a0126fd, /// 0x768
			 0x4b44d824, /// 0x76c
			 0x8024235d, /// 0x770
			 0x8b9952ce, /// 0x774
			 0xf3a93b83, /// 0x778
			 0x9de63e2b, /// 0x77c
			 0xef97719c, /// 0x780
			 0x718bfdea, /// 0x784
			 0xe3d3e634, /// 0x788
			 0x46b2981d, /// 0x78c
			 0x7b07730c, /// 0x790
			 0xb0a64f8e, /// 0x794
			 0xbe8abf2f, /// 0x798
			 0xe68dd2dd, /// 0x79c
			 0x85f810d0, /// 0x7a0
			 0xe9066d67, /// 0x7a4
			 0xd63700a3, /// 0x7a8
			 0x4c9b1cab, /// 0x7ac
			 0x49e9cd00, /// 0x7b0
			 0x8031c378, /// 0x7b4
			 0xc163aebf, /// 0x7b8
			 0x7359f448, /// 0x7bc
			 0x7f5fdd6a, /// 0x7c0
			 0x1cb82a3d, /// 0x7c4
			 0xbc66fc9b, /// 0x7c8
			 0x6790ed2d, /// 0x7cc
			 0x41d5ee68, /// 0x7d0
			 0x60ce30be, /// 0x7d4
			 0x152d5657, /// 0x7d8
			 0x0dcdf5f7, /// 0x7dc
			 0x6a9a87e6, /// 0x7e0
			 0xecdfb29e, /// 0x7e4
			 0x53d141c9, /// 0x7e8
			 0x81527baa, /// 0x7ec
			 0x8181c85d, /// 0x7f0
			 0xa08cf8b9, /// 0x7f4
			 0xd3a32629, /// 0x7f8
			 0x5dbb770f, /// 0x7fc
			 0xe6854a72, /// 0x800
			 0x313b885f, /// 0x804
			 0x49917ed7, /// 0x808
			 0x214108de, /// 0x80c
			 0x13d5ede3, /// 0x810
			 0xeb54e199, /// 0x814
			 0x4058282b, /// 0x818
			 0x2955783a, /// 0x81c
			 0x82de0897, /// 0x820
			 0x57f1548b, /// 0x824
			 0x99bf5d42, /// 0x828
			 0xf4d621d0, /// 0x82c
			 0xfd6ffc3a, /// 0x830
			 0xaa8e50f5, /// 0x834
			 0x7d4f636e, /// 0x838
			 0x70e4f730, /// 0x83c
			 0x3d4fe7fd, /// 0x840
			 0xf0ed2745, /// 0x844
			 0x5db376eb, /// 0x848
			 0x38fd23c6, /// 0x84c
			 0x0603b84d, /// 0x850
			 0x58f6c98f, /// 0x854
			 0x82a14755, /// 0x858
			 0xfb6bd35f, /// 0x85c
			 0x6cb374dc, /// 0x860
			 0xb4fc58f4, /// 0x864
			 0xfc650fed, /// 0x868
			 0x1c54525a, /// 0x86c
			 0xae5b591d, /// 0x870
			 0xb131c4b8, /// 0x874
			 0x9fa7a197, /// 0x878
			 0x1db98d69, /// 0x87c
			 0x55579f4d, /// 0x880
			 0x7c46e1da, /// 0x884
			 0xce734da8, /// 0x888
			 0xfae3d2f1, /// 0x88c
			 0xd8b13888, /// 0x890
			 0x7c5f465c, /// 0x894
			 0x8655f9f8, /// 0x898
			 0x4b06ef64, /// 0x89c
			 0x12e54ec0, /// 0x8a0
			 0xe84bd12e, /// 0x8a4
			 0x46a94919, /// 0x8a8
			 0x559a33c2, /// 0x8ac
			 0x6f8287bc, /// 0x8b0
			 0x53152079, /// 0x8b4
			 0xa3e36d6f, /// 0x8b8
			 0x179c074b, /// 0x8bc
			 0xc7a0cad6, /// 0x8c0
			 0x36c310af, /// 0x8c4
			 0xd0bdc563, /// 0x8c8
			 0xa26c6edf, /// 0x8cc
			 0x8caf8270, /// 0x8d0
			 0x74978182, /// 0x8d4
			 0xc58f6c88, /// 0x8d8
			 0xa4062789, /// 0x8dc
			 0x808e2eda, /// 0x8e0
			 0x98710f06, /// 0x8e4
			 0x681916be, /// 0x8e8
			 0x6e20799b, /// 0x8ec
			 0x733f86af, /// 0x8f0
			 0x8e06b802, /// 0x8f4
			 0x4df3dac5, /// 0x8f8
			 0xe9eb9232, /// 0x8fc
			 0xe0aa2aaf, /// 0x900
			 0x1410dafa, /// 0x904
			 0x756179a8, /// 0x908
			 0x8433ef2a, /// 0x90c
			 0x4c94dcca, /// 0x910
			 0xb32feb95, /// 0x914
			 0x65bc7064, /// 0x918
			 0x443d50fe, /// 0x91c
			 0x73e23d92, /// 0x920
			 0xfdcd2098, /// 0x924
			 0x2e66b40a, /// 0x928
			 0x05665da9, /// 0x92c
			 0x1f1b5004, /// 0x930
			 0x1a22f48f, /// 0x934
			 0xb13ad65e, /// 0x938
			 0x080298d2, /// 0x93c
			 0xd79d969e, /// 0x940
			 0x944665b6, /// 0x944
			 0xa8ad1620, /// 0x948
			 0x6296fcdc, /// 0x94c
			 0x72e354b4, /// 0x950
			 0x892ade18, /// 0x954
			 0xba6cbc69, /// 0x958
			 0xf8e67a3a, /// 0x95c
			 0x263cf716, /// 0x960
			 0xec988bf3, /// 0x964
			 0x39004fe4, /// 0x968
			 0x6d587076, /// 0x96c
			 0x1aa7df45, /// 0x970
			 0x724062b0, /// 0x974
			 0x33f71276, /// 0x978
			 0xf9418825, /// 0x97c
			 0x269afd33, /// 0x980
			 0xc9d306a9, /// 0x984
			 0x6d7c7fde, /// 0x988
			 0xd38dc35e, /// 0x98c
			 0x6835f08b, /// 0x990
			 0xd0fcde44, /// 0x994
			 0x5d25846f, /// 0x998
			 0x0647d514, /// 0x99c
			 0xbc7b4d88, /// 0x9a0
			 0xfe5a54cd, /// 0x9a4
			 0x9e2aee11, /// 0x9a8
			 0x1b1df0cc, /// 0x9ac
			 0x683b54ea, /// 0x9b0
			 0xf335564f, /// 0x9b4
			 0xeb583a87, /// 0x9b8
			 0xa7c4c3bb, /// 0x9bc
			 0x41a7987a, /// 0x9c0
			 0xc661e447, /// 0x9c4
			 0x276d554e, /// 0x9c8
			 0x18a9a00a, /// 0x9cc
			 0x0e83ee0d, /// 0x9d0
			 0x539b6a48, /// 0x9d4
			 0x86d460b8, /// 0x9d8
			 0x8844af74, /// 0x9dc
			 0x7560208a, /// 0x9e0
			 0xb4252897, /// 0x9e4
			 0x75b691c6, /// 0x9e8
			 0x0e49fdc1, /// 0x9ec
			 0x67edeece, /// 0x9f0
			 0xce5cffd9, /// 0x9f4
			 0x5d4baef7, /// 0x9f8
			 0xca4bfa6b, /// 0x9fc
			 0xdd433d16, /// 0xa00
			 0x97578edd, /// 0xa04
			 0xc7a69fc9, /// 0xa08
			 0x931047f7, /// 0xa0c
			 0x5f79ae11, /// 0xa10
			 0xf1547698, /// 0xa14
			 0x16eaf392, /// 0xa18
			 0x35924d03, /// 0xa1c
			 0x3b62bcb2, /// 0xa20
			 0x76e454f0, /// 0xa24
			 0x4e4b65f8, /// 0xa28
			 0x8b0e02d6, /// 0xa2c
			 0xcda87f82, /// 0xa30
			 0xc3e2f2bf, /// 0xa34
			 0x6bc529af, /// 0xa38
			 0x4a9ddbba, /// 0xa3c
			 0x3cd656ee, /// 0xa40
			 0xc551b25b, /// 0xa44
			 0xc9c4c6a4, /// 0xa48
			 0x8584754e, /// 0xa4c
			 0x911561c9, /// 0xa50
			 0x3280fb1c, /// 0xa54
			 0x3b9c0537, /// 0xa58
			 0x313fdffe, /// 0xa5c
			 0x87b0f15b, /// 0xa60
			 0xef4025c2, /// 0xa64
			 0xba20a890, /// 0xa68
			 0xa50ece51, /// 0xa6c
			 0x6842fa61, /// 0xa70
			 0x01af3080, /// 0xa74
			 0x0f6d6b3e, /// 0xa78
			 0xcd2f9e77, /// 0xa7c
			 0x561143e3, /// 0xa80
			 0x1713db64, /// 0xa84
			 0xfc7c06d8, /// 0xa88
			 0xbfaae30b, /// 0xa8c
			 0x6d376488, /// 0xa90
			 0x6b900cd4, /// 0xa94
			 0x41906384, /// 0xa98
			 0xd5e88976, /// 0xa9c
			 0x526dc221, /// 0xaa0
			 0x2cb09f5a, /// 0xaa4
			 0xbfe495d4, /// 0xaa8
			 0x91c55fb2, /// 0xaac
			 0x30906b34, /// 0xab0
			 0xd3cfe681, /// 0xab4
			 0x753e08c3, /// 0xab8
			 0x74311c1b, /// 0xabc
			 0x2cc936cb, /// 0xac0
			 0x83e435bc, /// 0xac4
			 0xbbef5db1, /// 0xac8
			 0x8d37316d, /// 0xacc
			 0xd9d84387, /// 0xad0
			 0xcdde9a5c, /// 0xad4
			 0x03f4d83d, /// 0xad8
			 0x0da4461d, /// 0xadc
			 0x295f323e, /// 0xae0
			 0x4b60c83d, /// 0xae4
			 0x08ea55a1, /// 0xae8
			 0xdb5d823d, /// 0xaec
			 0x8b7e2d47, /// 0xaf0
			 0xb038b173, /// 0xaf4
			 0x62894b33, /// 0xaf8
			 0x96ba9411, /// 0xafc
			 0x8915a84e, /// 0xb00
			 0xbf1d9674, /// 0xb04
			 0xdbbbfcb2, /// 0xb08
			 0x3693e786, /// 0xb0c
			 0x33ed068b, /// 0xb10
			 0x32146b29, /// 0xb14
			 0x972e72cc, /// 0xb18
			 0x7b316885, /// 0xb1c
			 0xb98c9b5d, /// 0xb20
			 0xcd84eb79, /// 0xb24
			 0xabd30e3f, /// 0xb28
			 0x8d1ac208, /// 0xb2c
			 0x131194ef, /// 0xb30
			 0xba2f1550, /// 0xb34
			 0xab80a4f2, /// 0xb38
			 0x4f5612a5, /// 0xb3c
			 0xec7370ba, /// 0xb40
			 0xfedb0122, /// 0xb44
			 0x627e9cbf, /// 0xb48
			 0x0a08628a, /// 0xb4c
			 0x24e7f7a2, /// 0xb50
			 0x2636e16c, /// 0xb54
			 0x9387f37f, /// 0xb58
			 0x87866248, /// 0xb5c
			 0x4d4c3e09, /// 0xb60
			 0xe6179dcb, /// 0xb64
			 0x1a51a40c, /// 0xb68
			 0x76710aca, /// 0xb6c
			 0xee374959, /// 0xb70
			 0xaf529886, /// 0xb74
			 0x9e4eb4f8, /// 0xb78
			 0x159ff2ba, /// 0xb7c
			 0x73a03557, /// 0xb80
			 0xec382d27, /// 0xb84
			 0x5ce16111, /// 0xb88
			 0xb0b68705, /// 0xb8c
			 0x110e4da1, /// 0xb90
			 0xae81eab2, /// 0xb94
			 0x208f75f2, /// 0xb98
			 0x542ef4f1, /// 0xb9c
			 0xa985b2b5, /// 0xba0
			 0x7450b5d5, /// 0xba4
			 0xd7015ba1, /// 0xba8
			 0x4c5e560a, /// 0xbac
			 0x460561ab, /// 0xbb0
			 0xcb13d46f, /// 0xbb4
			 0x12859980, /// 0xbb8
			 0xd7034d0c, /// 0xbbc
			 0x6104a35f, /// 0xbc0
			 0xa6e0b333, /// 0xbc4
			 0x70d6ad07, /// 0xbc8
			 0x9a948ef8, /// 0xbcc
			 0xc734d836, /// 0xbd0
			 0x73a191ec, /// 0xbd4
			 0x830738e9, /// 0xbd8
			 0xa3868b19, /// 0xbdc
			 0x60f4caac, /// 0xbe0
			 0x4ed78b36, /// 0xbe4
			 0x7fdc464c, /// 0xbe8
			 0xc645ae5c, /// 0xbec
			 0xb15bedcf, /// 0xbf0
			 0xe03ab63e, /// 0xbf4
			 0xd0217052, /// 0xbf8
			 0xff04b784, /// 0xbfc
			 0xbfec9823, /// 0xc00
			 0x07710049, /// 0xc04
			 0xa5982cc3, /// 0xc08
			 0x90443cf4, /// 0xc0c
			 0x75607a9c, /// 0xc10
			 0xa60c7d1f, /// 0xc14
			 0x5daca751, /// 0xc18
			 0x0ba69fc0, /// 0xc1c
			 0x22262a7a, /// 0xc20
			 0x854a6a60, /// 0xc24
			 0x4d0a4b6a, /// 0xc28
			 0x74e8864b, /// 0xc2c
			 0x0ab95f10, /// 0xc30
			 0xf5c06f1a, /// 0xc34
			 0xaa791ec3, /// 0xc38
			 0xe9a9fa0d, /// 0xc3c
			 0x164ad644, /// 0xc40
			 0x8e05ce62, /// 0xc44
			 0x4bb5b19c, /// 0xc48
			 0x2c8f13db, /// 0xc4c
			 0xf555d7ce, /// 0xc50
			 0x94fb6fea, /// 0xc54
			 0x17fee892, /// 0xc58
			 0x45395222, /// 0xc5c
			 0x9488bf15, /// 0xc60
			 0x56fcc53a, /// 0xc64
			 0x62560df8, /// 0xc68
			 0x94d5e762, /// 0xc6c
			 0xc3f9c0be, /// 0xc70
			 0x2f0edc1e, /// 0xc74
			 0x20a43d71, /// 0xc78
			 0xdc13c9e4, /// 0xc7c
			 0xca82beb8, /// 0xc80
			 0x0620711b, /// 0xc84
			 0xc9575c51, /// 0xc88
			 0x09ca62b6, /// 0xc8c
			 0x97254796, /// 0xc90
			 0x844825e0, /// 0xc94
			 0x43ccf3c9, /// 0xc98
			 0x232c7763, /// 0xc9c
			 0xc8cc3608, /// 0xca0
			 0xc92d579a, /// 0xca4
			 0xc16845fd, /// 0xca8
			 0x9b44fbd4, /// 0xcac
			 0x3c1eeb28, /// 0xcb0
			 0x6251fce6, /// 0xcb4
			 0x2b0bf084, /// 0xcb8
			 0xe8b1c24e, /// 0xcbc
			 0xbbc985a0, /// 0xcc0
			 0xc25cdbce, /// 0xcc4
			 0xe734bcf7, /// 0xcc8
			 0x1f754172, /// 0xccc
			 0x4c0fb0ee, /// 0xcd0
			 0x1c33f680, /// 0xcd4
			 0x02e76a4f, /// 0xcd8
			 0x53e827fb, /// 0xcdc
			 0xcdd8ce3f, /// 0xce0
			 0x0565132c, /// 0xce4
			 0x754d3259, /// 0xce8
			 0x27008864, /// 0xcec
			 0x13ecc8ca, /// 0xcf0
			 0xbdbc74f0, /// 0xcf4
			 0xe158e9cf, /// 0xcf8
			 0x151074e9, /// 0xcfc
			 0x2c929d51, /// 0xd00
			 0xbad55b87, /// 0xd04
			 0x12629d83, /// 0xd08
			 0xfa524ee8, /// 0xd0c
			 0x0e7ed03e, /// 0xd10
			 0x71d90feb, /// 0xd14
			 0xc53f80b5, /// 0xd18
			 0xa6c88c92, /// 0xd1c
			 0xa6929796, /// 0xd20
			 0x6996ca9f, /// 0xd24
			 0xbc0aae67, /// 0xd28
			 0xa2e1879a, /// 0xd2c
			 0x0de8ca69, /// 0xd30
			 0xcebee766, /// 0xd34
			 0x9efedc68, /// 0xd38
			 0x20fcb7a5, /// 0xd3c
			 0xa082ccb9, /// 0xd40
			 0xe96d9436, /// 0xd44
			 0x25469e18, /// 0xd48
			 0xd5a4b56a, /// 0xd4c
			 0xf87bfb65, /// 0xd50
			 0x5058cee1, /// 0xd54
			 0x5ceb33de, /// 0xd58
			 0x207c8e30, /// 0xd5c
			 0x382454c4, /// 0xd60
			 0x03561af6, /// 0xd64
			 0x800b3cce, /// 0xd68
			 0x7bea07ff, /// 0xd6c
			 0x0077b75d, /// 0xd70
			 0xf1a9e26c, /// 0xd74
			 0x9fdf7010, /// 0xd78
			 0xd64ddf64, /// 0xd7c
			 0xb6770397, /// 0xd80
			 0x44f584f3, /// 0xd84
			 0x82c22aff, /// 0xd88
			 0x66e3acb7, /// 0xd8c
			 0xa225971a, /// 0xd90
			 0xf28aae7e, /// 0xd94
			 0x7b561397, /// 0xd98
			 0x93c49051, /// 0xd9c
			 0x9f6715cc, /// 0xda0
			 0xc897eac9, /// 0xda4
			 0xe880f7b7, /// 0xda8
			 0x572485d4, /// 0xdac
			 0x20182246, /// 0xdb0
			 0xce846533, /// 0xdb4
			 0xe788989f, /// 0xdb8
			 0xe46a5b89, /// 0xdbc
			 0x8b29d890, /// 0xdc0
			 0xa31b9720, /// 0xdc4
			 0xf49b3cc5, /// 0xdc8
			 0xe2d277c9, /// 0xdcc
			 0x470ac541, /// 0xdd0
			 0xd4213818, /// 0xdd4
			 0xff3cf256, /// 0xdd8
			 0x42c87449, /// 0xddc
			 0xda241f64, /// 0xde0
			 0xd455abff, /// 0xde4
			 0xc8102ff5, /// 0xde8
			 0x61a985f6, /// 0xdec
			 0xf4f56479, /// 0xdf0
			 0xe90b6c61, /// 0xdf4
			 0x325f0e66, /// 0xdf8
			 0xc50a5dcf, /// 0xdfc
			 0x649493f8, /// 0xe00
			 0x1f131e4d, /// 0xe04
			 0x81a2a29e, /// 0xe08
			 0x61c74c09, /// 0xe0c
			 0xf9d3744a, /// 0xe10
			 0x14ca56f6, /// 0xe14
			 0x69a95d60, /// 0xe18
			 0x6ab60e85, /// 0xe1c
			 0x2b942279, /// 0xe20
			 0x31cbf197, /// 0xe24
			 0xe8447b29, /// 0xe28
			 0xdb41cf2a, /// 0xe2c
			 0x8d9612b8, /// 0xe30
			 0x216d0e34, /// 0xe34
			 0x3a02af3f, /// 0xe38
			 0x3ffac836, /// 0xe3c
			 0xf746b598, /// 0xe40
			 0x6c73bee4, /// 0xe44
			 0x64a4c0f0, /// 0xe48
			 0x09174f95, /// 0xe4c
			 0x01adcce4, /// 0xe50
			 0x2283db8c, /// 0xe54
			 0xf51b8fad, /// 0xe58
			 0xba70fbb3, /// 0xe5c
			 0xbd9fdaa5, /// 0xe60
			 0xd559d72a, /// 0xe64
			 0xcd278fcc, /// 0xe68
			 0xc899531a, /// 0xe6c
			 0xdc886b6c, /// 0xe70
			 0xb2893d58, /// 0xe74
			 0xe34ca740, /// 0xe78
			 0xdc2557af, /// 0xe7c
			 0x42e7d1ff, /// 0xe80
			 0x63ed85a9, /// 0xe84
			 0x6a8f94b9, /// 0xe88
			 0xf7108c77, /// 0xe8c
			 0x861082c0, /// 0xe90
			 0xd6d7c186, /// 0xe94
			 0xbdadf1fb, /// 0xe98
			 0xbe96ec77, /// 0xe9c
			 0x12e719d2, /// 0xea0
			 0xcdf558b7, /// 0xea4
			 0x636d6de1, /// 0xea8
			 0xe31e0ae9, /// 0xeac
			 0x4991a974, /// 0xeb0
			 0xdf17975d, /// 0xeb4
			 0x1d45710f, /// 0xeb8
			 0xf532f370, /// 0xebc
			 0x6990c0e3, /// 0xec0
			 0xad65136f, /// 0xec4
			 0x4afdc101, /// 0xec8
			 0xb86ded4b, /// 0xecc
			 0x6bcaeacf, /// 0xed0
			 0x31645cba, /// 0xed4
			 0x6be05262, /// 0xed8
			 0x45d5c648, /// 0xedc
			 0x85593cf6, /// 0xee0
			 0xdd84ea69, /// 0xee4
			 0x1bf6ad07, /// 0xee8
			 0x17486984, /// 0xeec
			 0x63a35b0e, /// 0xef0
			 0x62cf25a8, /// 0xef4
			 0x99238f73, /// 0xef8
			 0x502d3959, /// 0xefc
			 0x18fe41ea, /// 0xf00
			 0x167aca42, /// 0xf04
			 0x42d9b344, /// 0xf08
			 0xc4f947c8, /// 0xf0c
			 0xb61b149d, /// 0xf10
			 0xfcbac259, /// 0xf14
			 0xe104d823, /// 0xf18
			 0x57fdcb23, /// 0xf1c
			 0x850d8ae5, /// 0xf20
			 0x86418f68, /// 0xf24
			 0x4b299097, /// 0xf28
			 0xd2896680, /// 0xf2c
			 0x2ab82cf0, /// 0xf30
			 0xeb6b82b8, /// 0xf34
			 0x243d42bc, /// 0xf38
			 0x655d70c4, /// 0xf3c
			 0xf6379470, /// 0xf40
			 0x46570c9a, /// 0xf44
			 0xcf6c1166, /// 0xf48
			 0x8cb05691, /// 0xf4c
			 0x7edf9bb4, /// 0xf50
			 0x20c6904a, /// 0xf54
			 0xd81278c7, /// 0xf58
			 0x2ec8edc2, /// 0xf5c
			 0x2488d32e, /// 0xf60
			 0x2520c96a, /// 0xf64
			 0x31497f66, /// 0xf68
			 0x15038a19, /// 0xf6c
			 0xf5929453, /// 0xf70
			 0x440bcd16, /// 0xf74
			 0x070685e8, /// 0xf78
			 0x349da778, /// 0xf7c
			 0x8d4bf111, /// 0xf80
			 0xf53cfc8e, /// 0xf84
			 0x2ea5fa7c, /// 0xf88
			 0x558cee7c, /// 0xf8c
			 0x553bb353, /// 0xf90
			 0x8133acac, /// 0xf94
			 0x7da3a127, /// 0xf98
			 0x6ea94928, /// 0xf9c
			 0x8c148cb3, /// 0xfa0
			 0x603edc74, /// 0xfa4
			 0xd81b527e, /// 0xfa8
			 0xa65818f4, /// 0xfac
			 0x842ba984, /// 0xfb0
			 0xb2c8a7bd, /// 0xfb4
			 0x6366155e, /// 0xfb8
			 0xaaccfe7c, /// 0xfbc
			 0xac6bf2a2, /// 0xfc0
			 0xf3fcf3eb, /// 0xfc4
			 0xe879a2dd, /// 0xfc8
			 0xa3866c30, /// 0xfcc
			 0x51c9d285, /// 0xfd0
			 0x94e50e2e, /// 0xfd4
			 0x05712c94, /// 0xfd8
			 0x5de4be2b, /// 0xfdc
			 0x707a1010, /// 0xfe0
			 0xe933fd6b, /// 0xfe4
			 0x316c1f78, /// 0xfe8
			 0x291f60d0, /// 0xfec
			 0x71a41555, /// 0xff0
			 0x1a88b5c2, /// 0xff4
			 0x07d9f682, /// 0xff8
			 0x339cb5bb /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00004
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00010
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00014
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0001c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x0c400000,                                                  // Leading 1s:                                 /// 00024
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0002c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00030
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0003c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00040
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00044
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00048
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00050
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00054
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00058
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0005c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00060
			 0x0c600000,                                                  // Leading 1s:                                 /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0006c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00074
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00078
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0007c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00080
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00084
			 0xff800000,                                                  // -inf                                        /// 00088
			 0x7fc00001,                                                  // signaling NaN                               /// 0008c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00098
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f8
			 0xffc00001,                                                  // -signaling NaN                              /// 000fc
			 0x00000000,                                                  // zero                                        /// 00100
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00108
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00110
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00118
			 0x0c780000,                                                  // Leading 1s:                                 /// 0011c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00124
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00128
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0012c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00130
			 0x3f028f5c,                                                  // 0.51                                        /// 00134
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x33333333,                                                  // 4 random values                             /// 0013c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00150
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xcb000000,                                                  // -8388608.0                                  /// 00158
			 0x55555555,                                                  // 4 random values                             /// 0015c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00160
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00168
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0016c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00170
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00178
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0017c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80000000,                                                  // -zero                                       /// 00184
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00188
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00190
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00194
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00198
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b0
			 0x80000000,                                                  // -zero                                       /// 001b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x7fc00001,                                                  // signaling NaN                               /// 001ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00200
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00204
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00208
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x4b000000,                                                  // 8388608.0                                   /// 00210
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00214
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0021c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00228
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0022c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00230
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00234
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00238
			 0x80011111,                                                  // -9.7958E-41                                 /// 0023c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00240
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0024c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00250
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00254
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x33333333,                                                  // 4 random values                             /// 00260
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00264
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00268
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00274
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0027c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00280
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00284
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00288
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0028c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00290
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00294
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00298
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00304
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00308
			 0x0c780000,                                                  // Leading 1s:                                 /// 0030c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00314
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00338
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00340
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xffc00001,                                                  // -signaling NaN                              /// 00348
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00350
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00354
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00358
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0035c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00364
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00368
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0036c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00374
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00378
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xcb000000,                                                  // -8388608.0                                  /// 00384
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00388
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0039c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x7f800000,                                                  // inf                                         /// 003a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00400
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0040c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00410
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00414
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00418
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00420
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00424
			 0x0e000001,                                                  // Trailing 1s:                                /// 00428
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0042c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0c780000,                                                  // Leading 1s:                                 /// 00438
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0043c
			 0x7fc00001,                                                  // signaling NaN                               /// 00440
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00448
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00450
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00458
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00464
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0046c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00474
			 0x0c600000,                                                  // Leading 1s:                                 /// 00478
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00488
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0048c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00490
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00494
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004a4
			 0x3f028f5c,                                                  // 0.51                                        /// 004a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00500
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00508
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00510
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00518
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00520
			 0x0e000007,                                                  // Trailing 1s:                                /// 00524
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00530
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00540
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00544
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00548
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00554
			 0x4b000000,                                                  // 8388608.0                                   /// 00558
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0055c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00560
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00564
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00568
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000000,                                                  // -zero                                       /// 00570
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00574
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00578
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0057c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0058c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x3f028f5c,                                                  // 0.51                                        /// 00594
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00598
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0059c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00600
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00608
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0060c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00610
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00614
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00620
			 0x7f800000,                                                  // inf                                         /// 00624
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00628
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0062c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00634
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00638
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00654
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0xbf028f5c,                                                  // -0.51                                       /// 0065c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00660
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00668
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0066c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00670
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00678
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00680
			 0x7f800000,                                                  // inf                                         /// 00684
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00688
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0068c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00690
			 0x00011111,                                                  // 9.7958E-41                                  /// 00694
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006e4
			 0x33333333,                                                  // 4 random values                             /// 006e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00704
			 0x0c400000,                                                  // Leading 1s:                                 /// 00708
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0070c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00710
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00714
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00718
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00724
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00728
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0072c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00730
			 0x80011111,                                                  // -9.7958E-41                                 /// 00734
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00738
			 0x3f028f5c,                                                  // 0.51                                        /// 0073c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00740
			 0xbf028f5c,                                                  // -0.51                                       /// 00744
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00758
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0075c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00760
			 0x0c700000,                                                  // Leading 1s:                                 /// 00764
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00768
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0076c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00770
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0077c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00784
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00788
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0078c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0079c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00804
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00808
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0081c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00820
			 0x3f028f5c,                                                  // 0.51                                        /// 00824
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0082c
			 0xcb000000,                                                  // -8388608.0                                  /// 00830
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00834
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0083c
			 0xff800000,                                                  // -inf                                        /// 00840
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00844
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00848
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00850
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00858
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00860
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00864
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00868
			 0x0c400000,                                                  // Leading 1s:                                 /// 0086c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00870
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00874
			 0x0e000003,                                                  // Trailing 1s:                                /// 00878
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0087c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c400000,                                                  // Leading 1s:                                 /// 00888
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00890
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00894
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0089c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00900
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00904
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00908
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0090c
			 0x3f028f5c,                                                  // 0.51                                        /// 00910
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00918
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00924
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00928
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00930
			 0x00011111,                                                  // 9.7958E-41                                  /// 00934
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00938
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00940
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00948
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00950
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00954
			 0x0e000001,                                                  // Trailing 1s:                                /// 00958
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0095c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00964
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00970
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00974
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0097c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00980
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00984
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0xbf028f5c,                                                  // -0.51                                       /// 00990
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00994
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0c700000,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x33333333,                                                  // 4 random values                             /// 009e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a98
			 0x55555555,                                                  // 4 random values                             /// 00a9c
			 0x7f800000,                                                  // inf                                         /// 00aa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00abc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00acc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00adc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af8
			 0x4b000000,                                                  // 8388608.0                                   /// 00afc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b44
			 0x4b000000,                                                  // 8388608.0                                   /// 00b48
			 0x3f028f5c,                                                  // 0.51                                        /// 00b4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba0
			 0x55555555,                                                  // 4 random values                             /// 00ba4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bcc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00be0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x55555555,                                                  // 4 random values                             /// 00bec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c58
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x55555555,                                                  // 4 random values                             /// 00c64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c7c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ca4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ca8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd8
			 0x00000000,                                                  // zero                                        /// 00cdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ce0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ce8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cf4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d08
			 0x7fc00001,                                                  // signaling NaN                               /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d68
			 0x7f800000,                                                  // inf                                         /// 00d6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ddc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00df8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e28
			 0xbf028f5c,                                                  // -0.51                                       /// 00e2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80000000,                                                  // -zero                                       /// 00e50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ebc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ec4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ecc
			 0xff800000,                                                  // -inf                                        /// 00ed0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ef0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00efc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f10
			 0x55555555,                                                  // 4 random values                             /// 00f14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x7f800000,                                                  // inf                                         /// 00f70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fa4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fcc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff8
			 0x0e3fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x94283ea6, /// 0x0
			 0xc0476da8, /// 0x4
			 0x618893ce, /// 0x8
			 0x7f506c9d, /// 0xc
			 0xca2b378f, /// 0x10
			 0xd1a613c5, /// 0x14
			 0x626b5337, /// 0x18
			 0xd5ab1e4e, /// 0x1c
			 0xec2d05e3, /// 0x20
			 0x569d64a6, /// 0x24
			 0xc9698e11, /// 0x28
			 0x2365af3c, /// 0x2c
			 0x36b7d848, /// 0x30
			 0x037acc74, /// 0x34
			 0x3e696976, /// 0x38
			 0x42bab071, /// 0x3c
			 0x4cd82be1, /// 0x40
			 0x57db89dd, /// 0x44
			 0x330f66aa, /// 0x48
			 0x9cb6cd7f, /// 0x4c
			 0xd098e65e, /// 0x50
			 0xd45331c7, /// 0x54
			 0x048cfafa, /// 0x58
			 0x406c835b, /// 0x5c
			 0xe74b5e61, /// 0x60
			 0xb85a5280, /// 0x64
			 0x77b67dc3, /// 0x68
			 0x9aad6d4e, /// 0x6c
			 0x5aa4077f, /// 0x70
			 0x44c8e85d, /// 0x74
			 0x31052a2d, /// 0x78
			 0xd44fe03d, /// 0x7c
			 0xd81b5ebc, /// 0x80
			 0x0808830b, /// 0x84
			 0xa10f2864, /// 0x88
			 0xaa9d000e, /// 0x8c
			 0xf96d27f4, /// 0x90
			 0xc7ba4e04, /// 0x94
			 0x94d4427b, /// 0x98
			 0x9e162b79, /// 0x9c
			 0xc4c7de38, /// 0xa0
			 0xc363078c, /// 0xa4
			 0x77034659, /// 0xa8
			 0x68c572a7, /// 0xac
			 0x97646ed2, /// 0xb0
			 0xe7077f5d, /// 0xb4
			 0x1ccd56e0, /// 0xb8
			 0x4e02edb3, /// 0xbc
			 0xd6796595, /// 0xc0
			 0x72c6e2a8, /// 0xc4
			 0xf2bb9b80, /// 0xc8
			 0xb474ccb0, /// 0xcc
			 0x05b9fae0, /// 0xd0
			 0xa1715c85, /// 0xd4
			 0x6cb487c7, /// 0xd8
			 0xf9bee1c2, /// 0xdc
			 0x736cebb7, /// 0xe0
			 0x9c6167ac, /// 0xe4
			 0x52864546, /// 0xe8
			 0xa5215fed, /// 0xec
			 0xf3d57f59, /// 0xf0
			 0xcc8b2b1f, /// 0xf4
			 0x8af1f693, /// 0xf8
			 0xf3c5d81e, /// 0xfc
			 0x88fa076d, /// 0x100
			 0xdf4f3dae, /// 0x104
			 0xd790755f, /// 0x108
			 0x8e9323cb, /// 0x10c
			 0xdf6e9f02, /// 0x110
			 0x4b17efd5, /// 0x114
			 0xee2ad409, /// 0x118
			 0x7bf1350e, /// 0x11c
			 0xdb32a4ee, /// 0x120
			 0xd21f1604, /// 0x124
			 0xfae4b2b7, /// 0x128
			 0xc1d540f2, /// 0x12c
			 0x5c42abbe, /// 0x130
			 0x28f51302, /// 0x134
			 0xd5034ecc, /// 0x138
			 0xab6496db, /// 0x13c
			 0x6ff3b84d, /// 0x140
			 0x7664bc1b, /// 0x144
			 0xa6a18da0, /// 0x148
			 0x484c95ca, /// 0x14c
			 0x6ba737ac, /// 0x150
			 0x6bfdf346, /// 0x154
			 0x9c37ba61, /// 0x158
			 0xc1497e09, /// 0x15c
			 0xffdc7aa8, /// 0x160
			 0x04842bd7, /// 0x164
			 0x907a7851, /// 0x168
			 0x0f7ec340, /// 0x16c
			 0x172fe73f, /// 0x170
			 0x01d810bc, /// 0x174
			 0x562c70d6, /// 0x178
			 0x07d191f2, /// 0x17c
			 0x275ee9bc, /// 0x180
			 0x526fbbac, /// 0x184
			 0xd9de6024, /// 0x188
			 0xe60c8e16, /// 0x18c
			 0xa89a1ebc, /// 0x190
			 0xcf76fccb, /// 0x194
			 0xcfddf1a0, /// 0x198
			 0x7e7922bf, /// 0x19c
			 0x24f72e89, /// 0x1a0
			 0x3255d5b2, /// 0x1a4
			 0x047b54fa, /// 0x1a8
			 0x66177d3e, /// 0x1ac
			 0x1b836e96, /// 0x1b0
			 0x51ab5ea6, /// 0x1b4
			 0x14592a01, /// 0x1b8
			 0x2e5a78a7, /// 0x1bc
			 0xcba3ee92, /// 0x1c0
			 0xff2dc4ce, /// 0x1c4
			 0x962c50e8, /// 0x1c8
			 0xa730ca54, /// 0x1cc
			 0x8db30a17, /// 0x1d0
			 0x8564d11e, /// 0x1d4
			 0x8e2266f9, /// 0x1d8
			 0x71f2ac52, /// 0x1dc
			 0x5d610213, /// 0x1e0
			 0x70faec2c, /// 0x1e4
			 0xcaba1238, /// 0x1e8
			 0xa92f31fc, /// 0x1ec
			 0x652bfe05, /// 0x1f0
			 0x2c2e53e3, /// 0x1f4
			 0xe455f102, /// 0x1f8
			 0xd0de58d8, /// 0x1fc
			 0xd5f0273f, /// 0x200
			 0x3e559e1e, /// 0x204
			 0xf3d253be, /// 0x208
			 0xcf31cee3, /// 0x20c
			 0xb11b20bb, /// 0x210
			 0x7b88c00f, /// 0x214
			 0x854599c9, /// 0x218
			 0xf0985c6e, /// 0x21c
			 0x56f19044, /// 0x220
			 0x59c5a9d2, /// 0x224
			 0xd77eb6bb, /// 0x228
			 0xcdc3a945, /// 0x22c
			 0x7b99a383, /// 0x230
			 0x5494eec0, /// 0x234
			 0xb0c1cfd9, /// 0x238
			 0x33de0f81, /// 0x23c
			 0xc0b0e11c, /// 0x240
			 0x23208fc1, /// 0x244
			 0xf00bcfe9, /// 0x248
			 0xd862e148, /// 0x24c
			 0x1987106b, /// 0x250
			 0x52f0b63f, /// 0x254
			 0x79535b8e, /// 0x258
			 0x414c0cf2, /// 0x25c
			 0x702219e3, /// 0x260
			 0x824c9f43, /// 0x264
			 0xff3e623d, /// 0x268
			 0x2a4f6c6e, /// 0x26c
			 0x1d33f277, /// 0x270
			 0x8b80b50c, /// 0x274
			 0x175015ac, /// 0x278
			 0x6b3ac9ff, /// 0x27c
			 0x6befc3af, /// 0x280
			 0x0c6217ad, /// 0x284
			 0x2ccaffe7, /// 0x288
			 0xa6611660, /// 0x28c
			 0x1eb02cc8, /// 0x290
			 0xd021430e, /// 0x294
			 0x27017e1b, /// 0x298
			 0x75d80179, /// 0x29c
			 0x008c96fa, /// 0x2a0
			 0xcde0fee5, /// 0x2a4
			 0x1314cbcd, /// 0x2a8
			 0x6f75789f, /// 0x2ac
			 0xde4ce845, /// 0x2b0
			 0x89407eea, /// 0x2b4
			 0x9e283072, /// 0x2b8
			 0x85424372, /// 0x2bc
			 0xf099f08c, /// 0x2c0
			 0x43d77f13, /// 0x2c4
			 0x8c6b015b, /// 0x2c8
			 0x8234b794, /// 0x2cc
			 0xbf97831f, /// 0x2d0
			 0xd6de921b, /// 0x2d4
			 0x6f33e0a1, /// 0x2d8
			 0x8862ffaf, /// 0x2dc
			 0x2f8b246e, /// 0x2e0
			 0x6470bd15, /// 0x2e4
			 0xd31149b9, /// 0x2e8
			 0x22509860, /// 0x2ec
			 0x8113607c, /// 0x2f0
			 0xf9ff71dc, /// 0x2f4
			 0x366614c1, /// 0x2f8
			 0x61b6d280, /// 0x2fc
			 0x3c7a2e80, /// 0x300
			 0x5da8def6, /// 0x304
			 0xb3712d8e, /// 0x308
			 0x35aba0ac, /// 0x30c
			 0xfb1fe02e, /// 0x310
			 0x660370c9, /// 0x314
			 0x5d52dcf0, /// 0x318
			 0x74a4d016, /// 0x31c
			 0xcbd3aa16, /// 0x320
			 0x6a821b26, /// 0x324
			 0x5a17c059, /// 0x328
			 0x5e61c723, /// 0x32c
			 0xbdbcd561, /// 0x330
			 0xcc57a79e, /// 0x334
			 0x98aa8fa3, /// 0x338
			 0x08ed3c86, /// 0x33c
			 0x822b0389, /// 0x340
			 0x44f57004, /// 0x344
			 0x4c758500, /// 0x348
			 0xfd97d7fa, /// 0x34c
			 0x72d90cf3, /// 0x350
			 0x8c65da28, /// 0x354
			 0x26281082, /// 0x358
			 0xb5997f1e, /// 0x35c
			 0xd45c63cf, /// 0x360
			 0x94342f46, /// 0x364
			 0xa079fa7f, /// 0x368
			 0x303c25c2, /// 0x36c
			 0x73652414, /// 0x370
			 0x4c3650ec, /// 0x374
			 0xb5e5efd5, /// 0x378
			 0xa27b6427, /// 0x37c
			 0x25be092f, /// 0x380
			 0x0a92b2bf, /// 0x384
			 0x70d0ed87, /// 0x388
			 0xf952b08a, /// 0x38c
			 0x945db349, /// 0x390
			 0x63270fe5, /// 0x394
			 0xf5fbae56, /// 0x398
			 0x91c67cf7, /// 0x39c
			 0xaa4c44c2, /// 0x3a0
			 0x0430531a, /// 0x3a4
			 0x1f57b073, /// 0x3a8
			 0x4aed608d, /// 0x3ac
			 0x4fcf41d2, /// 0x3b0
			 0x2bdf39c6, /// 0x3b4
			 0x8d0d5fb8, /// 0x3b8
			 0x4eeaac6d, /// 0x3bc
			 0xba95de7e, /// 0x3c0
			 0xb6122868, /// 0x3c4
			 0x99992776, /// 0x3c8
			 0xc32a6177, /// 0x3cc
			 0xe837645b, /// 0x3d0
			 0x3797e946, /// 0x3d4
			 0x346033bf, /// 0x3d8
			 0x498b163e, /// 0x3dc
			 0xbe461da2, /// 0x3e0
			 0x67f946f0, /// 0x3e4
			 0xf5088cab, /// 0x3e8
			 0x7cc7b1ce, /// 0x3ec
			 0x8ef2528f, /// 0x3f0
			 0x323375a9, /// 0x3f4
			 0x4e342efc, /// 0x3f8
			 0x57b163fb, /// 0x3fc
			 0xd110d2c5, /// 0x400
			 0x8414e587, /// 0x404
			 0xa0a62ad5, /// 0x408
			 0x696d4fee, /// 0x40c
			 0xc08eb121, /// 0x410
			 0x716f6a44, /// 0x414
			 0x35518325, /// 0x418
			 0x34126bdc, /// 0x41c
			 0xa9816e62, /// 0x420
			 0x29e8db8d, /// 0x424
			 0x03bbdcc8, /// 0x428
			 0x0ec5c18e, /// 0x42c
			 0xd0d256ee, /// 0x430
			 0xcc8bcde1, /// 0x434
			 0x0e848131, /// 0x438
			 0x93d890d5, /// 0x43c
			 0x05385c0a, /// 0x440
			 0x96d55b1a, /// 0x444
			 0xf20651fb, /// 0x448
			 0x16d2a9e8, /// 0x44c
			 0xd46c0346, /// 0x450
			 0xd19cc8fb, /// 0x454
			 0xbf5baf29, /// 0x458
			 0x79c653c1, /// 0x45c
			 0x3b03f62a, /// 0x460
			 0x9f980552, /// 0x464
			 0xb7ff0721, /// 0x468
			 0xd886b23e, /// 0x46c
			 0x308fa922, /// 0x470
			 0x8c7c63db, /// 0x474
			 0x1a785c60, /// 0x478
			 0x7cd4cc42, /// 0x47c
			 0x8e1967e7, /// 0x480
			 0xcb1c9f59, /// 0x484
			 0x97d2416c, /// 0x488
			 0xd8f89612, /// 0x48c
			 0x3c4bed99, /// 0x490
			 0x2faeb677, /// 0x494
			 0x49c405d8, /// 0x498
			 0x4866f6e0, /// 0x49c
			 0x6753cbf0, /// 0x4a0
			 0x21d5f786, /// 0x4a4
			 0xfce91153, /// 0x4a8
			 0x6f89a8ac, /// 0x4ac
			 0xdf219db5, /// 0x4b0
			 0x37dacc8c, /// 0x4b4
			 0x4decb988, /// 0x4b8
			 0x6b954ede, /// 0x4bc
			 0xaf34c0ed, /// 0x4c0
			 0x49fd5405, /// 0x4c4
			 0x187b8d48, /// 0x4c8
			 0xf19ad0a6, /// 0x4cc
			 0xe4755db4, /// 0x4d0
			 0x8eb2fd2f, /// 0x4d4
			 0xd8f88768, /// 0x4d8
			 0xa4890f62, /// 0x4dc
			 0xacaae4d9, /// 0x4e0
			 0xb967d196, /// 0x4e4
			 0x7bb04489, /// 0x4e8
			 0xd2f5e1f0, /// 0x4ec
			 0xb8aea173, /// 0x4f0
			 0x501089ca, /// 0x4f4
			 0xdf5011e5, /// 0x4f8
			 0xa44b3da4, /// 0x4fc
			 0x2e28df89, /// 0x500
			 0xb8e9ce85, /// 0x504
			 0x8647b63d, /// 0x508
			 0x1fa58c40, /// 0x50c
			 0x35605d86, /// 0x510
			 0xc3199437, /// 0x514
			 0x7e7c8a99, /// 0x518
			 0x021c01c9, /// 0x51c
			 0xf34c1ea7, /// 0x520
			 0x3fdf9ad5, /// 0x524
			 0x994dfaab, /// 0x528
			 0x1c4b94ce, /// 0x52c
			 0xe828c6ca, /// 0x530
			 0x3cd91db7, /// 0x534
			 0x7cbf7970, /// 0x538
			 0xf2ca1917, /// 0x53c
			 0x4fcb6030, /// 0x540
			 0x03e4da35, /// 0x544
			 0x8d1b79d6, /// 0x548
			 0xeedc80a6, /// 0x54c
			 0xa9f07cdd, /// 0x550
			 0x245d44dd, /// 0x554
			 0xe20bab00, /// 0x558
			 0x767f27f5, /// 0x55c
			 0x90e2133a, /// 0x560
			 0x3f61b411, /// 0x564
			 0xec4afcb4, /// 0x568
			 0xdacc7f94, /// 0x56c
			 0x14b4a45b, /// 0x570
			 0x4452d844, /// 0x574
			 0xf63851af, /// 0x578
			 0x2d5f38b5, /// 0x57c
			 0x2ef40a7b, /// 0x580
			 0x53202a15, /// 0x584
			 0x1cacc844, /// 0x588
			 0x62d16dc7, /// 0x58c
			 0xc80d4dc6, /// 0x590
			 0x673aacbc, /// 0x594
			 0xd43bb19e, /// 0x598
			 0x6cfd6b30, /// 0x59c
			 0x1ef76ac6, /// 0x5a0
			 0x70dbb89e, /// 0x5a4
			 0xcd9d2b01, /// 0x5a8
			 0xa3853327, /// 0x5ac
			 0x722ff2c4, /// 0x5b0
			 0xeb723920, /// 0x5b4
			 0x73f570de, /// 0x5b8
			 0x3634540e, /// 0x5bc
			 0x49d5145f, /// 0x5c0
			 0xe02c186f, /// 0x5c4
			 0xa56d18d7, /// 0x5c8
			 0x56a7f11c, /// 0x5cc
			 0x64de2caf, /// 0x5d0
			 0xee9c2335, /// 0x5d4
			 0x73fb9a15, /// 0x5d8
			 0xf54a1171, /// 0x5dc
			 0x7745ab84, /// 0x5e0
			 0x7b92026e, /// 0x5e4
			 0xe2eda1b6, /// 0x5e8
			 0xebbaeeb4, /// 0x5ec
			 0x37575ea4, /// 0x5f0
			 0xaf9190aa, /// 0x5f4
			 0xab8e969d, /// 0x5f8
			 0x6097caa5, /// 0x5fc
			 0x0352b1ff, /// 0x600
			 0x0ba4c3b6, /// 0x604
			 0x7dde16c2, /// 0x608
			 0x25afbe71, /// 0x60c
			 0xe1b2cc42, /// 0x610
			 0x5a3aa876, /// 0x614
			 0x3c179ba5, /// 0x618
			 0x6b7e3238, /// 0x61c
			 0x5418d003, /// 0x620
			 0x5d9ccf20, /// 0x624
			 0x0ee84f32, /// 0x628
			 0xa671c6ce, /// 0x62c
			 0xb128722e, /// 0x630
			 0x76f7c08c, /// 0x634
			 0xf395bf96, /// 0x638
			 0x8221a72e, /// 0x63c
			 0x89bb6c85, /// 0x640
			 0xc3f19047, /// 0x644
			 0x9952aa13, /// 0x648
			 0x55286224, /// 0x64c
			 0x869602fa, /// 0x650
			 0x5722592b, /// 0x654
			 0xeeb6fd24, /// 0x658
			 0xc47192a8, /// 0x65c
			 0x7f62f3de, /// 0x660
			 0xa59ac351, /// 0x664
			 0x6179594c, /// 0x668
			 0x0631c182, /// 0x66c
			 0x682975ee, /// 0x670
			 0x7c8e0b20, /// 0x674
			 0xf7bd5dd4, /// 0x678
			 0xb6120700, /// 0x67c
			 0x6339876f, /// 0x680
			 0x45b41f13, /// 0x684
			 0xb32f404e, /// 0x688
			 0xbe88c0bb, /// 0x68c
			 0xdc9a5969, /// 0x690
			 0xa3756899, /// 0x694
			 0x64e0a5a5, /// 0x698
			 0xe0e2f352, /// 0x69c
			 0xf6d79afe, /// 0x6a0
			 0x082fc3e7, /// 0x6a4
			 0x4da204db, /// 0x6a8
			 0x308821f5, /// 0x6ac
			 0x5f21bc6f, /// 0x6b0
			 0xcb3fef09, /// 0x6b4
			 0x00c007a0, /// 0x6b8
			 0xc76c3aa9, /// 0x6bc
			 0xa7ab6791, /// 0x6c0
			 0xc8aeb0f8, /// 0x6c4
			 0x3175230a, /// 0x6c8
			 0x8132b929, /// 0x6cc
			 0x1251ccab, /// 0x6d0
			 0x6a19340a, /// 0x6d4
			 0x3bc8b335, /// 0x6d8
			 0x0bf2d66a, /// 0x6dc
			 0xf406e2a1, /// 0x6e0
			 0xf44e7421, /// 0x6e4
			 0xcbae12f7, /// 0x6e8
			 0x421f8a4a, /// 0x6ec
			 0xf3995e91, /// 0x6f0
			 0xa2eaf002, /// 0x6f4
			 0x20bef68e, /// 0x6f8
			 0xac8d0fb6, /// 0x6fc
			 0x1ed32d12, /// 0x700
			 0x074b4787, /// 0x704
			 0xb6c3ecc8, /// 0x708
			 0xd689de0c, /// 0x70c
			 0x1f3a9845, /// 0x710
			 0x72c2bae9, /// 0x714
			 0x156e139f, /// 0x718
			 0x510f6b35, /// 0x71c
			 0x79412eaa, /// 0x720
			 0x76dfe7e3, /// 0x724
			 0x7ae6998c, /// 0x728
			 0xccdf0967, /// 0x72c
			 0x6ffe20e8, /// 0x730
			 0x5ebaed3a, /// 0x734
			 0x7dba3baf, /// 0x738
			 0xf17f0d18, /// 0x73c
			 0x4571fbfa, /// 0x740
			 0x02a3c568, /// 0x744
			 0x5cfc0b3a, /// 0x748
			 0x39d7b2dd, /// 0x74c
			 0xa954d342, /// 0x750
			 0x075815b4, /// 0x754
			 0xa401431e, /// 0x758
			 0x646678ec, /// 0x75c
			 0x1d1b82a5, /// 0x760
			 0xaaa5bbc9, /// 0x764
			 0xa829fde6, /// 0x768
			 0xa51e189e, /// 0x76c
			 0x7f174a9c, /// 0x770
			 0x720d1b2b, /// 0x774
			 0x43fd0932, /// 0x778
			 0xd4ec5694, /// 0x77c
			 0x1cd1a061, /// 0x780
			 0x695ee454, /// 0x784
			 0x66706f5d, /// 0x788
			 0xb706cc14, /// 0x78c
			 0x5401e919, /// 0x790
			 0x1a8ae630, /// 0x794
			 0x94940ae7, /// 0x798
			 0xa24511a6, /// 0x79c
			 0xf76bd842, /// 0x7a0
			 0x25deb87c, /// 0x7a4
			 0xae37633d, /// 0x7a8
			 0xf8a75bf5, /// 0x7ac
			 0xa12963c5, /// 0x7b0
			 0x799e5d0f, /// 0x7b4
			 0xbaa80fa4, /// 0x7b8
			 0x376bb554, /// 0x7bc
			 0xf9959745, /// 0x7c0
			 0x4f1258ac, /// 0x7c4
			 0x49d5221d, /// 0x7c8
			 0x2bbbd536, /// 0x7cc
			 0xe7b6ba0d, /// 0x7d0
			 0x7bc1c76a, /// 0x7d4
			 0xc0914ee9, /// 0x7d8
			 0x2c9dc56a, /// 0x7dc
			 0xaeefec58, /// 0x7e0
			 0x87525b75, /// 0x7e4
			 0xf8b45a06, /// 0x7e8
			 0xc5f1eb50, /// 0x7ec
			 0x7869d317, /// 0x7f0
			 0x43fe3f4c, /// 0x7f4
			 0x840b401b, /// 0x7f8
			 0x9ed48b2d, /// 0x7fc
			 0x99452dae, /// 0x800
			 0x31e57b0d, /// 0x804
			 0xc317a280, /// 0x808
			 0x4a0729df, /// 0x80c
			 0x2139b5ab, /// 0x810
			 0xf82b6a17, /// 0x814
			 0x555b9007, /// 0x818
			 0x515214c3, /// 0x81c
			 0xdde147e4, /// 0x820
			 0x28974bca, /// 0x824
			 0x7d99660a, /// 0x828
			 0xa762fb23, /// 0x82c
			 0x41468dfc, /// 0x830
			 0x6b7bdc36, /// 0x834
			 0x5c710429, /// 0x838
			 0xf23b1733, /// 0x83c
			 0x133e2ab5, /// 0x840
			 0xe0fc9f2b, /// 0x844
			 0x2a8d4f60, /// 0x848
			 0x9f44b7e4, /// 0x84c
			 0x1adb815f, /// 0x850
			 0x1adb4abc, /// 0x854
			 0xa13024e6, /// 0x858
			 0xa829adb9, /// 0x85c
			 0x9d25c5ea, /// 0x860
			 0x8790373f, /// 0x864
			 0xa8198080, /// 0x868
			 0x701d033b, /// 0x86c
			 0x5cc2276b, /// 0x870
			 0x9176380f, /// 0x874
			 0xac182b2b, /// 0x878
			 0x90765adb, /// 0x87c
			 0x105c7281, /// 0x880
			 0xab9b80e6, /// 0x884
			 0x34f30781, /// 0x888
			 0xf8682fa6, /// 0x88c
			 0xa5b62d17, /// 0x890
			 0x8bdfd383, /// 0x894
			 0x0133ca68, /// 0x898
			 0xcb2dfa2c, /// 0x89c
			 0x3188bcd8, /// 0x8a0
			 0xfc46174a, /// 0x8a4
			 0x47d38792, /// 0x8a8
			 0x4a494162, /// 0x8ac
			 0x3e35819d, /// 0x8b0
			 0xb94e27f2, /// 0x8b4
			 0xf7cb84bf, /// 0x8b8
			 0x51487448, /// 0x8bc
			 0x83ebb806, /// 0x8c0
			 0x2a7c7ad9, /// 0x8c4
			 0x6076cd23, /// 0x8c8
			 0x036d114c, /// 0x8cc
			 0xf7ceb76f, /// 0x8d0
			 0x3f1b1e5d, /// 0x8d4
			 0xf3e85894, /// 0x8d8
			 0x29a820b7, /// 0x8dc
			 0xe9748aac, /// 0x8e0
			 0x67d4e9db, /// 0x8e4
			 0x596d334d, /// 0x8e8
			 0x613724c9, /// 0x8ec
			 0xc390294a, /// 0x8f0
			 0x6e714d79, /// 0x8f4
			 0xd9b2e5bd, /// 0x8f8
			 0xd5f701d3, /// 0x8fc
			 0x2a6a19f9, /// 0x900
			 0x3566e405, /// 0x904
			 0x53fef5bc, /// 0x908
			 0x3006d1b3, /// 0x90c
			 0x74ba9b3c, /// 0x910
			 0x893cad44, /// 0x914
			 0x88b7eb8a, /// 0x918
			 0x57b1c150, /// 0x91c
			 0x17cd7b45, /// 0x920
			 0x2c1bb31c, /// 0x924
			 0xd7b23380, /// 0x928
			 0x0fe084d2, /// 0x92c
			 0xe210a37b, /// 0x930
			 0xecc90b68, /// 0x934
			 0xfe4ef12b, /// 0x938
			 0xb1c2619c, /// 0x93c
			 0x43e0444b, /// 0x940
			 0x03f862ad, /// 0x944
			 0xa4064095, /// 0x948
			 0xe1c5e80a, /// 0x94c
			 0x4f086c60, /// 0x950
			 0xb4543329, /// 0x954
			 0x6ba3b6b1, /// 0x958
			 0xc7fa43ef, /// 0x95c
			 0x5d443215, /// 0x960
			 0xc52be72a, /// 0x964
			 0x7dae2217, /// 0x968
			 0x2b549ac2, /// 0x96c
			 0xad6f9b46, /// 0x970
			 0x936b2b77, /// 0x974
			 0x64a0afe9, /// 0x978
			 0xe497da99, /// 0x97c
			 0xbdfa2489, /// 0x980
			 0x206e716f, /// 0x984
			 0xa9f2ecb2, /// 0x988
			 0xc5814c01, /// 0x98c
			 0xab0baaf5, /// 0x990
			 0xbce1543f, /// 0x994
			 0x847f1416, /// 0x998
			 0x64f94a53, /// 0x99c
			 0x87ff3e8c, /// 0x9a0
			 0x19ba1c04, /// 0x9a4
			 0x296f9f98, /// 0x9a8
			 0x08b71a72, /// 0x9ac
			 0x6c5883b5, /// 0x9b0
			 0xa76c45e3, /// 0x9b4
			 0x2b0611a4, /// 0x9b8
			 0x538796b6, /// 0x9bc
			 0xf26b4597, /// 0x9c0
			 0x780304f7, /// 0x9c4
			 0x3d746a3b, /// 0x9c8
			 0x0343fa2f, /// 0x9cc
			 0xa8db6b15, /// 0x9d0
			 0xe4f5c004, /// 0x9d4
			 0x539a38fa, /// 0x9d8
			 0xd4939343, /// 0x9dc
			 0x3be61e42, /// 0x9e0
			 0x8471536a, /// 0x9e4
			 0x122ff48b, /// 0x9e8
			 0x6dc2dc9c, /// 0x9ec
			 0x56b0bbe4, /// 0x9f0
			 0xc36c0465, /// 0x9f4
			 0x40d40d9a, /// 0x9f8
			 0xd6149b87, /// 0x9fc
			 0xc396662e, /// 0xa00
			 0xe1041af8, /// 0xa04
			 0x99032750, /// 0xa08
			 0x226e50c9, /// 0xa0c
			 0xf0c18dd5, /// 0xa10
			 0x70f987fa, /// 0xa14
			 0x3f475c06, /// 0xa18
			 0x53fa6a26, /// 0xa1c
			 0x66d0ab43, /// 0xa20
			 0x47e29530, /// 0xa24
			 0x514be5eb, /// 0xa28
			 0xb8a6718b, /// 0xa2c
			 0xf473a70e, /// 0xa30
			 0x9c9ff601, /// 0xa34
			 0x6b8075fd, /// 0xa38
			 0x6abca305, /// 0xa3c
			 0x0033c2fa, /// 0xa40
			 0xe9c6cac6, /// 0xa44
			 0xfe77e9cb, /// 0xa48
			 0x20ee4898, /// 0xa4c
			 0x437b8b92, /// 0xa50
			 0xc94b676d, /// 0xa54
			 0xbf939c0d, /// 0xa58
			 0xef04f47a, /// 0xa5c
			 0xc1434e6e, /// 0xa60
			 0x1fad56bb, /// 0xa64
			 0x761d466c, /// 0xa68
			 0xfbc67429, /// 0xa6c
			 0x4fb45764, /// 0xa70
			 0x1f0079d9, /// 0xa74
			 0xe0d8ce14, /// 0xa78
			 0xf103daaf, /// 0xa7c
			 0x2d18b007, /// 0xa80
			 0x40bad076, /// 0xa84
			 0x238db3e9, /// 0xa88
			 0x44d22999, /// 0xa8c
			 0xdaa18c56, /// 0xa90
			 0x8916e6eb, /// 0xa94
			 0x8b491134, /// 0xa98
			 0x4a94c6e4, /// 0xa9c
			 0x6a6b2116, /// 0xaa0
			 0x55ac9540, /// 0xaa4
			 0xbff1e335, /// 0xaa8
			 0x5b0d6d84, /// 0xaac
			 0x84816a62, /// 0xab0
			 0x2b907a98, /// 0xab4
			 0x6cff744d, /// 0xab8
			 0x2c52c489, /// 0xabc
			 0xb204be31, /// 0xac0
			 0x23b46e57, /// 0xac4
			 0x6e65d260, /// 0xac8
			 0x5cba7a52, /// 0xacc
			 0x7ee09118, /// 0xad0
			 0xd1bf149e, /// 0xad4
			 0xe5798f29, /// 0xad8
			 0x6cadc3cc, /// 0xadc
			 0xb779bee5, /// 0xae0
			 0xe3ce31e0, /// 0xae4
			 0xd3591940, /// 0xae8
			 0x3059d52a, /// 0xaec
			 0x0c103629, /// 0xaf0
			 0x3135dcef, /// 0xaf4
			 0x0e8144a0, /// 0xaf8
			 0x253b1279, /// 0xafc
			 0x33deca93, /// 0xb00
			 0xd3cb7df3, /// 0xb04
			 0x2de05816, /// 0xb08
			 0x09ee9686, /// 0xb0c
			 0x53bae597, /// 0xb10
			 0xf2982e76, /// 0xb14
			 0x011c3047, /// 0xb18
			 0x60a12b87, /// 0xb1c
			 0xfec60eda, /// 0xb20
			 0xf0a9127e, /// 0xb24
			 0xeb595a49, /// 0xb28
			 0x0e25ff6d, /// 0xb2c
			 0x4b7cff22, /// 0xb30
			 0xb139d32d, /// 0xb34
			 0xc41d8053, /// 0xb38
			 0xa3f1c2a2, /// 0xb3c
			 0x08c258b4, /// 0xb40
			 0xa0b24383, /// 0xb44
			 0xcc15da8e, /// 0xb48
			 0x0132379b, /// 0xb4c
			 0x93f7fff5, /// 0xb50
			 0xd86044dd, /// 0xb54
			 0xdeedf454, /// 0xb58
			 0x20408fb8, /// 0xb5c
			 0xfc749a01, /// 0xb60
			 0xcf6ccde2, /// 0xb64
			 0xa55cce48, /// 0xb68
			 0x67f22313, /// 0xb6c
			 0x67c4df2d, /// 0xb70
			 0xeeed96e6, /// 0xb74
			 0x9f7878ec, /// 0xb78
			 0x15592b71, /// 0xb7c
			 0xc19c081f, /// 0xb80
			 0xbe191749, /// 0xb84
			 0x8fbdb163, /// 0xb88
			 0x335f05d0, /// 0xb8c
			 0xd929eea2, /// 0xb90
			 0x37793067, /// 0xb94
			 0xfc277055, /// 0xb98
			 0xf88a1e32, /// 0xb9c
			 0xac3f366c, /// 0xba0
			 0xfe825ea5, /// 0xba4
			 0x7d12b323, /// 0xba8
			 0x90a9978c, /// 0xbac
			 0xf7a52016, /// 0xbb0
			 0x87c23ac3, /// 0xbb4
			 0x4a0220eb, /// 0xbb8
			 0x44cd3e13, /// 0xbbc
			 0x985305df, /// 0xbc0
			 0x1c5455dc, /// 0xbc4
			 0x5a077513, /// 0xbc8
			 0x83bd00f7, /// 0xbcc
			 0x3d9419ab, /// 0xbd0
			 0x19622821, /// 0xbd4
			 0x0f9a5c91, /// 0xbd8
			 0x918971f9, /// 0xbdc
			 0xbfcc2f72, /// 0xbe0
			 0xeca1475b, /// 0xbe4
			 0x6a5557ce, /// 0xbe8
			 0xd36bf8a1, /// 0xbec
			 0x61eca095, /// 0xbf0
			 0x3b93d594, /// 0xbf4
			 0x8721cf16, /// 0xbf8
			 0x957d380c, /// 0xbfc
			 0xa19dd796, /// 0xc00
			 0x13bedc3e, /// 0xc04
			 0x2623b335, /// 0xc08
			 0xbe6f20eb, /// 0xc0c
			 0xa3d3fa85, /// 0xc10
			 0xefcfa7c2, /// 0xc14
			 0x79a72bf0, /// 0xc18
			 0x999774a4, /// 0xc1c
			 0x5c9eca17, /// 0xc20
			 0x4d6736cc, /// 0xc24
			 0xc59a703d, /// 0xc28
			 0x2e802984, /// 0xc2c
			 0xe3e4601c, /// 0xc30
			 0x79204fce, /// 0xc34
			 0x102e8801, /// 0xc38
			 0x82fc4797, /// 0xc3c
			 0x2579a1a8, /// 0xc40
			 0xbe6dd258, /// 0xc44
			 0xc6fc5d5d, /// 0xc48
			 0x58897a7e, /// 0xc4c
			 0x49eeb0b6, /// 0xc50
			 0x93b727ef, /// 0xc54
			 0xff5ee12f, /// 0xc58
			 0xe1372681, /// 0xc5c
			 0xb1511598, /// 0xc60
			 0x35721f31, /// 0xc64
			 0x7da97fcf, /// 0xc68
			 0x55eac170, /// 0xc6c
			 0x59aa9329, /// 0xc70
			 0xabc5c7c6, /// 0xc74
			 0xa5cf29f9, /// 0xc78
			 0xe16932c5, /// 0xc7c
			 0x5f07a0a7, /// 0xc80
			 0xffc952d9, /// 0xc84
			 0xc8984c91, /// 0xc88
			 0xb327b665, /// 0xc8c
			 0x06a8865a, /// 0xc90
			 0x7eb255ed, /// 0xc94
			 0x14e5044d, /// 0xc98
			 0x7b978315, /// 0xc9c
			 0x01c4d0a3, /// 0xca0
			 0xa407989b, /// 0xca4
			 0x56f7c944, /// 0xca8
			 0x4fc0bed9, /// 0xcac
			 0x6a52e254, /// 0xcb0
			 0x35ea74b7, /// 0xcb4
			 0xe519fc08, /// 0xcb8
			 0x2fa49283, /// 0xcbc
			 0x67d755d6, /// 0xcc0
			 0xa8d5dbb0, /// 0xcc4
			 0xe26ddb72, /// 0xcc8
			 0x54366dec, /// 0xccc
			 0xe737d703, /// 0xcd0
			 0x135332c5, /// 0xcd4
			 0xe4ca632d, /// 0xcd8
			 0x0b679dc3, /// 0xcdc
			 0x562f4bd5, /// 0xce0
			 0x1e0b5580, /// 0xce4
			 0x70b2e586, /// 0xce8
			 0xa159efd1, /// 0xcec
			 0x84a50f7c, /// 0xcf0
			 0x6b8c646c, /// 0xcf4
			 0x67b98614, /// 0xcf8
			 0xb21b2526, /// 0xcfc
			 0x6c4cff6d, /// 0xd00
			 0x6ea885f7, /// 0xd04
			 0xebc9064a, /// 0xd08
			 0xe7ebed0a, /// 0xd0c
			 0x7ff48ab0, /// 0xd10
			 0x6edb9cc8, /// 0xd14
			 0x6b4de156, /// 0xd18
			 0xee849eb0, /// 0xd1c
			 0x66564bd0, /// 0xd20
			 0xcb6dccd7, /// 0xd24
			 0xa7730530, /// 0xd28
			 0xe6388019, /// 0xd2c
			 0x3ee30b7f, /// 0xd30
			 0x78521841, /// 0xd34
			 0x29320926, /// 0xd38
			 0x81edd7eb, /// 0xd3c
			 0x97780926, /// 0xd40
			 0xf87356af, /// 0xd44
			 0xe9cd8c4c, /// 0xd48
			 0x310effac, /// 0xd4c
			 0xa3946aec, /// 0xd50
			 0xb87cd948, /// 0xd54
			 0x51803b3b, /// 0xd58
			 0xf0edb457, /// 0xd5c
			 0x8acb8dda, /// 0xd60
			 0x7b5ca07c, /// 0xd64
			 0xa68804b3, /// 0xd68
			 0xe93aae22, /// 0xd6c
			 0xdcfaab86, /// 0xd70
			 0x1103be72, /// 0xd74
			 0x7d123640, /// 0xd78
			 0x702e6dad, /// 0xd7c
			 0xec245c30, /// 0xd80
			 0x3767f9d2, /// 0xd84
			 0x68c776cd, /// 0xd88
			 0x96e7d357, /// 0xd8c
			 0x6d15c77a, /// 0xd90
			 0xbc160bd1, /// 0xd94
			 0x0c7458a8, /// 0xd98
			 0xaeb07be0, /// 0xd9c
			 0xc8d3c124, /// 0xda0
			 0x90c969d1, /// 0xda4
			 0xb4c2c489, /// 0xda8
			 0x6863e6fa, /// 0xdac
			 0x91678e6d, /// 0xdb0
			 0x369ef2a2, /// 0xdb4
			 0x9cdb6f0e, /// 0xdb8
			 0x8bb007f2, /// 0xdbc
			 0xe079aace, /// 0xdc0
			 0xa0a3daeb, /// 0xdc4
			 0x5202c279, /// 0xdc8
			 0x9ed1b13a, /// 0xdcc
			 0x8867a177, /// 0xdd0
			 0x61d29199, /// 0xdd4
			 0xa4241c68, /// 0xdd8
			 0x475a7841, /// 0xddc
			 0x4def5273, /// 0xde0
			 0xfe6c3b23, /// 0xde4
			 0x3cdf6da8, /// 0xde8
			 0xe1d6a1d6, /// 0xdec
			 0x711fae10, /// 0xdf0
			 0xeab2c4ec, /// 0xdf4
			 0x09839c5f, /// 0xdf8
			 0xc21b4124, /// 0xdfc
			 0x0210c8c7, /// 0xe00
			 0xb96fa045, /// 0xe04
			 0x0ed67d39, /// 0xe08
			 0x016e5cf6, /// 0xe0c
			 0x3d818355, /// 0xe10
			 0x7ed3744e, /// 0xe14
			 0x43c4d561, /// 0xe18
			 0xbb7c8b7a, /// 0xe1c
			 0x21f76bf2, /// 0xe20
			 0xbadfe87b, /// 0xe24
			 0x2fd0dd90, /// 0xe28
			 0xcdc3a8d7, /// 0xe2c
			 0x2f76de24, /// 0xe30
			 0xb8c4d03c, /// 0xe34
			 0x351f8b3c, /// 0xe38
			 0xc86bb2e1, /// 0xe3c
			 0xafe3ec2b, /// 0xe40
			 0x9ad0c9a9, /// 0xe44
			 0x88683d6b, /// 0xe48
			 0x33136750, /// 0xe4c
			 0x0a91b23a, /// 0xe50
			 0x88d3382b, /// 0xe54
			 0x82861bba, /// 0xe58
			 0x2bfe56bb, /// 0xe5c
			 0xb81cd5cb, /// 0xe60
			 0xffcf8c91, /// 0xe64
			 0x5943601f, /// 0xe68
			 0x5f4f085c, /// 0xe6c
			 0x0bf448d7, /// 0xe70
			 0x88af37a1, /// 0xe74
			 0x85964148, /// 0xe78
			 0x9f831f3f, /// 0xe7c
			 0x671482ae, /// 0xe80
			 0x983a70db, /// 0xe84
			 0xc3607cdf, /// 0xe88
			 0x0c9a3f2f, /// 0xe8c
			 0xb4d06dba, /// 0xe90
			 0x11ab7a31, /// 0xe94
			 0xd6458ea3, /// 0xe98
			 0x36e26ba2, /// 0xe9c
			 0xb249beee, /// 0xea0
			 0x19cf49c3, /// 0xea4
			 0x7f991b9b, /// 0xea8
			 0x4c4c2e99, /// 0xeac
			 0xb3e58394, /// 0xeb0
			 0xda3611df, /// 0xeb4
			 0x49b2b469, /// 0xeb8
			 0x91dcdc28, /// 0xebc
			 0xf9cd0d05, /// 0xec0
			 0xd8a4767a, /// 0xec4
			 0x58509ad9, /// 0xec8
			 0x18c93338, /// 0xecc
			 0x5f80170f, /// 0xed0
			 0xf40008ad, /// 0xed4
			 0x1e48e3ff, /// 0xed8
			 0xc95104dc, /// 0xedc
			 0xe06e61c2, /// 0xee0
			 0x2bfc1cea, /// 0xee4
			 0xbf0d4394, /// 0xee8
			 0x4c49819f, /// 0xeec
			 0xab99ed2d, /// 0xef0
			 0x66988e7f, /// 0xef4
			 0x785775ef, /// 0xef8
			 0xd15593c9, /// 0xefc
			 0x38fc1d64, /// 0xf00
			 0xafc0f828, /// 0xf04
			 0xdee1e4b9, /// 0xf08
			 0xc04bebb6, /// 0xf0c
			 0x1e803b27, /// 0xf10
			 0xa3ac9422, /// 0xf14
			 0xe6929d69, /// 0xf18
			 0xa8762a03, /// 0xf1c
			 0x80ed9249, /// 0xf20
			 0xe08ee6d1, /// 0xf24
			 0xf18fd553, /// 0xf28
			 0x5572c63c, /// 0xf2c
			 0x240e4d53, /// 0xf30
			 0x65afbe51, /// 0xf34
			 0x6c09724b, /// 0xf38
			 0xdc24d8b1, /// 0xf3c
			 0x14461198, /// 0xf40
			 0xe25ae4d0, /// 0xf44
			 0x99ff5ae4, /// 0xf48
			 0x126b9810, /// 0xf4c
			 0xbe70ada6, /// 0xf50
			 0xee60f586, /// 0xf54
			 0xb332f2e8, /// 0xf58
			 0x753a97fe, /// 0xf5c
			 0x65585f26, /// 0xf60
			 0xf1a959de, /// 0xf64
			 0x07e6f8b5, /// 0xf68
			 0xeaec1efe, /// 0xf6c
			 0x8e63781d, /// 0xf70
			 0x145b00c0, /// 0xf74
			 0xf4b74d5a, /// 0xf78
			 0x949a84a3, /// 0xf7c
			 0xe1ee16e5, /// 0xf80
			 0x66cbf926, /// 0xf84
			 0x29ce708d, /// 0xf88
			 0xcd649904, /// 0xf8c
			 0x929cd00d, /// 0xf90
			 0xd9c7041b, /// 0xf94
			 0x72419b92, /// 0xf98
			 0xe4694f1d, /// 0xf9c
			 0x7f5491f4, /// 0xfa0
			 0xd24bbba1, /// 0xfa4
			 0xbdb8a855, /// 0xfa8
			 0xa8c8043f, /// 0xfac
			 0x1a8ef108, /// 0xfb0
			 0x1fd4ea55, /// 0xfb4
			 0xff3f8771, /// 0xfb8
			 0xb63607b6, /// 0xfbc
			 0x38656441, /// 0xfc0
			 0x59a5f1fd, /// 0xfc4
			 0x0647a8b9, /// 0xfc8
			 0x42d40811, /// 0xfcc
			 0x4075b460, /// 0xfd0
			 0x7583d566, /// 0xfd4
			 0xb35d8709, /// 0xfd8
			 0xf780a35e, /// 0xfdc
			 0x98e64271, /// 0xfe0
			 0x8cf0b002, /// 0xfe4
			 0x8653e595, /// 0xfe8
			 0x5739a959, /// 0xfec
			 0xb5649fc4, /// 0xff0
			 0x388b05e0, /// 0xff4
			 0x961a58b0, /// 0xff8
			 0x72615010 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00004
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00008
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0000c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00010
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00014
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0001c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00020
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00024
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00028
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0002c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00030
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00034
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x7fc00001,                                                  // signaling NaN                               /// 00040
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00044
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0004c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00054
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00060
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0c700000,                                                  // Leading 1s:                                 /// 0006c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00070
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00074
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00088
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0008c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00090
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00094
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00098
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0009c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0xcb000000,                                                  // -8388608.0                                  /// 000fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00108
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0010c
			 0x7fc00001,                                                  // signaling NaN                               /// 00110
			 0x33333333,                                                  // 4 random values                             /// 00114
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0011c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00120
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0c400000,                                                  // Leading 1s:                                 /// 00128
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0012c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00130
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00138
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0013c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00140
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00144
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00148
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0014c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00158
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0015c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00160
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00168
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00174
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0017c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00180
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00188
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0018c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00190
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00194
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001dc
			 0x4b000000,                                                  // 8388608.0                                   /// 001e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00200
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00208
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0020c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00210
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00214
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00218
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0021c
			 0x55555555,                                                  // 4 random values                             /// 00220
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00228
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0xcb000000,                                                  // -8388608.0                                  /// 00240
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00244
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0024c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00258
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0025c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00264
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00268
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0026c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00270
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00274
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00278
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0027c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00280
			 0xffc00001,                                                  // -signaling NaN                              /// 00284
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00288
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00294
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0029c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b0
			 0xcb000000,                                                  // -8388608.0                                  /// 002b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00300
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00308
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0030c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00314
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00320
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00328
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e000007,                                                  // Trailing 1s:                                /// 00334
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00338
			 0x4b000000,                                                  // 8388608.0                                   /// 0033c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00344
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00348
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00358
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0035c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00360
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00368
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00374
			 0x33333333,                                                  // 4 random values                             /// 00378
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00380
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0038c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00390
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00394
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00398
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0039c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003fc
			 0x55555555,                                                  // 4 random values                             /// 00400
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00404
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00408
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0040c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00410
			 0x0c600000,                                                  // Leading 1s:                                 /// 00414
			 0x7fc00001,                                                  // signaling NaN                               /// 00418
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00420
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00424
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00428
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xffc00001,                                                  // -signaling NaN                              /// 00430
			 0x0c780000,                                                  // Leading 1s:                                 /// 00434
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00444
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00448
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00450
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00458
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0045c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00460
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00464
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00468
			 0x7fc00001,                                                  // signaling NaN                               /// 0046c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00474
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00478
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0047c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00480
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00488
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0c600000,                                                  // Leading 1s:                                 /// 00494
			 0x0c600000,                                                  // Leading 1s:                                 /// 00498
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0049c
			 0x7f800000,                                                  // inf                                         /// 004a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0xbf028f5c,                                                  // -0.51                                       /// 004b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00508
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0050c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00514
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0c600000,                                                  // Leading 1s:                                 /// 00524
			 0x80000000,                                                  // -zero                                       /// 00528
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00530
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00538
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00544
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00548
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0054c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00550
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00554
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00558
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00560
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00564
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00568
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00570
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0057c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00580
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00584
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00588
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0058c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00590
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00598
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0059c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005ec
			 0xff800000,                                                  // -inf                                        /// 005f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00600
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00604
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00610
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00618
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00620
			 0x80000000,                                                  // -zero                                       /// 00624
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00628
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00634
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00638
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0063c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00640
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00644
			 0x3f028f5c,                                                  // 0.51                                        /// 00648
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00664
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00670
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0067c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00688
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0068c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006dc
			 0x55555555,                                                  // 4 random values                             /// 006e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006ec
			 0x33333333,                                                  // 4 random values                             /// 006f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00700
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00704
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0070c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00710
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00714
			 0x4b000000,                                                  // 8388608.0                                   /// 00718
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00730
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00734
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00738
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x4b000000,                                                  // 8388608.0                                   /// 00744
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00748
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00750
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00754
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00760
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0e000007,                                                  // Trailing 1s:                                /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x7fc00001,                                                  // signaling NaN                               /// 00770
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00774
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00778
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00784
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00788
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0078c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00790
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00794
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00798
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0079c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007b0
			 0xcb000000,                                                  // -8388608.0                                  /// 007b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xff800000,                                                  // -inf                                        /// 007c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007e0
			 0x7fc00001,                                                  // signaling NaN                               /// 007e4
			 0x33333333,                                                  // 4 random values                             /// 007e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00804
			 0x33333333,                                                  // 4 random values                             /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00818
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00824
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00828
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x55555555,                                                  // 4 random values                             /// 00834
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xffc00001,                                                  // -signaling NaN                              /// 00848
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0084c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00850
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00854
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0085c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00860
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00864
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00868
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0086c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00870
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00874
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00878
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0087c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00000000,                                                  // zero                                        /// 00888
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00890
			 0x0e000001,                                                  // Trailing 1s:                                /// 00894
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xffc00001,                                                  // -signaling NaN                              /// 0089c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0xff800000,                                                  // -inf                                        /// 008c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00908
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0090c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0091c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00920
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00928
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0092c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00930
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00934
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00938
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0093c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00944
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00948
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0095c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00960
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00964
			 0x7fc00001,                                                  // signaling NaN                               /// 00968
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0096c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00970
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0097c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00980
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c400000,                                                  // Leading 1s:                                 /// 00988
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00990
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00994
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009a4
			 0xbf028f5c,                                                  // -0.51                                       /// 009a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009c4
			 0x33333333,                                                  // 4 random values                             /// 009c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009dc
			 0x7fc00001,                                                  // signaling NaN                               /// 009e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a48
			 0x80000000,                                                  // -zero                                       /// 00a4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x7f800000,                                                  // inf                                         /// 00a84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ab0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00abc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ae8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00af8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00afc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b1c
			 0x7f800000,                                                  // inf                                         /// 00b20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b70
			 0xff800000,                                                  // -inf                                        /// 00b74
			 0xbf028f5c,                                                  // -0.51                                       /// 00b78
			 0x7f800000,                                                  // inf                                         /// 00b7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ba0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ba4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bcc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bdc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00be4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bf0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c44
			 0x00000000,                                                  // zero                                        /// 00c48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c7c
			 0xff800000,                                                  // -inf                                        /// 00c80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ca4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ca8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cb0
			 0x33333333,                                                  // 4 random values                             /// 00cb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ccc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ce4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d64
			 0x7f800000,                                                  // inf                                         /// 00d68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d78
			 0x7f800000,                                                  // inf                                         /// 00d7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00db8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dbc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dcc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00de8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00df8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e90
			 0x80000000,                                                  // -zero                                       /// 00e94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x4b000000,                                                  // 8388608.0                                   /// 00e9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ec0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ecc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ed4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ee4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xcb000000,                                                  // -8388608.0                                  /// 00eec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0xff800000,                                                  // -inf                                        /// 00f0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f64
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f70
			 0x7f800000,                                                  // inf                                         /// 00f74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x4b000000,                                                  // 8388608.0                                   /// 00f88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0x7f800000,                                                  // inf                                         /// 00fb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x55555555,                                                  // 4 random values                             /// 00fc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fcc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fd0
			 0x7f800000,                                                  // inf                                         /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0xff800000,                                                  // -inf                                        /// 00fe4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff8
			 0x0c7ffe00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e000007,                                                  // Trailing 1s:                                /// 00008
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0000c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00010
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00014
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00018
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0001c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00020
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00024
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00028
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00034
			 0x80000000,                                                  // -zero                                       /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0003c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00044
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00048
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0004c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00050
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00054
			 0x7fc00001,                                                  // signaling NaN                               /// 00058
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0005c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00060
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00068
			 0x0e000001,                                                  // Trailing 1s:                                /// 0006c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00070
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00078
			 0x0c700000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00080
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00084
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00090
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00094
			 0x80000000,                                                  // -zero                                       /// 00098
			 0x0e000003,                                                  // Trailing 1s:                                /// 0009c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000b0
			 0x7f800000,                                                  // inf                                         /// 000b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00100
			 0x0e000003,                                                  // Trailing 1s:                                /// 00104
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0010c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00110
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00114
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x55555555,                                                  // 4 random values                             /// 0011c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00120
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00124
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00128
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0012c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00130
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00134
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00138
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0013c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00140
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00144
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00148
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00150
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00154
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00158
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0015c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00160
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00164
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00168
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00170
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00174
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00178
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00180
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00184
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00188
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0018c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00190
			 0x0e000007,                                                  // Trailing 1s:                                /// 00194
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0019c
			 0x55555555,                                                  // 4 random values                             /// 001a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x80000000,                                                  // -zero                                       /// 001b8
			 0x00000000,                                                  // zero                                        /// 001bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c4
			 0xbf028f5c,                                                  // -0.51                                       /// 001c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x55555555,                                                  // 4 random values                             /// 001e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00200
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0020c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00210
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00218
			 0x0e000001,                                                  // Trailing 1s:                                /// 0021c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00220
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00228
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00234
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00238
			 0x00000000,                                                  // zero                                        /// 0023c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00240
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00244
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00250
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00254
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x3f028f5c,                                                  // 0.51                                        /// 00268
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00270
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00278
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0027c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00280
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00284
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00288
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00290
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00294
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00298
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0029c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00300
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00308
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0030c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00310
			 0x00000000,                                                  // zero                                        /// 00314
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00318
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0031c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00320
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00328
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0032c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00330
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00338
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7f800000,                                                  // inf                                         /// 00344
			 0x4b000000,                                                  // 8388608.0                                   /// 00348
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0034c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00350
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00358
			 0xbf028f5c,                                                  // -0.51                                       /// 0035c
			 0x3f028f5c,                                                  // 0.51                                        /// 00360
			 0x7fc00001,                                                  // signaling NaN                               /// 00364
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00368
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0036c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00370
			 0x80000000,                                                  // -zero                                       /// 00374
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00378
			 0x33333333,                                                  // 4 random values                             /// 0037c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00380
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00384
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00388
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x7fc00001,                                                  // signaling NaN                               /// 00390
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00398
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e8
			 0xcb000000,                                                  // -8388608.0                                  /// 003ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00400
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00408
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0040c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00410
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00414
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00418
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00420
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00424
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00430
			 0x0e000007,                                                  // Trailing 1s:                                /// 00434
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00438
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0043c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00444
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00448
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00454
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0045c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00464
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0046c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00470
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00474
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x33333333,                                                  // 4 random values                             /// 0047c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00480
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00488
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0048c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00494
			 0x0e000003,                                                  // Trailing 1s:                                /// 00498
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0049c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d0
			 0x00000000,                                                  // zero                                        /// 004d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00500
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00504
			 0x7fc00001,                                                  // signaling NaN                               /// 00508
			 0x00000000,                                                  // zero                                        /// 0050c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00518
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0051c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00520
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00524
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00528
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0c600000,                                                  // Leading 1s:                                 /// 00534
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00540
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00544
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00548
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00550
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00554
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00558
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0055c
			 0x7f800000,                                                  // inf                                         /// 00560
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00564
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00568
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0056c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00570
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0057c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00580
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00584
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00588
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0058c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00590
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00598
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a0
			 0x4b000000,                                                  // 8388608.0                                   /// 005a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b4
			 0x3f028f5c,                                                  // 0.51                                        /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e8
			 0x55555555,                                                  // 4 random values                             /// 005ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00600
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00604
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00614
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0061c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00620
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00624
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00628
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0062c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00634
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e000003,                                                  // Trailing 1s:                                /// 00648
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0064c
			 0xff800000,                                                  // -inf                                        /// 00650
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00658
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0065c
			 0x4b000000,                                                  // 8388608.0                                   /// 00660
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00668
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0066c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00670
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00674
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00678
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00680
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00684
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00688
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00690
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00694
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0069c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00700
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00704
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00708
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00710
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00718
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00724
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00728
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00730
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00734
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0073c
			 0x55555555,                                                  // 4 random values                             /// 00740
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00744
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00748
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0075c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00764
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00768
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0076c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00770
			 0x7fc00001,                                                  // signaling NaN                               /// 00774
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00778
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0077c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00790
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00798
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x80000000,                                                  // -zero                                       /// 007a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b0
			 0x55555555,                                                  // 4 random values                             /// 007b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00800
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00804
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00818
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00820
			 0x80000000,                                                  // -zero                                       /// 00824
			 0x7fc00001,                                                  // signaling NaN                               /// 00828
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0082c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00834
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00838
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00840
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00844
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00848
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0084c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00854
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00860
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00864
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00868
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0086c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00870
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00874
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00878
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0087c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00880
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x0c600000,                                                  // Leading 1s:                                 /// 00888
			 0xcb000000,                                                  // -8388608.0                                  /// 0088c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00890
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00894
			 0x00000000,                                                  // zero                                        /// 00898
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0089c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a0
			 0xbf028f5c,                                                  // -0.51                                       /// 008a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00900
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00904
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00908
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00910
			 0xff800000,                                                  // -inf                                        /// 00914
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00918
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0091c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00924
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00930
			 0x7f800000,                                                  // inf                                         /// 00934
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00938
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00940
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00948
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0094c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00950
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0095c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0096c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00970
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00978
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0097c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00980
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00988
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x3f028f5c,                                                  // 0.51                                        /// 009b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009b8
			 0x33333333,                                                  // 4 random values                             /// 009bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c8
			 0xbf028f5c,                                                  // -0.51                                       /// 009cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a20
			 0xffc00001,                                                  // -signaling NaN                              /// 00a24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a30
			 0x7fc00001,                                                  // signaling NaN                               /// 00a34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xff800000,                                                  // -inf                                        /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000000,                                                  // zero                                        /// 00a58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x33333333,                                                  // 4 random values                             /// 00ab8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00abc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00adc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ae4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00af0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00afc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x55555555,                                                  // 4 random values                             /// 00b94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ba0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ba4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00be8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000000,                                                  // -zero                                       /// 00c08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c30
			 0x4b000000,                                                  // 8388608.0                                   /// 00c34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c78
			 0x33333333,                                                  // 4 random values                             /// 00c7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ca8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cb0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cc0
			 0x80000000,                                                  // -zero                                       /// 00cc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d60
			 0xff800000,                                                  // -inf                                        /// 00d64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00da0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00da4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00db0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00df0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eb4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ebc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ec4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ef0
			 0xff800000,                                                  // -inf                                        /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00efc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x80000000,                                                  // -zero                                       /// 00f08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x7fc00001,                                                  // signaling NaN                               /// 00f74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x3f028f5c,                                                  // 0.51                                        /// 00f9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fe0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fe4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe8
			 0xff800000,                                                  // -inf                                        /// 00fec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80200000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x00000048,
			 0x00000198,
			 0x000005d0,
			 0x000002d0,
			 0x0000019c,
			 0x000004b8,
			 0x00000604,
			 0x00000258,

			 /// vpu register f2
			 0x41000000,
			 0x41980000,
			 0x41c80000,
			 0x40c00000,
			 0x40800000,
			 0x41c80000,
			 0x41e00000,
			 0x3f800000,

			 /// vpu register f3
			 0x41d80000,
			 0x41d80000,
			 0x41000000,
			 0x41900000,
			 0x41f00000,
			 0x41200000,
			 0x40c00000,
			 0x41980000,

			 /// vpu register f4
			 0x41a80000,
			 0x41c80000,
			 0x41200000,
			 0x41b80000,
			 0x40000000,
			 0x41700000,
			 0x40400000,
			 0x41e00000,

			 /// vpu register f5
			 0x41500000,
			 0x41980000,
			 0x41e00000,
			 0x41700000,
			 0x41b80000,
			 0x41d80000,
			 0x41900000,
			 0x41f80000,

			 /// vpu register f6
			 0x41400000,
			 0x41d00000,
			 0x41c00000,
			 0x41500000,
			 0x41f00000,
			 0x41800000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f7
			 0x40400000,
			 0x42000000,
			 0x41b80000,
			 0x40a00000,
			 0x40e00000,
			 0x40000000,
			 0x41980000,
			 0x41b00000,

			 /// vpu register f8
			 0x41100000,
			 0x41600000,
			 0x41c80000,
			 0x41d00000,
			 0x41b80000,
			 0x41b80000,
			 0x41d80000,
			 0x41c00000,

			 /// vpu register f9
			 0x3f800000,
			 0x41200000,
			 0x41b80000,
			 0x41e80000,
			 0x41f80000,
			 0x41900000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f10
			 0x41800000,
			 0x40a00000,
			 0x41f80000,
			 0x40000000,
			 0x41600000,
			 0x41a00000,
			 0x42000000,
			 0x41200000,

			 /// vpu register f11
			 0x40c00000,
			 0x41700000,
			 0x3f800000,
			 0x40c00000,
			 0x41d80000,
			 0x41700000,
			 0x41300000,
			 0x41b00000,

			 /// vpu register f12
			 0x40800000,
			 0x40e00000,
			 0x41a80000,
			 0x41980000,
			 0x40a00000,
			 0x41e00000,
			 0x41b80000,
			 0x40e00000,

			 /// vpu register f13
			 0x41e00000,
			 0x41a00000,
			 0x40c00000,
			 0x40a00000,
			 0x3f800000,
			 0x40c00000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f14
			 0x40000000,
			 0x41e80000,
			 0x41f00000,
			 0x41800000,
			 0x41000000,
			 0x41d80000,
			 0x3f800000,
			 0x40400000,

			 /// vpu register f15
			 0x41d00000,
			 0x41700000,
			 0x40000000,
			 0x41800000,
			 0x41b80000,
			 0x41100000,
			 0x40e00000,
			 0x40800000,

			 /// vpu register f16
			 0x40800000,
			 0x41900000,
			 0x41100000,
			 0x40e00000,
			 0x41900000,
			 0x3f800000,
			 0x41b00000,
			 0x40400000,

			 /// vpu register f17
			 0x41e80000,
			 0x40e00000,
			 0x41c80000,
			 0x40c00000,
			 0x41800000,
			 0x41100000,
			 0x41e80000,
			 0x40a00000,

			 /// vpu register f18
			 0x41e80000,
			 0x41400000,
			 0x40a00000,
			 0x40c00000,
			 0x3f800000,
			 0x41c00000,
			 0x40800000,
			 0x41d00000,

			 /// vpu register f19
			 0x41300000,
			 0x41980000,
			 0x41800000,
			 0x41b00000,
			 0x41f80000,
			 0x41000000,
			 0x41d00000,
			 0x41880000,

			 /// vpu register f20
			 0x41f00000,
			 0x41b00000,
			 0x40e00000,
			 0x41f00000,
			 0x41980000,
			 0x41b00000,
			 0x41100000,
			 0x41d00000,

			 /// vpu register f21
			 0x41c80000,
			 0x41f00000,
			 0x40e00000,
			 0x41600000,
			 0x40a00000,
			 0x41900000,
			 0x41600000,
			 0x40800000,

			 /// vpu register f22
			 0x41400000,
			 0x41200000,
			 0x40c00000,
			 0x3f800000,
			 0x41f80000,
			 0x40400000,
			 0x41500000,
			 0x41f80000,

			 /// vpu register f23
			 0x41d00000,
			 0x41980000,
			 0x40000000,
			 0x40800000,
			 0x41c00000,
			 0x41f00000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f24
			 0x40c00000,
			 0x41c80000,
			 0x41200000,
			 0x42000000,
			 0x41800000,
			 0x41100000,
			 0x41800000,
			 0x41000000,

			 /// vpu register f25
			 0x41c80000,
			 0x3f800000,
			 0x40e00000,
			 0x41f80000,
			 0x40800000,
			 0x41880000,
			 0x41000000,
			 0x42000000,

			 /// vpu register f26
			 0x41f80000,
			 0x41000000,
			 0x41b80000,
			 0x40400000,
			 0x41980000,
			 0x40400000,
			 0x40000000,
			 0x41f80000,

			 /// vpu register f27
			 0x41000000,
			 0x41400000,
			 0x41980000,
			 0x41200000,
			 0x42000000,
			 0x41800000,
			 0x41e80000,
			 0x41b00000,

			 /// vpu register f28
			 0x3f800000,
			 0x41200000,
			 0x41c00000,
			 0x41a80000,
			 0x41600000,
			 0x41700000,
			 0x41d00000,
			 0x41400000,

			 /// vpu register f29
			 0x40800000,
			 0x41100000,
			 0x41e80000,
			 0x41800000,
			 0x3f800000,
			 0x42000000,
			 0x41300000,
			 0x41d00000,

			 /// vpu register f30
			 0x41300000,
			 0x41900000,
			 0x40000000,
			 0x40c00000,
			 0x40e00000,
			 0x41200000,
			 0x41e80000,
			 0x40000000,

			 /// vpu register f31
			 0x41b80000,
			 0x41980000,
			 0x40800000,
			 0x41600000,
			 0x41b80000,
			 0x40000000,
			 0x41e00000,
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
		"fcvt.ps.un16 f30, f24\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f19, f3\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f27, f27\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f17, f0\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f9, f10\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f13, f19\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f22, f0\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f16, f14\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f8, f23\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f24, f1\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f6, f2\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f29, f20\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f10, f7\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f11, f3\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f22, f20\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f30, f12\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f22, f20\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f12, f26\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f11, f27\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f13, f8\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f16, f29\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f13, f24\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f29, f17\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f29, f1\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f15, f22\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f24, f27\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f29, f30\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f10, f21\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f27, f26\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f2, f24\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f19, f3\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f16, f7\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f6, f16\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f16, f15\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f5, f7\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f23, f12\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f22, f12\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f26, f12\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f29, f11\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f25, f27\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f13, f13\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f19, f27\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f4, f27\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f15, f22\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f7, f19\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f28, f6\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f6, f25\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f16, f8\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f17, f6\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f23, f23\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f15, f16\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f28, f27\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f30, f0\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f18, f12\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f25, f28\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f27, f12\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f3, f28\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f14, f8\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f14, f20\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f3, f7\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f30, f12\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f11, f27\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f29, f26\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f17, f9\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f2, f6\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f9, f23\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f14, f24\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f8, f11\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f15, f3\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f15, f16\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f7, f19\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f7, f10\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f21, f4\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f24, f25\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f4, f4\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f14, f14\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f16, f27\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f18, f14\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f11, f3\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f11, f8\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f11, f16\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f22, f28\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f18, f1\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f5, f4\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f20, f11\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f7, f14\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f20, f23\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f26, f26\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f14, f14\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f12, f26\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f14, f30\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f4, f23\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f17, f26\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f2, f8\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f23, f6\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f7, f0\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f23, f27\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f3, f4\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un16 f6, f9\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
